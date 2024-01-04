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

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#include "libnbd.h"
#include "internal.h"

/* Run the state machine based on an external event until it would block. */
int
nbd_internal_run (struct nbd_handle *h, enum external_event ev)
{
  int r;
  bool blocked;

  /* Validate and handle the external event. */
  switch (get_next_state (h))
  {
  case STATE_START:
    switch (ev)
    {
    case cmd_create:
      goto ok;
    case cmd_connect_sockaddr:
      set_next_state (h, STATE_CONNECT_START);
#ifdef LIBNBD_STATE_VERBOSE
      debug (h, "event %s: %s -> %s", "CmdConnectSockAddr", "START",
             "CONNECT.START");
#endif
      goto ok;
    case cmd_connect_tcp:
      set_next_state (h, STATE_CONNECT_TCP_START);
#ifdef LIBNBD_STATE_VERBOSE
      debug (h, "event %s: %s -> %s", "CmdConnectTCP", "START",
             "CONNECT_TCP.START");
#endif
      goto ok;
    case cmd_connect_command:
      set_next_state (h, STATE_CONNECT_COMMAND_START);
#ifdef LIBNBD_STATE_VERBOSE
      debug (h, "event %s: %s -> %s", "CmdConnectCommand", "START",
             "CONNECT_COMMAND.START");
#endif
      goto ok;
    case cmd_connect_sa:
      set_next_state (h, STATE_CONNECT_SA_START);
#ifdef LIBNBD_STATE_VERBOSE
      debug (h, "event %s: %s -> %s", "CmdConnectSA", "START",
             "CONNECT_SA.START");
#endif
      goto ok;
    case cmd_connect_socket:
      set_next_state (h, STATE_MAGIC_START);
#ifdef LIBNBD_STATE_VERBOSE
      debug (h, "event %s: %s -> %s", "CmdConnectSocket", "START",
             "MAGIC.START");
#endif
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_CONNECT_START:
    switch (ev)
    {
    case notify_write:
      set_next_state (h, STATE_CONNECT_CONNECTING);
#ifdef LIBNBD_STATE_VERBOSE
      debug (h, "event %s: %s -> %s", "NotifyWrite", "CONNECT.START",
             "CONNECT.CONNECTING");
#endif
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_CONNECT_CONNECTING:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_CONNECT_TCP_START:
    break;
  case STATE_CONNECT_TCP_CONNECT:
    switch (ev)
    {
    case notify_write:
      set_next_state (h, STATE_CONNECT_TCP_CONNECTING);
#ifdef LIBNBD_STATE_VERBOSE
      debug (h, "event %s: %s -> %s", "NotifyWrite", "CONNECT_TCP.CONNECT",
             "CONNECT_TCP.CONNECTING");
#endif
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_CONNECT_TCP_CONNECTING:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_CONNECT_TCP_NEXT_ADDRESS:
    break;
  case STATE_CONNECT_COMMAND_START:
    break;
  case STATE_CONNECT_SA_START:
    break;
  case STATE_MAGIC_START:
    break;
  case STATE_MAGIC_RECV_MAGIC:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_MAGIC_CHECK_MAGIC:
    break;
  case STATE_OLDSTYLE_START:
    break;
  case STATE_OLDSTYLE_RECV_REMAINING:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_OLDSTYLE_CHECK:
    break;
  case STATE_NEWSTYLE_START:
    break;
  case STATE_NEWSTYLE_RECV_GFLAGS:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_CHECK_GFLAGS:
    break;
  case STATE_NEWSTYLE_SEND_CFLAGS:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_START:
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_SEND:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY_PAYLOAD:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_CHECK_REPLY:
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE:
    break;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START:
    break;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_SEND:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY_PAYLOAD:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_CHECK_REPLY:
    break;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START:
    break;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_SEND:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY_PAYLOAD:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_CHECK_REPLY:
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_START:
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAMELEN:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAME:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_NRQUERIES:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY:
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERYLEN:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERY:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY:
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY_PAYLOAD:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_CHECK_REPLY:
    break;
  case STATE_NEWSTYLE_OPT_GO_START:
    break;
  case STATE_NEWSTYLE_OPT_GO_SEND:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_GO_SEND_EXPORTNAMELEN:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_GO_SEND_EXPORT:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_GO_SEND_NRINFOS:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_GO_SEND_INFO:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_GO_RECV_REPLY:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_GO_RECV_REPLY_PAYLOAD:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_GO_CHECK_REPLY:
    break;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_START:
    break;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND_EXPORT:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_RECV_REPLY:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_CHECK_REPLY:
    break;
  case STATE_NEWSTYLE_OPT_LIST_START:
    break;
  case STATE_NEWSTYLE_OPT_LIST_SEND:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_LIST_RECV_REPLY:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_LIST_RECV_REPLY_PAYLOAD:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_OPT_LIST_CHECK_REPLY:
    break;
  case STATE_NEWSTYLE_PREPARE_OPT_ABORT:
    break;
  case STATE_NEWSTYLE_SEND_OPT_ABORT:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_SEND_OPTION_SHUTDOWN:
    switch (ev)
    {
    case notify_write:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_NEWSTYLE_FINISHED:
    break;
  case STATE_NEGOTIATING:
    switch (ev)
    {
    case cmd_issue:
      set_next_state (h, STATE_NEWSTYLE_START);
#ifdef LIBNBD_STATE_VERBOSE
      debug (h, "event %s: %s -> %s", "CmdIssue", "NEGOTIATING",
             "NEWSTYLE.START");
#endif
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_READY:
    switch (ev)
    {
    case cmd_issue:
      set_next_state (h, STATE_ISSUE_COMMAND_START);
#ifdef LIBNBD_STATE_VERBOSE
      debug (h, "event %s: %s -> %s", "CmdIssue", "READY",
             "ISSUE_COMMAND.START");
#endif
      goto ok;
    case notify_read:
      set_next_state (h, STATE_REPLY_START);
#ifdef LIBNBD_STATE_VERBOSE
      debug (h, "event %s: %s -> %s", "NotifyRead", "READY",
             "REPLY.START");
#endif
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_ISSUE_COMMAND_START:
    break;
  case STATE_ISSUE_COMMAND_SEND_REQUEST:
    switch (ev)
    {
    case notify_write:
      goto ok;
    case notify_read:
      set_next_state (h, STATE_ISSUE_COMMAND_PAUSE_SEND_REQUEST);
#ifdef LIBNBD_STATE_VERBOSE
      debug (h, "event %s: %s -> %s", "NotifyRead",
             "ISSUE_COMMAND.SEND_REQUEST",
             "ISSUE_COMMAND.PAUSE_SEND_REQUEST");
#endif
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_ISSUE_COMMAND_PAUSE_SEND_REQUEST:
    break;
  case STATE_ISSUE_COMMAND_PREPARE_WRITE_PAYLOAD:
    break;
  case STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD:
    switch (ev)
    {
    case notify_write:
      goto ok;
    case notify_read:
      set_next_state (h, STATE_ISSUE_COMMAND_PAUSE_WRITE_PAYLOAD);
#ifdef LIBNBD_STATE_VERBOSE
      debug (h, "event %s: %s -> %s", "NotifyRead",
             "ISSUE_COMMAND.SEND_WRITE_PAYLOAD",
             "ISSUE_COMMAND.PAUSE_WRITE_PAYLOAD");
#endif
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_ISSUE_COMMAND_PAUSE_WRITE_PAYLOAD:
    break;
  case STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN:
    switch (ev)
    {
    case notify_write:
      goto ok;
    case notify_read:
      set_next_state (h, STATE_ISSUE_COMMAND_PAUSE_WRITE_SHUTDOWN);
#ifdef LIBNBD_STATE_VERBOSE
      debug (h, "event %s: %s -> %s", "NotifyRead",
             "ISSUE_COMMAND.SEND_WRITE_SHUTDOWN",
             "ISSUE_COMMAND.PAUSE_WRITE_SHUTDOWN");
#endif
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_ISSUE_COMMAND_PAUSE_WRITE_SHUTDOWN:
    break;
  case STATE_ISSUE_COMMAND_FINISH:
    break;
  case STATE_REPLY_START:
    switch (ev)
    {
    case notify_read:
      goto ok;
    default: ; /* nothing, silence GCC warning */
    }
    break;
  case STATE_REPLY_RECV_REPLY:
    break;
  case STATE_REPLY_CHECK_REPLY_MAGIC:
    break;
  case STATE_REPLY_RECV_STRUCTURED_REMAINING:
    break;
  case STATE_REPLY_SIMPLE_REPLY_START:
    break;
  case STATE_REPLY_SIMPLE_REPLY_RECV_READ_PAYLOAD:
    break;
  case STATE_REPLY_CHUNK_REPLY_START:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR_MESSAGE:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR_TAIL:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA_DATA:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_HOLE:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_BS_HEADER:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_BS_ENTRIES:
    break;
  case STATE_REPLY_CHUNK_REPLY_RESYNC:
    break;
  case STATE_REPLY_CHUNK_REPLY_FINISH:
    break;
  case STATE_REPLY_FINISH_COMMAND:
    break;
  case STATE_DEAD:
    break;
  case STATE_CLOSED:
    break;
  }

  set_error (EINVAL, "external event %d is invalid in state %s",
             ev, nbd_internal_state_short_string (get_next_state (h)));
  return -1;

 ok:
  do {
    blocked = true;

    /* Run a single step. */
    switch (get_next_state (h))
    {
    case STATE_START:
      r = nbd_internal_enter_STATE_START (h, &blocked);
      break;
    case STATE_CONNECT_START:
      r = nbd_internal_enter_STATE_CONNECT_START (h, &blocked);
      break;
    case STATE_CONNECT_CONNECTING:
      r = nbd_internal_enter_STATE_CONNECT_CONNECTING (h, &blocked);
      break;
    case STATE_CONNECT_TCP_START:
      r = nbd_internal_enter_STATE_CONNECT_TCP_START (h, &blocked);
      break;
    case STATE_CONNECT_TCP_CONNECT:
      r = nbd_internal_enter_STATE_CONNECT_TCP_CONNECT (h, &blocked);
      break;
    case STATE_CONNECT_TCP_CONNECTING:
      r = nbd_internal_enter_STATE_CONNECT_TCP_CONNECTING (h, &blocked);
      break;
    case STATE_CONNECT_TCP_NEXT_ADDRESS:
      r = nbd_internal_enter_STATE_CONNECT_TCP_NEXT_ADDRESS (h, &blocked);
      break;
    case STATE_CONNECT_COMMAND_START:
      r = nbd_internal_enter_STATE_CONNECT_COMMAND_START (h, &blocked);
      break;
    case STATE_CONNECT_SA_START:
      r = nbd_internal_enter_STATE_CONNECT_SA_START (h, &blocked);
      break;
    case STATE_MAGIC_START:
      r = nbd_internal_enter_STATE_MAGIC_START (h, &blocked);
      break;
    case STATE_MAGIC_RECV_MAGIC:
      r = nbd_internal_enter_STATE_MAGIC_RECV_MAGIC (h, &blocked);
      break;
    case STATE_MAGIC_CHECK_MAGIC:
      r = nbd_internal_enter_STATE_MAGIC_CHECK_MAGIC (h, &blocked);
      break;
    case STATE_OLDSTYLE_START:
      r = nbd_internal_enter_STATE_OLDSTYLE_START (h, &blocked);
      break;
    case STATE_OLDSTYLE_RECV_REMAINING:
      r = nbd_internal_enter_STATE_OLDSTYLE_RECV_REMAINING (h, &blocked);
      break;
    case STATE_OLDSTYLE_CHECK:
      r = nbd_internal_enter_STATE_OLDSTYLE_CHECK (h, &blocked);
      break;
    case STATE_NEWSTYLE_START:
      r = nbd_internal_enter_STATE_NEWSTYLE_START (h, &blocked);
      break;
    case STATE_NEWSTYLE_RECV_GFLAGS:
      r = nbd_internal_enter_STATE_NEWSTYLE_RECV_GFLAGS (h, &blocked);
      break;
    case STATE_NEWSTYLE_CHECK_GFLAGS:
      r = nbd_internal_enter_STATE_NEWSTYLE_CHECK_GFLAGS (h, &blocked);
      break;
    case STATE_NEWSTYLE_SEND_CFLAGS:
      r = nbd_internal_enter_STATE_NEWSTYLE_SEND_CFLAGS (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_STARTTLS_START:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_START (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_STARTTLS_SEND:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_SEND (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY_PAYLOAD:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY_PAYLOAD (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_STARTTLS_CHECK_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_CHECK_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_SEND:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_SEND (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY_PAYLOAD:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY_PAYLOAD (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_CHECK_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_CHECK_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_SEND:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_SEND (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY_PAYLOAD:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY_PAYLOAD (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_CHECK_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_CHECK_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_META_CONTEXT_START:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_START (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAMELEN:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAMELEN (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAME:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAME (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_NRQUERIES:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_NRQUERIES (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERYLEN:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERYLEN (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY_PAYLOAD:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY_PAYLOAD (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_META_CONTEXT_CHECK_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_CHECK_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_GO_START:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_START (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_GO_SEND:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_GO_SEND_EXPORTNAMELEN:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND_EXPORTNAMELEN (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_GO_SEND_EXPORT:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND_EXPORT (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_GO_SEND_NRINFOS:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND_NRINFOS (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_GO_SEND_INFO:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND_INFO (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_GO_RECV_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_RECV_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_GO_RECV_REPLY_PAYLOAD:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_RECV_REPLY_PAYLOAD (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_GO_CHECK_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_CHECK_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_EXPORT_NAME_START:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_START (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND_EXPORT:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND_EXPORT (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_EXPORT_NAME_RECV_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_RECV_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_EXPORT_NAME_CHECK_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_CHECK_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_LIST_START:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_START (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_LIST_SEND:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_SEND (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_LIST_RECV_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_RECV_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_LIST_RECV_REPLY_PAYLOAD:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_RECV_REPLY_PAYLOAD (h, &blocked);
      break;
    case STATE_NEWSTYLE_OPT_LIST_CHECK_REPLY:
      r = nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_CHECK_REPLY (h, &blocked);
      break;
    case STATE_NEWSTYLE_PREPARE_OPT_ABORT:
      r = nbd_internal_enter_STATE_NEWSTYLE_PREPARE_OPT_ABORT (h, &blocked);
      break;
    case STATE_NEWSTYLE_SEND_OPT_ABORT:
      r = nbd_internal_enter_STATE_NEWSTYLE_SEND_OPT_ABORT (h, &blocked);
      break;
    case STATE_NEWSTYLE_SEND_OPTION_SHUTDOWN:
      r = nbd_internal_enter_STATE_NEWSTYLE_SEND_OPTION_SHUTDOWN (h, &blocked);
      break;
    case STATE_NEWSTYLE_FINISHED:
      r = nbd_internal_enter_STATE_NEWSTYLE_FINISHED (h, &blocked);
      break;
    case STATE_NEGOTIATING:
      r = nbd_internal_enter_STATE_NEGOTIATING (h, &blocked);
      break;
    case STATE_READY:
      r = nbd_internal_enter_STATE_READY (h, &blocked);
      break;
    case STATE_ISSUE_COMMAND_START:
      r = nbd_internal_enter_STATE_ISSUE_COMMAND_START (h, &blocked);
      break;
    case STATE_ISSUE_COMMAND_SEND_REQUEST:
      r = nbd_internal_enter_STATE_ISSUE_COMMAND_SEND_REQUEST (h, &blocked);
      break;
    case STATE_ISSUE_COMMAND_PAUSE_SEND_REQUEST:
      r = nbd_internal_enter_STATE_ISSUE_COMMAND_PAUSE_SEND_REQUEST (h, &blocked);
      break;
    case STATE_ISSUE_COMMAND_PREPARE_WRITE_PAYLOAD:
      r = nbd_internal_enter_STATE_ISSUE_COMMAND_PREPARE_WRITE_PAYLOAD (h, &blocked);
      break;
    case STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD:
      r = nbd_internal_enter_STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD (h, &blocked);
      break;
    case STATE_ISSUE_COMMAND_PAUSE_WRITE_PAYLOAD:
      r = nbd_internal_enter_STATE_ISSUE_COMMAND_PAUSE_WRITE_PAYLOAD (h, &blocked);
      break;
    case STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN:
      r = nbd_internal_enter_STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN (h, &blocked);
      break;
    case STATE_ISSUE_COMMAND_PAUSE_WRITE_SHUTDOWN:
      r = nbd_internal_enter_STATE_ISSUE_COMMAND_PAUSE_WRITE_SHUTDOWN (h, &blocked);
      break;
    case STATE_ISSUE_COMMAND_FINISH:
      r = nbd_internal_enter_STATE_ISSUE_COMMAND_FINISH (h, &blocked);
      break;
    case STATE_REPLY_START:
      r = nbd_internal_enter_STATE_REPLY_START (h, &blocked);
      break;
    case STATE_REPLY_RECV_REPLY:
      r = nbd_internal_enter_STATE_REPLY_RECV_REPLY (h, &blocked);
      break;
    case STATE_REPLY_CHECK_REPLY_MAGIC:
      r = nbd_internal_enter_STATE_REPLY_CHECK_REPLY_MAGIC (h, &blocked);
      break;
    case STATE_REPLY_RECV_STRUCTURED_REMAINING:
      r = nbd_internal_enter_STATE_REPLY_RECV_STRUCTURED_REMAINING (h, &blocked);
      break;
    case STATE_REPLY_SIMPLE_REPLY_START:
      r = nbd_internal_enter_STATE_REPLY_SIMPLE_REPLY_START (h, &blocked);
      break;
    case STATE_REPLY_SIMPLE_REPLY_RECV_READ_PAYLOAD:
      r = nbd_internal_enter_STATE_REPLY_SIMPLE_REPLY_RECV_READ_PAYLOAD (h, &blocked);
      break;
    case STATE_REPLY_CHUNK_REPLY_START:
      r = nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_START (h, &blocked);
      break;
    case STATE_REPLY_CHUNK_REPLY_RECV_ERROR:
      r = nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR (h, &blocked);
      break;
    case STATE_REPLY_CHUNK_REPLY_RECV_ERROR_MESSAGE:
      r = nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR_MESSAGE (h, &blocked);
      break;
    case STATE_REPLY_CHUNK_REPLY_RECV_ERROR_TAIL:
      r = nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR_TAIL (h, &blocked);
      break;
    case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA:
      r = nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA (h, &blocked);
      break;
    case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA_DATA:
      r = nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA_DATA (h, &blocked);
      break;
    case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_HOLE:
      r = nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_HOLE (h, &blocked);
      break;
    case STATE_REPLY_CHUNK_REPLY_RECV_BS_HEADER:
      r = nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_BS_HEADER (h, &blocked);
      break;
    case STATE_REPLY_CHUNK_REPLY_RECV_BS_ENTRIES:
      r = nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_BS_ENTRIES (h, &blocked);
      break;
    case STATE_REPLY_CHUNK_REPLY_RESYNC:
      r = nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RESYNC (h, &blocked);
      break;
    case STATE_REPLY_CHUNK_REPLY_FINISH:
      r = nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_FINISH (h, &blocked);
      break;
    case STATE_REPLY_FINISH_COMMAND:
      r = nbd_internal_enter_STATE_REPLY_FINISH_COMMAND (h, &blocked);
      break;
    case STATE_DEAD:
      r = nbd_internal_enter_STATE_DEAD (h, &blocked);
      break;
    case STATE_CLOSED:
      r = nbd_internal_enter_STATE_CLOSED (h, &blocked);
      break;
    default:
      abort (); /* Should never happen, but keeps GCC happy. */
    }

    if (r == -1) {
      assert (nbd_get_error () != NULL);
      return -1;
    }
  } while (!blocked);
  return 0;
}

/* Returns whether in the given state read or write would be valid.
 * NB: is_locked = false, may_set_error = false.
 */
int
nbd_internal_aio_get_direction (enum state state)
{
  int r = 0;

  switch (state)
  {
  case STATE_START:
    break;
  case STATE_CONNECT_START:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_CONNECT_CONNECTING:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_CONNECT_TCP_START:
    break;
  case STATE_CONNECT_TCP_CONNECT:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_CONNECT_TCP_CONNECTING:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_CONNECT_TCP_NEXT_ADDRESS:
    break;
  case STATE_CONNECT_COMMAND_START:
    break;
  case STATE_CONNECT_SA_START:
    break;
  case STATE_MAGIC_START:
    break;
  case STATE_MAGIC_RECV_MAGIC:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_MAGIC_CHECK_MAGIC:
    break;
  case STATE_OLDSTYLE_START:
    break;
  case STATE_OLDSTYLE_RECV_REMAINING:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_OLDSTYLE_CHECK:
    break;
  case STATE_NEWSTYLE_START:
    break;
  case STATE_NEWSTYLE_RECV_GFLAGS:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_CHECK_GFLAGS:
    break;
  case STATE_NEWSTYLE_SEND_CFLAGS:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_START:
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_SEND:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY_PAYLOAD:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_CHECK_REPLY:
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE:
    break;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START:
    break;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_SEND:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY_PAYLOAD:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_CHECK_REPLY:
    break;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START:
    break;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_SEND:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY_PAYLOAD:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_CHECK_REPLY:
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_START:
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAMELEN:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAME:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_NRQUERIES:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY:
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERYLEN:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERY:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY:
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY_PAYLOAD:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_CHECK_REPLY:
    break;
  case STATE_NEWSTYLE_OPT_GO_START:
    break;
  case STATE_NEWSTYLE_OPT_GO_SEND:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_GO_SEND_EXPORTNAMELEN:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_GO_SEND_EXPORT:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_GO_SEND_NRINFOS:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_GO_SEND_INFO:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_GO_RECV_REPLY:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_OPT_GO_RECV_REPLY_PAYLOAD:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_OPT_GO_CHECK_REPLY:
    break;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_START:
    break;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND_EXPORT:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_RECV_REPLY:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_CHECK_REPLY:
    break;
  case STATE_NEWSTYLE_OPT_LIST_START:
    break;
  case STATE_NEWSTYLE_OPT_LIST_SEND:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_OPT_LIST_RECV_REPLY:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_OPT_LIST_RECV_REPLY_PAYLOAD:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_NEWSTYLE_OPT_LIST_CHECK_REPLY:
    break;
  case STATE_NEWSTYLE_PREPARE_OPT_ABORT:
    break;
  case STATE_NEWSTYLE_SEND_OPT_ABORT:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_SEND_OPTION_SHUTDOWN:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    break;
  case STATE_NEWSTYLE_FINISHED:
    break;
  case STATE_NEGOTIATING:
    break;
  case STATE_READY:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_ISSUE_COMMAND_START:
    break;
  case STATE_ISSUE_COMMAND_SEND_REQUEST:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_ISSUE_COMMAND_PAUSE_SEND_REQUEST:
    break;
  case STATE_ISSUE_COMMAND_PREPARE_WRITE_PAYLOAD:
    break;
  case STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_ISSUE_COMMAND_PAUSE_WRITE_PAYLOAD:
    break;
  case STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN:
    r |= LIBNBD_AIO_DIRECTION_WRITE;
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_ISSUE_COMMAND_PAUSE_WRITE_SHUTDOWN:
    break;
  case STATE_ISSUE_COMMAND_FINISH:
    break;
  case STATE_REPLY_START:
    r |= LIBNBD_AIO_DIRECTION_READ;
    break;
  case STATE_REPLY_RECV_REPLY:
    break;
  case STATE_REPLY_CHECK_REPLY_MAGIC:
    break;
  case STATE_REPLY_RECV_STRUCTURED_REMAINING:
    break;
  case STATE_REPLY_SIMPLE_REPLY_START:
    break;
  case STATE_REPLY_SIMPLE_REPLY_RECV_READ_PAYLOAD:
    break;
  case STATE_REPLY_CHUNK_REPLY_START:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR_MESSAGE:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR_TAIL:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA_DATA:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_HOLE:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_BS_HEADER:
    break;
  case STATE_REPLY_CHUNK_REPLY_RECV_BS_ENTRIES:
    break;
  case STATE_REPLY_CHUNK_REPLY_RESYNC:
    break;
  case STATE_REPLY_CHUNK_REPLY_FINISH:
    break;
  case STATE_REPLY_FINISH_COMMAND:
    break;
  case STATE_DEAD:
    break;
  case STATE_CLOSED:
    break;
  }

  return r;
}

/* Other functions associated with the state machine. */
const char *
nbd_internal_state_short_string (enum state state)
{
  switch (state)
  {
  case STATE_START:
    return "START";
  case STATE_CONNECT_START:
    return "CONNECT.START";
  case STATE_CONNECT_CONNECTING:
    return "CONNECT.CONNECTING";
  case STATE_CONNECT_TCP_START:
    return "CONNECT_TCP.START";
  case STATE_CONNECT_TCP_CONNECT:
    return "CONNECT_TCP.CONNECT";
  case STATE_CONNECT_TCP_CONNECTING:
    return "CONNECT_TCP.CONNECTING";
  case STATE_CONNECT_TCP_NEXT_ADDRESS:
    return "CONNECT_TCP.NEXT_ADDRESS";
  case STATE_CONNECT_COMMAND_START:
    return "CONNECT_COMMAND.START";
  case STATE_CONNECT_SA_START:
    return "CONNECT_SA.START";
  case STATE_MAGIC_START:
    return "MAGIC.START";
  case STATE_MAGIC_RECV_MAGIC:
    return "MAGIC.RECV_MAGIC";
  case STATE_MAGIC_CHECK_MAGIC:
    return "MAGIC.CHECK_MAGIC";
  case STATE_OLDSTYLE_START:
    return "OLDSTYLE.START";
  case STATE_OLDSTYLE_RECV_REMAINING:
    return "OLDSTYLE.RECV_REMAINING";
  case STATE_OLDSTYLE_CHECK:
    return "OLDSTYLE.CHECK";
  case STATE_NEWSTYLE_START:
    return "NEWSTYLE.START";
  case STATE_NEWSTYLE_RECV_GFLAGS:
    return "NEWSTYLE.RECV_GFLAGS";
  case STATE_NEWSTYLE_CHECK_GFLAGS:
    return "NEWSTYLE.CHECK_GFLAGS";
  case STATE_NEWSTYLE_SEND_CFLAGS:
    return "NEWSTYLE.SEND_CFLAGS";
  case STATE_NEWSTYLE_OPT_STARTTLS_START:
    return "NEWSTYLE.OPT_STARTTLS.START";
  case STATE_NEWSTYLE_OPT_STARTTLS_SEND:
    return "NEWSTYLE.OPT_STARTTLS.SEND";
  case STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY:
    return "NEWSTYLE.OPT_STARTTLS.RECV_REPLY";
  case STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY_PAYLOAD:
    return "NEWSTYLE.OPT_STARTTLS.RECV_REPLY_PAYLOAD";
  case STATE_NEWSTYLE_OPT_STARTTLS_CHECK_REPLY:
    return "NEWSTYLE.OPT_STARTTLS.CHECK_REPLY";
  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ:
    return "NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_READ";
  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE:
    return "NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_WRITE";
  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE:
    return "NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_DONE";
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START:
    return "NEWSTYLE.OPT_EXTENDED_HEADERS.START";
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_SEND:
    return "NEWSTYLE.OPT_EXTENDED_HEADERS.SEND";
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY:
    return "NEWSTYLE.OPT_EXTENDED_HEADERS.RECV_REPLY";
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY_PAYLOAD:
    return "NEWSTYLE.OPT_EXTENDED_HEADERS.RECV_REPLY_PAYLOAD";
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_CHECK_REPLY:
    return "NEWSTYLE.OPT_EXTENDED_HEADERS.CHECK_REPLY";
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START:
    return "NEWSTYLE.OPT_STRUCTURED_REPLY.START";
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_SEND:
    return "NEWSTYLE.OPT_STRUCTURED_REPLY.SEND";
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY:
    return "NEWSTYLE.OPT_STRUCTURED_REPLY.RECV_REPLY";
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY_PAYLOAD:
    return "NEWSTYLE.OPT_STRUCTURED_REPLY.RECV_REPLY_PAYLOAD";
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_CHECK_REPLY:
    return "NEWSTYLE.OPT_STRUCTURED_REPLY.CHECK_REPLY";
  case STATE_NEWSTYLE_OPT_META_CONTEXT_START:
    return "NEWSTYLE.OPT_META_CONTEXT.START";
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND:
    return "NEWSTYLE.OPT_META_CONTEXT.SEND";
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAMELEN:
    return "NEWSTYLE.OPT_META_CONTEXT.SEND_EXPORTNAMELEN";
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAME:
    return "NEWSTYLE.OPT_META_CONTEXT.SEND_EXPORTNAME";
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_NRQUERIES:
    return "NEWSTYLE.OPT_META_CONTEXT.SEND_NRQUERIES";
  case STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY:
    return "NEWSTYLE.OPT_META_CONTEXT.PREPARE_NEXT_QUERY";
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERYLEN:
    return "NEWSTYLE.OPT_META_CONTEXT.SEND_QUERYLEN";
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERY:
    return "NEWSTYLE.OPT_META_CONTEXT.SEND_QUERY";
  case STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY:
    return "NEWSTYLE.OPT_META_CONTEXT.PREPARE_FOR_REPLY";
  case STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY:
    return "NEWSTYLE.OPT_META_CONTEXT.RECV_REPLY";
  case STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY_PAYLOAD:
    return "NEWSTYLE.OPT_META_CONTEXT.RECV_REPLY_PAYLOAD";
  case STATE_NEWSTYLE_OPT_META_CONTEXT_CHECK_REPLY:
    return "NEWSTYLE.OPT_META_CONTEXT.CHECK_REPLY";
  case STATE_NEWSTYLE_OPT_GO_START:
    return "NEWSTYLE.OPT_GO.START";
  case STATE_NEWSTYLE_OPT_GO_SEND:
    return "NEWSTYLE.OPT_GO.SEND";
  case STATE_NEWSTYLE_OPT_GO_SEND_EXPORTNAMELEN:
    return "NEWSTYLE.OPT_GO.SEND_EXPORTNAMELEN";
  case STATE_NEWSTYLE_OPT_GO_SEND_EXPORT:
    return "NEWSTYLE.OPT_GO.SEND_EXPORT";
  case STATE_NEWSTYLE_OPT_GO_SEND_NRINFOS:
    return "NEWSTYLE.OPT_GO.SEND_NRINFOS";
  case STATE_NEWSTYLE_OPT_GO_SEND_INFO:
    return "NEWSTYLE.OPT_GO.SEND_INFO";
  case STATE_NEWSTYLE_OPT_GO_RECV_REPLY:
    return "NEWSTYLE.OPT_GO.RECV_REPLY";
  case STATE_NEWSTYLE_OPT_GO_RECV_REPLY_PAYLOAD:
    return "NEWSTYLE.OPT_GO.RECV_REPLY_PAYLOAD";
  case STATE_NEWSTYLE_OPT_GO_CHECK_REPLY:
    return "NEWSTYLE.OPT_GO.CHECK_REPLY";
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_START:
    return "NEWSTYLE.OPT_EXPORT_NAME.START";
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND:
    return "NEWSTYLE.OPT_EXPORT_NAME.SEND";
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND_EXPORT:
    return "NEWSTYLE.OPT_EXPORT_NAME.SEND_EXPORT";
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_RECV_REPLY:
    return "NEWSTYLE.OPT_EXPORT_NAME.RECV_REPLY";
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_CHECK_REPLY:
    return "NEWSTYLE.OPT_EXPORT_NAME.CHECK_REPLY";
  case STATE_NEWSTYLE_OPT_LIST_START:
    return "NEWSTYLE.OPT_LIST.START";
  case STATE_NEWSTYLE_OPT_LIST_SEND:
    return "NEWSTYLE.OPT_LIST.SEND";
  case STATE_NEWSTYLE_OPT_LIST_RECV_REPLY:
    return "NEWSTYLE.OPT_LIST.RECV_REPLY";
  case STATE_NEWSTYLE_OPT_LIST_RECV_REPLY_PAYLOAD:
    return "NEWSTYLE.OPT_LIST.RECV_REPLY_PAYLOAD";
  case STATE_NEWSTYLE_OPT_LIST_CHECK_REPLY:
    return "NEWSTYLE.OPT_LIST.CHECK_REPLY";
  case STATE_NEWSTYLE_PREPARE_OPT_ABORT:
    return "NEWSTYLE.PREPARE_OPT_ABORT";
  case STATE_NEWSTYLE_SEND_OPT_ABORT:
    return "NEWSTYLE.SEND_OPT_ABORT";
  case STATE_NEWSTYLE_SEND_OPTION_SHUTDOWN:
    return "NEWSTYLE.SEND_OPTION_SHUTDOWN";
  case STATE_NEWSTYLE_FINISHED:
    return "NEWSTYLE.FINISHED";
  case STATE_NEGOTIATING:
    return "NEGOTIATING";
  case STATE_READY:
    return "READY";
  case STATE_ISSUE_COMMAND_START:
    return "ISSUE_COMMAND.START";
  case STATE_ISSUE_COMMAND_SEND_REQUEST:
    return "ISSUE_COMMAND.SEND_REQUEST";
  case STATE_ISSUE_COMMAND_PAUSE_SEND_REQUEST:
    return "ISSUE_COMMAND.PAUSE_SEND_REQUEST";
  case STATE_ISSUE_COMMAND_PREPARE_WRITE_PAYLOAD:
    return "ISSUE_COMMAND.PREPARE_WRITE_PAYLOAD";
  case STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD:
    return "ISSUE_COMMAND.SEND_WRITE_PAYLOAD";
  case STATE_ISSUE_COMMAND_PAUSE_WRITE_PAYLOAD:
    return "ISSUE_COMMAND.PAUSE_WRITE_PAYLOAD";
  case STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN:
    return "ISSUE_COMMAND.SEND_WRITE_SHUTDOWN";
  case STATE_ISSUE_COMMAND_PAUSE_WRITE_SHUTDOWN:
    return "ISSUE_COMMAND.PAUSE_WRITE_SHUTDOWN";
  case STATE_ISSUE_COMMAND_FINISH:
    return "ISSUE_COMMAND.FINISH";
  case STATE_REPLY_START:
    return "REPLY.START";
  case STATE_REPLY_RECV_REPLY:
    return "REPLY.RECV_REPLY";
  case STATE_REPLY_CHECK_REPLY_MAGIC:
    return "REPLY.CHECK_REPLY_MAGIC";
  case STATE_REPLY_RECV_STRUCTURED_REMAINING:
    return "REPLY.RECV_STRUCTURED_REMAINING";
  case STATE_REPLY_SIMPLE_REPLY_START:
    return "REPLY.SIMPLE_REPLY.START";
  case STATE_REPLY_SIMPLE_REPLY_RECV_READ_PAYLOAD:
    return "REPLY.SIMPLE_REPLY.RECV_READ_PAYLOAD";
  case STATE_REPLY_CHUNK_REPLY_START:
    return "REPLY.CHUNK_REPLY.START";
  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR:
    return "REPLY.CHUNK_REPLY.RECV_ERROR";
  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR_MESSAGE:
    return "REPLY.CHUNK_REPLY.RECV_ERROR_MESSAGE";
  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR_TAIL:
    return "REPLY.CHUNK_REPLY.RECV_ERROR_TAIL";
  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA:
    return "REPLY.CHUNK_REPLY.RECV_OFFSET_DATA";
  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA_DATA:
    return "REPLY.CHUNK_REPLY.RECV_OFFSET_DATA_DATA";
  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_HOLE:
    return "REPLY.CHUNK_REPLY.RECV_OFFSET_HOLE";
  case STATE_REPLY_CHUNK_REPLY_RECV_BS_HEADER:
    return "REPLY.CHUNK_REPLY.RECV_BS_HEADER";
  case STATE_REPLY_CHUNK_REPLY_RECV_BS_ENTRIES:
    return "REPLY.CHUNK_REPLY.RECV_BS_ENTRIES";
  case STATE_REPLY_CHUNK_REPLY_RESYNC:
    return "REPLY.CHUNK_REPLY.RESYNC";
  case STATE_REPLY_CHUNK_REPLY_FINISH:
    return "REPLY.CHUNK_REPLY.FINISH";
  case STATE_REPLY_FINISH_COMMAND:
    return "REPLY.FINISH_COMMAND";
  case STATE_DEAD:
    return "DEAD";
  case STATE_CLOSED:
    return "CLOSED";
  }

  /* This function is only used for debug messages, and
   * this should never happen.
   */
  return "UNKNOWN!";
}

const char *
nbd_unlocked_connection_state (struct nbd_handle *h)
{
  switch (get_next_state (h))
  {
  case STATE_START:
    return "START" ": "
           "Handle after being initially created";

  case STATE_CONNECT_START:
    return "CONNECT.START" ": "
           "Initial call to connect(2) on the socket";

  case STATE_CONNECT_CONNECTING:
    return "CONNECT.CONNECTING" ": "
           "Connecting to the remote server";

  case STATE_CONNECT_TCP_START:
    return "CONNECT_TCP.START" ": "
           "Connect to a remote TCP server";

  case STATE_CONNECT_TCP_CONNECT:
    return "CONNECT_TCP.CONNECT" ": "
           "Initial call to connect(2) on a TCP socket";

  case STATE_CONNECT_TCP_CONNECTING:
    return "CONNECT_TCP.CONNECTING" ": "
           "Connecting to the remote server over a TCP socket";

  case STATE_CONNECT_TCP_NEXT_ADDRESS:
    return "CONNECT_TCP.NEXT_ADDRESS" ": "
           "Connecting to the next address over a TCP socket";

  case STATE_CONNECT_COMMAND_START:
    return "CONNECT_COMMAND.START" ": "
           "Connect to a subprocess";

  case STATE_CONNECT_SA_START:
    return "CONNECT_SA.START" ": "
           "Connect to a subprocess with systemd socket activation";

  case STATE_MAGIC_START:
    return "MAGIC.START" ": "
           "Prepare to receive the magic identification from remote";

  case STATE_MAGIC_RECV_MAGIC:
    return "MAGIC.RECV_MAGIC" ": "
           "Receive initial magic identification from remote";

  case STATE_MAGIC_CHECK_MAGIC:
    return "MAGIC.CHECK_MAGIC" ": "
           "Check magic and version sent by remote";

  case STATE_OLDSTYLE_START:
    return "OLDSTYLE.START" ": "
           "Prepare to receive remainder of oldstyle header";

  case STATE_OLDSTYLE_RECV_REMAINING:
    return "OLDSTYLE.RECV_REMAINING" ": "
           "Receive remainder of oldstyle header";

  case STATE_OLDSTYLE_CHECK:
    return "OLDSTYLE.CHECK" ": "
           "Check oldstyle header";

  case STATE_NEWSTYLE_START:
    return "NEWSTYLE.START" ": "
           "Prepare to receive newstyle gflags from remote";

  case STATE_NEWSTYLE_RECV_GFLAGS:
    return "NEWSTYLE.RECV_GFLAGS" ": "
           "Receive newstyle gflags from remote";

  case STATE_NEWSTYLE_CHECK_GFLAGS:
    return "NEWSTYLE.CHECK_GFLAGS" ": "
           "Check global flags sent by remote";

  case STATE_NEWSTYLE_SEND_CFLAGS:
    return "NEWSTYLE.SEND_CFLAGS" ": "
           "Send newstyle client flags to remote";

  case STATE_NEWSTYLE_OPT_STARTTLS_START:
    return "NEWSTYLE.OPT_STARTTLS.START" ": "
           "Try to send newstyle NBD_OPT_STARTTLS to upgrade to TLS";

  case STATE_NEWSTYLE_OPT_STARTTLS_SEND:
    return "NEWSTYLE.OPT_STARTTLS.SEND" ": "
           "Send newstyle NBD_OPT_STARTTLS to upgrade to TLS";

  case STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY:
    return "NEWSTYLE.OPT_STARTTLS.RECV_REPLY" ": "
           "Receive newstyle NBD_OPT_STARTTLS reply";

  case STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY_PAYLOAD:
    return "NEWSTYLE.OPT_STARTTLS.RECV_REPLY_PAYLOAD" ": "
           "Receive any newstyle NBD_OPT_STARTTLS reply payload";

  case STATE_NEWSTYLE_OPT_STARTTLS_CHECK_REPLY:
    return "NEWSTYLE.OPT_STARTTLS.CHECK_REPLY" ": "
           "Check newstyle NBD_OPT_STARTTLS reply";

  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ:
    return "NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_READ" ": "
           "TLS handshake (reading)";

  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE:
    return "NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_WRITE" ": "
           "TLS handshake (writing)";

  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE:
    return "NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_DONE" ": "
           "TLS handshake complete";

  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START:
    return "NEWSTYLE.OPT_EXTENDED_HEADERS.START" ": "
           "Try to negotiate newstyle NBD_OPT_EXTENDED_HEADERS";

  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_SEND:
    return "NEWSTYLE.OPT_EXTENDED_HEADERS.SEND" ": "
           "Send newstyle NBD_OPT_EXTENDED_HEADERS negotiation request";

  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY:
    return "NEWSTYLE.OPT_EXTENDED_HEADERS.RECV_REPLY" ": "
           "Receive newstyle NBD_OPT_EXTENDED_HEADERS option reply";

  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY_PAYLOAD:
    return "NEWSTYLE.OPT_EXTENDED_HEADERS.RECV_REPLY_PAYLOAD" ": "
           "Receive any newstyle NBD_OPT_EXTENDED_HEADERS reply payload";

  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_CHECK_REPLY:
    return "NEWSTYLE.OPT_EXTENDED_HEADERS.CHECK_REPLY" ": "
           "Check newstyle NBD_OPT_EXTENDED_HEADERS option reply";

  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START:
    return "NEWSTYLE.OPT_STRUCTURED_REPLY.START" ": "
           "Try to negotiate newstyle NBD_OPT_STRUCTURED_REPLY";

  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_SEND:
    return "NEWSTYLE.OPT_STRUCTURED_REPLY.SEND" ": "
           "Send newstyle NBD_OPT_STRUCTURED_REPLY negotiation request";

  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY:
    return "NEWSTYLE.OPT_STRUCTURED_REPLY.RECV_REPLY" ": "
           "Receive newstyle NBD_OPT_STRUCTURED_REPLY option reply";

  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY_PAYLOAD:
    return "NEWSTYLE.OPT_STRUCTURED_REPLY.RECV_REPLY_PAYLOAD" ": "
           "Receive any newstyle NBD_OPT_STRUCTURED_REPLY reply payload";

  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_CHECK_REPLY:
    return "NEWSTYLE.OPT_STRUCTURED_REPLY.CHECK_REPLY" ": "
           "Check newstyle NBD_OPT_STRUCTURED_REPLY option reply";

  case STATE_NEWSTYLE_OPT_META_CONTEXT_START:
    return "NEWSTYLE.OPT_META_CONTEXT.START" ": "
           "Try to negotiate newstyle NBD_OPT_SET_META_CONTEXT";

  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND:
    return "NEWSTYLE.OPT_META_CONTEXT.SEND" ": "
           "Send newstyle NBD_OPT_SET_META_CONTEXT";

  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAMELEN:
    return "NEWSTYLE.OPT_META_CONTEXT.SEND_EXPORTNAMELEN" ": "
           "Send newstyle NBD_OPT_SET_META_CONTEXT export name length";

  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAME:
    return "NEWSTYLE.OPT_META_CONTEXT.SEND_EXPORTNAME" ": "
           "Send newstyle NBD_OPT_SET_META_CONTEXT export name";

  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_NRQUERIES:
    return "NEWSTYLE.OPT_META_CONTEXT.SEND_NRQUERIES" ": "
           "Send newstyle NBD_OPT_SET_META_CONTEXT number of queries";

  case STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY:
    return "NEWSTYLE.OPT_META_CONTEXT.PREPARE_NEXT_QUERY" ": "
           "Prepare to send newstyle NBD_OPT_SET_META_CONTEXT query";

  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERYLEN:
    return "NEWSTYLE.OPT_META_CONTEXT.SEND_QUERYLEN" ": "
           "Send newstyle NBD_OPT_SET_META_CONTEXT query length";

  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERY:
    return "NEWSTYLE.OPT_META_CONTEXT.SEND_QUERY" ": "
           "Send newstyle NBD_OPT_SET_META_CONTEXT query";

  case STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY:
    return "NEWSTYLE.OPT_META_CONTEXT.PREPARE_FOR_REPLY" ": "
           "Prepare to receive newstyle NBD_OPT_SET_META_CONTEXT option reply";

  case STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY:
    return "NEWSTYLE.OPT_META_CONTEXT.RECV_REPLY" ": "
           "Receive newstyle NBD_OPT_SET_META_CONTEXT option reply";

  case STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY_PAYLOAD:
    return "NEWSTYLE.OPT_META_CONTEXT.RECV_REPLY_PAYLOAD" ": "
           "Receive newstyle NBD_OPT_SET_META_CONTEXT option reply payload";

  case STATE_NEWSTYLE_OPT_META_CONTEXT_CHECK_REPLY:
    return "NEWSTYLE.OPT_META_CONTEXT.CHECK_REPLY" ": "
           "Check newstyle NBD_OPT_SET_META_CONTEXT option reply";

  case STATE_NEWSTYLE_OPT_GO_START:
    return "NEWSTYLE.OPT_GO.START" ": "
           "Try to send newstyle NBD_OPT_GO to end handshake";

  case STATE_NEWSTYLE_OPT_GO_SEND:
    return "NEWSTYLE.OPT_GO.SEND" ": "
           "Send newstyle NBD_OPT_GO to end handshake";

  case STATE_NEWSTYLE_OPT_GO_SEND_EXPORTNAMELEN:
    return "NEWSTYLE.OPT_GO.SEND_EXPORTNAMELEN" ": "
           "Send newstyle NBD_OPT_GO export name length";

  case STATE_NEWSTYLE_OPT_GO_SEND_EXPORT:
    return "NEWSTYLE.OPT_GO.SEND_EXPORT" ": "
           "Send newstyle NBD_OPT_GO export name";

  case STATE_NEWSTYLE_OPT_GO_SEND_NRINFOS:
    return "NEWSTYLE.OPT_GO.SEND_NRINFOS" ": "
           "Send newstyle NBD_OPT_GO number of infos";

  case STATE_NEWSTYLE_OPT_GO_SEND_INFO:
    return "NEWSTYLE.OPT_GO.SEND_INFO" ": "
           "Send newstyle NBD_OPT_GO request for NBD_INFO_BLOCK_SIZE";

  case STATE_NEWSTYLE_OPT_GO_RECV_REPLY:
    return "NEWSTYLE.OPT_GO.RECV_REPLY" ": "
           "Receive newstyle NBD_OPT_GO reply";

  case STATE_NEWSTYLE_OPT_GO_RECV_REPLY_PAYLOAD:
    return "NEWSTYLE.OPT_GO.RECV_REPLY_PAYLOAD" ": "
           "Receive newstyle NBD_OPT_GO reply payload";

  case STATE_NEWSTYLE_OPT_GO_CHECK_REPLY:
    return "NEWSTYLE.OPT_GO.CHECK_REPLY" ": "
           "Check newstyle NBD_OPT_GO reply";

  case STATE_NEWSTYLE_OPT_EXPORT_NAME_START:
    return "NEWSTYLE.OPT_EXPORT_NAME.START" ": "
           "Try to send newstyle NBD_OPT_EXPORT_NAME to end handshake";

  case STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND:
    return "NEWSTYLE.OPT_EXPORT_NAME.SEND" ": "
           "Send newstyle NBD_OPT_EXPORT_NAME to end handshake";

  case STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND_EXPORT:
    return "NEWSTYLE.OPT_EXPORT_NAME.SEND_EXPORT" ": "
           "Send newstyle NBD_OPT_EXPORT_NAME export name";

  case STATE_NEWSTYLE_OPT_EXPORT_NAME_RECV_REPLY:
    return "NEWSTYLE.OPT_EXPORT_NAME.RECV_REPLY" ": "
           "Receive newstyle NBD_OPT_EXPORT_NAME reply";

  case STATE_NEWSTYLE_OPT_EXPORT_NAME_CHECK_REPLY:
    return "NEWSTYLE.OPT_EXPORT_NAME.CHECK_REPLY" ": "
           "Check newstyle NBD_OPT_EXPORT_NAME reply";

  case STATE_NEWSTYLE_OPT_LIST_START:
    return "NEWSTYLE.OPT_LIST.START" ": "
           "Start listing exports if in list mode.";

  case STATE_NEWSTYLE_OPT_LIST_SEND:
    return "NEWSTYLE.OPT_LIST.SEND" ": "
           "Send newstyle NBD_OPT_LIST to begin listing exports";

  case STATE_NEWSTYLE_OPT_LIST_RECV_REPLY:
    return "NEWSTYLE.OPT_LIST.RECV_REPLY" ": "
           "Receive NBD_REP_SERVER reply";

  case STATE_NEWSTYLE_OPT_LIST_RECV_REPLY_PAYLOAD:
    return "NEWSTYLE.OPT_LIST.RECV_REPLY_PAYLOAD" ": "
           "Receive NBD_REP_SERVER reply payload";

  case STATE_NEWSTYLE_OPT_LIST_CHECK_REPLY:
    return "NEWSTYLE.OPT_LIST.CHECK_REPLY" ": "
           "Check NBD_REP_SERVER reply";

  case STATE_NEWSTYLE_PREPARE_OPT_ABORT:
    return "NEWSTYLE.PREPARE_OPT_ABORT" ": "
           "Prepare to send NBD_OPT_ABORT";

  case STATE_NEWSTYLE_SEND_OPT_ABORT:
    return "NEWSTYLE.SEND_OPT_ABORT" ": "
           "Send NBD_OPT_ABORT to end negotiation";

  case STATE_NEWSTYLE_SEND_OPTION_SHUTDOWN:
    return "NEWSTYLE.SEND_OPTION_SHUTDOWN" ": "
           "Sending write shutdown notification to the remote server";

  case STATE_NEWSTYLE_FINISHED:
    return "NEWSTYLE.FINISHED" ": "
           "Finish off newstyle negotiation";

  case STATE_NEGOTIATING:
    return "NEGOTIATING" ": "
           "Connection is ready to negotiate an NBD option";

  case STATE_READY:
    return "READY" ": "
           "Connection is ready to process NBD commands";

  case STATE_ISSUE_COMMAND_START:
    return "ISSUE_COMMAND.START" ": "
           "Begin issuing a command to the remote server";

  case STATE_ISSUE_COMMAND_SEND_REQUEST:
    return "ISSUE_COMMAND.SEND_REQUEST" ": "
           "Sending a request to the remote server";

  case STATE_ISSUE_COMMAND_PAUSE_SEND_REQUEST:
    return "ISSUE_COMMAND.PAUSE_SEND_REQUEST" ": "
           "Interrupt send request to receive an earlier command's reply";

  case STATE_ISSUE_COMMAND_PREPARE_WRITE_PAYLOAD:
    return "ISSUE_COMMAND.PREPARE_WRITE_PAYLOAD" ": "
           "Prepare the write payload to send to the remote server";

  case STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD:
    return "ISSUE_COMMAND.SEND_WRITE_PAYLOAD" ": "
           "Sending the write payload to the remote server";

  case STATE_ISSUE_COMMAND_PAUSE_WRITE_PAYLOAD:
    return "ISSUE_COMMAND.PAUSE_WRITE_PAYLOAD" ": "
           "Interrupt write payload to receive an earlier command's reply";

  case STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN:
    return "ISSUE_COMMAND.SEND_WRITE_SHUTDOWN" ": "
           "Sending write shutdown notification to the remote server";

  case STATE_ISSUE_COMMAND_PAUSE_WRITE_SHUTDOWN:
    return "ISSUE_COMMAND.PAUSE_WRITE_SHUTDOWN" ": "
           "Interrupt write shutdown to receive an earlier command's reply";

  case STATE_ISSUE_COMMAND_FINISH:
    return "ISSUE_COMMAND.FINISH" ": "
           "Finish issuing a command";

  case STATE_REPLY_START:
    return "REPLY.START" ": "
           "Prepare to receive a reply from the remote server";

  case STATE_REPLY_RECV_REPLY:
    return "REPLY.RECV_REPLY" ": "
           "Receive a reply from the remote server";

  case STATE_REPLY_CHECK_REPLY_MAGIC:
    return "REPLY.CHECK_REPLY_MAGIC" ": "
           "Check if the reply has expected magic";

  case STATE_REPLY_RECV_STRUCTURED_REMAINING:
    return "REPLY.RECV_STRUCTURED_REMAINING" ": "
           "Receiving the remaining part of a structured reply header";

  case STATE_REPLY_SIMPLE_REPLY_START:
    return "REPLY.SIMPLE_REPLY.START" ": "
           "Parse a simple reply from the server";

  case STATE_REPLY_SIMPLE_REPLY_RECV_READ_PAYLOAD:
    return "REPLY.SIMPLE_REPLY.RECV_READ_PAYLOAD" ": "
           "Receiving the read payload for a simple reply";

  case STATE_REPLY_CHUNK_REPLY_START:
    return "REPLY.CHUNK_REPLY.START" ": "
           "Start parsing a chunk reply payload from the server";

  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR:
    return "REPLY.CHUNK_REPLY.RECV_ERROR" ": "
           "Receive a chunk reply error header";

  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR_MESSAGE:
    return "REPLY.CHUNK_REPLY.RECV_ERROR_MESSAGE" ": "
           "Receive a chunk reply error message";

  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR_TAIL:
    return "REPLY.CHUNK_REPLY.RECV_ERROR_TAIL" ": "
           "Receive a chunk reply error tail";

  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA:
    return "REPLY.CHUNK_REPLY.RECV_OFFSET_DATA" ": "
           "Receive a chunk reply offset-data header";

  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA_DATA:
    return "REPLY.CHUNK_REPLY.RECV_OFFSET_DATA_DATA" ": "
           "Receive a chunk reply offset-data block of data";

  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_HOLE:
    return "REPLY.CHUNK_REPLY.RECV_OFFSET_HOLE" ": "
           "Receive a chunk reply offset-hole header";

  case STATE_REPLY_CHUNK_REPLY_RECV_BS_HEADER:
    return "REPLY.CHUNK_REPLY.RECV_BS_HEADER" ": "
           "Receive header of a chunk reply block-status payload";

  case STATE_REPLY_CHUNK_REPLY_RECV_BS_ENTRIES:
    return "REPLY.CHUNK_REPLY.RECV_BS_ENTRIES" ": "
           "Receive entries array of chunk reply block-status payload";

  case STATE_REPLY_CHUNK_REPLY_RESYNC:
    return "REPLY.CHUNK_REPLY.RESYNC" ": "
           "Ignore payload of an unexpected chunk reply";

  case STATE_REPLY_CHUNK_REPLY_FINISH:
    return "REPLY.CHUNK_REPLY.FINISH" ": "
           "Finish receiving a chunk reply";

  case STATE_REPLY_FINISH_COMMAND:
    return "REPLY.FINISH_COMMAND" ": "
           "Finish receiving a command";

  case STATE_DEAD:
    return "DEAD" ": "
           "Connection is in an unrecoverable error state, can only be closed";

  case STATE_CLOSED:
    return "CLOSED" ": "
           "Connection is closed";

  }

  return NULL;
}

/* Map a state to its group name. */
enum state_group
nbd_internal_state_group (enum state state)
{
  switch (state) {
  case STATE_START:
    return GROUP_TOP;
  case STATE_CONNECT_START:
    return GROUP_CONNECT;
  case STATE_CONNECT_CONNECTING:
    return GROUP_CONNECT;
  case STATE_CONNECT_TCP_START:
    return GROUP_CONNECT_TCP;
  case STATE_CONNECT_TCP_CONNECT:
    return GROUP_CONNECT_TCP;
  case STATE_CONNECT_TCP_CONNECTING:
    return GROUP_CONNECT_TCP;
  case STATE_CONNECT_TCP_NEXT_ADDRESS:
    return GROUP_CONNECT_TCP;
  case STATE_CONNECT_COMMAND_START:
    return GROUP_CONNECT_COMMAND;
  case STATE_CONNECT_SA_START:
    return GROUP_CONNECT_SA;
  case STATE_MAGIC_START:
    return GROUP_MAGIC;
  case STATE_MAGIC_RECV_MAGIC:
    return GROUP_MAGIC;
  case STATE_MAGIC_CHECK_MAGIC:
    return GROUP_MAGIC;
  case STATE_OLDSTYLE_START:
    return GROUP_OLDSTYLE;
  case STATE_OLDSTYLE_RECV_REMAINING:
    return GROUP_OLDSTYLE;
  case STATE_OLDSTYLE_CHECK:
    return GROUP_OLDSTYLE;
  case STATE_NEWSTYLE_START:
    return GROUP_NEWSTYLE;
  case STATE_NEWSTYLE_RECV_GFLAGS:
    return GROUP_NEWSTYLE;
  case STATE_NEWSTYLE_CHECK_GFLAGS:
    return GROUP_NEWSTYLE;
  case STATE_NEWSTYLE_SEND_CFLAGS:
    return GROUP_NEWSTYLE;
  case STATE_NEWSTYLE_OPT_STARTTLS_START:
    return GROUP_NEWSTYLE_OPT_STARTTLS;
  case STATE_NEWSTYLE_OPT_STARTTLS_SEND:
    return GROUP_NEWSTYLE_OPT_STARTTLS;
  case STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY:
    return GROUP_NEWSTYLE_OPT_STARTTLS;
  case STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY_PAYLOAD:
    return GROUP_NEWSTYLE_OPT_STARTTLS;
  case STATE_NEWSTYLE_OPT_STARTTLS_CHECK_REPLY:
    return GROUP_NEWSTYLE_OPT_STARTTLS;
  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ:
    return GROUP_NEWSTYLE_OPT_STARTTLS;
  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE:
    return GROUP_NEWSTYLE_OPT_STARTTLS;
  case STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE:
    return GROUP_NEWSTYLE_OPT_STARTTLS;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START:
    return GROUP_NEWSTYLE_OPT_EXTENDED_HEADERS;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_SEND:
    return GROUP_NEWSTYLE_OPT_EXTENDED_HEADERS;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY:
    return GROUP_NEWSTYLE_OPT_EXTENDED_HEADERS;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY_PAYLOAD:
    return GROUP_NEWSTYLE_OPT_EXTENDED_HEADERS;
  case STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_CHECK_REPLY:
    return GROUP_NEWSTYLE_OPT_EXTENDED_HEADERS;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START:
    return GROUP_NEWSTYLE_OPT_STRUCTURED_REPLY;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_SEND:
    return GROUP_NEWSTYLE_OPT_STRUCTURED_REPLY;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY:
    return GROUP_NEWSTYLE_OPT_STRUCTURED_REPLY;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY_PAYLOAD:
    return GROUP_NEWSTYLE_OPT_STRUCTURED_REPLY;
  case STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_CHECK_REPLY:
    return GROUP_NEWSTYLE_OPT_STRUCTURED_REPLY;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_START:
    return GROUP_NEWSTYLE_OPT_META_CONTEXT;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND:
    return GROUP_NEWSTYLE_OPT_META_CONTEXT;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAMELEN:
    return GROUP_NEWSTYLE_OPT_META_CONTEXT;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAME:
    return GROUP_NEWSTYLE_OPT_META_CONTEXT;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_NRQUERIES:
    return GROUP_NEWSTYLE_OPT_META_CONTEXT;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY:
    return GROUP_NEWSTYLE_OPT_META_CONTEXT;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERYLEN:
    return GROUP_NEWSTYLE_OPT_META_CONTEXT;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERY:
    return GROUP_NEWSTYLE_OPT_META_CONTEXT;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY:
    return GROUP_NEWSTYLE_OPT_META_CONTEXT;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY:
    return GROUP_NEWSTYLE_OPT_META_CONTEXT;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY_PAYLOAD:
    return GROUP_NEWSTYLE_OPT_META_CONTEXT;
  case STATE_NEWSTYLE_OPT_META_CONTEXT_CHECK_REPLY:
    return GROUP_NEWSTYLE_OPT_META_CONTEXT;
  case STATE_NEWSTYLE_OPT_GO_START:
    return GROUP_NEWSTYLE_OPT_GO;
  case STATE_NEWSTYLE_OPT_GO_SEND:
    return GROUP_NEWSTYLE_OPT_GO;
  case STATE_NEWSTYLE_OPT_GO_SEND_EXPORTNAMELEN:
    return GROUP_NEWSTYLE_OPT_GO;
  case STATE_NEWSTYLE_OPT_GO_SEND_EXPORT:
    return GROUP_NEWSTYLE_OPT_GO;
  case STATE_NEWSTYLE_OPT_GO_SEND_NRINFOS:
    return GROUP_NEWSTYLE_OPT_GO;
  case STATE_NEWSTYLE_OPT_GO_SEND_INFO:
    return GROUP_NEWSTYLE_OPT_GO;
  case STATE_NEWSTYLE_OPT_GO_RECV_REPLY:
    return GROUP_NEWSTYLE_OPT_GO;
  case STATE_NEWSTYLE_OPT_GO_RECV_REPLY_PAYLOAD:
    return GROUP_NEWSTYLE_OPT_GO;
  case STATE_NEWSTYLE_OPT_GO_CHECK_REPLY:
    return GROUP_NEWSTYLE_OPT_GO;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_START:
    return GROUP_NEWSTYLE_OPT_EXPORT_NAME;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND:
    return GROUP_NEWSTYLE_OPT_EXPORT_NAME;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND_EXPORT:
    return GROUP_NEWSTYLE_OPT_EXPORT_NAME;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_RECV_REPLY:
    return GROUP_NEWSTYLE_OPT_EXPORT_NAME;
  case STATE_NEWSTYLE_OPT_EXPORT_NAME_CHECK_REPLY:
    return GROUP_NEWSTYLE_OPT_EXPORT_NAME;
  case STATE_NEWSTYLE_OPT_LIST_START:
    return GROUP_NEWSTYLE_OPT_LIST;
  case STATE_NEWSTYLE_OPT_LIST_SEND:
    return GROUP_NEWSTYLE_OPT_LIST;
  case STATE_NEWSTYLE_OPT_LIST_RECV_REPLY:
    return GROUP_NEWSTYLE_OPT_LIST;
  case STATE_NEWSTYLE_OPT_LIST_RECV_REPLY_PAYLOAD:
    return GROUP_NEWSTYLE_OPT_LIST;
  case STATE_NEWSTYLE_OPT_LIST_CHECK_REPLY:
    return GROUP_NEWSTYLE_OPT_LIST;
  case STATE_NEWSTYLE_PREPARE_OPT_ABORT:
    return GROUP_NEWSTYLE;
  case STATE_NEWSTYLE_SEND_OPT_ABORT:
    return GROUP_NEWSTYLE;
  case STATE_NEWSTYLE_SEND_OPTION_SHUTDOWN:
    return GROUP_NEWSTYLE;
  case STATE_NEWSTYLE_FINISHED:
    return GROUP_NEWSTYLE;
  case STATE_NEGOTIATING:
    return GROUP_TOP;
  case STATE_READY:
    return GROUP_TOP;
  case STATE_ISSUE_COMMAND_START:
    return GROUP_ISSUE_COMMAND;
  case STATE_ISSUE_COMMAND_SEND_REQUEST:
    return GROUP_ISSUE_COMMAND;
  case STATE_ISSUE_COMMAND_PAUSE_SEND_REQUEST:
    return GROUP_ISSUE_COMMAND;
  case STATE_ISSUE_COMMAND_PREPARE_WRITE_PAYLOAD:
    return GROUP_ISSUE_COMMAND;
  case STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD:
    return GROUP_ISSUE_COMMAND;
  case STATE_ISSUE_COMMAND_PAUSE_WRITE_PAYLOAD:
    return GROUP_ISSUE_COMMAND;
  case STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN:
    return GROUP_ISSUE_COMMAND;
  case STATE_ISSUE_COMMAND_PAUSE_WRITE_SHUTDOWN:
    return GROUP_ISSUE_COMMAND;
  case STATE_ISSUE_COMMAND_FINISH:
    return GROUP_ISSUE_COMMAND;
  case STATE_REPLY_START:
    return GROUP_REPLY;
  case STATE_REPLY_RECV_REPLY:
    return GROUP_REPLY;
  case STATE_REPLY_CHECK_REPLY_MAGIC:
    return GROUP_REPLY;
  case STATE_REPLY_RECV_STRUCTURED_REMAINING:
    return GROUP_REPLY;
  case STATE_REPLY_SIMPLE_REPLY_START:
    return GROUP_REPLY_SIMPLE_REPLY;
  case STATE_REPLY_SIMPLE_REPLY_RECV_READ_PAYLOAD:
    return GROUP_REPLY_SIMPLE_REPLY;
  case STATE_REPLY_CHUNK_REPLY_START:
    return GROUP_REPLY_CHUNK_REPLY;
  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR:
    return GROUP_REPLY_CHUNK_REPLY;
  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR_MESSAGE:
    return GROUP_REPLY_CHUNK_REPLY;
  case STATE_REPLY_CHUNK_REPLY_RECV_ERROR_TAIL:
    return GROUP_REPLY_CHUNK_REPLY;
  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA:
    return GROUP_REPLY_CHUNK_REPLY;
  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA_DATA:
    return GROUP_REPLY_CHUNK_REPLY;
  case STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_HOLE:
    return GROUP_REPLY_CHUNK_REPLY;
  case STATE_REPLY_CHUNK_REPLY_RECV_BS_HEADER:
    return GROUP_REPLY_CHUNK_REPLY;
  case STATE_REPLY_CHUNK_REPLY_RECV_BS_ENTRIES:
    return GROUP_REPLY_CHUNK_REPLY;
  case STATE_REPLY_CHUNK_REPLY_RESYNC:
    return GROUP_REPLY_CHUNK_REPLY;
  case STATE_REPLY_CHUNK_REPLY_FINISH:
    return GROUP_REPLY_CHUNK_REPLY;
  case STATE_REPLY_FINISH_COMMAND:
    return GROUP_REPLY;
  case STATE_DEAD:
    return GROUP_TOP;
  case STATE_CLOSED:
    return GROUP_TOP;
  default:
    abort (); /* Should never happen, but keeps GCC happy. */
  }
}

/* Map a state group to its parent group. */
enum state_group
nbd_internal_state_group_parent (enum state_group group)
{
  switch (group) {
  case GROUP_TOP:
    return GROUP_TOP;
  case GROUP_CONNECT:
    return GROUP_TOP;
  case GROUP_CONNECT_TCP:
    return GROUP_TOP;
  case GROUP_CONNECT_COMMAND:
    return GROUP_TOP;
  case GROUP_CONNECT_SA:
    return GROUP_TOP;
  case GROUP_MAGIC:
    return GROUP_TOP;
  case GROUP_OLDSTYLE:
    return GROUP_TOP;
  case GROUP_NEWSTYLE:
    return GROUP_TOP;
  case GROUP_NEWSTYLE_OPT_STARTTLS:
    return GROUP_NEWSTYLE;
  case GROUP_NEWSTYLE_OPT_EXTENDED_HEADERS:
    return GROUP_NEWSTYLE;
  case GROUP_NEWSTYLE_OPT_STRUCTURED_REPLY:
    return GROUP_NEWSTYLE;
  case GROUP_NEWSTYLE_OPT_META_CONTEXT:
    return GROUP_NEWSTYLE;
  case GROUP_NEWSTYLE_OPT_GO:
    return GROUP_NEWSTYLE;
  case GROUP_NEWSTYLE_OPT_EXPORT_NAME:
    return GROUP_NEWSTYLE;
  case GROUP_NEWSTYLE_OPT_LIST:
    return GROUP_NEWSTYLE;
  case GROUP_ISSUE_COMMAND:
    return GROUP_TOP;
  case GROUP_REPLY:
    return GROUP_TOP;
  case GROUP_REPLY_SIMPLE_REPLY:
    return GROUP_REPLY;
  case GROUP_REPLY_CHUNK_REPLY:
    return GROUP_REPLY;
  default:
    abort (); /* Should never happen, but keeps GCC happy. */
  }
};
