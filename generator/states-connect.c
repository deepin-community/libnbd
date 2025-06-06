/* nbd client library in userspace: state machine
 * Copyright Red Hat
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/* State machines related to connecting to the server. */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <netdb.h>
#include <assert.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>

extern char **environ;

/* Disable Nagle's algorithm on the socket, but don't fail. */
static void
disable_nagle (int sock)
{
  const int flag = 1;

  setsockopt (sock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof flag);
}

/* Disable SIGPIPE on FreeBSD & MacOS.
 *
 * Does nothing on other platforms, but if those platforms have
 * MSG_NOSIGNAL then we will set that when writing.  (FreeBSD has both.)
 */
static void
disable_sigpipe (int sock)
{
#ifdef SO_NOSIGPIPE
  const int flag = 1;

  setsockopt (sock, SOL_SOCKET, SO_NOSIGPIPE, &flag, sizeof flag);
#endif
}

STATE_MACHINE {
 CONNECT.START:
  sa_family_t family;
  int fd, r;

  assert (!h->sock);
  family = h->connaddr.ss_family;
  fd = nbd_internal_socket (family, SOCK_STREAM, 0, true);
  if (fd == -1) {
    SET_NEXT_STATE (%.DEAD);
    set_error (errno, "socket");
    return 0;
  }
  h->sock = nbd_internal_socket_create (fd);
  if (!h->sock) {
    SET_NEXT_STATE (%.DEAD);
    return 0;
  }

  disable_nagle (fd);
  disable_sigpipe (fd);

  r = connect (fd, (struct sockaddr *)&h->connaddr, h->connaddrlen);
  if (r == 0 || (r == -1 && errno == EINPROGRESS))
    return 0;
  assert (r == -1);
#ifdef __linux__
  if (errno == EAGAIN && family == AF_UNIX) {
    /* This can happen on Linux when connecting to a Unix domain
     * socket, if the server's backlog is full.  Unfortunately there
     * is nothing good we can do on the client side when this happens
     * since any solution would involve sleeping or busy-waiting.  The
     * only solution is on the server side, increasing the backlog.
     * But at least improve the error message.
     * https://bugzilla.redhat.com/1925045
     */
    SET_NEXT_STATE (%.DEAD);
    set_error (errno, "connect: server backlog overflowed, "
               "see https://bugzilla.redhat.com/1925045");
    return 0;
  }
#endif
  SET_NEXT_STATE (%.DEAD);
  set_error (errno, "connect");
  return 0;

 CONNECT.CONNECTING:
  int status;
  socklen_t len = sizeof status;

  if (getsockopt (h->sock->ops->get_fd (h->sock),
                  SOL_SOCKET, SO_ERROR, &status, &len) == -1) {
    SET_NEXT_STATE (%.DEAD);
    set_error (errno, "getsockopt: SO_ERROR");
    return 0;
  }
  /* This checks the status of the original connect call. */
  if (status == 0) {
    SET_NEXT_STATE (%^MAGIC.START);
    return 0;
  }
  else {
    SET_NEXT_STATE (%.DEAD);
    set_error (status, "connect");
    return 0;
  }

 CONNECT_TCP.START:
  int r;

  assert (h->hostname != NULL);
  assert (h->port != NULL);

  if (h->result) {
    freeaddrinfo (h->result);
    h->result = NULL;
  }

  h->connect_errno = 0;

  memset (&h->hints, 0, sizeof h->hints);
  h->hints.ai_family = AF_UNSPEC;
  h->hints.ai_socktype = SOCK_STREAM;
  h->hints.ai_flags = 0;
  h->hints.ai_protocol = 0;

  /* XXX Unfortunately getaddrinfo blocks.  getaddrinfo_a isn't
   * portable and in any case isn't an alternative because it can't be
   * integrated into a main loop.
   */
  r = getaddrinfo (h->hostname, h->port, &h->hints, &h->result);
  if (r != 0) {
    SET_NEXT_STATE (%^START);
    set_error (0, "getaddrinfo: hostname \"%s\" port \"%s\": %s",
               h->hostname, h->port, gai_strerror (r));
    return -1;
  }

  h->rp = h->result;
  SET_NEXT_STATE (%CONNECT);
  return 0;

 CONNECT_TCP.CONNECT:
  int fd;

  assert (!h->sock);

  if (h->rp == NULL) {
    /* We tried all the results from getaddrinfo without success.
     * Save errno from most recent connect(2) call. XXX
     */
    SET_NEXT_STATE (%^START);
    set_error (h->connect_errno,
               "connect: %s:%s: could not connect to remote host",
               h->hostname, h->port);
    return -1;
  }

  fd = nbd_internal_socket (h->rp->ai_family,
                            h->rp->ai_socktype,
                            h->rp->ai_protocol,
                            true);
  if (fd == -1) {
    SET_NEXT_STATE (%NEXT_ADDRESS);
    return 0;
  }
  h->sock = nbd_internal_socket_create (fd);
  if (!h->sock) {
    SET_NEXT_STATE (%.DEAD);
    return 0;
  }

  disable_nagle (fd);
  disable_sigpipe (fd);

  if (connect (fd, h->rp->ai_addr, h->rp->ai_addrlen) == -1) {
    if (errno != EINPROGRESS) {
      if (h->connect_errno == 0)
        h->connect_errno = errno;
      SET_NEXT_STATE (%NEXT_ADDRESS);
      return 0;
    }
  }

  SET_NEXT_STATE (%CONNECTING);
  return 0;

 CONNECT_TCP.CONNECTING:
  int status;
  socklen_t len = sizeof status;

  if (getsockopt (h->sock->ops->get_fd (h->sock),
                  SOL_SOCKET, SO_ERROR, &status, &len) == -1) {
    SET_NEXT_STATE (%.DEAD);
    set_error (errno, "getsockopt: SO_ERROR");
    return 0;
  }
  /* This checks the status of the original connect call. */
  if (status == 0)
    SET_NEXT_STATE (%^MAGIC.START);
  else {
    if (h->connect_errno == 0)
      h->connect_errno = status;
    SET_NEXT_STATE (%NEXT_ADDRESS);
  }
  return 0;

 CONNECT_TCP.NEXT_ADDRESS:
  if (h->sock) {
    h->sock->ops->close (h->sock);
    h->sock = NULL;
  }
  if (h->rp)
    h->rp = h->rp->ai_next;
  SET_NEXT_STATE (%CONNECT);
  return 0;

 CONNECT_COMMAND.START:
  enum state next;
  bool parentfd_transferred;
  int sv[2];
  int flags;
  struct socket *sock;
  struct execvpe execvpe_ctx;
  pid_t pid;

  assert (!h->sock);
  assert (h->argv.ptr);
  assert (h->argv.ptr[0]);

  next = %.DEAD;
  parentfd_transferred = false;

  if (nbd_internal_socketpair (AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
    set_error (errno, "socketpair");
    goto done;
  }

  /* A process is effectively in an unusable state if any of STDIN_FILENO
   * (fd#0), STDOUT_FILENO (fd#1) and STDERR_FILENO (fd#2) don't exist. If they
   * exist however, then the socket pair created above will not intersect with
   * the fd set { 0, 1, 2 }. This is relevant for the child-side dup2() logic
   * below.
   */
  assert (sv[0] > STDERR_FILENO);
  assert (sv[1] > STDERR_FILENO);

  /* Only the parent-side end of the socket pair must be set to non-blocking,
   * because the child may not be expecting a non-blocking socket.
   */
  flags = fcntl (sv[0], F_GETFL, 0);
  if (flags == -1 ||
      fcntl (sv[0], F_SETFL, flags|O_NONBLOCK) == -1) {
    set_error (errno, "fcntl");
    goto close_socket_pair;
  }

  sock = nbd_internal_socket_create (sv[0]);
  if (!sock)
    /* nbd_internal_socket_create() calls set_error() internally */
    goto close_socket_pair;
  parentfd_transferred = true;

  if (nbd_internal_execvpe_init (&execvpe_ctx, h->argv.ptr[0], h->argv.len) ==
      -1) {
    set_error (errno, "nbd_internal_execvpe_init");
    goto close_high_level_socket;
  }

  pid = fork ();
  if (pid == -1) {
    set_error (errno, "fork");
    goto uninit_execvpe;
  }

  if (pid == 0) {         /* child - run command */
    if (close (sv[0]) == -1) {
      nbd_internal_fork_safe_perror ("close");
      _exit (126);
    }
    if (dup2 (sv[1], STDIN_FILENO) == -1 ||
        dup2 (sv[1], STDOUT_FILENO) == -1) {
      nbd_internal_fork_safe_perror ("dup2");
      _exit (126);
    }
    NBD_INTERNAL_FORK_SAFE_ASSERT (sv[1] != STDIN_FILENO);
    NBD_INTERNAL_FORK_SAFE_ASSERT (sv[1] != STDOUT_FILENO);
    if (close (sv[1]) == -1) {
      nbd_internal_fork_safe_perror ("close");
      _exit (126);
    }

    /* Restore SIGPIPE back to SIG_DFL. */
    if (signal (SIGPIPE, SIG_DFL) == SIG_ERR) {
      nbd_internal_fork_safe_perror ("signal");
      _exit (126);
    }

    (void)nbd_internal_fork_safe_execvpe (&execvpe_ctx, &h->argv, environ);
    nbd_internal_fork_safe_perror (h->argv.ptr[0]);
    if (errno == ENOENT)
      _exit (127);
    else
      _exit (126);
  }

  /* Parent -- we're done; commit. */
  h->pid = pid;
  h->sock = sock;

  /* The sockets are connected already, we can jump directly to
   * receiving the server magic.
   */
  next = %^MAGIC.START;

  /* fall through, for releasing the temporaries */

uninit_execvpe:
  nbd_internal_execvpe_uninit (&execvpe_ctx);

close_high_level_socket:
  if (next == %.DEAD)
    sock->ops->close (sock);

close_socket_pair:
  assert (next == %.DEAD || parentfd_transferred);
  if (!parentfd_transferred)
    close (sv[0]);
  close (sv[1]);

done:
  SET_NEXT_STATE (next);
  return 0;

} /* END STATE MACHINE */
