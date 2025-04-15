/* NBD client library in userspace
 * WARNING: THIS FILE IS GENERATED FROM
 * generator/generator generator/states*.c
 * ANY CHANGES YOU MAKE TO THIS FILE WILL BE LOST.
 *
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

#line 1 "generator/states-connect-socket-activation.c"
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

/* State machine related to connecting with systemd socket activation. */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "internal.h"
#include "compiler-macros.h"
#include "unique-name.h"
#include "array-size.h"
#include "checked-overflow.h"

/* This is baked into the systemd socket activation API. */
#define FIRST_SOCKET_ACTIVATION_FD 3

/* Describes a systemd socket activation environment variable. */
struct sact_var {
  const char *prefix; /* variable name and equal sign */
  size_t prefix_len;
  const char *value;
  size_t value_len;
};

/* Determine the length of a string, using "sizeof" whenever possible.
 *
 * Do not use this macro on an argument that has side effects, as no guarantees
 * are given regarding the number of times the argument may be evaluated.
 * TYPE_IS_ARRAY(s) itself may contribute a different number of evaluations
 * dependent on whether "s" has variably modified type, and then the conditional
 * operator either evaluates "sizeof s" (which contributes 0 or 1 evaluations,
 * dependent on whether "s" has variably modified type) or strlen(s) (which
 * contributes 1 evaluation). Also note that the argument of the "sizeof"
 * operator is *only* parenthesized because "s" is a macro parameter here.
*/
#define STRLEN1(s) ((TYPE_IS_ARRAY (s) ? sizeof (s) - 1 : strlen (s)))

/* Push a new element to an array of "sact_var" structures.
 *
 * "vars" is the array to extend. "num_vars" (of type (size_t *)) points to the
 * number of elements that the array, on input, contains; (*num_vars) is
 * increased by one on output. "prefix" and "value" serve as the values for
 * setting the fields in the new element. "ofs" (of type (size_t *)) may be
 * NULL; if it isn't, then on output, (*ofs) is set to the input value of
 * (*num_vars): the offset of the just-pushed element.
 *
 * Avoid arguments with side-effects here as well.
 */
#define SACT_VAR_PUSH(vars, num_vars, prefix, value, ofs)       \
  SACT_VAR_PUSH1 ((vars), (num_vars), (prefix), (value), (ofs), \
                  NBDKIT_UNIQUE_NAME (_ofs))
#define SACT_VAR_PUSH1(vars, num_vars, prefix, value, ofs, ofs1)             \
  do {                                                                       \
    size_t *ofs1;                                                            \
                                                                             \
    assert (*(num_vars) < ARRAY_SIZE (vars));                                \
    ofs1 = (ofs);                                                            \
    if (ofs1 != NULL)                                                        \
      *ofs1 = *(num_vars);                                                   \
    (vars)[(*(num_vars))++] = (struct sact_var){ (prefix), STRLEN1 (prefix), \
                                                 (value), STRLEN1 (value) }; \
  } while (0)

extern char **environ;

/* Prepare environment for calling execvp when doing systemd socket activation.
 * Takes the current environment and copies it. Removes any existing socket
 * activation variables and replaces them with new ones. Variables in "sact_var"
 * will be placed at the front of "env", preserving the order from "sact_var".
 */
static int
prepare_socket_activation_environment (string_vector *env,
                                       const struct sact_var *sact_var,
                                       size_t num_vars)
{
  const struct sact_var *var_end;
  char *new_var;
  const struct sact_var *var;
  size_t i;

  *env = (string_vector)empty_vector;

  /* Set the exclusive limit for loops over "sact_var". */
  var_end = sact_var + num_vars;

  /* New environment variable being constructed for "env". */
  new_var = NULL;

  /* Copy "sact_var" to the front of "env". */
  for (var = sact_var; var < var_end; ++var) {
    size_t new_var_size;
    char *p;

    /* Calculate the size of the "NAME=value" string. */
    if (ADD_OVERFLOW (var->prefix_len, var->value_len, &new_var_size) ||
        ADD_OVERFLOW (new_var_size, 1u, &new_var_size)) {
      errno = EOVERFLOW;
      goto err;
    }

    /* Allocate and format "NAME=value". */
    new_var = malloc (new_var_size);
    if (new_var == NULL)
      goto err;
    p = new_var;

    memcpy (p, var->prefix, var->prefix_len);
    p += var->prefix_len;

    memcpy (p, var->value, var->value_len);
    p += var->value_len;

    *p++ = '\0';

    /* Push "NAME=value" to the vector. */
    if (string_vector_append (env, new_var) == -1)
      goto err;
    /* Ownership transferred. */
    new_var = NULL;
  }

  /* Append the current environment to "env", but remove "sact_var". */
  for (i = 0; environ[i] != NULL; ++i) {
    for (var = sact_var; var < var_end; ++var) {
      if (strncmp (environ[i], var->prefix, var->prefix_len) == 0)
        break;
    }
    /* Drop known socket activation variable from the current environment. */
    if (var < var_end)
      continue;

    new_var = strdup (environ[i]);
    if (new_var == NULL)
      goto err;

    if (string_vector_append (env, new_var) == -1)
      goto err;
    /* Ownership transferred. */
    new_var = NULL;
  }

  /* The environ must be NULL-terminated. */
  if (string_vector_append (env, NULL) == -1)
    goto err;

  return 0;

 err:
  set_error (errno, "malloc");
  free (new_var);
  string_vector_empty (env);
  return -1;
}


#line 1 "generator/states-connect.c"
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


#line 1 "generator/states-issue-command.c"
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

/* State machine for issuing commands (requests) to the server. */


#line 1 "generator/states-magic.c"
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

/* State machine for parsing the initial magic number from the server. */


#line 1 "generator/states-newstyle-opt-export-name.c"
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

/* State machine for ending newstyle handshake with NBD_OPT_EXPORT_NAME. */


#line 1 "generator/states-newstyle-opt-extended-headers.c"
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

/* State machine for negotiating NBD_OPT_EXTENDED_HEADERS. */


#line 1 "generator/states-newstyle-opt-go.c"
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

/* State machine for ending fixed newstyle handshake with NBD_OPT_GO. */


#line 1 "generator/states-newstyle-opt-list.c"
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

/* State machine for sending NBD_OPT_LIST to list exports.
 *
 * This is only reached via nbd_opt_list during opt_mode.
 */


#line 1 "generator/states-newstyle-opt-meta-context.c"
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

/* State machine for negotiating NBD_OPT_SET/LIST_META_CONTEXT. */


#line 1 "generator/states-newstyle-opt-starttls.c"
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

/* State machine for sending NBD_OPT_STARTTLS. */


#line 1 "generator/states-newstyle-opt-structured-reply.c"
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

/* State machine for negotiating NBD_OPT_STRUCTURED_REPLY. */


#line 1 "generator/states-newstyle.c"
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

#include <assert.h>

#include "ascii-ctype.h"
#include "internal.h"

/* Common code for parsing a reply to NBD_OPT_*. */
static int
prepare_for_reply_payload (struct nbd_handle *h, uint32_t opt)
{
  const size_t maxpayload = sizeof h->sbuf.or.payload;
  uint64_t magic;
  uint32_t option;
  uint32_t reply;
  uint32_t len;

  magic = be64toh (h->sbuf.or.option_reply.magic);
  option = be32toh (h->sbuf.or.option_reply.option);
  reply = be32toh (h->sbuf.or.option_reply.reply);
  len = be32toh (h->sbuf.or.option_reply.replylen);
  if (magic != NBD_REP_MAGIC || option != opt) {
    set_error (0, "handshake: invalid option reply magic or option");
    return -1;
  }

  /* Validate lengths that the state machine depends on. */
  switch (reply) {
  case NBD_REP_ACK:
    if (len != 0) {
      set_error (0, "handshake: invalid NBD_REP_ACK option reply length");
      return -1;
    }
    break;
  case NBD_REP_INFO:
    /* Can't enforce an upper bound, thanks to unknown INFOs */
    if (len < sizeof h->sbuf.or.payload.export.info) {
      set_error (0, "handshake: NBD_REP_INFO reply length too small");
      return -1;
    }
    break;
  case NBD_REP_META_CONTEXT:
    if (len <= sizeof h->sbuf.or.payload.context.context ||
        len > sizeof h->sbuf.or.payload.context) {
      set_error (0, "handshake: invalid NBD_REP_META_CONTEXT reply length");
      return -1;
    }
    break;
  }

  /* Read the following payload if it is short enough to fit in the
   * static buffer.  If it's too long, skip it.
   */
  len = be32toh (h->sbuf.or.option_reply.replylen);
  if (len > MAX_REQUEST_SIZE) {
    set_error (0, "handshake: invalid option reply length");
    return -1;
  }
  else if (len <= maxpayload)
    h->rbuf = &h->sbuf.or.payload;
  else
    h->rbuf = NULL;
  h->rlen = len;
  return 0;
}

/* Check an unexpected server reply error.
 *
 * This calls set_error with a descriptive error message and returns
 * 0.  Unless there is a further unexpected error while processing
 * this error, in which case it calls set_error and returns -1.
 */
static int
handle_reply_error (struct nbd_handle *h)
{
  uint32_t reply;
  uint32_t replylen;
  FILE *fp;
  char *s = NULL;
  size_t len = 0;
  int err = 0;

  reply = be32toh (h->sbuf.or.option_reply.reply);
  if (!NBD_REP_IS_ERR (reply)) {
    set_error (0, "handshake: unexpected option reply type %d", reply);
    return -1;
  }

  replylen = be32toh (h->sbuf.or.option_reply.replylen);
  assert (NBD_MAX_STRING < sizeof h->sbuf.or.payload);
  if (replylen > NBD_MAX_STRING) {
    set_error (0, "handshake: option error string too long");
    return -1;
  }

  /* Decode expected errors into a nicer string. */
  fp = open_memstream (&s, &len);
  if (fp == NULL) {
    set_error (errno, "open_memstream");
    return -1;
  }

  switch (reply) {
  case NBD_REP_ERR_UNSUP:
    err = ENOTSUP;
    fprintf (fp, "the operation is not supported by the server");
    break;
  case NBD_REP_ERR_POLICY:
    fprintf (fp, "server policy prevents the operation");
    break;
  case NBD_REP_ERR_PLATFORM:
    fprintf (fp, "the operation is not supported by the server platform");
    break;
  case NBD_REP_ERR_INVALID:
    err = EINVAL;
    fprintf (fp, "the server rejected this operation as invalid");
    break;
  case NBD_REP_ERR_TOO_BIG:
    err = EINVAL;
    fprintf (fp, "the operation is too large to process");
    break;
  case NBD_REP_ERR_TLS_REQD:
    err = ENOTSUP;
    fprintf (fp, "the server requires TLS encryption first");
    break;
  case NBD_REP_ERR_UNKNOWN:
    err = ENOENT;
    fprintf (fp, "the server has no export named '%s'", h->export_name);
    break;
  case NBD_REP_ERR_SHUTDOWN:
    err = ESHUTDOWN;
    fprintf (fp, "the server is shutting down");
    break;
  case NBD_REP_ERR_BLOCK_SIZE_REQD:
    err = EINVAL;
    fprintf (fp, "the server requires specific block sizes");
    break;
  default:
    fprintf (fp, "handshake: unknown reply from the server: 0x%" PRIx32,
             reply);
  }

  if (replylen > 0) {
    /* Since this message comes from the server, take steps to quote it. */
    uint32_t i;
    const unsigned char *msg = (unsigned char *) h->sbuf.or.payload.err_msg;

    fprintf (fp, ": ");
    for (i = 0; i < replylen; ++i) {
      if (ascii_isprint (msg[i]))
        fputc (msg[i], fp);
      else
        fprintf (fp, "\\x%02x", msg[i]);
    }
  }

  fclose (fp);

  set_error (err, "%s", s);
  free (s);

  return 0;
}

/* State machine for parsing the fixed newstyle handshake. */


#line 1 "generator/states-oldstyle.c"
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

/* State machine for parsing the oldstyle handshake. */


#line 1 "generator/states-reply-chunk.c"
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

/* State machine for parsing structured reply chunk payloads from the server. */

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

#include "minmax.h"

/* Structured reply must be completely inside the bounds of the
 * requesting command.
 */
static bool
structured_reply_in_bounds (uint64_t offset, uint32_t length,
                            const struct command *cmd)
{
  if (offset < cmd->offset ||
      offset >= cmd->offset + cmd->count ||
      offset + length > cmd->offset + cmd->count) {
    set_error (0, "range of structured reply is out of bounds, "
               "offset=%" PRIu64 ", cmd->offset=%" PRIu64 ", "
               "length=%" PRIu32 ", cmd->count=%" PRIu64 ": "
               "this is likely to be a bug in the NBD server",
               offset, cmd->offset, length, cmd->count);
    return false;
  }

  return true;
}

/* Return true if payload length of block status reply is valid.
 */
static bool
bs_reply_length_ok (uint16_t type, uint32_t length)
{
  size_t prefix_len;
  size_t extent_len;

  if (type == NBD_REPLY_TYPE_BLOCK_STATUS) {
    prefix_len = sizeof (struct nbd_chunk_block_status_32);
    extent_len = sizeof (struct nbd_block_descriptor_32);
  }
  else {
    assert (type == NBD_REPLY_TYPE_BLOCK_STATUS_EXT);
    prefix_len = sizeof (struct nbd_chunk_block_status_64);
    extent_len = sizeof (struct nbd_block_descriptor_64);
  }

  /* At least one descriptor is needed after id prefix */
  if (length < prefix_len + extent_len)
    return false;

  /* There must be an integral number of extents */
  length -= prefix_len;
  if (length % extent_len != 0)
    return false;

  return true;
}


#line 1 "generator/states-reply-simple.c"
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

/* State machine for parsing simple replies from the server. */


#line 1 "generator/states-reply.c"
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

#include <assert.h>
#include <stddef.h>

#define ASSERT_MEMBER_ALIAS(type, member_a, member_b) \
  STATIC_ASSERT (offsetof (type, member_a) == offsetof (type, member_b) && \
                 sizeof ((type *)NULL)->member_a == \
                 sizeof ((type *)NULL)->member_b, member_alias)

/* State machine for receiving reply messages from the server.
 *
 * Note that we never block while in this sub-group. If there is
 * insufficient data to finish parsing a reply, requiring us to block
 * until POLLIN, we instead track where in the state machine we left
 * off, then return to READY to actually block. Then, on entry to
 * REPLY.START, we can tell if this is the start of a new reply (rlen
 * is 0, stay put), a continuation of the preamble (reply_state is
 * STATE_START, resume with RECV_REPLY), or a continuation from any
 * other location (reply_state contains the state to jump to).
 */

static void
save_reply_state (struct nbd_handle *h)
{
  assert (h->rlen);
  assert (h->reply_state == STATE_START);
  h->reply_state = get_next_state (h);
  assert (h->reply_state != STATE_START);
}


#line 1 "generator/states.c"
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

/* This isn't "real" C code.  It is read by the generator, parsed, and
 * put into generated files.  Also it won't make much sense unless you
 * read the generator state machine and documentation in
 * generator/README.state-machine.md first.
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "minmax.h"

#include "internal.h"

/* Uncomment this to dump received protocol packets to stderr. */
/*#define DUMP_PACKETS 1*/

static int
recv_into_rbuf (struct nbd_handle *h)
{
  ssize_t r;
  void *rbuf;
  size_t rlen;

  /* As a special case h->rbuf is allowed to be NULL, meaning
   * throw away the data.
   *
   * When building with DUMP_PACKETS, it's worth debugging even
   * discarded packets; this makes our stack frame larger, but
   * DUMP_PACKETS is already for developers.  Otherwise, we share a
   * single static sink buffer across all nbd handles; we don't care
   * about thread-safety issues with two clients discarding data at
   * the same time, because we never read the sink.
   */
#ifdef DUMP_PACKETS
  char sink[1024];
#else
  static char sink[BUFSIZ];
#endif

  if (h->rlen == 0)
    return 0;                   /* move to next state */

  if (h->rbuf) {
    rbuf = h->rbuf;
    rlen = h->rlen;
  }
  else {
    rbuf = sink;
    rlen = MIN (h->rlen, sizeof sink);
  }

  r = h->sock->ops->recv (h, h->sock, rbuf, rlen);
  if (r == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK)
      return 1;                 /* more data */
    /* sock->ops->recv called set_error already. */
    return -1;
  }
  if (r == 0) {
    set_error (0, "recv: server disconnected unexpectedly");
    return -1;
  }
#ifdef DUMP_PACKETS
  nbd_internal_hexdump (rbuf, r, stderr);
#endif
  h->bytes_received += r;
  if (h->rbuf)
    h->rbuf = (char *)h->rbuf + r;
  h->rlen -= r;
  if (h->rlen == 0)
    return 0;                   /* move to next state */
  else
    return 1;                   /* more data */
}

static int
send_from_wbuf (struct nbd_handle *h)
{
  ssize_t r;

  if (h->wlen == 0)
    goto next_state;
  r = h->sock->ops->send (h, h->sock, h->wbuf, h->wlen, h->wflags);
  if (r == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK)
      return 1;                 /* more data */
    /* sock->ops->send called set_error already. */
    return -1;
  }
  h->bytes_sent += r;
  h->wbuf = (char *)h->wbuf + r;
  h->wlen -= r;
  if (h->wlen == 0)
    goto next_state;
  else
    return 1;                   /* more data */

 next_state:
  h->wflags = 0;                /* reset this when moving to next state */
  return 0;                     /* move to next state */
}

/* Forcefully fail any in-flight option */
static void
abort_option (struct nbd_handle *h)
{
  int err = nbd_get_errno () ? : ENOTCONN;

  CALL_CALLBACK (h->opt_cb.completion, &err);
  nbd_internal_free_option (h);
}

/* Forcefully fail any remaining in-flight commands in list */
void
nbd_internal_abort_commands (struct nbd_handle *h, struct command **list)
{
  struct command *next, *cmd;

  for (cmd = *list, *list = NULL; cmd != NULL; cmd = next) {
    bool retire = cmd->type == NBD_CMD_DISC;

    next = cmd->next;
    if (CALLBACK_IS_NOT_NULL (cmd->cb.completion)) {
      int error = cmd->error ? cmd->error : ENOTCONN;
      int r;

      assert (cmd->type != NBD_CMD_DISC);
      r = CALL_CALLBACK (cmd->cb.completion, &error);
      switch (r) {
      case -1:
        if (error)
          cmd->error = error;
        break;
      case 1:
        retire = true;
        break;
      }
    }
    if (cmd->error == 0)
      cmd->error = ENOTCONN;
    if (retire)
      nbd_internal_retire_and_free_command (cmd);
    else {
      cmd->next = NULL;
      if (h->cmds_done_tail)
        h->cmds_done_tail->next = cmd;
      else {
        assert (h->cmds_done == NULL);
        h->cmds_done = cmd;
      }
      h->cmds_done_tail = cmd;
    }
  }
}




#define SET_NEXT_STATE(s) (*blocked = false, *next_state = (s))
#define SET_NEXT_STATE_AND_BLOCK(s) (*next_state = (s))

/* START: Handle after being initially created */
static int
enter_STATE_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
  return 0;
}

#line 1025 "lib/states.c"
int
nbd_internal_enter_STATE_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_START;
  r = enter_STATE_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* CONNECT.START: Initial call to connect(2) on the socket */
static int
enter_STATE_CONNECT_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 67 "generator/states-connect.c"

  sa_family_t family;
  int fd, r;

  assert (!h->sock);
  family = h->connaddr.ss_family;
  fd = nbd_internal_socket (family, SOCK_STREAM, 0, true);
  if (fd == -1) {
    SET_NEXT_STATE (STATE_DEAD);
    set_error (errno, "socket");
    return 0;
  }
  h->sock = nbd_internal_socket_create (fd);
  if (!h->sock) {
    SET_NEXT_STATE (STATE_DEAD);
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
    SET_NEXT_STATE (STATE_DEAD);
    set_error (errno, "connect: server backlog overflowed, "
               "see https://bugzilla.redhat.com/1925045");
    return 0;
  }
#endif
  SET_NEXT_STATE (STATE_DEAD);
  set_error (errno, "connect");
  return 0;

}

#line 1103 "lib/states.c"
int
nbd_internal_enter_STATE_CONNECT_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_CONNECT_START;
  r = enter_STATE_CONNECT_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "CONNECT.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* CONNECT.CONNECTING: Connecting to the remote server */
static int
enter_STATE_CONNECT_CONNECTING (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 112 "generator/states-connect.c"

  int status;
  socklen_t len = sizeof status;

  if (getsockopt (h->sock->ops->get_fd (h->sock),
                  SOL_SOCKET, SO_ERROR, &status, &len) == -1) {
    SET_NEXT_STATE (STATE_DEAD);
    set_error (errno, "getsockopt: SO_ERROR");
    return 0;
  }
  /* This checks the status of the original connect call. */
  if (status == 0) {
    SET_NEXT_STATE (STATE_MAGIC_START);
    return 0;
  }
  else {
    SET_NEXT_STATE (STATE_DEAD);
    set_error (status, "connect");
    return 0;
  }

}

#line 1157 "lib/states.c"
int
nbd_internal_enter_STATE_CONNECT_CONNECTING (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_CONNECT_CONNECTING;
  r = enter_STATE_CONNECT_CONNECTING (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "CONNECT.CONNECTING",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* CONNECT_TCP.START: Connect to a remote TCP server */
static int
enter_STATE_CONNECT_TCP_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 133 "generator/states-connect.c"

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
    SET_NEXT_STATE (STATE_START);
    set_error (0, "getaddrinfo: hostname \"%s\" port \"%s\": %s",
               h->hostname, h->port, gai_strerror (r));
    return -1;
  }

  h->rp = h->result;
  SET_NEXT_STATE (STATE_CONNECT_TCP_CONNECT);
  return 0;

}

#line 1225 "lib/states.c"
int
nbd_internal_enter_STATE_CONNECT_TCP_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_CONNECT_TCP_START;
  r = enter_STATE_CONNECT_TCP_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "CONNECT_TCP.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* CONNECT_TCP.CONNECT: Initial call to connect(2) on a TCP socket */
static int
enter_STATE_CONNECT_TCP_CONNECT (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 168 "generator/states-connect.c"

  int fd;

  assert (!h->sock);

  if (h->rp == NULL) {
    /* We tried all the results from getaddrinfo without success.
     * Save errno from most recent connect(2) call. XXX
     */
    SET_NEXT_STATE (STATE_START);
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
    SET_NEXT_STATE (STATE_CONNECT_TCP_NEXT_ADDRESS);
    return 0;
  }
  h->sock = nbd_internal_socket_create (fd);
  if (!h->sock) {
    SET_NEXT_STATE (STATE_DEAD);
    return 0;
  }

  disable_nagle (fd);
  disable_sigpipe (fd);

  if (connect (fd, h->rp->ai_addr, h->rp->ai_addrlen) == -1) {
    if (errno != EINPROGRESS) {
      if (h->connect_errno == 0)
        h->connect_errno = errno;
      SET_NEXT_STATE (STATE_CONNECT_TCP_NEXT_ADDRESS);
      return 0;
    }
  }

  SET_NEXT_STATE (STATE_CONNECT_TCP_CONNECTING);
  return 0;

}

#line 1303 "lib/states.c"
int
nbd_internal_enter_STATE_CONNECT_TCP_CONNECT (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_CONNECT_TCP_CONNECT;
  r = enter_STATE_CONNECT_TCP_CONNECT (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "CONNECT_TCP.CONNECT",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* CONNECT_TCP.CONNECTING: Connecting to the remote server over a TCP socket */
static int
enter_STATE_CONNECT_TCP_CONNECTING (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 213 "generator/states-connect.c"

  int status;
  socklen_t len = sizeof status;

  if (getsockopt (h->sock->ops->get_fd (h->sock),
                  SOL_SOCKET, SO_ERROR, &status, &len) == -1) {
    SET_NEXT_STATE (STATE_DEAD);
    set_error (errno, "getsockopt: SO_ERROR");
    return 0;
  }
  /* This checks the status of the original connect call. */
  if (status == 0)
    SET_NEXT_STATE (STATE_MAGIC_START);
  else {
    if (h->connect_errno == 0)
      h->connect_errno = status;
    SET_NEXT_STATE (STATE_CONNECT_TCP_NEXT_ADDRESS);
  }
  return 0;

}

#line 1356 "lib/states.c"
int
nbd_internal_enter_STATE_CONNECT_TCP_CONNECTING (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_CONNECT_TCP_CONNECTING;
  r = enter_STATE_CONNECT_TCP_CONNECTING (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "CONNECT_TCP.CONNECTING",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* CONNECT_TCP.NEXT_ADDRESS: Connecting to the next address over a TCP socket */
static int
enter_STATE_CONNECT_TCP_NEXT_ADDRESS (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 233 "generator/states-connect.c"

  if (h->sock) {
    h->sock->ops->close (h->sock);
    h->sock = NULL;
  }
  if (h->rp)
    h->rp = h->rp->ai_next;
  SET_NEXT_STATE (STATE_CONNECT_TCP_CONNECT);
  return 0;

}

#line 1399 "lib/states.c"
int
nbd_internal_enter_STATE_CONNECT_TCP_NEXT_ADDRESS (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_CONNECT_TCP_NEXT_ADDRESS;
  r = enter_STATE_CONNECT_TCP_NEXT_ADDRESS (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "CONNECT_TCP.NEXT_ADDRESS",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* CONNECT_COMMAND.START: Connect to a subprocess */
static int
enter_STATE_CONNECT_COMMAND_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 243 "generator/states-connect.c"

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

  next = STATE_DEAD;
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
  next = STATE_MAGIC_START;

  /* fall through, for releasing the temporaries */

uninit_execvpe:
  nbd_internal_execvpe_uninit (&execvpe_ctx);

close_high_level_socket:
  if (next == STATE_DEAD)
    sock->ops->close (sock);

close_socket_pair:
  assert (next == STATE_DEAD || parentfd_transferred);
  if (!parentfd_transferred)
    close (sv[0]);
  close (sv[1]);

done:
  SET_NEXT_STATE (next);
  return 0;

}

#line 1549 "lib/states.c"
int
nbd_internal_enter_STATE_CONNECT_COMMAND_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_CONNECT_COMMAND_START;
  r = enter_STATE_CONNECT_COMMAND_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "CONNECT_COMMAND.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* CONNECT_SA.START: Connect to a subprocess with systemd socket activation */
static int
enter_STATE_CONNECT_SA_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 180 "generator/states-connect-socket-activation.c"

  enum state next;
  char *tmpdir;
  char *sockpath;
  int s;
  struct sockaddr_un addr;
  struct execvpe execvpe_ctx;
  size_t num_vars;
  struct sact_var sact_var[3];
  size_t pid_ofs;
  string_vector env;
  pid_t pid;

  assert (!h->sock);
  assert (h->argv.ptr);
  assert (h->argv.ptr[0]);

  next = STATE_DEAD;

  /* Use /tmp instead of TMPDIR because we must ensure the path is
   * short enough to store in the sockaddr_un.  On some platforms this
   * may cause problems so we may need to revisit it.  XXX
   */
  tmpdir = strdup ("/tmp/libnbdXXXXXX");
  if (tmpdir == NULL) {
    set_error (errno, "strdup");
    goto done;
  }

  if (mkdtemp (tmpdir) == NULL) {
    set_error (errno, "mkdtemp");
    goto free_tmpdir;
  }

  if (asprintf (&sockpath, "%s/sock", tmpdir) == -1) {
    set_error (errno, "asprintf");
    goto rmdir_tmpdir;
  }

  s = nbd_internal_socket (AF_UNIX, SOCK_STREAM, 0, false);
  if (s == -1) {
    set_error (errno, "socket");
    goto free_sockpath;
  }

  addr.sun_family = AF_UNIX;
  memcpy (addr.sun_path, sockpath, strlen (sockpath) + 1);
  if (bind (s, (struct sockaddr *)&addr, sizeof addr) == -1) {
    set_error (errno, "bind: %s", sockpath);
    goto close_socket;
  }

  if (listen (s, SOMAXCONN) == -1) {
    set_error (errno, "listen");
    goto unlink_sockpath;
  }

  if (nbd_internal_execvpe_init (&execvpe_ctx, h->argv.ptr[0], h->argv.len) ==
      -1) {
    set_error (errno, "nbd_internal_execvpe_init");
    goto unlink_sockpath;
  }

  num_vars = 0;
  SACT_VAR_PUSH (sact_var, &num_vars,
                 "LISTEN_PID=", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", &pid_ofs);
  SACT_VAR_PUSH (sact_var, &num_vars,
                 "LISTEN_FDS=", "1", NULL);
  /* Push LISTEN_FDNAMES unconditionally. This ensures we overwrite any
   * inherited LISTEN_FDNAMES. If "h->sact_name" is NULL, then push
   * "LISTEN_FDNAMES=unknown"; it will have the same effect on the child process
   * as unsetting LISTEN_FDNAMES would (with LISTEN_FDS being set to 1):
   * <https://www.freedesktop.org/software/systemd/man/sd_listen_fds.html>.
   */
  SACT_VAR_PUSH (sact_var, &num_vars,
                 "LISTEN_FDNAMES=",
                 h->sact_name == NULL ? "unknown" : h->sact_name,
                 NULL);
  if (prepare_socket_activation_environment (&env, sact_var, num_vars) == -1)
    /* prepare_socket_activation_environment() calls set_error() internally */
    goto uninit_execvpe;

  pid = fork ();
  if (pid == -1) {
    set_error (errno, "fork");
    goto empty_env;
  }

  if (pid == 0) {         /* child - run command */
    if (s != FIRST_SOCKET_ACTIVATION_FD) {
      if (dup2 (s, FIRST_SOCKET_ACTIVATION_FD) == -1) {
        nbd_internal_fork_safe_perror ("dup2");
        _exit (126);
      }
      if (close (s) == -1) {
        nbd_internal_fork_safe_perror ("close");
        _exit (126);
      }
    }
    else {
      /* We must unset CLOEXEC on the fd.  (dup2 above does this
       * implicitly because CLOEXEC is set on the fd, not on the
       * socket).
       */
      int flags = fcntl (s, F_GETFD, 0);
      if (flags == -1) {
        nbd_internal_fork_safe_perror ("fcntl: F_GETFD");
        _exit (126);
      }
      if (fcntl (s, F_SETFD, (int)(flags & ~(unsigned)FD_CLOEXEC)) == -1) {
        nbd_internal_fork_safe_perror ("fcntl: F_SETFD");
        _exit (126);
      }
    }

    char buf[32];
    const char *v = nbd_internal_fork_safe_itoa (getpid (), buf, sizeof buf);
    NBD_INTERNAL_FORK_SAFE_ASSERT (strlen (v) <= sact_var[pid_ofs].value_len);
    strcpy (env.ptr[pid_ofs] + sact_var[pid_ofs].prefix_len, v);

    /* Restore SIGPIPE back to SIG_DFL. */
    if (signal (SIGPIPE, SIG_DFL) == SIG_ERR) {
      nbd_internal_fork_safe_perror ("signal");
      _exit (126);
    }

    (void)nbd_internal_fork_safe_execvpe (&execvpe_ctx, &h->argv, env.ptr);
    nbd_internal_fork_safe_perror (h->argv.ptr[0]);
    if (errno == ENOENT)
      _exit (127);
    else
      _exit (126);
  }

  /* Parent -- we're done; commit. */
  h->sact_tmpdir = tmpdir;
  h->sact_sockpath = sockpath;
  h->pid = pid;

  h->connaddrlen = sizeof addr;
  memcpy (&h->connaddr, &addr, h->connaddrlen);
  next = STATE_CONNECT_START;

  /* fall through, for releasing the temporaries */

empty_env:
  string_vector_empty (&env);

uninit_execvpe:
  nbd_internal_execvpe_uninit (&execvpe_ctx);

unlink_sockpath:
  if (next == STATE_DEAD)
    unlink (sockpath);

close_socket:
  close (s);

free_sockpath:
  if (next == STATE_DEAD)
    free (sockpath);

rmdir_tmpdir:
  if (next == STATE_DEAD)
    rmdir (tmpdir);

free_tmpdir:
  if (next == STATE_DEAD)
    free (tmpdir);

done:
  SET_NEXT_STATE (next);
  return 0;
}

#line 1755 "lib/states.c"
int
nbd_internal_enter_STATE_CONNECT_SA_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_CONNECT_SA_START;
  r = enter_STATE_CONNECT_SA_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "CONNECT_SA.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* MAGIC.START: Prepare to receive the magic identification from remote */
static int
enter_STATE_MAGIC_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 22 "generator/states-magic.c"

  h->rbuf = &h->sbuf;
  h->rlen = 16;
  SET_NEXT_STATE (STATE_MAGIC_RECV_MAGIC);
  return 0;

}

#line 1794 "lib/states.c"
int
nbd_internal_enter_STATE_MAGIC_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_MAGIC_START;
  r = enter_STATE_MAGIC_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "MAGIC.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* MAGIC.RECV_MAGIC: Receive initial magic identification from remote */
static int
enter_STATE_MAGIC_RECV_MAGIC (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 28 "generator/states-magic.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:  SET_NEXT_STATE (STATE_MAGIC_CHECK_MAGIC);
  }
  return 0;

}

#line 1834 "lib/states.c"
int
nbd_internal_enter_STATE_MAGIC_RECV_MAGIC (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_MAGIC_RECV_MAGIC;
  r = enter_STATE_MAGIC_RECV_MAGIC (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "MAGIC.RECV_MAGIC",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* MAGIC.CHECK_MAGIC: Check magic and version sent by remote */
static int
enter_STATE_MAGIC_CHECK_MAGIC (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 35 "generator/states-magic.c"

  uint64_t version;

  if (be64toh (h->sbuf.new_handshake.nbdmagic) != NBD_MAGIC) {
    SET_NEXT_STATE (STATE_DEAD);
    set_error (0, "handshake: server did not send expected NBD magic");
    return 0;
  }

  version = be64toh (h->sbuf.new_handshake.version);
  if (version == NBD_NEW_VERSION) {
    assert (h->opt_current == 0);
    h->chunks_received++;
    SET_NEXT_STATE (STATE_NEWSTYLE_START);
  }
  else if (version == NBD_OLD_VERSION) {
    h->chunks_received++;
    SET_NEXT_STATE (STATE_OLDSTYLE_START);
  }
  else {
    SET_NEXT_STATE (STATE_DEAD);
    set_error (0, "handshake: server is not either an oldstyle or "
               "fixed newstyle NBD server");
    return 0;
  }
  return 0;

}

#line 1894 "lib/states.c"
int
nbd_internal_enter_STATE_MAGIC_CHECK_MAGIC (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_MAGIC_CHECK_MAGIC;
  r = enter_STATE_MAGIC_CHECK_MAGIC (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "MAGIC.CHECK_MAGIC",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* OLDSTYLE.START: Prepare to receive remainder of oldstyle header */
static int
enter_STATE_OLDSTYLE_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 22 "generator/states-oldstyle.c"

  /* We've already read the first 16 bytes of the handshake, we must
   * now read the remainder.
   */
  h->rbuf = &h->sbuf.old_handshake;
  h->rlen = sizeof h->sbuf.old_handshake;
  h->rbuf = (char *)h->rbuf + 16;
  h->rlen -= 16;
  SET_NEXT_STATE (STATE_OLDSTYLE_RECV_REMAINING);
  return 0;

}

#line 1938 "lib/states.c"
int
nbd_internal_enter_STATE_OLDSTYLE_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_OLDSTYLE_START;
  r = enter_STATE_OLDSTYLE_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "OLDSTYLE.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* OLDSTYLE.RECV_REMAINING: Receive remainder of oldstyle header */
static int
enter_STATE_OLDSTYLE_RECV_REMAINING (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 33 "generator/states-oldstyle.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:  SET_NEXT_STATE (STATE_OLDSTYLE_CHECK);
  }
  return 0;

}

#line 1978 "lib/states.c"
int
nbd_internal_enter_STATE_OLDSTYLE_RECV_REMAINING (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_OLDSTYLE_RECV_REMAINING;
  r = enter_STATE_OLDSTYLE_RECV_REMAINING (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "OLDSTYLE.RECV_REMAINING",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* OLDSTYLE.CHECK: Check oldstyle header */
static int
enter_STATE_OLDSTYLE_CHECK (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 40 "generator/states-oldstyle.c"

  uint64_t exportsize;
  uint16_t gflags, eflags;

  /* We already checked the magic and version in MAGIC.CHECK_MAGIC. */
  exportsize = be64toh (h->sbuf.old_handshake.exportsize);
  gflags = be16toh (h->sbuf.old_handshake.gflags);
  eflags = be16toh (h->sbuf.old_handshake.eflags);

  /* Server is unable to upgrade to TLS.  If h->tls is not 'require' (2)
   * then we can continue unencrypted.
   */
  if (h->tls == LIBNBD_TLS_REQUIRE) {
    SET_NEXT_STATE (STATE_DEAD);
    set_error (ENOTSUP, "handshake: server is oldstyle, "
               "but handle TLS setting is 'require' (2)");
    return 0;
  }

  h->gflags = gflags;
  debug (h, "gflags: 0x%" PRIx16, gflags);
  if (gflags) {
    set_error (0, "handshake: oldstyle server should not set gflags");
    SET_NEXT_STATE (STATE_DEAD);
    return 0;
  }

  if (nbd_internal_set_size_and_flags (h, exportsize, eflags) == -1) {
    SET_NEXT_STATE (STATE_DEAD);
    return 0;
  }
  nbd_internal_set_payload (h);

  h->protocol = "oldstyle";

  SET_NEXT_STATE (STATE_READY);

  return 0;

}

#line 2050 "lib/states.c"
int
nbd_internal_enter_STATE_OLDSTYLE_CHECK (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_OLDSTYLE_CHECK;
  r = enter_STATE_OLDSTYLE_CHECK (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "OLDSTYLE.CHECK",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.START: Prepare to receive newstyle gflags from remote */
static int
enter_STATE_NEWSTYLE_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 184 "generator/states-newstyle.c"

  if (h->opt_mode) {
    /* NEWSTYLE can be entered multiple times, from MAGIC.CHECK_MAGIC
     * (h->opt_current is 0, run through OPT_STRUCTURED_REPLY for
     * opt_mode, or OPT_GO otherwise) and during various nbd_opt_*
     * calls during NEGOTIATING (h->opt_current is set, run just the
     * states needed).  Each previous state has informed us what we
     * still need to do.
     */
    switch (h->opt_current) {
    case NBD_OPT_GO:
    case NBD_OPT_INFO:
      if ((h->gflags & LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE) == 0)
        SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXPORT_NAME_START);
      else
        SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_START);
      return 0;
    case NBD_OPT_LIST:
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_LIST_START);
      return 0;
    case NBD_OPT_ABORT:
      if ((h->gflags & LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE) == 0) {
        SET_NEXT_STATE (STATE_DEAD);
        set_error (ENOTSUP, "handshake: server is not using fixed newstyle");
        return 0;
      }
      SET_NEXT_STATE (STATE_NEWSTYLE_PREPARE_OPT_ABORT);
      return 0;
    case NBD_OPT_LIST_META_CONTEXT:
    case NBD_OPT_SET_META_CONTEXT:
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_START);
      return 0;
    case NBD_OPT_STRUCTURED_REPLY:
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START);
      return 0;
    case NBD_OPT_EXTENDED_HEADERS:
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START);
      return 0;
    case NBD_OPT_STARTTLS:
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STARTTLS_START);
      return 0;
    case 0:
      break;
    default:
      abort ();
    }
  }

  assert (h->opt_current == 0);
  h->rbuf = &h->sbuf;
  h->rlen = sizeof h->sbuf.gflags;
  SET_NEXT_STATE (STATE_NEWSTYLE_RECV_GFLAGS);
  return 0;

}

#line 2137 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_START;
  r = enter_STATE_NEWSTYLE_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.RECV_GFLAGS: Receive newstyle gflags from remote */
static int
enter_STATE_NEWSTYLE_RECV_GFLAGS (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 238 "generator/states-newstyle.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:  SET_NEXT_STATE (STATE_NEWSTYLE_CHECK_GFLAGS);
  }
  return 0;

}

#line 2177 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_RECV_GFLAGS (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_RECV_GFLAGS;
  r = enter_STATE_NEWSTYLE_RECV_GFLAGS (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.RECV_GFLAGS",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.CHECK_GFLAGS: Check global flags sent by remote */
static int
enter_STATE_NEWSTYLE_CHECK_GFLAGS (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 245 "generator/states-newstyle.c"

  uint32_t cflags;

  h->gflags &= be16toh (h->sbuf.gflags);
  if ((h->gflags & LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE) == 0 &&
      h->tls == LIBNBD_TLS_REQUIRE) {
    SET_NEXT_STATE (STATE_DEAD);
    set_error (ENOTSUP, "handshake: server is not using fixed newstyle, "
               "but handle TLS setting is 'require' (2)");
    return 0;
  }

  if ((h->gflags & LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE) == 0)
    h->protocol = "newstyle";
  else
    h->protocol = "newstyle-fixed";

  cflags = h->gflags;
  h->sbuf.cflags = htobe32 (cflags);
  h->wbuf = &h->sbuf;
  h->wlen = 4;
  SET_NEXT_STATE (STATE_NEWSTYLE_SEND_CFLAGS);
  return 0;

}

#line 2234 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_CHECK_GFLAGS (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_CHECK_GFLAGS;
  r = enter_STATE_NEWSTYLE_CHECK_GFLAGS (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.CHECK_GFLAGS",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.SEND_CFLAGS: Send newstyle client flags to remote */
static int
enter_STATE_NEWSTYLE_SEND_CFLAGS (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 269 "generator/states-newstyle.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    /* Start sending options. */
    if ((h->gflags & LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE) == 0) {
      if (h->opt_mode)
        SET_NEXT_STATE (STATE_NEGOTIATING);
      else
        SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXPORT_NAME_START);
    }
    else
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STARTTLS_START);
  }
  return 0;

}

#line 2283 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_SEND_CFLAGS (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_SEND_CFLAGS;
  r = enter_STATE_NEWSTYLE_SEND_CFLAGS (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.SEND_CFLAGS",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_STARTTLS.START: Try to send newstyle NBD_OPT_STARTTLS to upgrade
 * to TLS
 */
static int
enter_STATE_NEWSTYLE_OPT_STARTTLS_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 22 "generator/states-newstyle-opt-starttls.c"

  assert (h->gflags & LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE);
  if (h->opt_current == NBD_OPT_STARTTLS)
    assert (h->opt_mode);
  else {
    /* If TLS was not requested we skip this option and go to the next one. */
    if (h->tls == LIBNBD_TLS_DISABLE) {
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START);
      return 0;
    }
    assert (CALLBACK_IS_NULL (h->opt_cb.completion));
  }

  h->sbuf.option.version = htobe64 (NBD_NEW_VERSION);
  h->sbuf.option.option = htobe32 (NBD_OPT_STARTTLS);
  h->sbuf.option.optlen = 0;
  h->chunks_sent++;
  h->wbuf = &h->sbuf;
  h->wlen = sizeof (h->sbuf.option);
  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STARTTLS_SEND);
  return 0;

}

#line 2340 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_STARTTLS_START;
  r = enter_STATE_NEWSTYLE_OPT_STARTTLS_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_STARTTLS.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_STARTTLS.SEND: Send newstyle NBD_OPT_STARTTLS to upgrade to TLS
 */
static int
enter_STATE_NEWSTYLE_OPT_STARTTLS_SEND (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 44 "generator/states-newstyle-opt-starttls.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->rbuf = &h->sbuf;
    h->rlen = sizeof (h->sbuf.or.option_reply);
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY);
  }
  return 0;

}

#line 2384 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_SEND (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_STARTTLS_SEND;
  r = enter_STATE_NEWSTYLE_OPT_STARTTLS_SEND (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_STARTTLS.SEND",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_STARTTLS.RECV_REPLY: Receive newstyle NBD_OPT_STARTTLS reply */
static int
enter_STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 54 "generator/states-newstyle-opt-starttls.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    if (prepare_for_reply_payload (h, NBD_OPT_STARTTLS) == -1) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY_PAYLOAD);
  }
  return 0;

}

#line 2429 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_STARTTLS.RECV_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_STARTTLS.RECV_REPLY_PAYLOAD: Receive any newstyle
 * NBD_OPT_STARTTLS reply payload
 */
static int
enter_STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY_PAYLOAD (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 66 "generator/states-newstyle-opt-starttls.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STARTTLS_CHECK_REPLY);
  }
  return 0;

}

#line 2471 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY_PAYLOAD (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY_PAYLOAD;
  r = enter_STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY_PAYLOAD (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_STARTTLS.RECV_REPLY_PAYLOAD",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_STARTTLS.CHECK_REPLY: Check newstyle NBD_OPT_STARTTLS reply */
static int
enter_STATE_NEWSTYLE_OPT_STARTTLS_CHECK_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 73 "generator/states-newstyle-opt-starttls.c"

  uint32_t reply;
  struct socket *new_sock;
  int err = ENOTSUP;

  reply = be32toh (h->sbuf.or.option_reply.reply);
  switch (reply) {
  case NBD_REP_ACK:
    if (h->tls_negotiated) {
      set_error (EPROTO,
                 "handshake: unable to support server accepting TLS twice");
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }
    nbd_internal_reset_size_and_flags (h);
    h->structured_replies = false;
    h->extended_headers = false;
    h->meta_valid = false;
    new_sock = nbd_internal_crypto_create_session (h, h->sock);
    if (new_sock == NULL) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }
    h->sock = new_sock;
    if (nbd_internal_crypto_is_reading (h))
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ);
    else
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE);
    return 0;

  case NBD_REP_ERR_INVALID:
    err = EINVAL;
    /* fallthrough */
  default:
    if (handle_reply_error (h) == -1) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }

    /* Server refused to upgrade to TLS.  If h->tls is not 'require' (2)
     * then we can continue unencrypted.
     */
    if (h->tls == LIBNBD_TLS_REQUIRE) {
      SET_NEXT_STATE (STATE_NEWSTYLE_PREPARE_OPT_ABORT);
      set_error (ENOTSUP, "handshake: server refused TLS, "
                 "but handle TLS setting is 'require' (2)");
      return 0;
    }

    debug (h, "server refused TLS (%s)",
           reply == NBD_REP_ERR_POLICY ? "policy" :
           reply == NBD_REP_ERR_INVALID ? "invalid request" : "not supported");
    CALL_CALLBACK (h->opt_cb.completion, &err);
    nbd_internal_free_option (h);
    if (h->opt_current == NBD_OPT_STARTTLS)
      SET_NEXT_STATE (STATE_NEGOTIATING);
    else {
      debug (h, "continuing with unencrypted connection");
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START);
    }
    return 0;
  }
  return 0;

}

#line 2568 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_CHECK_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_STARTTLS_CHECK_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_STARTTLS_CHECK_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_STARTTLS.CHECK_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_READ: TLS handshake (reading) */
static int
enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 137 "generator/states-newstyle-opt-starttls.c"

  int r;

  r = nbd_internal_crypto_handshake (h);
  if (r == -1) {
    SET_NEXT_STATE (STATE_DEAD);
    return 0;
  }
  if (r == 0) {
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE);
    return 0;
  }
  /* Continue handshake. */
  if (nbd_internal_crypto_is_reading (h))
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ);
  else
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE);
  return 0;

}

#line 2620 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ;
  r = enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_READ",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_WRITE: TLS handshake (writing) */
static int
enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 156 "generator/states-newstyle-opt-starttls.c"

  int r;

  r = nbd_internal_crypto_handshake (h);
  if (r == -1) {
    SET_NEXT_STATE (STATE_DEAD);
    return 0;
  }
  if (r == 0) {
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE);
    return 0;
  }
  /* Continue handshake. */
  if (nbd_internal_crypto_is_reading (h))
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ);
  else
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE);
  return 0;

}

#line 2672 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE;
  r = enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_WRITE",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_DONE: TLS handshake complete */
static int
enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 175 "generator/states-newstyle-opt-starttls.c"

  int err = 0;

  /* Finished handshake. */
  h->tls_negotiated = true;
  nbd_internal_crypto_debug_tls_enabled (h);
  CALL_CALLBACK (h->opt_cb.completion, &err);
  nbd_internal_free_option (h);

  /* Continue with option negotiation. */
  if (h->opt_current == NBD_OPT_STARTTLS)
    SET_NEXT_STATE (STATE_NEGOTIATING);
  else
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START);
  return 0;

}

#line 2721 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE;
  r = enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_DONE",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_EXTENDED_HEADERS.START: Try to negotiate newstyle
 * NBD_OPT_EXTENDED_HEADERS
 */
static int
enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 22 "generator/states-newstyle-opt-extended-headers.c"

  assert (h->gflags & LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE);
  if (h->opt_current == NBD_OPT_EXTENDED_HEADERS)
    assert (h->opt_mode);
  else {
    assert (CALLBACK_IS_NULL (h->opt_cb.completion));
    if (!h->request_eh || !h->request_sr) {
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START);
      return 0;
    }
  }

  h->sbuf.option.version = htobe64 (NBD_NEW_VERSION);
  h->sbuf.option.option = htobe32 (NBD_OPT_EXTENDED_HEADERS);
  h->sbuf.option.optlen = htobe32 (0);
  h->chunks_sent++;
  h->wbuf = &h->sbuf;
  h->wlen = sizeof h->sbuf.option;
  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_SEND);
  return 0;

}

#line 2777 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START;
  r = enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_EXTENDED_HEADERS.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_EXTENDED_HEADERS.SEND: Send newstyle NBD_OPT_EXTENDED_HEADERS
 * negotiation request
 */
static int
enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_SEND (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 43 "generator/states-newstyle-opt-extended-headers.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->rbuf = &h->sbuf;
    h->rlen = sizeof h->sbuf.or.option_reply;
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY);
  }
  return 0;

}

#line 2822 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_SEND (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_SEND;
  r = enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_SEND (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_EXTENDED_HEADERS.SEND",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_EXTENDED_HEADERS.RECV_REPLY: Receive newstyle
 * NBD_OPT_EXTENDED_HEADERS option reply
 */
static int
enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 53 "generator/states-newstyle-opt-extended-headers.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    if (prepare_for_reply_payload (h, NBD_OPT_EXTENDED_HEADERS) == -1) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY_PAYLOAD);
  }
  return 0;

}

#line 2869 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_EXTENDED_HEADERS.RECV_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_EXTENDED_HEADERS.RECV_REPLY_PAYLOAD: Receive any newstyle
 * NBD_OPT_EXTENDED_HEADERS reply payload
 */
static int
enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY_PAYLOAD (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 65 "generator/states-newstyle-opt-extended-headers.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_CHECK_REPLY);
  }
  return 0;

}

#line 2911 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY_PAYLOAD (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY_PAYLOAD;
  r = enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY_PAYLOAD (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_EXTENDED_HEADERS.RECV_REPLY_PAYLOAD",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_EXTENDED_HEADERS.CHECK_REPLY: Check newstyle
 * NBD_OPT_EXTENDED_HEADERS option reply
 */
static int
enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_CHECK_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 72 "generator/states-newstyle-opt-extended-headers.c"

  uint32_t reply;
  int err = ENOTSUP;

  reply = be32toh (h->sbuf.or.option_reply.reply);
  switch (reply) {
  case NBD_REP_ACK:
    debug (h, "negotiated extended headers on this connection");
    h->extended_headers = true;
    /* Extended headers trump structured replies, so skip ahead. */
    h->structured_replies = true;
    err = 0;
    break;
  case NBD_REP_ERR_INVALID:
    err = EINVAL;
    /* fallthrough */
  default:
    if (handle_reply_error (h) == -1) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }

    if (h->extended_headers)
      debug (h, "extended headers already negotiated");
    else
      debug (h, "extended headers are not supported by this server");
    break;
  }

  /* Next option. */
  if (h->opt_current == NBD_OPT_EXTENDED_HEADERS)
    SET_NEXT_STATE (STATE_NEGOTIATING);
  else
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START);
  CALL_CALLBACK (h->opt_cb.completion, &err);
  nbd_internal_free_option (h);
  return 0;

}

#line 2984 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_CHECK_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_CHECK_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_CHECK_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_EXTENDED_HEADERS.CHECK_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_STRUCTURED_REPLY.START: Try to negotiate newstyle
 * NBD_OPT_STRUCTURED_REPLY
 */
static int
enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 22 "generator/states-newstyle-opt-structured-reply.c"

  assert (h->gflags & LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE);
  if (h->opt_current == NBD_OPT_STRUCTURED_REPLY)
    assert (h->opt_mode);
  else {
    assert (CALLBACK_IS_NULL (h->opt_cb.completion));
    if (!h->request_sr || h->structured_replies) {
      if (h->opt_mode)
        SET_NEXT_STATE (STATE_NEGOTIATING);
      else
        SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_START);
      return 0;
    }
  }

  h->sbuf.option.version = htobe64 (NBD_NEW_VERSION);
  h->sbuf.option.option = htobe32 (NBD_OPT_STRUCTURED_REPLY);
  h->sbuf.option.optlen = htobe32 (0);
  h->chunks_sent++;
  h->wbuf = &h->sbuf;
  h->wlen = sizeof h->sbuf.option;
  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_SEND);
  return 0;

}

#line 3043 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START;
  r = enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_STRUCTURED_REPLY.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_STRUCTURED_REPLY.SEND: Send newstyle NBD_OPT_STRUCTURED_REPLY
 * negotiation request
 */
static int
enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_SEND (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 46 "generator/states-newstyle-opt-structured-reply.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->rbuf = &h->sbuf;
    h->rlen = sizeof h->sbuf.or.option_reply;
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY);
  }
  return 0;

}

#line 3088 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_SEND (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_SEND;
  r = enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_SEND (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_STRUCTURED_REPLY.SEND",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_STRUCTURED_REPLY.RECV_REPLY: Receive newstyle
 * NBD_OPT_STRUCTURED_REPLY option reply
 */
static int
enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 56 "generator/states-newstyle-opt-structured-reply.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    if (prepare_for_reply_payload (h, NBD_OPT_STRUCTURED_REPLY) == -1) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY_PAYLOAD);
  }
  return 0;

}

#line 3135 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_STRUCTURED_REPLY.RECV_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_STRUCTURED_REPLY.RECV_REPLY_PAYLOAD: Receive any newstyle
 * NBD_OPT_STRUCTURED_REPLY reply payload
 */
static int
enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY_PAYLOAD (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 68 "generator/states-newstyle-opt-structured-reply.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_CHECK_REPLY);
  }
  return 0;

}

#line 3177 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY_PAYLOAD (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY_PAYLOAD;
  r = enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY_PAYLOAD (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_STRUCTURED_REPLY.RECV_REPLY_PAYLOAD",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_STRUCTURED_REPLY.CHECK_REPLY: Check newstyle
 * NBD_OPT_STRUCTURED_REPLY option reply
 */
static int
enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_CHECK_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 75 "generator/states-newstyle-opt-structured-reply.c"

  uint32_t reply;
  int err = ENOTSUP;

  reply = be32toh (h->sbuf.or.option_reply.reply);
  switch (reply) {
  case NBD_REP_ACK:
    debug (h, "negotiated structured replies on this connection");
    h->structured_replies = true;
    err = 0;
    break;
  case NBD_REP_ERR_INVALID:
  case NBD_REP_ERR_EXT_HEADER_REQD:
    err = EINVAL;
    /* fallthrough */
  default:
    if (handle_reply_error (h) == -1) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }

    if (h->structured_replies)
      debug (h, "structured replies already negotiated");
    else
      debug (h, "structured replies are not supported by this server");
    break;
  }

  /* Next option. */
  if (h->opt_mode)
    SET_NEXT_STATE (STATE_NEGOTIATING);
  else
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_START);
  CALL_CALLBACK (h->opt_cb.completion, &err);
  nbd_internal_free_option (h);
  return 0;

}

#line 3249 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_CHECK_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_CHECK_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_CHECK_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_STRUCTURED_REPLY.CHECK_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_META_CONTEXT.START: Try to negotiate newstyle
 * NBD_OPT_SET_META_CONTEXT
 */
static int
enter_STATE_NEWSTYLE_OPT_META_CONTEXT_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 22 "generator/states-newstyle-opt-meta-context.c"

  size_t i;
  uint32_t len, opt;

  /* This state group is reached from:
   * h->opt_mode == false (h->opt_current == 0):
   *   nbd_connect_*()
   *     -> conditionally use SET, next state OPT_GO for NBD_OPT_GO
   * h->opt_mode == true (h->opt_current matches calling API):
   *   nbd_opt_info()
   *     -> conditionally use SET, next state OPT_GO for NBD_OPT_INFO
   *   nbd_opt_go()
   *     -> conditionally use SET, next state OPT_GO for NBD_OPT_GO
   *   nbd_opt_list_meta_context()
   *     -> unconditionally use LIST, next state NEGOTIATING
   *   nbd_opt_set_meta_context()
   *     -> unconditionally use SET, next state NEGOTIATING
   *
   * If SET is conditional, we skip it if h->request_meta is false, if
   * structured replies were not negotiated, or if no contexts to request.
   * SET then manipulates h->meta_contexts, and sets h->meta_valid on
   * success, while LIST is stateless.
   * If OPT_GO is later successful, it populates h->exportsize and friends,
   * and also sets h->meta_valid if h->request_meta but we skipped SET here.
   * There is a callback if and only if the command is unconditional.
   */
  assert (h->gflags & LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE);
  if (h->opt_current == NBD_OPT_LIST_META_CONTEXT) {
    assert (h->opt_mode);
    assert (CALLBACK_IS_NOT_NULL (h->opt_cb.fn.context));
    opt = h->opt_current;
  }
  else {
    if (h->opt_current == NBD_OPT_SET_META_CONTEXT)
      assert (CALLBACK_IS_NOT_NULL (h->opt_cb.fn.context));
    else
      assert (CALLBACK_IS_NULL (h->opt_cb.fn.context));
    opt = NBD_OPT_SET_META_CONTEXT;
    if (h->request_meta || h->opt_current == opt) {
      for (i = 0; i < h->meta_contexts.len; ++i)
        free (h->meta_contexts.ptr[i].name);
      meta_vector_reset (&h->meta_contexts);
      h->meta_valid = false;
    }
  }
  if (opt != h->opt_current) {
    if (!h->request_meta || !h->structured_replies ||
        h->request_meta_contexts.len == 0) {
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_GO_START);
      return 0;
    }
    if (nbd_internal_set_querylist (h, NULL) == -1) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }
  }

  /* Calculate the length of the option request data. */
  len = 4 /* exportname len */ + strlen (h->export_name) + 4 /* nr queries */;
  for (i = 0; i < h->querylist.len; ++i)
    len += 4 /* length of query */ + strlen (h->querylist.ptr[i]);

  h->sbuf.option.version = htobe64 (NBD_NEW_VERSION);
  h->sbuf.option.option = htobe32 (opt);
  h->sbuf.option.optlen = htobe32 (len);
  h->chunks_sent++;
  h->wbuf = &h->sbuf;
  h->wlen = sizeof (h->sbuf.option);
  h->wflags = MSG_MORE;
  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_SEND);
  return 0;

}

#line 3356 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_META_CONTEXT_START;
  r = enter_STATE_NEWSTYLE_OPT_META_CONTEXT_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_META_CONTEXT.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_META_CONTEXT.SEND: Send newstyle NBD_OPT_SET_META_CONTEXT */
static int
enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 94 "generator/states-newstyle-opt-meta-context.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->sbuf.len = htobe32 (strlen (h->export_name));
    h->wbuf = &h->sbuf.len;
    h->wlen = sizeof h->sbuf.len;
    h->wflags = MSG_MORE;
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAMELEN);
  }
  return 0;

}

#line 3401 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_META_CONTEXT_SEND;
  r = enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_META_CONTEXT.SEND",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_META_CONTEXT.SEND_EXPORTNAMELEN: Send newstyle
 * NBD_OPT_SET_META_CONTEXT export name length
 */
static int
enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAMELEN (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 106 "generator/states-newstyle-opt-meta-context.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->wbuf = h->export_name;
    h->wlen = strlen (h->export_name);
    h->wflags = MSG_MORE;
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAME);
  }
  return 0;

}

#line 3447 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAMELEN (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAMELEN;
  r = enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAMELEN (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_META_CONTEXT.SEND_EXPORTNAMELEN",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_META_CONTEXT.SEND_EXPORTNAME: Send newstyle
 * NBD_OPT_SET_META_CONTEXT export name
 */
static int
enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAME (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 117 "generator/states-newstyle-opt-meta-context.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->sbuf.nrqueries = htobe32 (h->querylist.len);
    h->wbuf = &h->sbuf;
    h->wlen = sizeof h->sbuf.nrqueries;
    h->wflags = MSG_MORE;
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_NRQUERIES);
  }
  return 0;

}

#line 3494 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAME (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAME;
  r = enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAME (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_META_CONTEXT.SEND_EXPORTNAME",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_META_CONTEXT.SEND_NRQUERIES: Send newstyle
 * NBD_OPT_SET_META_CONTEXT number of queries
 */
static int
enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_NRQUERIES (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 129 "generator/states-newstyle-opt-meta-context.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->querynum = 0;
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY);
  }
  return 0;

}

#line 3538 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_NRQUERIES (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_NRQUERIES;
  r = enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_NRQUERIES (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_META_CONTEXT.SEND_NRQUERIES",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_META_CONTEXT.PREPARE_NEXT_QUERY: Prepare to send newstyle
 * NBD_OPT_SET_META_CONTEXT query
 */
static int
enter_STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 138 "generator/states-newstyle-opt-meta-context.c"

  if (h->querynum >= h->querylist.len) {
    /* end of list of requested meta contexts */
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY);
    return 0;
  }
  const char *query = h->querylist.ptr[h->querynum];

  h->sbuf.len = htobe32 (strlen (query));
  h->wbuf = &h->sbuf.len;
  h->wlen = sizeof h->sbuf.len;
  h->wflags = MSG_MORE;
  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERYLEN);
  return 0;

}

#line 3588 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY;
  r = enter_STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_META_CONTEXT.PREPARE_NEXT_QUERY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_META_CONTEXT.SEND_QUERYLEN: Send newstyle
 * NBD_OPT_SET_META_CONTEXT query length
 */
static int
enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERYLEN (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 153 "generator/states-newstyle-opt-meta-context.c"

  const char *query = h->querylist.ptr[h->querynum];

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->wbuf = query;
    h->wlen = strlen (query);
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERY);
  }
  return 0;

}

#line 3635 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERYLEN (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERYLEN;
  r = enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERYLEN (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_META_CONTEXT.SEND_QUERYLEN",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_META_CONTEXT.SEND_QUERY: Send newstyle NBD_OPT_SET_META_CONTEXT
 * query
 */
static int
enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 165 "generator/states-newstyle-opt-meta-context.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->querynum++;
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY);
  }
  return 0;

}

#line 3679 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERY;
  r = enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_META_CONTEXT.SEND_QUERY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_META_CONTEXT.PREPARE_FOR_REPLY: Prepare to receive newstyle
 * NBD_OPT_SET_META_CONTEXT option reply
 */
static int
enter_STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 174 "generator/states-newstyle-opt-meta-context.c"

  h->rbuf = &h->sbuf.or.option_reply;
  h->rlen = sizeof h->sbuf.or.option_reply;
  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY);
  return 0;

}

#line 3720 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_META_CONTEXT.PREPARE_FOR_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_META_CONTEXT.RECV_REPLY: Receive newstyle
 * NBD_OPT_SET_META_CONTEXT option reply
 */
static int
enter_STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 180 "generator/states-newstyle-opt-meta-context.c"

  uint32_t opt;

  if (h->opt_current == NBD_OPT_LIST_META_CONTEXT)
    opt = h->opt_current;
  else
    opt = NBD_OPT_SET_META_CONTEXT;

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    if (prepare_for_reply_payload (h, opt) == -1) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY_PAYLOAD);
  }
  return 0;

}

#line 3774 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_META_CONTEXT.RECV_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_META_CONTEXT.RECV_REPLY_PAYLOAD: Receive newstyle
 * NBD_OPT_SET_META_CONTEXT option reply payload
 */
static int
enter_STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY_PAYLOAD (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 199 "generator/states-newstyle-opt-meta-context.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_CHECK_REPLY);
  }
  return 0;

}

#line 3816 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY_PAYLOAD (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY_PAYLOAD;
  r = enter_STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY_PAYLOAD (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_META_CONTEXT.RECV_REPLY_PAYLOAD",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_META_CONTEXT.CHECK_REPLY: Check newstyle
 * NBD_OPT_SET_META_CONTEXT option reply
 */
static int
enter_STATE_NEWSTYLE_OPT_META_CONTEXT_CHECK_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 206 "generator/states-newstyle-opt-meta-context.c"

  uint32_t reply;
  uint32_t len;
  const size_t maxpayload = sizeof h->sbuf.or.payload.context;
  struct meta_context meta_context;
  uint32_t opt;
  int err = 0;

  if (h->opt_current == NBD_OPT_LIST_META_CONTEXT)
    opt = h->opt_current;
  else
    opt = NBD_OPT_SET_META_CONTEXT;

  reply = be32toh (h->sbuf.or.option_reply.reply);
  len = be32toh (h->sbuf.or.option_reply.replylen);
  switch (reply) {
  case NBD_REP_ACK:           /* End of list of replies. */
    if (opt == NBD_OPT_SET_META_CONTEXT)
      h->meta_valid = true;
    if (opt == h->opt_current) {
      SET_NEXT_STATE (STATE_NEGOTIATING);
      CALL_CALLBACK (h->opt_cb.completion, &err);
      nbd_internal_free_option (h);
    }
    else
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_GO_START);
    break;
  case NBD_REP_META_CONTEXT:  /* A context. */
    if (len > maxpayload)
      debug (h, "skipping too large meta context");
    else {
      assert (len > sizeof h->sbuf.or.payload.context.context.context_id);
      meta_context.context_id =
        be32toh (h->sbuf.or.payload.context.context.context_id);
      /* String payload is not NUL-terminated. */
      meta_context.name = strndup (h->sbuf.or.payload.context.str,
                                   len - sizeof meta_context.context_id);
      if (meta_context.name == NULL) {
        set_error (errno, "strdup");
        SET_NEXT_STATE (STATE_DEAD);
        return 0;
      }
      debug (h, "negotiated %s with context ID %" PRIu32,
             meta_context.name, meta_context.context_id);
      if (CALLBACK_IS_NOT_NULL (h->opt_cb.fn.context))
        CALL_CALLBACK (h->opt_cb.fn.context, meta_context.name);
      if (opt == NBD_OPT_LIST_META_CONTEXT)
        free (meta_context.name);
      else if (meta_vector_append (&h->meta_contexts, meta_context) == -1) {
        set_error (errno, "realloc");
        free (meta_context.name);
        SET_NEXT_STATE (STATE_DEAD);
        return 0;
      }
    }
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY);
    break;
  default:
    /* Anything else is an error, report it for explicit LIST/SET, ignore it
     * for automatic progress (nbd_connect_*, nbd_opt_info, nbd_opt_go).
     */
    if (handle_reply_error (h) == -1) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }

    if (opt == h->opt_current) {
      debug (h, "unexpected response, possibly the server does not "
             "support meta contexts");
      err = ENOTSUP;
      CALL_CALLBACK (h->opt_cb.completion, &err);
      nbd_internal_free_option (h);
      SET_NEXT_STATE (STATE_NEGOTIATING);
    }
    else {
      /* Treat ignored error as success only if list is still empty */
      if (opt == NBD_OPT_SET_META_CONTEXT)
        h->meta_valid = h->meta_contexts.len == 0;
      debug (h, "handshake: ignoring unexpected error from "
             "NBD_OPT_SET_META_CONTEXT (%" PRIu32 ")", reply);
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_GO_START);
    }
    break;
  }
  return 0;

}

#line 3937 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_CHECK_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_META_CONTEXT_CHECK_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_META_CONTEXT_CHECK_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_META_CONTEXT.CHECK_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_GO.START: Try to send newstyle NBD_OPT_GO to end handshake */
static int
enter_STATE_NEWSTYLE_OPT_GO_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 22 "generator/states-newstyle-opt-go.c"

  uint16_t nrinfos = 0;

  nbd_internal_reset_size_and_flags (h);
  if (h->request_block_size)
    nrinfos++;
  if (h->full_info)
    nrinfos += 2;

  assert (h->gflags & LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE);
  if (h->opt_current == NBD_OPT_INFO)
    assert (h->opt_mode);
  else if (!h->opt_current) {
    assert (!h->opt_mode);
    assert (CALLBACK_IS_NULL (h->opt_cb.completion));
    h->opt_current = NBD_OPT_GO;
  }
  h->sbuf.option.version = htobe64 (NBD_NEW_VERSION);
  h->sbuf.option.option = htobe32 (h->opt_current);
  h->sbuf.option.optlen =
    htobe32 (/* exportnamelen */ 4 + strlen (h->export_name)
             + sizeof nrinfos + 2 * nrinfos);
  h->chunks_sent++;
  h->wbuf = &h->sbuf;
  h->wlen = sizeof h->sbuf.option;
  h->wflags = MSG_MORE;
  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_GO_SEND);
  return 0;

}

#line 3999 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_GO_START;
  r = enter_STATE_NEWSTYLE_OPT_GO_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_GO.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_GO.SEND: Send newstyle NBD_OPT_GO to end handshake */
static int
enter_STATE_NEWSTYLE_OPT_GO_SEND (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 51 "generator/states-newstyle-opt-go.c"

  const uint32_t exportnamelen = strlen (h->export_name);

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->sbuf.len = htobe32 (exportnamelen);
    h->wbuf = &h->sbuf;
    h->wlen = 4;
    h->wflags = MSG_MORE;
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_GO_SEND_EXPORTNAMELEN);
  }
  return 0;

}

#line 4046 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_GO_SEND;
  r = enter_STATE_NEWSTYLE_OPT_GO_SEND (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_GO.SEND",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_GO.SEND_EXPORTNAMELEN: Send newstyle NBD_OPT_GO export name
 * length
 */
static int
enter_STATE_NEWSTYLE_OPT_GO_SEND_EXPORTNAMELEN (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 65 "generator/states-newstyle-opt-go.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->wbuf = h->export_name;
    h->wlen = strlen (h->export_name);
    h->wflags = MSG_MORE;
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_GO_SEND_EXPORT);
  }
  return 0;

}

#line 4092 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND_EXPORTNAMELEN (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_GO_SEND_EXPORTNAMELEN;
  r = enter_STATE_NEWSTYLE_OPT_GO_SEND_EXPORTNAMELEN (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_GO.SEND_EXPORTNAMELEN",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_GO.SEND_EXPORT: Send newstyle NBD_OPT_GO export name */
static int
enter_STATE_NEWSTYLE_OPT_GO_SEND_EXPORT (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 76 "generator/states-newstyle-opt-go.c"

  uint16_t nrinfos = 0;

  if (h->request_block_size)
    nrinfos++;
  if (h->full_info)
    nrinfos += 2;

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->sbuf.nrinfos = htobe16 (nrinfos);
    h->wbuf = &h->sbuf;
    h->wlen = sizeof h->sbuf.nrinfos;
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_GO_SEND_NRINFOS);
  }
  return 0;

}

#line 4143 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND_EXPORT (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_GO_SEND_EXPORT;
  r = enter_STATE_NEWSTYLE_OPT_GO_SEND_EXPORT (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_GO.SEND_EXPORT",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_GO.SEND_NRINFOS: Send newstyle NBD_OPT_GO number of infos */
static int
enter_STATE_NEWSTYLE_OPT_GO_SEND_NRINFOS (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 94 "generator/states-newstyle-opt-go.c"

  uint16_t nrinfos = 0;

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    if (h->request_block_size)
      h->sbuf.info[nrinfos++] = htobe16 (NBD_INFO_BLOCK_SIZE);
    if (h->full_info) {
      h->sbuf.info[nrinfos++] = htobe16 (NBD_INFO_NAME);
      h->sbuf.info[nrinfos++] = htobe16 (NBD_INFO_DESCRIPTION);
    }
    h->wbuf = &h->sbuf;
    h->wlen = sizeof h->sbuf.info[0] * nrinfos;
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_GO_SEND_INFO);
  }
  return 0;

}

#line 4194 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND_NRINFOS (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_GO_SEND_NRINFOS;
  r = enter_STATE_NEWSTYLE_OPT_GO_SEND_NRINFOS (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_GO.SEND_NRINFOS",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_GO.SEND_INFO: Send newstyle NBD_OPT_GO request for
 * NBD_INFO_BLOCK_SIZE
 */
static int
enter_STATE_NEWSTYLE_OPT_GO_SEND_INFO (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 112 "generator/states-newstyle-opt-go.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->rbuf = &h->sbuf;
    h->rlen = sizeof h->sbuf.or.option_reply;
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_GO_RECV_REPLY);
  }
  return 0;

}

#line 4239 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND_INFO (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_GO_SEND_INFO;
  r = enter_STATE_NEWSTYLE_OPT_GO_SEND_INFO (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_GO.SEND_INFO",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_GO.RECV_REPLY: Receive newstyle NBD_OPT_GO reply */
static int
enter_STATE_NEWSTYLE_OPT_GO_RECV_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 122 "generator/states-newstyle-opt-go.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    if (prepare_for_reply_payload (h, h->opt_current) == -1) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_GO_RECV_REPLY_PAYLOAD);
  }
  return 0;

}

#line 4284 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_RECV_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_GO_RECV_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_GO_RECV_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_GO.RECV_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_GO.RECV_REPLY_PAYLOAD: Receive newstyle NBD_OPT_GO reply payload
 */
static int
enter_STATE_NEWSTYLE_OPT_GO_RECV_REPLY_PAYLOAD (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 134 "generator/states-newstyle-opt-go.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_GO_CHECK_REPLY);
  }
  return 0;

}

#line 4325 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_RECV_REPLY_PAYLOAD (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_GO_RECV_REPLY_PAYLOAD;
  r = enter_STATE_NEWSTYLE_OPT_GO_RECV_REPLY_PAYLOAD (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_GO.RECV_REPLY_PAYLOAD",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_GO.CHECK_REPLY: Check newstyle NBD_OPT_GO reply */
static int
enter_STATE_NEWSTYLE_OPT_GO_CHECK_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 141 "generator/states-newstyle-opt-go.c"

  uint32_t reply;
  uint32_t len;
  const size_t maxpayload = sizeof h->sbuf.or.payload;
  int err;

  reply = be32toh (h->sbuf.or.option_reply.reply);
  len = be32toh (h->sbuf.or.option_reply.replylen);

  switch (reply) {
  case NBD_REP_INFO:
    if (len > maxpayload) {
      /* See prepare_for_reply_payload, used in RECV_REPLY */
      assert (h->rbuf == NULL);
      debug (h, "skipping large NBD_REP_INFO");
    }
    else {
      uint16_t info;
      uint64_t exportsize;
      uint16_t eflags;
      uint32_t min, pref, max;

      assert (len >= sizeof h->sbuf.or.payload.export.info);
      info = be16toh (h->sbuf.or.payload.export.info);
      switch (info) {
      case NBD_INFO_EXPORT:
        if (len != sizeof h->sbuf.or.payload.export) {
          SET_NEXT_STATE (STATE_DEAD);
          set_error (0, "handshake: incorrect NBD_INFO_EXPORT option reply "
                     "length");
          return 0;
        }
        exportsize = be64toh (h->sbuf.or.payload.export.exportsize);
        eflags = be16toh (h->sbuf.or.payload.export.eflags);
        if (nbd_internal_set_size_and_flags (h, exportsize, eflags) == -1) {
          SET_NEXT_STATE (STATE_DEAD);
          return 0;
        }
        break;
      case NBD_INFO_BLOCK_SIZE:
        if (len != sizeof h->sbuf.or.payload.block_size) {
          SET_NEXT_STATE (STATE_DEAD);
          set_error (0, "handshake: incorrect NBD_INFO_BLOCK_SIZE option "
                     "reply length");
          return 0;
        }
        min = be32toh (h->sbuf.or.payload.block_size.minimum);
        pref = be32toh (h->sbuf.or.payload.block_size.preferred);
        max = be32toh (h->sbuf.or.payload.block_size.maximum);
        if (nbd_internal_set_block_size (h, min, pref, max) == -1) {
          SET_NEXT_STATE (STATE_DEAD);
          return 0;
        }
        break;
      case NBD_INFO_NAME:
        if (len > sizeof h->sbuf.or.payload.name_desc.info + NBD_MAX_STRING ||
            len < sizeof h->sbuf.or.payload.name_desc.info) {
          SET_NEXT_STATE (STATE_DEAD);
          set_error (0, "handshake: incorrect NBD_INFO_NAME option reply "
                     "length");
          return 0;
        }
        free (h->canonical_name);
        h->canonical_name = strndup (h->sbuf.or.payload.name_desc.str, len - 2);
        if (h->canonical_name == NULL) {
          SET_NEXT_STATE (STATE_DEAD);
          set_error (errno, "strndup");
          return 0;
        }
        break;
      case NBD_INFO_DESCRIPTION:
        if (len > sizeof h->sbuf.or.payload.name_desc.info + NBD_MAX_STRING ||
            len < sizeof h->sbuf.or.payload.name_desc.info) {
          SET_NEXT_STATE (STATE_DEAD);
          set_error (0, "handshake: incorrect NBD_INFO_DESCRIPTION option "
                     "reply length");
          return 0;
        }
        free (h->description);
        h->description = strndup (h->sbuf.or.payload.name_desc.str, len - 2);
        if (h->description == NULL) {
          SET_NEXT_STATE (STATE_DEAD);
          set_error (errno, "strndup");
          return 0;
        }
        break;
      default:
        debug (h, "skipping unknown NBD_REP_INFO type %d",
               be16toh (h->sbuf.or.payload.export.info));
        break;
      }
    }
    /* Server is allowed to send any number of NBD_REP_INFO, read next one. */
    h->rbuf = &h->sbuf;
    h->rlen = sizeof (h->sbuf.or.option_reply);
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_GO_RECV_REPLY);
    return 0;
  case NBD_REP_ERR_UNSUP:
    if (h->opt_current == NBD_OPT_GO) {
      debug (h, "server is confused by NBD_OPT_GO, continuing anyway");
      SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXPORT_NAME_START);
      return 0;
    }
    /* fallthrough */
  default:
    if (handle_reply_error (h) == -1) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }
    if (reply == NBD_REP_ERR_UNSUP)
      assert (h->opt_current == NBD_OPT_INFO);

    nbd_internal_reset_size_and_flags (h);
    h->meta_valid = false;
    err = nbd_get_errno () ? : ENOTSUP;
    break;
  case NBD_REP_ACK:
    nbd_internal_set_payload (h);
    err = 0;
    break;
  }

  if (err == 0 && h->opt_current == NBD_OPT_GO)
    SET_NEXT_STATE (STATE_NEWSTYLE_FINISHED);
  else if (h->opt_mode)
    SET_NEXT_STATE (STATE_NEGOTIATING);
  else
    SET_NEXT_STATE (STATE_NEWSTYLE_PREPARE_OPT_ABORT);
  CALL_CALLBACK (h->opt_cb.completion, &err);
  nbd_internal_free_option (h);
  return 0;

}

#line 4490 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_CHECK_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_GO_CHECK_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_GO_CHECK_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_GO.CHECK_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_EXPORT_NAME.START: Try to send newstyle NBD_OPT_EXPORT_NAME to
 * end handshake
 */
static int
enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 22 "generator/states-newstyle-opt-export-name.c"

  h->sbuf.option.version = htobe64 (NBD_NEW_VERSION);
  h->sbuf.option.option = htobe32 (NBD_OPT_EXPORT_NAME);
  h->sbuf.option.optlen = htobe32 (strlen (h->export_name));
  h->chunks_sent++;
  h->wbuf = &h->sbuf;
  h->wlen = sizeof h->sbuf.option;
  h->wflags = MSG_MORE;
  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND);
  return 0;

}

#line 4536 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_EXPORT_NAME_START;
  r = enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_EXPORT_NAME.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_EXPORT_NAME.SEND: Send newstyle NBD_OPT_EXPORT_NAME to end
 * handshake
 */
static int
enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 33 "generator/states-newstyle-opt-export-name.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->wbuf = h->export_name;
    h->wlen = strlen (h->export_name);
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND_EXPORT);
  }
  return 0;

}

#line 4581 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND;
  r = enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_EXPORT_NAME.SEND",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_EXPORT_NAME.SEND_EXPORT: Send newstyle NBD_OPT_EXPORT_NAME
 * export name
 */
static int
enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND_EXPORT (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 43 "generator/states-newstyle-opt-export-name.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->rbuf = &h->sbuf;
    h->rlen = sizeof h->sbuf.export_name_reply;
    if ((h->gflags & LIBNBD_HANDSHAKE_FLAG_NO_ZEROES) != 0)
      h->rlen -= sizeof h->sbuf.export_name_reply.zeroes;
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXPORT_NAME_RECV_REPLY);
  }
  return 0;

}

#line 4628 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND_EXPORT (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND_EXPORT;
  r = enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND_EXPORT (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_EXPORT_NAME.SEND_EXPORT",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_EXPORT_NAME.RECV_REPLY: Receive newstyle NBD_OPT_EXPORT_NAME
 * reply
 */
static int
enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_RECV_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 55 "generator/states-newstyle-opt-export-name.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_EXPORT_NAME_CHECK_REPLY);
  }
  return 0;

}

#line 4670 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_RECV_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_EXPORT_NAME_RECV_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_RECV_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_EXPORT_NAME.RECV_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_EXPORT_NAME.CHECK_REPLY: Check newstyle NBD_OPT_EXPORT_NAME
 * reply
 */
static int
enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_CHECK_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 62 "generator/states-newstyle-opt-export-name.c"

  uint64_t exportsize;
  uint16_t eflags;
  int err = 0;

  exportsize = be64toh (h->sbuf.export_name_reply.exportsize);
  eflags = be16toh (h->sbuf.export_name_reply.eflags);
  if (nbd_internal_set_size_and_flags (h, exportsize, eflags) == -1) {
    SET_NEXT_STATE (STATE_DEAD);
    return 0;
  }
  nbd_internal_set_payload (h);
  SET_NEXT_STATE (STATE_NEWSTYLE_FINISHED);
  CALL_CALLBACK (h->opt_cb.completion, &err);
  nbd_internal_free_option (h);
  return 0;

}

#line 4722 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_CHECK_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_EXPORT_NAME_CHECK_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_CHECK_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_EXPORT_NAME.CHECK_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_LIST.START: Start listing exports if in list mode. */
static int
enter_STATE_NEWSTYLE_OPT_LIST_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 25 "generator/states-newstyle-opt-list.c"

  assert (h->gflags & LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE);
  assert (h->opt_mode && h->opt_current == NBD_OPT_LIST);
  assert (CALLBACK_IS_NOT_NULL (h->opt_cb.fn.list));
  h->sbuf.option.version = htobe64 (NBD_NEW_VERSION);
  h->sbuf.option.option = htobe32 (NBD_OPT_LIST);
  h->sbuf.option.optlen = 0;
  h->chunks_sent++;
  h->wbuf = &h->sbuf;
  h->wlen = sizeof (h->sbuf.option);
  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_LIST_SEND);
  return 0;

}

#line 4768 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_LIST_START;
  r = enter_STATE_NEWSTYLE_OPT_LIST_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_LIST.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_LIST.SEND: Send newstyle NBD_OPT_LIST to begin listing exports
 */
static int
enter_STATE_NEWSTYLE_OPT_LIST_SEND (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 38 "generator/states-newstyle-opt-list.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    h->rbuf = &h->sbuf;
    h->rlen = sizeof (h->sbuf.or.option_reply);
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_LIST_RECV_REPLY);
  }
  return 0;

}

#line 4812 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_SEND (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_LIST_SEND;
  r = enter_STATE_NEWSTYLE_OPT_LIST_SEND (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_LIST.SEND",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_LIST.RECV_REPLY: Receive NBD_REP_SERVER reply */
static int
enter_STATE_NEWSTYLE_OPT_LIST_RECV_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 48 "generator/states-newstyle-opt-list.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    if (prepare_for_reply_payload (h, NBD_OPT_LIST) == -1) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_LIST_RECV_REPLY_PAYLOAD);
  }
  return 0;

}

#line 4857 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_RECV_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_LIST_RECV_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_LIST_RECV_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_LIST.RECV_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_LIST.RECV_REPLY_PAYLOAD: Receive NBD_REP_SERVER reply payload */
static int
enter_STATE_NEWSTYLE_OPT_LIST_RECV_REPLY_PAYLOAD (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 60 "generator/states-newstyle-opt-list.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:  SET_NEXT_STATE (STATE_NEWSTYLE_OPT_LIST_CHECK_REPLY);
  }
  return 0;

}

#line 4897 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_RECV_REPLY_PAYLOAD (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_LIST_RECV_REPLY_PAYLOAD;
  r = enter_STATE_NEWSTYLE_OPT_LIST_RECV_REPLY_PAYLOAD (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_LIST.RECV_REPLY_PAYLOAD",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.OPT_LIST.CHECK_REPLY: Check NBD_REP_SERVER reply */
static int
enter_STATE_NEWSTYLE_OPT_LIST_CHECK_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 67 "generator/states-newstyle-opt-list.c"

  const size_t maxpayload = sizeof h->sbuf.or.payload.server;
  uint32_t reply;
  uint32_t len;
  char *tmp;
  int err;

  reply = be32toh (h->sbuf.or.option_reply.reply);
  len = be32toh (h->sbuf.or.option_reply.replylen);
  switch (reply) {
  case NBD_REP_SERVER:
    /* Got one export. */
    if (len >= maxpayload)
      debug (h, "skipping too large export name reply");
    else {
      uint32_t elen;
      const char *name;
      const char *desc;

      /* server.str is oversized for trailing NUL byte convenience */
      h->sbuf.or.payload.server.str[len - 4] = '\0';
      elen = be32toh (h->sbuf.or.payload.server.server.export_name_len);
      if (elen > len - 4 || elen > NBD_MAX_STRING ||
          len - 4 - elen > NBD_MAX_STRING) {
        set_error (0, "invalid export length");
        SET_NEXT_STATE (STATE_DEAD);
        return 0;
      }
      if (elen == len + 4) {
        tmp = NULL;
        name = h->sbuf.or.payload.server.str;
        desc = "";
      }
      else {
        tmp = strndup (h->sbuf.or.payload.server.str, elen);
        if (tmp == NULL) {
          set_error (errno, "strdup");
          SET_NEXT_STATE (STATE_DEAD);
          return 0;
        }
        name = tmp;
        desc = h->sbuf.or.payload.server.str + elen;
      }
      CALL_CALLBACK (h->opt_cb.fn.list, name, desc);
      free (tmp);
    }

    /* Wait for more replies. */
    h->rbuf = &h->sbuf;
    h->rlen = sizeof (h->sbuf.or.option_reply);
    SET_NEXT_STATE (STATE_NEWSTYLE_OPT_LIST_RECV_REPLY);
    return 0;

  case NBD_REP_ACK:
    /* Finished receiving the list. */
    err = 0;
    break;

  default:
    if (handle_reply_error (h) == -1) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }
    debug (h, "unexpected response, possibly the server does not "
           "support listing exports");
    err = ENOTSUP;
    break;
  }

  CALL_CALLBACK (h->opt_cb.completion, &err);
  nbd_internal_free_option (h);
  SET_NEXT_STATE (STATE_NEGOTIATING);
  return 0;

}

#line 5004 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_CHECK_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_OPT_LIST_CHECK_REPLY;
  r = enter_STATE_NEWSTYLE_OPT_LIST_CHECK_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.OPT_LIST.CHECK_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.PREPARE_OPT_ABORT: Prepare to send NBD_OPT_ABORT */
static int
enter_STATE_NEWSTYLE_PREPARE_OPT_ABORT (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 285 "generator/states-newstyle.c"

  assert ((h->gflags & LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE) != 0);
  h->sbuf.option.version = htobe64 (NBD_NEW_VERSION);
  h->sbuf.option.option = htobe32 (NBD_OPT_ABORT);
  h->sbuf.option.optlen = htobe32 (0);
  h->chunks_sent++;
  h->wbuf = &h->sbuf;
  h->wlen = sizeof h->sbuf.option;
  SET_NEXT_STATE (STATE_NEWSTYLE_SEND_OPT_ABORT);
  return 0;

}

#line 5048 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_PREPARE_OPT_ABORT (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_PREPARE_OPT_ABORT;
  r = enter_STATE_NEWSTYLE_PREPARE_OPT_ABORT (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.PREPARE_OPT_ABORT",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.SEND_OPT_ABORT: Send NBD_OPT_ABORT to end negotiation */
static int
enter_STATE_NEWSTYLE_SEND_OPT_ABORT (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 296 "generator/states-newstyle.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:
    SET_NEXT_STATE (STATE_NEWSTYLE_SEND_OPTION_SHUTDOWN);
  }
  return 0;

}

#line 5089 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_SEND_OPT_ABORT (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_SEND_OPT_ABORT;
  r = enter_STATE_NEWSTYLE_SEND_OPT_ABORT (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.SEND_OPT_ABORT",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.SEND_OPTION_SHUTDOWN: Sending write shutdown notification to the
 * remote server
 */
static int
enter_STATE_NEWSTYLE_SEND_OPTION_SHUTDOWN (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 304 "generator/states-newstyle.c"

  /* We don't care if the server replies to NBD_OPT_ABORT.  However,
   * unless we are in opt mode, we want to preserve the error message
   * from a failed OPT_GO by moving to DEAD instead.
   */
  if (h->sock->ops->shut_writes (h, h->sock)) {
    if (h->opt_mode)
      SET_NEXT_STATE (STATE_CLOSED);
    else
      SET_NEXT_STATE (STATE_DEAD);
  }
  return 0;

}

#line 5137 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_SEND_OPTION_SHUTDOWN (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_SEND_OPTION_SHUTDOWN;
  r = enter_STATE_NEWSTYLE_SEND_OPTION_SHUTDOWN (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.SEND_OPTION_SHUTDOWN",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEWSTYLE.FINISHED: Finish off newstyle negotiation */
static int
enter_STATE_NEWSTYLE_FINISHED (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 317 "generator/states-newstyle.c"

  SET_NEXT_STATE (STATE_READY);
  return 0;

}

#line 5174 "lib/states.c"
int
nbd_internal_enter_STATE_NEWSTYLE_FINISHED (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEWSTYLE_FINISHED;
  r = enter_STATE_NEWSTYLE_FINISHED (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEWSTYLE.FINISHED",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* NEGOTIATING: Connection is ready to negotiate an NBD option */
static int
enter_STATE_NEGOTIATING (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
  return 0;
}

#line 5207 "lib/states.c"
int
nbd_internal_enter_STATE_NEGOTIATING (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_NEGOTIATING;
  r = enter_STATE_NEGOTIATING (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "NEGOTIATING",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* READY: Connection is ready to process NBD commands */
static int
enter_STATE_READY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 183 "generator/states.c"

  if (h->cmds_to_issue)
    SET_NEXT_STATE (STATE_ISSUE_COMMAND_START);
  else {
    assert (h->sock);
    if (h->sock->ops->pending && h->sock->ops->pending (h->sock))
      SET_NEXT_STATE (STATE_REPLY_START);
  }
  return 0;

}

#line 5250 "lib/states.c"
int
nbd_internal_enter_STATE_READY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_READY;
  r = enter_STATE_READY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "READY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* ISSUE_COMMAND.START: Begin issuing a command to the remote server */
static int
enter_STATE_ISSUE_COMMAND_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 22 "generator/states-issue-command.c"

  struct command *cmd;

  assert (h->cmds_to_issue != NULL);
  cmd = h->cmds_to_issue;

  /* Were we interrupted by reading a reply to an earlier command? If
   * so, we can only get back here after a non-blocking jaunt through
   * the REPLY engine, which means we are unlikely to be unblocked for
   * writes yet; we want to advance back to the correct state but
   * without trying a send_from_wbuf that will likely return 1.
   */
  if (h->in_write_shutdown)
    SET_NEXT_STATE_AND_BLOCK (STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN);
  else if (h->wlen) {
    if (h->in_write_payload)
      SET_NEXT_STATE_AND_BLOCK (STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD);
    else
      SET_NEXT_STATE_AND_BLOCK (STATE_ISSUE_COMMAND_SEND_REQUEST);
    return 0;
  }

  /* These fields are coincident between req.compact and req.extended */
  h->req.compact.flags = htobe16 (cmd->flags);
  h->req.compact.type = htobe16 (cmd->type);
  h->req.compact.cookie = htobe64 (cmd->cookie);
  h->req.compact.offset = htobe64 (cmd->offset);
  if (h->extended_headers) {
    h->req.extended.magic = htobe32 (NBD_EXTENDED_REQUEST_MAGIC);
    h->req.extended.count = htobe64 (cmd->count);
    h->wlen = sizeof (h->req.extended);
  }
  else {
    assert (cmd->count <= UINT32_MAX);
    h->req.compact.magic = htobe32 (NBD_REQUEST_MAGIC);
    h->req.compact.count = htobe32 (cmd->count);
    h->wlen = sizeof (h->req.compact);
  }
  h->chunks_sent++;
  h->wbuf = &h->req;
  if (cmd->type == NBD_CMD_WRITE || cmd->next)
    h->wflags = MSG_MORE;
  SET_NEXT_STATE (STATE_ISSUE_COMMAND_SEND_REQUEST);
  return 0;

}

#line 5328 "lib/states.c"
int
nbd_internal_enter_STATE_ISSUE_COMMAND_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_ISSUE_COMMAND_START;
  r = enter_STATE_ISSUE_COMMAND_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "ISSUE_COMMAND.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* ISSUE_COMMAND.SEND_REQUEST: Sending a request to the remote server */
static int
enter_STATE_ISSUE_COMMAND_SEND_REQUEST (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 67 "generator/states-issue-command.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:  SET_NEXT_STATE (STATE_ISSUE_COMMAND_PREPARE_WRITE_PAYLOAD);
  }
  return 0;

}

#line 5368 "lib/states.c"
int
nbd_internal_enter_STATE_ISSUE_COMMAND_SEND_REQUEST (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_ISSUE_COMMAND_SEND_REQUEST;
  r = enter_STATE_ISSUE_COMMAND_SEND_REQUEST (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "ISSUE_COMMAND.SEND_REQUEST",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* ISSUE_COMMAND.PAUSE_SEND_REQUEST: Interrupt send request to receive an
 * earlier command's reply
 */
static int
enter_STATE_ISSUE_COMMAND_PAUSE_SEND_REQUEST (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 74 "generator/states-issue-command.c"

  assert (h->wlen);
  assert (h->cmds_to_issue != NULL);
  h->in_write_payload = false;
  SET_NEXT_STATE (STATE_REPLY_START);
  return 0;

}

#line 5410 "lib/states.c"
int
nbd_internal_enter_STATE_ISSUE_COMMAND_PAUSE_SEND_REQUEST (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_ISSUE_COMMAND_PAUSE_SEND_REQUEST;
  r = enter_STATE_ISSUE_COMMAND_PAUSE_SEND_REQUEST (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "ISSUE_COMMAND.PAUSE_SEND_REQUEST",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* ISSUE_COMMAND.PREPARE_WRITE_PAYLOAD: Prepare the write payload to send to the
 * remote server
 */
static int
enter_STATE_ISSUE_COMMAND_PREPARE_WRITE_PAYLOAD (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 81 "generator/states-issue-command.c"

  struct command *cmd;

  assert (h->cmds_to_issue != NULL);
  cmd = h->cmds_to_issue;
  assert (cmd->cookie == be64toh (h->req.compact.cookie));
  if (cmd->type == NBD_CMD_WRITE ||
      (h->extended_headers && cmd->type == NBD_CMD_BLOCK_STATUS &&
       cmd->flags & NBD_CMD_FLAG_PAYLOAD_LEN)) {
    h->wbuf = cmd->data;
    h->wlen = cmd->count;
    if (cmd->next && cmd->count < 64 * 1024)
      h->wflags = MSG_MORE;
    SET_NEXT_STATE (STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD);
  }
  else if (cmd->type == NBD_CMD_DISC) {
    h->in_write_shutdown = true;
    SET_NEXT_STATE (STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN);
  }
  else
    SET_NEXT_STATE (STATE_ISSUE_COMMAND_FINISH);
  return 0;

}

#line 5468 "lib/states.c"
int
nbd_internal_enter_STATE_ISSUE_COMMAND_PREPARE_WRITE_PAYLOAD (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_ISSUE_COMMAND_PREPARE_WRITE_PAYLOAD;
  r = enter_STATE_ISSUE_COMMAND_PREPARE_WRITE_PAYLOAD (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "ISSUE_COMMAND.PREPARE_WRITE_PAYLOAD",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* ISSUE_COMMAND.SEND_WRITE_PAYLOAD: Sending the write payload to the remote
 * server
 */
static int
enter_STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 104 "generator/states-issue-command.c"

  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 0:  SET_NEXT_STATE (STATE_ISSUE_COMMAND_FINISH);
  }
  return 0;

}

#line 5510 "lib/states.c"
int
nbd_internal_enter_STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD;
  r = enter_STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "ISSUE_COMMAND.SEND_WRITE_PAYLOAD",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* ISSUE_COMMAND.PAUSE_WRITE_PAYLOAD: Interrupt write payload to receive an
 * earlier command's reply
 */
static int
enter_STATE_ISSUE_COMMAND_PAUSE_WRITE_PAYLOAD (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 111 "generator/states-issue-command.c"

  assert (h->wlen);
  assert (h->cmds_to_issue != NULL);
  h->in_write_payload = true;
  SET_NEXT_STATE (STATE_REPLY_START);
  return 0;

}

#line 5552 "lib/states.c"
int
nbd_internal_enter_STATE_ISSUE_COMMAND_PAUSE_WRITE_PAYLOAD (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_ISSUE_COMMAND_PAUSE_WRITE_PAYLOAD;
  r = enter_STATE_ISSUE_COMMAND_PAUSE_WRITE_PAYLOAD (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "ISSUE_COMMAND.PAUSE_WRITE_PAYLOAD",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* ISSUE_COMMAND.SEND_WRITE_SHUTDOWN: Sending write shutdown notification to the
 * remote server
 */
static int
enter_STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 118 "generator/states-issue-command.c"

  if (h->sock->ops->shut_writes (h, h->sock))
    SET_NEXT_STATE (STATE_ISSUE_COMMAND_FINISH);
  return 0;

}

#line 5592 "lib/states.c"
int
nbd_internal_enter_STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN;
  r = enter_STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "ISSUE_COMMAND.SEND_WRITE_SHUTDOWN",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* ISSUE_COMMAND.PAUSE_WRITE_SHUTDOWN: Interrupt write shutdown to receive an
 * earlier command's reply
 */
static int
enter_STATE_ISSUE_COMMAND_PAUSE_WRITE_SHUTDOWN (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 123 "generator/states-issue-command.c"

  assert (h->in_write_shutdown);
  SET_NEXT_STATE (STATE_REPLY_START);
  return 0;

}

#line 5632 "lib/states.c"
int
nbd_internal_enter_STATE_ISSUE_COMMAND_PAUSE_WRITE_SHUTDOWN (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_ISSUE_COMMAND_PAUSE_WRITE_SHUTDOWN;
  r = enter_STATE_ISSUE_COMMAND_PAUSE_WRITE_SHUTDOWN (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "ISSUE_COMMAND.PAUSE_WRITE_SHUTDOWN",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* ISSUE_COMMAND.FINISH: Finish issuing a command */
static int
enter_STATE_ISSUE_COMMAND_FINISH (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 128 "generator/states-issue-command.c"

  struct command *cmd;

  assert (!h->wlen);
  assert (h->cmds_to_issue != NULL);
  cmd = h->cmds_to_issue;
  assert (cmd->cookie == be64toh (h->req.compact.cookie));
  h->cmds_to_issue = cmd->next;
  if (h->cmds_to_issue_tail == cmd)
    h->cmds_to_issue_tail = NULL;
  cmd->next = h->cmds_in_flight;
  h->cmds_in_flight = cmd;
  SET_NEXT_STATE (STATE_READY);
  return 0;

}

#line 5680 "lib/states.c"
int
nbd_internal_enter_STATE_ISSUE_COMMAND_FINISH (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_ISSUE_COMMAND_FINISH;
  r = enter_STATE_ISSUE_COMMAND_FINISH (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "ISSUE_COMMAND.FINISH",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.START: Prepare to receive a reply from the remote server */
static int
enter_STATE_REPLY_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 49 "generator/states-reply.c"

  /* If rlen is non-zero, we are resuming an earlier reply cycle. */
  if (h->rlen > 0) {
    if (h->reply_state != STATE_START) {
      assert (nbd_internal_is_state_processing (h->reply_state));
      SET_NEXT_STATE (h->reply_state);
      h->reply_state = STATE_START;
    }
    else
      SET_NEXT_STATE (STATE_REPLY_RECV_REPLY);
    return 0;
  }

  /* This state is entered when a read notification is received in the
   * READY state.  Therefore we know the socket is readable here.
   * Reading a zero length now would indicate that the socket has been
   * closed by the server and so we should jump to the CLOSED state.
   * However recv_into_rbuf will fail in this case, so test it as a
   * special case.
   */
  ssize_t r;

  /* With extended headers, there is only one size to read, so we can
   * do it all in one syscall.  But for older structured replies, we
   * don't know if we have a simple or structured reply until we read
   * the magic number, requiring a two-part read with
   * CHECK_REPLY_MAGIC below.  This works because the structured_reply
   * header is larger, and because the last member of a simple reply,
   * cookie, is coincident between all three structs (intentional
   * design decisions in the NBD spec when structured and extended
   * replies were added).
   */
  ASSERT_MEMBER_ALIAS (union reply_header, simple.magic, magic);
  ASSERT_MEMBER_ALIAS (union reply_header, simple.cookie, cookie);
  ASSERT_MEMBER_ALIAS (union reply_header, structured.magic, magic);
  ASSERT_MEMBER_ALIAS (union reply_header, structured.cookie, cookie);
  ASSERT_MEMBER_ALIAS (union reply_header, extended.magic, magic);
  ASSERT_MEMBER_ALIAS (union reply_header, extended.cookie, cookie);
  assert (h->reply_cmd == NULL);
  assert (h->rlen == 0);

  h->rbuf = &h->sbuf.reply.hdr;
  if (h->extended_headers)
    h->rlen = sizeof h->sbuf.reply.hdr.extended;
  else
    h->rlen = sizeof h->sbuf.reply.hdr.simple;

  r = h->sock->ops->recv (h, h->sock, h->rbuf, h->rlen);
  if (r == -1) {
    /* In theory this should never happen because when we enter this
     * state we should have notification that the socket is ready to
     * read.  However it can in fact happen when using TLS in
     * conjunction with a slow, remote server.  If it does happen,
     * ignore it - we will reenter this same state again next time the
     * socket is ready to read.
     */
    if (errno == EAGAIN || errno == EWOULDBLOCK)
      return 0;

    /* sock->ops->recv called set_error already. */
    SET_NEXT_STATE (STATE_DEAD);
    return 0;
  }
  if (r == 0) {
    SET_NEXT_STATE (STATE_CLOSED);
    return 0;
  }
#ifdef DUMP_PACKETS
  nbd_internal_hexdump (h->rbuf, r, stderr);
#endif

  h->bytes_received += r;
  h->rbuf = (char *)h->rbuf + r;
  h->rlen -= r;
  SET_NEXT_STATE (STATE_REPLY_RECV_REPLY);
  return 0;

}

#line 5790 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_START;
  r = enter_STATE_REPLY_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.RECV_REPLY: Receive a reply from the remote server */
static int
enter_STATE_REPLY_RECV_REPLY (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 126 "generator/states-reply.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 1: SET_NEXT_STATE (STATE_READY);
    /* Special case: if we have a short read, but got at least far
     * enough to decode the magic number, we can check if the server
     * is matching our expectations. This lets us avoid deadlocking if
     * we are blocked waiting for a 32-byte extended reply, while a
     * buggy server only sent a shorter simple or structured reply.
     * Magic number checks here must be repeated in CHECK_REPLY_MAGIC,
     * since we do not always encounter a short read.
     */
    if (h->extended_headers &&
        (char *)h->rbuf >=
        (char *)&h->sbuf.reply.hdr + sizeof h->sbuf.reply.hdr.magic) {
      uint32_t magic = be32toh (h->sbuf.reply.hdr.magic);
      if (magic != NBD_EXTENDED_REPLY_MAGIC) {
        SET_NEXT_STATE (STATE_DEAD); /* We've probably lost synchronization. */
        set_error (0, "invalid or unexpected reply magic 0x%" PRIx32, magic);
      }
    }
    return 0;
  case 0: SET_NEXT_STATE (STATE_REPLY_CHECK_REPLY_MAGIC);
  }
  return 0;

}

#line 5849 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_RECV_REPLY (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_RECV_REPLY;
  r = enter_STATE_REPLY_RECV_REPLY (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.RECV_REPLY",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.CHECK_REPLY_MAGIC: Check if the reply has expected magic */
static int
enter_STATE_REPLY_CHECK_REPLY_MAGIC (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 152 "generator/states-reply.c"

  struct command *cmd;
  uint32_t magic;
  uint64_t cookie;

  magic = be32toh (h->sbuf.reply.hdr.magic);
  switch (magic) {
  case NBD_SIMPLE_REPLY_MAGIC:
    if (h->extended_headers)
      /* Server is non-compliant, and we've already read more bytes
       * than a simple header contains; no recovery possible
       */
      goto invalid;

    /* All other payload checks handled in the simple payload engine */
    SET_NEXT_STATE (STATE_REPLY_SIMPLE_REPLY_START);
    break;

  case NBD_STRUCTURED_REPLY_MAGIC:
    if (h->extended_headers)
      /* Server is non-compliant, and we've already read more bytes
       * than a structured header contains; no recovery possible
       */
      goto invalid;

    /* We've only read the bytes that fill hdr.simple.  But
     * hdr.structured is longer, so prepare to read the remaining
     * bytes.  We depend on the memory aliasing in union sbuf to
     * overlay the two reply types.
     */
    STATIC_ASSERT (sizeof h->sbuf.reply.hdr.simple ==
                   offsetof (struct nbd_structured_reply, length),
                   simple_structured_overlap);
    assert (h->rbuf == (char *)&h->sbuf + sizeof h->sbuf.reply.hdr.simple);
    h->rlen = sizeof h->sbuf.reply.hdr.structured;
    h->rlen -= sizeof h->sbuf.reply.hdr.simple;
    SET_NEXT_STATE (STATE_REPLY_RECV_STRUCTURED_REMAINING);
    break;

  case NBD_EXTENDED_REPLY_MAGIC:
    if (!h->extended_headers)
      /* Server is non-compliant.  We could continue reading bytes up
       * to the length of an extended reply to regain sync, but old
       * servers are unlikely to send this magic, so it's just as easy
       * to punt.
       */
      goto invalid;

    /* All other payload checks handled in the chunk payload engine */
    SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_START);
    break;

  default:
    goto invalid;
  }

  /* NB: This works for all three reply types, even though we haven't
   * finished reading a structured header yet, because the cookie is
   * stored at the same offset.  See the ASSERT_MEMBER_ALIAS above in
   * state REPLY.START that confirmed this.
   */
  h->chunks_received++;
  cookie = be64toh (h->sbuf.reply.hdr.cookie);
  /* Find the command amongst the commands in flight. If the server sends
   * a reply for an unknown cookie, FINISH will diagnose that later.
   */
  for (cmd = h->cmds_in_flight; cmd != NULL; cmd = cmd->next) {
    if (cmd->cookie == cookie)
      break;
  }
  h->reply_cmd = cmd;
  return 0;

 invalid:
  SET_NEXT_STATE (STATE_DEAD); /* We've probably lost synchronization. */
  set_error (0, "invalid or unexpected reply magic 0x%" PRIx32, magic);
#if 0 /* uncomment to see desynchronized data */
  nbd_internal_hexdump (&h->sbuf.reply.hdr.simple,
                        sizeof (h->sbuf.reply.hdr.simple),
                        stderr);
#endif
  return 0;

}

#line 5965 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_CHECK_REPLY_MAGIC (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_CHECK_REPLY_MAGIC;
  r = enter_STATE_REPLY_CHECK_REPLY_MAGIC (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.CHECK_REPLY_MAGIC",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.RECV_STRUCTURED_REMAINING: Receiving the remaining part of a structured
 * reply header
 */
static int
enter_STATE_REPLY_RECV_STRUCTURED_REMAINING (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 235 "generator/states-reply.c"

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 1:
    save_reply_state (h);
    SET_NEXT_STATE (STATE_READY);
    return 0;
  case 0: SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_START);
  }
  return 0;

}

#line 6011 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_RECV_STRUCTURED_REMAINING (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_RECV_STRUCTURED_REMAINING;
  r = enter_STATE_REPLY_RECV_STRUCTURED_REMAINING (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.RECV_STRUCTURED_REMAINING",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.SIMPLE_REPLY.START: Parse a simple reply from the server */
static int
enter_STATE_REPLY_SIMPLE_REPLY_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 22 "generator/states-reply-simple.c"

  struct command *cmd = h->reply_cmd;
  uint32_t error;

  error = be32toh (h->sbuf.reply.hdr.simple.error);

  if (cmd == NULL) {
    /* Unexpected reply.  If error was set or we have structured
     * replies, we know there should be no payload, so the next byte
     * on the wire (if any) will be another reply, and we can let
     * FINISH_COMMAND diagnose/ignore the server bug.  If not, we lack
     * context to know whether the server thinks it was responding to
     * NBD_CMD_READ, so it is safer to move to DEAD now than to risk
     * consuming a server's potential data payload as a reply stream
     * (even though we would be likely to produce a magic number
     * mismatch on the next pass that would also move us to DEAD).
     */
    if (error || h->structured_replies)
      SET_NEXT_STATE (STATE_REPLY_FINISH_COMMAND);
    else {
      uint64_t cookie = be64toh (h->sbuf.reply.hdr.simple.cookie);
      SET_NEXT_STATE (STATE_DEAD);
      set_error (EPROTO,
                 "no matching cookie %" PRIu64 " found for server reply, "
                 "this is probably a server bug", cookie);
    }
    return 0;
  }

  /* Although a server with structured replies negotiated is in error
   * for using a simple reply to NBD_CMD_READ, we can cope with the
   * packet, but diagnose it by failing the read with EPROTO.
   */
  if (cmd->type == NBD_CMD_READ && h->structured_replies) {
    debug (h, "server sent unexpected simple reply for read");
    if (cmd->error == 0)
      cmd->error = EPROTO;
  }

  error = nbd_internal_errno_of_nbd_error (error);
  if (cmd->error == 0)
    cmd->error = error;
  if (error == 0 && cmd->type == NBD_CMD_READ) {
    h->rbuf = cmd->data;
    h->rlen = cmd->count;
    cmd->data_seen += cmd->count;
    SET_NEXT_STATE (STATE_REPLY_SIMPLE_REPLY_RECV_READ_PAYLOAD);
  }
  else {
    SET_NEXT_STATE (STATE_REPLY_FINISH_COMMAND);
  }
  return 0;

}

#line 6097 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_SIMPLE_REPLY_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_SIMPLE_REPLY_START;
  r = enter_STATE_REPLY_SIMPLE_REPLY_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.SIMPLE_REPLY.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.SIMPLE_REPLY.RECV_READ_PAYLOAD: Receiving the read payload for a simple
 * reply
 */
static int
enter_STATE_REPLY_SIMPLE_REPLY_RECV_READ_PAYLOAD (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 75 "generator/states-reply-simple.c"

  struct command *cmd = h->reply_cmd;

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 1:
    save_reply_state (h);
    SET_NEXT_STATE (STATE_READY);
    return 0;
  case 0:
    /* guaranteed by START */
    assert (cmd);
    if (CALLBACK_IS_NOT_NULL (cmd->cb.fn.chunk)) {
      int error = cmd->error;

      if (CALL_CALLBACK (cmd->cb.fn.chunk,
                         cmd->data, cmd->count,
                         cmd->offset, LIBNBD_READ_DATA,
                         &error) == -1)
        cmd->error = error ? error : EPROTO;
    }

    SET_NEXT_STATE (STATE_REPLY_FINISH_COMMAND);
  }
  return 0;

}

#line 6158 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_SIMPLE_REPLY_RECV_READ_PAYLOAD (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_SIMPLE_REPLY_RECV_READ_PAYLOAD;
  r = enter_STATE_REPLY_SIMPLE_REPLY_RECV_READ_PAYLOAD (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.SIMPLE_REPLY.RECV_READ_PAYLOAD",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.CHUNK_REPLY.START: Start parsing a chunk reply payload from the server
 */
static int
enter_STATE_REPLY_CHUNK_REPLY_START (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 79 "generator/states-reply-chunk.c"

  struct command *cmd = h->reply_cmd;
  uint16_t flags, type;
  uint64_t length;
  uint64_t offset = -1;

  flags = be16toh (h->sbuf.reply.hdr.structured.flags);
  type = be16toh (h->sbuf.reply.hdr.structured.type);
  if (h->extended_headers) {
    length = be64toh (h->sbuf.reply.hdr.extended.length);
    offset = be64toh (h->sbuf.reply.hdr.extended.offset);
  }
  else
    length = be32toh (h->sbuf.reply.hdr.structured.length);

  /* Reject a server that replies with too much information, but don't
   * reject a single structured reply to NBD_CMD_READ on the largest
   * size we were willing to send. The most likely culprit is a server
   * that replies with block status with way too many extents, but any
   * oversized reply is going to take long enough to resync that it is
   * not worth keeping the connection alive.
   */
  if (length > MAX_REQUEST_SIZE + sizeof h->sbuf.reply.payload.offset_data) {
    set_error (0, "invalid server reply length %" PRIu64, length);
    SET_NEXT_STATE (STATE_DEAD);
    return 0;
  }

  /* Skip an unexpected structured reply, including to an unknown cookie. */
  if (cmd == NULL || !h->structured_replies ||
      (h->extended_headers && offset != cmd->offset))
    goto resync;
  h->payload_left = length;

  switch (type) {
  case NBD_REPLY_TYPE_NONE:
    if (length != 0 || !(flags & NBD_REPLY_FLAG_DONE))
      goto resync;
    SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_FINISH);
    break;

  case NBD_REPLY_TYPE_OFFSET_DATA:
    /* The spec states that 0-length requests are unspecified, but
     * 0-length replies are broken. Still, it's easy enough to support
     * them as an extension, so we use < instead of <=.
     */
    if (cmd->type != NBD_CMD_READ ||
        length < sizeof h->sbuf.reply.payload.offset_data)
      goto resync;
    h->rbuf = &h->sbuf.reply.payload.offset_data;
    h->rlen = sizeof h->sbuf.reply.payload.offset_data;
    h->payload_left -= h->rlen;
    SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA);
    break;

  case NBD_REPLY_TYPE_OFFSET_HOLE:
    if (cmd->type != NBD_CMD_READ ||
        length != sizeof h->sbuf.reply.payload.offset_hole)
      goto resync;
    h->rbuf = &h->sbuf.reply.payload.offset_hole;
    h->rlen = sizeof h->sbuf.reply.payload.offset_hole;
    h->payload_left -= h->rlen;
    SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_HOLE);
    break;

  case NBD_REPLY_TYPE_BLOCK_STATUS:
  case NBD_REPLY_TYPE_BLOCK_STATUS_EXT:
    if (cmd->type != NBD_CMD_BLOCK_STATUS ||
        !h->meta_valid || h->meta_contexts.len == 0 ||
        !bs_reply_length_ok (type, length))
      goto resync;
    ASSERT_MEMBER_ALIAS (struct command_cb, fn.extent32, fn.extent64);
    assert (CALLBACK_IS_NOT_NULL (cmd->cb.fn.extent32));
    if (h->extended_headers != (type == NBD_REPLY_TYPE_BLOCK_STATUS_EXT)) {
      debug (h, "wrong block status reply type detected, "
             "this is probably a server bug");
      if (cmd->error == 0)
        cmd->error = EPROTO;
    }
    /* Start by reading the context ID. */
    h->rbuf = &h->sbuf.reply.payload;
    if (type == NBD_REPLY_TYPE_BLOCK_STATUS)
      h->rlen = sizeof h->sbuf.reply.payload.bs_hdr_32;
    else
      h->rlen = sizeof h->sbuf.reply.payload.bs_hdr_64;
    SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_RECV_BS_HEADER);
    break;

  default:
    if (NBD_REPLY_TYPE_IS_ERR (type)) {
      /* Any payload shorter than uint32_t cannot even carry an errno
       * value; anything longer, even if it is not long enough to be
       * compliant, will favor the wire error over EPROTO during more
       * length checks in RECV_ERROR_MESSAGE and RECV_ERROR_TAIL.
       */
      if (length < sizeof h->sbuf.reply.payload.error.error.error)
        goto resync;
      h->rbuf = &h->sbuf.reply.payload.error.error;
      h->rlen = MIN (length, sizeof h->sbuf.reply.payload.error.error);
      SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_RECV_ERROR);
    }
    else
      goto resync;
    break;
  }
  return 0;

 resync:
  h->rbuf = NULL;
  h->rlen = h->payload_left;
  h->payload_left = 0;
  SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_RESYNC);
  return 0;

}

#line 6306 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_START (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_CHUNK_REPLY_START;
  r = enter_STATE_REPLY_CHUNK_REPLY_START (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.CHUNK_REPLY.START",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.CHUNK_REPLY.RECV_ERROR: Receive a chunk reply error header */
static int
enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 193 "generator/states-reply-chunk.c"

  struct command *cmd = h->reply_cmd;
  uint32_t length, msglen, error;

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 1:
    save_reply_state (h);
    SET_NEXT_STATE (STATE_READY);
    return 0;
  case 0:
    length = h->payload_left;
    h->payload_left -= MIN (length, sizeof h->sbuf.reply.payload.error.error);
    assert (length >= sizeof h->sbuf.reply.payload.error.error.error);
    assert (cmd);

    if (length < sizeof h->sbuf.reply.payload.error.error)
      goto resync;

    msglen = be16toh (h->sbuf.reply.payload.error.error.len);
    if (msglen > h->payload_left ||
        msglen > sizeof h->sbuf.reply.payload.error.msg)
      goto resync;

    h->rbuf = h->sbuf.reply.payload.error.msg;
    h->rlen = msglen;
    h->payload_left -= h->rlen;
    SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_RECV_ERROR_MESSAGE);
  }
  return 0;

 resync:
  /* Favor the error packet's errno over RESYNC's EPROTO. */
  error = be32toh (h->sbuf.reply.payload.error.error.error);
  if (cmd->error == 0)
    cmd->error = nbd_internal_errno_of_nbd_error (error);
  h->rbuf = NULL;
  h->rlen = h->payload_left;
  h->payload_left = 0;
  SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_RESYNC);
  return 0;

}

#line 6381 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_CHUNK_REPLY_RECV_ERROR;
  r = enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.CHUNK_REPLY.RECV_ERROR",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.CHUNK_REPLY.RECV_ERROR_MESSAGE: Receive a chunk reply error message */
static int
enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR_MESSAGE (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 235 "generator/states-reply-chunk.c"

  uint32_t msglen;
  uint16_t type;

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 1:
    save_reply_state (h);
    SET_NEXT_STATE (STATE_READY);
    return 0;
  case 0:
    msglen = be16toh (h->sbuf.reply.payload.error.error.len);
    type = be16toh (h->sbuf.reply.hdr.structured.type);

    if (msglen)
      debug (h, "structured error server message: %.*s", (int)msglen,
             h->sbuf.reply.payload.error.msg);

    /* Special case two specific errors; silently ignore tail for all others */
    h->rbuf = NULL;
    h->rlen = h->payload_left;
    switch (type) {
    case NBD_REPLY_TYPE_ERROR:
      if (h->payload_left != 0)
        debug (h, "ignoring unexpected slop after error message, "
               "the server may have a bug");
      break;
    case NBD_REPLY_TYPE_ERROR_OFFSET:
      if (h->payload_left != sizeof h->sbuf.reply.payload.error.offset)
        debug (h, "unable to safely extract error offset, "
               "the server may have a bug");
      else
        h->rbuf = &h->sbuf.reply.payload.error.offset;
      break;
    }
    h->payload_left = 0;
    SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_RECV_ERROR_TAIL);
  }
  return 0;

}

#line 6454 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR_MESSAGE (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_CHUNK_REPLY_RECV_ERROR_MESSAGE;
  r = enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR_MESSAGE (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.CHUNK_REPLY.RECV_ERROR_MESSAGE",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.CHUNK_REPLY.RECV_ERROR_TAIL: Receive a chunk reply error tail */
static int
enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR_TAIL (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 275 "generator/states-reply-chunk.c"

  struct command *cmd = h->reply_cmd;
  uint32_t error;
  uint16_t type;

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 1:
    save_reply_state (h);
    SET_NEXT_STATE (STATE_READY);
    return 0;
  case 0:
    error = be32toh (h->sbuf.reply.payload.error.error.error);
    type = be16toh (h->sbuf.reply.hdr.structured.type);

    assert (cmd); /* guaranteed by CHECK */

    /* The spec requires the server to send a non-zero error */
    error = nbd_internal_errno_of_nbd_error (error);
    if (error == 0) {
      debug (h, "server forgot to set error; using EPROTO");
      error = EPROTO;
    }

    /* Sanity check that any error offset is in range, then invoke
     * user callback if present.  Ignore the offset if it was bogus.
     */
    if (type == NBD_REPLY_TYPE_ERROR_OFFSET && h->rbuf) {
      uint64_t offset = be64toh (h->sbuf.reply.payload.error.offset);
      if (structured_reply_in_bounds (offset, 0, cmd) &&
          cmd->type == NBD_CMD_READ &&
          CALLBACK_IS_NOT_NULL (cmd->cb.fn.chunk)) {
        int scratch = error;

        /* Different from successful reads: inform the callback about the
         * current error rather than any earlier one. If the callback fails
         * without setting errno, then use the server's error below.
         */
        if (CALL_CALLBACK (cmd->cb.fn.chunk,
                           (char *)cmd->data + (offset - cmd->offset),
                           0, offset, LIBNBD_READ_ERROR,
                           &scratch) == -1)
          if (cmd->error == 0)
            cmd->error = scratch;
      }
      else
        debug (h, "no use for error offset %" PRIu64, offset);
    }

    /* Preserve first error encountered */
    if (cmd->error == 0)
      cmd->error = error;

    SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_FINISH);
  }
  return 0;

}

#line 6544 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR_TAIL (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_CHUNK_REPLY_RECV_ERROR_TAIL;
  r = enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR_TAIL (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.CHUNK_REPLY.RECV_ERROR_TAIL",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.CHUNK_REPLY.RECV_OFFSET_DATA: Receive a chunk reply offset-data header
 */
static int
enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 332 "generator/states-reply-chunk.c"

  struct command *cmd = h->reply_cmd;
  uint64_t offset;

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 1:
    save_reply_state (h);
    SET_NEXT_STATE (STATE_READY);
    return 0;
  case 0:
    offset = be64toh (h->sbuf.reply.payload.offset_data.offset);

    assert (cmd); /* guaranteed by CHECK */
    assert (cmd->data && cmd->type == NBD_CMD_READ);

    /* Is the data within bounds? */
    if (! structured_reply_in_bounds (offset, h->payload_left, cmd)) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }
    if (cmd->data_seen <= cmd->count)
      cmd->data_seen += h->payload_left;
    /* Now this is the byte offset in the read buffer. */
    offset -= cmd->offset;

    /* Set up to receive the data directly to the user buffer. */
    h->rbuf = (char *)cmd->data + offset;
    h->rlen = h->payload_left;
    SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA_DATA);
  }
  return 0;

}

#line 6611 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA;
  r = enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.CHUNK_REPLY.RECV_OFFSET_DATA",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.CHUNK_REPLY.RECV_OFFSET_DATA_DATA: Receive a chunk reply offset-data
 * block of data
 */
static int
enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA_DATA (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 365 "generator/states-reply-chunk.c"

  struct command *cmd = h->reply_cmd;
  uint64_t offset;

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 1:
    save_reply_state (h);
    SET_NEXT_STATE (STATE_READY);
    return 0;
  case 0:
    offset = be64toh (h->sbuf.reply.payload.offset_data.offset);

    assert (cmd); /* guaranteed by CHECK */
    if (CALLBACK_IS_NOT_NULL (cmd->cb.fn.chunk)) {
      int error = cmd->error;

      if (CALL_CALLBACK (cmd->cb.fn.chunk,
                         (char *)cmd->data + (offset - cmd->offset),
                         h->payload_left, offset,
                         LIBNBD_READ_DATA, &error) == -1)
        if (cmd->error == 0)
          cmd->error = error ? error : EPROTO;
    }
    h->payload_left = 0;

    SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_FINISH);
  }
  return 0;

}

#line 6676 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA_DATA (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA_DATA;
  r = enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA_DATA (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.CHUNK_REPLY.RECV_OFFSET_DATA_DATA",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.CHUNK_REPLY.RECV_OFFSET_HOLE: Receive a chunk reply offset-hole header
 */
static int
enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_HOLE (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 395 "generator/states-reply-chunk.c"

  struct command *cmd = h->reply_cmd;
  uint64_t offset;
  uint32_t length;

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 1:
    save_reply_state (h);
    SET_NEXT_STATE (STATE_READY);
    return 0;
  case 0:
    offset = be64toh (h->sbuf.reply.payload.offset_hole.offset);
    length = be32toh (h->sbuf.reply.payload.offset_hole.length);

    assert (cmd); /* guaranteed by CHECK */
    assert (cmd->data && cmd->type == NBD_CMD_READ);

    /* Is the data within bounds? */
    if (! structured_reply_in_bounds (offset, length, cmd)) {
      SET_NEXT_STATE (STATE_DEAD);
      return 0;
    }
    if (cmd->data_seen <= cmd->count)
      cmd->data_seen += length;
    /* Now this is the byte offset in the read buffer. */
    offset -= cmd->offset;

    /* The spec states that 0-length requests are unspecified, but
     * 0-length replies are broken. Still, it's easy enough to support
     * them as an extension, and this works even when length == 0.
     */
    if (!cmd->initialized)
      memset ((char *)cmd->data + offset, 0, length);
    if (CALLBACK_IS_NOT_NULL (cmd->cb.fn.chunk)) {
      int error = cmd->error;

      if (CALL_CALLBACK (cmd->cb.fn.chunk,
                         (char *)cmd->data + offset, length,
                         cmd->offset + offset,
                         LIBNBD_READ_HOLE, &error) == -1)
        if (cmd->error == 0)
          cmd->error = error ? error : EPROTO;
    }

    SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_FINISH);
  }
  return 0;

}

#line 6759 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_HOLE (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_HOLE;
  r = enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_HOLE (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.CHUNK_REPLY.RECV_OFFSET_HOLE",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.CHUNK_REPLY.RECV_BS_HEADER: Receive header of a chunk reply
 * block-status payload
 */
static int
enter_STATE_REPLY_CHUNK_REPLY_RECV_BS_HEADER (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 444 "generator/states-reply-chunk.c"

  struct command *cmd = h->reply_cmd;
  uint16_t type;

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 1:
    save_reply_state (h);
    SET_NEXT_STATE (STATE_READY);
    return 0;
  case 0:
    type = be16toh (h->sbuf.reply.hdr.structured.type);

    assert (cmd); /* guaranteed by CHECK */
    assert (cmd->type == NBD_CMD_BLOCK_STATUS);
    assert (bs_reply_length_ok (type, h->payload_left));
    STATIC_ASSERT (sizeof (struct nbd_block_descriptor_32) ==
                   2 * sizeof *h->bs_cooked.narrow,
                   _block_desc_is_multiple_of_bs_entry);
    ASSERT_MEMBER_ALIAS (union chunk_payload, bs_hdr_32.context_id,
                         bs_hdr_64.context_id);

    if (type == NBD_REPLY_TYPE_BLOCK_STATUS) {
      h->payload_left -= sizeof h->sbuf.reply.payload.bs_hdr_32;
      assert (h->payload_left % sizeof *h->bs_raw.narrow == 0);
      h->bs_count = h->payload_left / sizeof *h->bs_raw.narrow;
    }
    else {
      assert (type == NBD_REPLY_TYPE_BLOCK_STATUS_EXT);
      h->payload_left -= sizeof h->sbuf.reply.payload.bs_hdr_64;
      assert (h->payload_left % sizeof *h->bs_raw.wide == 0);
      h->bs_count = h->payload_left / sizeof *h->bs_raw.wide;
      if (h->bs_count != be32toh (h->sbuf.reply.payload.bs_hdr_64.count)) {
        h->rbuf = NULL;
        h->rlen = h->payload_left;
        h->payload_left = 0;
        SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_RESYNC);
        return 0;
      }
    }

    free (h->bs_raw.storage);
    free (h->bs_cooked.storage);
    h->bs_raw.storage = malloc (h->payload_left);
    if (cmd->cb.wide)
      h->bs_cooked.storage = malloc (h->bs_count * sizeof *h->bs_cooked.wide);
    else
      h->bs_cooked.storage = malloc (2 * h->bs_count *
                                     sizeof *h->bs_cooked.narrow);
    if (h->bs_raw.storage == NULL || h->bs_cooked.storage == NULL) {
      SET_NEXT_STATE (STATE_DEAD);
      set_error (errno, "malloc");
      free (h->bs_raw.storage);
      free (h->bs_cooked.storage);
      h->bs_raw.storage = NULL;
      h->bs_cooked.storage = NULL;
      return 0;
    }

    h->rbuf = h->bs_raw.storage;
    h->rlen = h->payload_left;
    h->payload_left = 0;
    SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_RECV_BS_ENTRIES);
  }
  return 0;

}

#line 6860 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_BS_HEADER (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_CHUNK_REPLY_RECV_BS_HEADER;
  r = enter_STATE_REPLY_CHUNK_REPLY_RECV_BS_HEADER (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.CHUNK_REPLY.RECV_BS_HEADER",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.CHUNK_REPLY.RECV_BS_ENTRIES: Receive entries array of chunk reply
 * block-status payload
 */
static int
enter_STATE_REPLY_CHUNK_REPLY_RECV_BS_ENTRIES (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 510 "generator/states-reply-chunk.c"

  struct command *cmd = h->reply_cmd;
  uint16_t type;
  size_t i;
  uint32_t context_id;
  int error;
  const char *name;
  uint64_t orig_len, len, flags;
  uint64_t total, cap;
  bool stop;
  int ret;

  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 1:
    save_reply_state (h);
    SET_NEXT_STATE (STATE_READY);
    return 0;
  case 0:
    type = be16toh (h->sbuf.reply.hdr.structured.type);

    assert (cmd); /* guaranteed by CHECK */
    assert (cmd->type == NBD_CMD_BLOCK_STATUS);
    assert (CALLBACK_IS_NOT_NULL (cmd->cb.fn.extent32));
    assert (h->bs_count && h->bs_raw.storage);
    assert (h->meta_valid);

    /* Look up the context ID. Depends on ASSERT_MEMBER_ALIAS above. */
    context_id = be32toh (h->sbuf.reply.payload.bs_hdr_32.context_id);
    for (i = 0; i < h->meta_contexts.len; ++i)
      if (context_id == h->meta_contexts.ptr[i].context_id)
        break;

    SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_FINISH);
    if (i == h->meta_contexts.len) {
      /* Emit a debug message, but ignore it. */
      debug (h, "server sent unexpected meta context ID %" PRIu32,
             context_id);
      break;
    }

    /* Be careful to avoid arithmetic overflow, even when the user
     * disabled LIBNBD_STRICT_BOUNDS to pass a suspect offset, or the
     * server returns suspect lengths or advertised exportsize larger
     * than 63 bits.  We guarantee that callbacks will not see a
     * length exceeding INT64_MAX or the advertised h->exportsize.
     */
    name = h->meta_contexts.ptr[i].name;
    total = cap = 0;
    if (cmd->offset <= h->exportsize)
      cap = h->exportsize - cmd->offset;

    /* Need to byte-swap the entries returned into the callback size
     * requested by the caller.  The NBD protocol allows truncation as
     * long as progress is made; the client cannot tell the difference
     * between a server's truncation or if we truncate on a length we
     * don't like.  We stop iterating on a zero-length extent (error
     * only if it is the first extent), on an extent beyond the
     * exportsize (unconditional error after truncating to
     * exportsize), and on an extent exceeding a callback length limit
     * (no error, and to simplify alignment, we truncate to 64M before
     * the limit); but we do not diagnose issues with the server's
     * length alignments, flag values, nor compliance with the REQ_ONE
     * command flag.
     */
    for (i = 0, stop = false; i < h->bs_count && !stop; ++i) {
      if (type == NBD_REPLY_TYPE_BLOCK_STATUS) {
        orig_len = len = be32toh (h->bs_raw.narrow[i].length);
        flags = be32toh (h->bs_raw.narrow[i].status_flags);
      }
      else {
        orig_len = len = be64toh (h->bs_raw.wide[i].length);
        if (len > INT64_MAX) {
          /* Pick an aligned value rather than overflowing 64-bit
           * callback; this does not require an error.
           */
          stop = true;
          len = INT64_MAX + 1ULL - MAX_REQUEST_SIZE;
        }
        if (len > UINT32_MAX && !cmd->cb.wide) {
          /* Pick an aligned value rather than overflowing 32-bit
           * callback; this does not require an error.
           */
          stop = true;
          len = (uint32_t)-MAX_REQUEST_SIZE;
        }
        flags = be64toh (h->bs_raw.wide[i].status_flags);
        if (flags > UINT32_MAX && !cmd->cb.wide) {
          stop = true;
          if (i > 0)
            break; /* Skip this and later extents; we already made progress */
          /* Expose this extent as an error; we made no progress */
          cmd->error = cmd->error ? : EOVERFLOW;
          flags = (uint32_t)flags;
        }
      }

      assert (total <= cap);
      if (len > cap - total) {
        /* Truncate and expose this extent as an error */
        len = cap - total;
        stop = true;
        cmd->error = cmd->error ? : EPROTO;
      }
      if (len == 0) {
        stop = true;
        if (i > 0)
          break; /* Skip this and later extents; we already made progress */
        /* Expose this extent as an error; we made no progress */
        cmd->error = cmd->error ? : EPROTO;
      }
      total += len;
      if (cmd->cb.wide) {
        h->bs_cooked.wide[i].length = len;
        h->bs_cooked.wide[i].flags = flags;
      }
      else {
        assert ((len | flags) <= UINT32_MAX);
        h->bs_cooked.narrow[i * 2] = len;
        h->bs_cooked.narrow[i * 2 + 1] = flags;
      }
    }

    /* Call the caller's extent function.  Yes, our 32-bit public API
     * foolishly tracks the number of uint32_t instead of block
     * descriptors; see _block_desc_is_multiple_of_bs_entry above.
     */
    if (stop)
      debug (h, "truncating server's response at unexpected extent length %"
             PRIu64 " and total %" PRIu64 " near extent %zu",
             orig_len, total, i);
    error = cmd->error;
    if (cmd->cb.wide)
      ret = CALL_CALLBACK (cmd->cb.fn.extent64, name, cmd->offset,
                           h->bs_cooked.wide, i, &error);
    else
      ret = CALL_CALLBACK (cmd->cb.fn.extent32, name, cmd->offset,
                           h->bs_cooked.narrow, i * 2, &error);
    if (ret == -1 && cmd->error == 0)
      cmd->error = error ? error : EPROTO;
  }
  return 0;

}

#line 7038 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_BS_ENTRIES (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_CHUNK_REPLY_RECV_BS_ENTRIES;
  r = enter_STATE_REPLY_CHUNK_REPLY_RECV_BS_ENTRIES (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.CHUNK_REPLY.RECV_BS_ENTRIES",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.CHUNK_REPLY.RESYNC: Ignore payload of an unexpected chunk reply */
static int
enter_STATE_REPLY_CHUNK_REPLY_RESYNC (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 653 "generator/states-reply-chunk.c"

  struct command *cmd = h->reply_cmd;
  uint16_t type;
  uint64_t length;
  uint64_t offset = -1;

  assert (h->rbuf == NULL);
  switch (recv_into_rbuf (h)) {
  case -1: SET_NEXT_STATE (STATE_DEAD); return 0;
  case 1:
    save_reply_state (h);
    SET_NEXT_STATE (STATE_READY);
    return 0;
  case 0:
    /* If this reply is to an unknown command, FINISH_COMMAND will
     * diagnose and ignore the server bug.  Otherwise, ensure the
     * pending command sees a failure of EPROTO if it does not already
     * have an error.
     */
    if (cmd == NULL) {
      SET_NEXT_STATE (STATE_REPLY_FINISH_COMMAND);
      return 0;
    }
    type = be16toh (h->sbuf.reply.hdr.structured.type);
    if (h->extended_headers) {
      length = be64toh (h->sbuf.reply.hdr.extended.length);
      offset = be64toh (h->sbuf.reply.hdr.extended.offset);
      if (offset != cmd->offset)
        debug (h, "unexpected reply offset %" PRIu64 " for cookie %" PRIu64
               " and command %" PRIu32 ", this is probably a server bug",
               length, cmd->cookie, cmd->type);
      else
        offset = -1;
    }
    else
      length = be32toh (h->sbuf.reply.hdr.structured.length);
    if (offset == -1)
      debug (h, "unexpected reply type %u or payload length %" PRIu64
             " for cookie %" PRIu64 " and command %" PRIu32
             ", this is probably a server bug",
             type, length, cmd->cookie, cmd->type);
    if (cmd->error == 0)
      cmd->error = EPROTO;
    SET_NEXT_STATE (STATE_REPLY_CHUNK_REPLY_FINISH);
  }
  return 0;

}

#line 7118 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RESYNC (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_CHUNK_REPLY_RESYNC;
  r = enter_STATE_REPLY_CHUNK_REPLY_RESYNC (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.CHUNK_REPLY.RESYNC",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.CHUNK_REPLY.FINISH: Finish receiving a chunk reply */
static int
enter_STATE_REPLY_CHUNK_REPLY_FINISH (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 700 "generator/states-reply-chunk.c"

  uint16_t flags;

  assert (h->payload_left == 0);
  flags = be16toh (h->sbuf.reply.hdr.structured.flags);
  if (flags & NBD_REPLY_FLAG_DONE) {
    SET_NEXT_STATE (STATE_REPLY_FINISH_COMMAND);
  }
  else {
    h->reply_cmd = NULL;
    SET_NEXT_STATE (STATE_READY);
  }
  return 0;

}

#line 7165 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_FINISH (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_CHUNK_REPLY_FINISH;
  r = enter_STATE_REPLY_CHUNK_REPLY_FINISH (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.CHUNK_REPLY.FINISH",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* REPLY.FINISH_COMMAND: Finish receiving a command */
static int
enter_STATE_REPLY_FINISH_COMMAND (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 246 "generator/states-reply.c"

  struct command *prev_cmd, *cmd;
  uint64_t cookie;
  bool retire;

  /* NB: This works for both simple and structured replies because the
   * handle (our cookie) is stored at the same offset.  See the
   * STATIC_ASSERT above in state REPLY.START that confirmed this.
   */
  cookie = be64toh (h->sbuf.reply.hdr.cookie);
  /* Find the command amongst the commands in flight. */
  for (cmd = h->cmds_in_flight, prev_cmd = NULL;
       cmd != NULL;
       prev_cmd = cmd, cmd = cmd->next) {
    if (cmd->cookie == cookie)
      break;
  }
  assert (h->reply_cmd == cmd);
  if (cmd == NULL) {
    debug (h, "skipped reply for unexpected cookie %" PRIu64
           ", this is probably a bug in the server", cookie);
    SET_NEXT_STATE (STATE_READY);
    return 0;
  }

  retire = cmd->type == NBD_CMD_DISC;
  h->reply_cmd = NULL;

  /* Notify the user */
  if (CALLBACK_IS_NOT_NULL (cmd->cb.completion)) {
    int error = cmd->error;
    int r;

    assert (cmd->type != NBD_CMD_DISC);
    r = CALL_CALLBACK (cmd->cb.completion, &error);
    switch (r) {
    case -1:
      if (error)
        cmd->error = error;
      break;
    case 1:
      retire = true;
      break;
    }
  }

  /* Move it to the end of the cmds_done list. */
  if (prev_cmd != NULL)
    prev_cmd->next = cmd->next;
  else
    h->cmds_in_flight = cmd->next;
  cmd->next = NULL;
  if (retire)
    nbd_internal_retire_and_free_command (cmd);
  else {
    if (h->cmds_done_tail != NULL)
      h->cmds_done_tail = h->cmds_done_tail->next = cmd;
    else {
      assert (h->cmds_done == NULL);
      h->cmds_done = h->cmds_done_tail = cmd;
    }
  }
  h->in_flight--;
  assert (h->in_flight >= 0);

  SET_NEXT_STATE (STATE_READY);
  return 0;

}

#line 7266 "lib/states.c"
int
nbd_internal_enter_STATE_REPLY_FINISH_COMMAND (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_REPLY_FINISH_COMMAND;
  r = enter_STATE_REPLY_FINISH_COMMAND (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "REPLY.FINISH_COMMAND",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* DEAD: Connection is in an unrecoverable error state, can only be closed */
static int
enter_STATE_DEAD (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 193 "generator/states.c"

  const char *err = nbd_get_error ();

  /* The caller should have used set_error() before reaching here */
  assert (err != NULL);
  debug (h, "handle dead: %s", err);

  abort_option (h);
  nbd_internal_abort_commands (h, &h->cmds_to_issue);
  nbd_internal_abort_commands (h, &h->cmds_in_flight);
  h->in_flight = 0;
  if (h->sock) {
    h->sock->ops->close (h->sock);
    h->sock = NULL;
  }
  return -1;

}

#line 7316 "lib/states.c"
int
nbd_internal_enter_STATE_DEAD (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_DEAD;
  r = enter_STATE_DEAD (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "DEAD",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}

/* CLOSED: Connection is closed */
static int
enter_STATE_CLOSED (
  struct nbd_handle *h, enum state *next_state, bool *blocked
)
{
#line 210 "generator/states.c"

  abort_option (h);
  nbd_internal_abort_commands (h, &h->cmds_to_issue);
  nbd_internal_abort_commands (h, &h->cmds_in_flight);
  h->in_flight = 0;
  if (h->sock) {
    h->sock->ops->close (h->sock);
    h->sock = NULL;
  }
  return 0;

}

#line 7360 "lib/states.c"
int
nbd_internal_enter_STATE_CLOSED (
  struct nbd_handle *h, bool *blocked
)
{
  int r;
  enum state next;

  next = STATE_CLOSED;
  r = enter_STATE_CLOSED (
        h, &next, blocked
      );
  if (get_next_state (h) != next) {
#ifdef LIBNBD_STATE_VERBOSE
    debug (h, "transition: %s -> %s",
           "CLOSED",
           nbd_internal_state_short_string (next));
#endif
    set_next_state (h, next);
  }
  return r;
}
