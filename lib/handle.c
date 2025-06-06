/* NBD client library in userspace
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

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ascii-ctype.h"
#include "internal.h"

static void
free_cmd_list (struct command *list)
{
  struct command *cmd, *cmd_next;

  for (cmd = list; cmd != NULL; cmd = cmd_next) {
    int error = cmd->error ? : ENOTCONN;

    cmd_next = cmd->next;
    CALL_CALLBACK (cmd->cb.completion, &error);
    nbd_internal_retire_and_free_command (cmd);
  }
}

struct nbd_handle *
nbd_create (void)
{
  static _Atomic int hnums = 1;
  struct nbd_handle *h;
  const char *s;

  nbd_internal_set_error_context ("nbd_create");

  h = calloc (1, sizeof *h);
  if (h == NULL) {
    set_error (errno, "calloc");
    goto error1;
  }
  h->magic = NBD_HANDLE_MAGIC;

  if (asprintf (&h->hname, "nbd%d", hnums++) == -1) {
    set_error (errno, "asprintf");
    goto error1;
  }

  h->unique = 1;
  h->tls_verify_peer = true;
  h->request_eh = true;
  h->request_sr = true;
  h->request_meta = true;
  h->request_block_size = true;
  h->pread_initialize = true;

  h->uri_allow_transports = LIBNBD_ALLOW_TRANSPORT_MASK;
  h->uri_allow_tls = LIBNBD_TLS_ALLOW;
  h->uri_allow_local_file = false;

  h->gflags = LIBNBD_HANDSHAKE_FLAG_MASK;

  s = getenv ("LIBNBD_DEBUG");
  h->debug = s && strcmp (s, "1") == 0;

  h->strict = LIBNBD_STRICT_MASK;

  h->public_state = STATE_START;
  h->state = STATE_START;
  h->pid = -1;

  h->export_name = strdup ("");
  if (h->export_name == NULL) {
    set_error (errno, "strdup");
    goto error1;
  }

  errno = pthread_mutex_init (&h->lock, NULL);
  if (errno != 0) {
    set_error (errno, "pthread_mutex_init");
    goto error1;
  }

  if (nbd_internal_run (h, cmd_create) == -1)
    goto error2;

  debug (h, "opening handle");
  /*debug (h, "sizeof *h = %zu", sizeof *h);*/
  return h;

 error2:
  pthread_mutex_destroy (&h->lock);
 error1:
  if (h) {
    free (h->export_name);
    free (h->hname);
    free (h);
  }
  return NULL;
}

void
nbd_close (struct nbd_handle *h)
{
  size_t i;

  nbd_internal_set_error_context ("nbd_close");

  if (h == NULL)
    return;

  assert (h->magic == NBD_HANDLE_MAGIC);

  debug (h, "closing handle");

  /* Free user callbacks first. */
  nbd_unlocked_clear_debug_callback (h);

  string_vector_empty (&h->querylist);
  free (h->bs_raw.storage);
  free (h->bs_cooked.storage);
  nbd_internal_reset_size_and_flags (h);
  for (i = 0; i < h->meta_contexts.len; ++i)
    free (h->meta_contexts.ptr[i].name);
  meta_vector_reset (&h->meta_contexts);
  nbd_internal_free_option (h);
  free_cmd_list (h->cmds_to_issue);
  free_cmd_list (h->cmds_in_flight);
  free_cmd_list (h->cmds_done);
  string_vector_empty (&h->argv);
  if (h->sact_sockpath) {
    if (h->pid > 0)
      kill (h->pid, SIGTERM);
    unlink (h->sact_sockpath);
    free (h->sact_sockpath);
  }
  if (h->sact_tmpdir) {
    rmdir (h->sact_tmpdir);
    free (h->sact_tmpdir);
  }
  free (h->hostname);
  free (h->port);
  if (h->result)
    freeaddrinfo (h->result);
  if (h->sock)
    h->sock->ops->close (h->sock);
  if (h->pid > 0)
    waitpid (h->pid, NULL, 0);

  free (h->export_name);
  free (h->sact_name);
  free (h->tls_certificates);
  free (h->tls_username);
  free (h->tls_hostname);
  free (h->tls_psk_file);
  string_vector_empty (&h->request_meta_contexts);
  free (h->hname);
  pthread_mutex_destroy (&h->lock);

  /* Although we are about to free the handle, set the h->magic field
   * to something invalid so we can catch use-after-free.
   */
  h->magic = 0xdeaddead;
  free (h);
}

int
nbd_unlocked_set_handle_name (struct nbd_handle *h, const char *handle_name)
{
  char *new_name;

  new_name = strdup (handle_name);
  if (!new_name) {
    set_error (errno, "strdup");
    return -1;
  }

  free (h->hname);
  h->hname = new_name;
  return 0;
}

char *
nbd_unlocked_get_handle_name (struct nbd_handle *h)
{
  char *copy = strdup (h->hname);

  if (!copy) {
    set_error (errno, "strdup");
    return NULL;
  }

  return copy;
}

int
nbd_unlocked_set_socket_activation_name (struct nbd_handle *h,
                                         const char *name)
{
  size_t i, len;
  char *new_name;

  len = strlen (name);

  /* Setting it to empty string stores NULL in the handle. */
  if (len == 0) {
    free (h->sact_name);
    h->sact_name = NULL;
    return 0;
  }

  /* Check the proposed name is short and alphanumeric. */
  if (len > 32) {
    set_error (ENAMETOOLONG, "socket activation name should be "
               "<= 32 characters");
    return -1;
  }
  for (i = 0; i < len; ++i) {
    if (! ascii_isalnum (name[i])) {
      set_error (EINVAL, "socket activation name should contain "
                 "only alphanumeric ASCII characters");
      return -1;
    }
  }

  new_name = strdup (name);
  if (!new_name) {
    set_error (errno, "strdup");
    return -1;
  }

  free (h->sact_name);
  h->sact_name = new_name;
  return 0;
}

char *
nbd_unlocked_get_socket_activation_name (struct nbd_handle *h)
{
  char *copy = strdup (h->sact_name ? h->sact_name : "");

  if (!copy) {
    set_error (errno, "strdup");
    return NULL;
  }

  return copy;
}

uintptr_t
nbd_unlocked_set_private_data (struct nbd_handle *h, uintptr_t data)
{
  uintptr_t old_data;

  /* atomic_exchange? XXX */
  old_data = h->private_data;
  h->private_data = data;
  return old_data;
}

uintptr_t
nbd_unlocked_get_private_data (struct nbd_handle *h)
{
  return h->private_data;
}

int
nbd_unlocked_set_export_name (struct nbd_handle *h, const char *export_name)
{
  char *new_name;

  if (strnlen (export_name, NBD_MAX_STRING + 1) > NBD_MAX_STRING) {
    set_error (ENAMETOOLONG, "export name too long for NBD protocol");
    return -1;
  }

  if (strcmp (export_name, h->export_name) == 0)
    return 0;

  new_name = strdup (export_name);
  if (!new_name) {
    set_error (errno, "strdup");
    return -1;
  }

  free (h->export_name);
  h->export_name = new_name;
  nbd_internal_reset_size_and_flags (h);
  h->meta_valid = false;
  return 0;
}

char *
nbd_unlocked_get_export_name (struct nbd_handle *h)
{
  char *copy = strdup (h->export_name);

  if (!copy) {
    set_error (errno, "strdup");
    return NULL;
  }

  return copy;
}

int
nbd_unlocked_set_request_block_size (struct nbd_handle *h, bool request)
{
  h->request_block_size = request;
  return 0;
}

int
nbd_unlocked_get_request_block_size (struct nbd_handle *h)
{
  return h->request_block_size;
}

int
nbd_unlocked_set_full_info (struct nbd_handle *h, bool request)
{
  h->full_info = request;
  return 0;
}

int
nbd_unlocked_get_full_info (struct nbd_handle *h)
{
  return h->full_info;
}

char *
nbd_unlocked_get_canonical_export_name (struct nbd_handle *h)
{
  char *r;

  if (h->eflags == 0) {
    set_error (EINVAL, "server has not returned export flags, "
               "you need to connect to the server first");
    return NULL;
  }
  if (h->canonical_name == NULL) {
    set_error (ENOTSUP, "server did not advertise a canonical name");
    return NULL;
  }

  r = strdup (h->canonical_name);
  if (r == NULL) {
    set_error (errno, "strdup");
    return NULL;
  }
  return r;
}

char *
nbd_unlocked_get_export_description (struct nbd_handle *h)
{
  char *r;

  if (h->eflags == 0) {
    set_error (EINVAL, "server has not returned export flags, "
               "you need to connect to the server first");
    return NULL;
  }
  if (h->description == NULL) {
    set_error (ENOTSUP, "server did not advertise a description");
    return NULL;
  }

  r = strdup (h->description);
  if (r == NULL) {
    set_error (errno, "strdup");
    return NULL;
  }
  return r;
}

int
nbd_unlocked_add_meta_context (struct nbd_handle *h, const char *name)
{
  char *copy;

  if (strnlen (name, NBD_MAX_STRING + 1) > NBD_MAX_STRING) {
    set_error (ENAMETOOLONG, "meta context name too long for NBD protocol");
    return -1;
  }

  copy = strdup (name);
  if (!copy) {
    set_error (errno, "strdup");
    return -1;
  }

  if (string_vector_append (&h->request_meta_contexts, copy) == -1) {
    free (copy);
    set_error (errno, "realloc");
    return -1;
  }

  return 0;
}

ssize_t
nbd_unlocked_get_nr_meta_contexts (struct nbd_handle *h)
{
  return h->request_meta_contexts.len;
}

char *
nbd_unlocked_get_meta_context (struct nbd_handle *h, size_t i)
{
  char *ret;

  if (i >= h->request_meta_contexts.len) {
    set_error (EINVAL, "meta context request out of range");
    return NULL;
  }

  ret = strdup (h->request_meta_contexts.ptr[i]);
  if (ret == NULL)
    set_error (errno, "strdup");

  return ret;
}

int
nbd_unlocked_clear_meta_contexts (struct nbd_handle *h)
{
  string_vector_empty (&h->request_meta_contexts);
  return 0;
}


int
nbd_unlocked_set_request_extended_headers (struct nbd_handle *h,
                                           bool request)
{
  h->request_eh = request;
  return 0;
}

/* NB: may_set_error = false. */
int
nbd_unlocked_get_request_extended_headers (struct nbd_handle *h)
{
  return h->request_eh;
}

int
nbd_unlocked_get_extended_headers_negotiated (struct nbd_handle *h)
{
  return h->extended_headers;
}

int
nbd_unlocked_set_request_structured_replies (struct nbd_handle *h,
                                             bool request)
{
  h->request_sr = request;
  return 0;
}

/* NB: may_set_error = false. */
int
nbd_unlocked_get_request_structured_replies (struct nbd_handle *h)
{
  return h->request_sr;
}

int
nbd_unlocked_set_request_meta_context (struct nbd_handle *h,
                                       bool request)
{
  h->request_meta = request;
  return 0;
}

/* NB: may_set_error = false. */
int
nbd_unlocked_get_request_meta_context (struct nbd_handle *h)
{
  return h->request_meta;
}

int
nbd_unlocked_get_structured_replies_negotiated (struct nbd_handle *h)
{
  return h->structured_replies;
}

int
nbd_unlocked_set_handshake_flags (struct nbd_handle *h,
                                  uint32_t flags)
{
  /* The generator already ensured flags was in range. */
  h->gflags = flags;
  return 0;
}

/* NB: may_set_error = false. */
uint32_t
nbd_unlocked_get_handshake_flags (struct nbd_handle *h)
{
  return h->gflags;
}

int
nbd_unlocked_set_pread_initialize (struct nbd_handle *h, bool request)
{
  h->pread_initialize = request;
  return 0;
}

/* NB: may_set_error = false. */
int
nbd_unlocked_get_pread_initialize (struct nbd_handle *h)
{
  return h->pread_initialize;
}

int
nbd_unlocked_set_strict_mode (struct nbd_handle *h, uint32_t flags)
{
  h->strict = flags;
  return 0;
}

/* NB: may_set_error = false. */
uint32_t
nbd_unlocked_get_strict_mode (struct nbd_handle *h)
{
  return h->strict;
}

const char *
nbd_unlocked_get_package_name (struct nbd_handle *h)
{
  return PACKAGE_NAME;
}

const char *
nbd_unlocked_get_version (struct nbd_handle *h)
{
  return PACKAGE_VERSION;
}

int
nbd_unlocked_kill_subprocess (struct nbd_handle *h, int signum)
{
  if (h->pid == -1) {
    set_error (ESRCH, "no subprocess exists");
    return -1;
  }
  assert (h->pid > 0);

  if (signum == 0)
    signum = SIGTERM;
  if (signum < 0) {
    set_error (EINVAL, "invalid signal number: %d", signum);
    return -1;
  }

  if (kill (h->pid, signum) == -1) {
    set_error (errno, "kill");
    return -1;
  }

  return 0;
}

int64_t
nbd_unlocked_get_subprocess_pid (struct nbd_handle *h)
{
  if (h->pid == -1) {
    set_error (ESRCH, "no subprocess exists");
    return -1;
  }

  /* Probably this is always > 0, but we make no guarantees as this
   * function is only useful for debugging.
   */
  return h->pid;
}

/* NB: is_locked = false, may_set_error = false. */
int
nbd_unlocked_supports_tls (struct nbd_handle *h)
{
#ifdef HAVE_GNUTLS
  return 1;
#else
  return 0;
#endif
}

/* NB: is_locked = false, may_set_error = false. */
int
nbd_unlocked_supports_vsock (struct nbd_handle *h)
{
#if HAVE_STRUCT_SOCKADDR_VM
  return 1;
#else
  return 0;
#endif
}

/* NB: is_locked = false, may_set_error = false. */
int
nbd_unlocked_supports_uri (struct nbd_handle *h)
{
#ifdef HAVE_LIBXML2
  return 1;
#else
  return 0;
#endif
}

const char *
nbd_unlocked_get_protocol (struct nbd_handle *h)
{
  /* I believe that if we reach the Connected or Closed permitted
   * states, then the state machine must have set h->protocol.  So if
   * this assertion is hit then it indicates a bug in libnbd.
   */
  assert (h->protocol);

  return h->protocol;
}

int
nbd_unlocked_set_uri_allow_transports (struct nbd_handle *h, uint32_t mask)
{
  h->uri_allow_transports = mask;
  return 0;
}

int
nbd_unlocked_set_uri_allow_tls (struct nbd_handle *h, int tls)
{
  h->uri_allow_tls = tls;
  return 0;
}

int
nbd_unlocked_set_uri_allow_local_file (struct nbd_handle *h, bool allow)
{
  h->uri_allow_local_file = allow;
  return 0;
}

/* NB: may_set_error = false. */
uint64_t
nbd_unlocked_stats_bytes_sent (struct nbd_handle *h)
{
  return h->bytes_sent;
}

/* NB: may_set_error = false. */
uint64_t
nbd_unlocked_stats_chunks_sent (struct nbd_handle *h)
{
  return h->chunks_sent;
}

/* NB: may_set_error = false. */
uint64_t
nbd_unlocked_stats_bytes_received (struct nbd_handle *h)
{
  return h->bytes_received;
}

/* NB: may_set_error = false. */
uint64_t
nbd_unlocked_stats_chunks_received (struct nbd_handle *h)
{
  return h->chunks_received;
}
