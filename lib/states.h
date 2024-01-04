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

enum state {
  /* START: Handle after being initially created */
  STATE_START,

  /* CONNECT.START: Initial call to connect(2) on the socket */
  STATE_CONNECT_START,

  /* CONNECT.CONNECTING: Connecting to the remote server */
  STATE_CONNECT_CONNECTING,

  /* CONNECT_TCP.START: Connect to a remote TCP server */
  STATE_CONNECT_TCP_START,

  /* CONNECT_TCP.CONNECT: Initial call to connect(2) on a TCP socket */
  STATE_CONNECT_TCP_CONNECT,

  /* CONNECT_TCP.CONNECTING: Connecting to the remote server over a TCP socket
   */
  STATE_CONNECT_TCP_CONNECTING,

  /* CONNECT_TCP.NEXT_ADDRESS: Connecting to the next address over a TCP socket
   */
  STATE_CONNECT_TCP_NEXT_ADDRESS,

  /* CONNECT_COMMAND.START: Connect to a subprocess */
  STATE_CONNECT_COMMAND_START,

  /* CONNECT_SA.START: Connect to a subprocess with systemd socket activation */
  STATE_CONNECT_SA_START,

  /* MAGIC.START: Prepare to receive the magic identification from remote */
  STATE_MAGIC_START,

  /* MAGIC.RECV_MAGIC: Receive initial magic identification from remote */
  STATE_MAGIC_RECV_MAGIC,

  /* MAGIC.CHECK_MAGIC: Check magic and version sent by remote */
  STATE_MAGIC_CHECK_MAGIC,

  /* OLDSTYLE.START: Prepare to receive remainder of oldstyle header */
  STATE_OLDSTYLE_START,

  /* OLDSTYLE.RECV_REMAINING: Receive remainder of oldstyle header */
  STATE_OLDSTYLE_RECV_REMAINING,

  /* OLDSTYLE.CHECK: Check oldstyle header */
  STATE_OLDSTYLE_CHECK,

  /* NEWSTYLE.START: Prepare to receive newstyle gflags from remote */
  STATE_NEWSTYLE_START,

  /* NEWSTYLE.RECV_GFLAGS: Receive newstyle gflags from remote */
  STATE_NEWSTYLE_RECV_GFLAGS,

  /* NEWSTYLE.CHECK_GFLAGS: Check global flags sent by remote */
  STATE_NEWSTYLE_CHECK_GFLAGS,

  /* NEWSTYLE.SEND_CFLAGS: Send newstyle client flags to remote */
  STATE_NEWSTYLE_SEND_CFLAGS,

  /* NEWSTYLE.OPT_STARTTLS.START: Try to send newstyle NBD_OPT_STARTTLS to
   * upgrade to TLS
   */
  STATE_NEWSTYLE_OPT_STARTTLS_START,

  /* NEWSTYLE.OPT_STARTTLS.SEND: Send newstyle NBD_OPT_STARTTLS to upgrade to
   * TLS
   */
  STATE_NEWSTYLE_OPT_STARTTLS_SEND,

  /* NEWSTYLE.OPT_STARTTLS.RECV_REPLY: Receive newstyle NBD_OPT_STARTTLS reply
   */
  STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY,

  /* NEWSTYLE.OPT_STARTTLS.RECV_REPLY_PAYLOAD: Receive any newstyle
   * NBD_OPT_STARTTLS reply payload
   */
  STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY_PAYLOAD,

  /* NEWSTYLE.OPT_STARTTLS.CHECK_REPLY: Check newstyle NBD_OPT_STARTTLS reply */
  STATE_NEWSTYLE_OPT_STARTTLS_CHECK_REPLY,

  /* NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_READ: TLS handshake (reading) */
  STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ,

  /* NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_WRITE: TLS handshake (writing) */
  STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE,

  /* NEWSTYLE.OPT_STARTTLS.TLS_HANDSHAKE_DONE: TLS handshake complete */
  STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE,

  /* NEWSTYLE.OPT_EXTENDED_HEADERS.START: Try to negotiate newstyle
   * NBD_OPT_EXTENDED_HEADERS
   */
  STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START,

  /* NEWSTYLE.OPT_EXTENDED_HEADERS.SEND: Send newstyle NBD_OPT_EXTENDED_HEADERS
   * negotiation request
   */
  STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_SEND,

  /* NEWSTYLE.OPT_EXTENDED_HEADERS.RECV_REPLY: Receive newstyle
   * NBD_OPT_EXTENDED_HEADERS option reply
   */
  STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY,

  /* NEWSTYLE.OPT_EXTENDED_HEADERS.RECV_REPLY_PAYLOAD: Receive any newstyle
   * NBD_OPT_EXTENDED_HEADERS reply payload
   */
  STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY_PAYLOAD,

  /* NEWSTYLE.OPT_EXTENDED_HEADERS.CHECK_REPLY: Check newstyle
   * NBD_OPT_EXTENDED_HEADERS option reply
   */
  STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_CHECK_REPLY,

  /* NEWSTYLE.OPT_STRUCTURED_REPLY.START: Try to negotiate newstyle
   * NBD_OPT_STRUCTURED_REPLY
   */
  STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START,

  /* NEWSTYLE.OPT_STRUCTURED_REPLY.SEND: Send newstyle NBD_OPT_STRUCTURED_REPLY
   * negotiation request
   */
  STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_SEND,

  /* NEWSTYLE.OPT_STRUCTURED_REPLY.RECV_REPLY: Receive newstyle
   * NBD_OPT_STRUCTURED_REPLY option reply
   */
  STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY,

  /* NEWSTYLE.OPT_STRUCTURED_REPLY.RECV_REPLY_PAYLOAD: Receive any newstyle
   * NBD_OPT_STRUCTURED_REPLY reply payload
   */
  STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY_PAYLOAD,

  /* NEWSTYLE.OPT_STRUCTURED_REPLY.CHECK_REPLY: Check newstyle
   * NBD_OPT_STRUCTURED_REPLY option reply
   */
  STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_CHECK_REPLY,

  /* NEWSTYLE.OPT_META_CONTEXT.START: Try to negotiate newstyle
   * NBD_OPT_SET_META_CONTEXT
   */
  STATE_NEWSTYLE_OPT_META_CONTEXT_START,

  /* NEWSTYLE.OPT_META_CONTEXT.SEND: Send newstyle NBD_OPT_SET_META_CONTEXT */
  STATE_NEWSTYLE_OPT_META_CONTEXT_SEND,

  /* NEWSTYLE.OPT_META_CONTEXT.SEND_EXPORTNAMELEN: Send newstyle
   * NBD_OPT_SET_META_CONTEXT export name length
   */
  STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAMELEN,

  /* NEWSTYLE.OPT_META_CONTEXT.SEND_EXPORTNAME: Send newstyle
   * NBD_OPT_SET_META_CONTEXT export name
   */
  STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAME,

  /* NEWSTYLE.OPT_META_CONTEXT.SEND_NRQUERIES: Send newstyle
   * NBD_OPT_SET_META_CONTEXT number of queries
   */
  STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_NRQUERIES,

  /* NEWSTYLE.OPT_META_CONTEXT.PREPARE_NEXT_QUERY: Prepare to send newstyle
   * NBD_OPT_SET_META_CONTEXT query
   */
  STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY,

  /* NEWSTYLE.OPT_META_CONTEXT.SEND_QUERYLEN: Send newstyle
   * NBD_OPT_SET_META_CONTEXT query length
   */
  STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERYLEN,

  /* NEWSTYLE.OPT_META_CONTEXT.SEND_QUERY: Send newstyle
   * NBD_OPT_SET_META_CONTEXT query
   */
  STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERY,

  /* NEWSTYLE.OPT_META_CONTEXT.PREPARE_FOR_REPLY: Prepare to receive newstyle
   * NBD_OPT_SET_META_CONTEXT option reply
   */
  STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY,

  /* NEWSTYLE.OPT_META_CONTEXT.RECV_REPLY: Receive newstyle
   * NBD_OPT_SET_META_CONTEXT option reply
   */
  STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY,

  /* NEWSTYLE.OPT_META_CONTEXT.RECV_REPLY_PAYLOAD: Receive newstyle
   * NBD_OPT_SET_META_CONTEXT option reply payload
   */
  STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY_PAYLOAD,

  /* NEWSTYLE.OPT_META_CONTEXT.CHECK_REPLY: Check newstyle
   * NBD_OPT_SET_META_CONTEXT option reply
   */
  STATE_NEWSTYLE_OPT_META_CONTEXT_CHECK_REPLY,

  /* NEWSTYLE.OPT_GO.START: Try to send newstyle NBD_OPT_GO to end handshake */
  STATE_NEWSTYLE_OPT_GO_START,

  /* NEWSTYLE.OPT_GO.SEND: Send newstyle NBD_OPT_GO to end handshake */
  STATE_NEWSTYLE_OPT_GO_SEND,

  /* NEWSTYLE.OPT_GO.SEND_EXPORTNAMELEN: Send newstyle NBD_OPT_GO export name
   * length
   */
  STATE_NEWSTYLE_OPT_GO_SEND_EXPORTNAMELEN,

  /* NEWSTYLE.OPT_GO.SEND_EXPORT: Send newstyle NBD_OPT_GO export name */
  STATE_NEWSTYLE_OPT_GO_SEND_EXPORT,

  /* NEWSTYLE.OPT_GO.SEND_NRINFOS: Send newstyle NBD_OPT_GO number of infos */
  STATE_NEWSTYLE_OPT_GO_SEND_NRINFOS,

  /* NEWSTYLE.OPT_GO.SEND_INFO: Send newstyle NBD_OPT_GO request for
   * NBD_INFO_BLOCK_SIZE
   */
  STATE_NEWSTYLE_OPT_GO_SEND_INFO,

  /* NEWSTYLE.OPT_GO.RECV_REPLY: Receive newstyle NBD_OPT_GO reply */
  STATE_NEWSTYLE_OPT_GO_RECV_REPLY,

  /* NEWSTYLE.OPT_GO.RECV_REPLY_PAYLOAD: Receive newstyle NBD_OPT_GO reply
   * payload
   */
  STATE_NEWSTYLE_OPT_GO_RECV_REPLY_PAYLOAD,

  /* NEWSTYLE.OPT_GO.CHECK_REPLY: Check newstyle NBD_OPT_GO reply */
  STATE_NEWSTYLE_OPT_GO_CHECK_REPLY,

  /* NEWSTYLE.OPT_EXPORT_NAME.START: Try to send newstyle NBD_OPT_EXPORT_NAME to
   * end handshake
   */
  STATE_NEWSTYLE_OPT_EXPORT_NAME_START,

  /* NEWSTYLE.OPT_EXPORT_NAME.SEND: Send newstyle NBD_OPT_EXPORT_NAME to end
   * handshake
   */
  STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND,

  /* NEWSTYLE.OPT_EXPORT_NAME.SEND_EXPORT: Send newstyle NBD_OPT_EXPORT_NAME
   * export name
   */
  STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND_EXPORT,

  /* NEWSTYLE.OPT_EXPORT_NAME.RECV_REPLY: Receive newstyle NBD_OPT_EXPORT_NAME
   * reply
   */
  STATE_NEWSTYLE_OPT_EXPORT_NAME_RECV_REPLY,

  /* NEWSTYLE.OPT_EXPORT_NAME.CHECK_REPLY: Check newstyle NBD_OPT_EXPORT_NAME
   * reply
   */
  STATE_NEWSTYLE_OPT_EXPORT_NAME_CHECK_REPLY,

  /* NEWSTYLE.OPT_LIST.START: Start listing exports if in list mode. */
  STATE_NEWSTYLE_OPT_LIST_START,

  /* NEWSTYLE.OPT_LIST.SEND: Send newstyle NBD_OPT_LIST to begin listing exports
   */
  STATE_NEWSTYLE_OPT_LIST_SEND,

  /* NEWSTYLE.OPT_LIST.RECV_REPLY: Receive NBD_REP_SERVER reply */
  STATE_NEWSTYLE_OPT_LIST_RECV_REPLY,

  /* NEWSTYLE.OPT_LIST.RECV_REPLY_PAYLOAD: Receive NBD_REP_SERVER reply payload
   */
  STATE_NEWSTYLE_OPT_LIST_RECV_REPLY_PAYLOAD,

  /* NEWSTYLE.OPT_LIST.CHECK_REPLY: Check NBD_REP_SERVER reply */
  STATE_NEWSTYLE_OPT_LIST_CHECK_REPLY,

  /* NEWSTYLE.PREPARE_OPT_ABORT: Prepare to send NBD_OPT_ABORT */
  STATE_NEWSTYLE_PREPARE_OPT_ABORT,

  /* NEWSTYLE.SEND_OPT_ABORT: Send NBD_OPT_ABORT to end negotiation */
  STATE_NEWSTYLE_SEND_OPT_ABORT,

  /* NEWSTYLE.SEND_OPTION_SHUTDOWN: Sending write shutdown notification to the
   * remote server
   */
  STATE_NEWSTYLE_SEND_OPTION_SHUTDOWN,

  /* NEWSTYLE.FINISHED: Finish off newstyle negotiation */
  STATE_NEWSTYLE_FINISHED,

  /* NEGOTIATING: Connection is ready to negotiate an NBD option */
  STATE_NEGOTIATING,

  /* READY: Connection is ready to process NBD commands */
  STATE_READY,

  /* ISSUE_COMMAND.START: Begin issuing a command to the remote server */
  STATE_ISSUE_COMMAND_START,

  /* ISSUE_COMMAND.SEND_REQUEST: Sending a request to the remote server */
  STATE_ISSUE_COMMAND_SEND_REQUEST,

  /* ISSUE_COMMAND.PAUSE_SEND_REQUEST: Interrupt send request to receive an
   * earlier command's reply
   */
  STATE_ISSUE_COMMAND_PAUSE_SEND_REQUEST,

  /* ISSUE_COMMAND.PREPARE_WRITE_PAYLOAD: Prepare the write payload to send to
   * the remote server
   */
  STATE_ISSUE_COMMAND_PREPARE_WRITE_PAYLOAD,

  /* ISSUE_COMMAND.SEND_WRITE_PAYLOAD: Sending the write payload to the remote
   * server
   */
  STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD,

  /* ISSUE_COMMAND.PAUSE_WRITE_PAYLOAD: Interrupt write payload to receive an
   * earlier command's reply
   */
  STATE_ISSUE_COMMAND_PAUSE_WRITE_PAYLOAD,

  /* ISSUE_COMMAND.SEND_WRITE_SHUTDOWN: Sending write shutdown notification to
   * the remote server
   */
  STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN,

  /* ISSUE_COMMAND.PAUSE_WRITE_SHUTDOWN: Interrupt write shutdown to receive an
   * earlier command's reply
   */
  STATE_ISSUE_COMMAND_PAUSE_WRITE_SHUTDOWN,

  /* ISSUE_COMMAND.FINISH: Finish issuing a command */
  STATE_ISSUE_COMMAND_FINISH,

  /* REPLY.START: Prepare to receive a reply from the remote server */
  STATE_REPLY_START,

  /* REPLY.RECV_REPLY: Receive a reply from the remote server */
  STATE_REPLY_RECV_REPLY,

  /* REPLY.CHECK_REPLY_MAGIC: Check if the reply has expected magic */
  STATE_REPLY_CHECK_REPLY_MAGIC,

  /* REPLY.RECV_STRUCTURED_REMAINING: Receiving the remaining part of a
   * structured reply header
   */
  STATE_REPLY_RECV_STRUCTURED_REMAINING,

  /* REPLY.SIMPLE_REPLY.START: Parse a simple reply from the server */
  STATE_REPLY_SIMPLE_REPLY_START,

  /* REPLY.SIMPLE_REPLY.RECV_READ_PAYLOAD: Receiving the read payload for a
   * simple reply
   */
  STATE_REPLY_SIMPLE_REPLY_RECV_READ_PAYLOAD,

  /* REPLY.CHUNK_REPLY.START: Start parsing a chunk reply payload from the
   * server
   */
  STATE_REPLY_CHUNK_REPLY_START,

  /* REPLY.CHUNK_REPLY.RECV_ERROR: Receive a chunk reply error header */
  STATE_REPLY_CHUNK_REPLY_RECV_ERROR,

  /* REPLY.CHUNK_REPLY.RECV_ERROR_MESSAGE: Receive a chunk reply error message
   */
  STATE_REPLY_CHUNK_REPLY_RECV_ERROR_MESSAGE,

  /* REPLY.CHUNK_REPLY.RECV_ERROR_TAIL: Receive a chunk reply error tail */
  STATE_REPLY_CHUNK_REPLY_RECV_ERROR_TAIL,

  /* REPLY.CHUNK_REPLY.RECV_OFFSET_DATA: Receive a chunk reply offset-data
   * header
   */
  STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA,

  /* REPLY.CHUNK_REPLY.RECV_OFFSET_DATA_DATA: Receive a chunk reply offset-data
   * block of data
   */
  STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA_DATA,

  /* REPLY.CHUNK_REPLY.RECV_OFFSET_HOLE: Receive a chunk reply offset-hole
   * header
   */
  STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_HOLE,

  /* REPLY.CHUNK_REPLY.RECV_BS_HEADER: Receive header of a chunk reply
   * block-status payload
   */
  STATE_REPLY_CHUNK_REPLY_RECV_BS_HEADER,

  /* REPLY.CHUNK_REPLY.RECV_BS_ENTRIES: Receive entries array of chunk reply
   * block-status payload
   */
  STATE_REPLY_CHUNK_REPLY_RECV_BS_ENTRIES,

  /* REPLY.CHUNK_REPLY.RESYNC: Ignore payload of an unexpected chunk reply */
  STATE_REPLY_CHUNK_REPLY_RESYNC,

  /* REPLY.CHUNK_REPLY.FINISH: Finish receiving a chunk reply */
  STATE_REPLY_CHUNK_REPLY_FINISH,

  /* REPLY.FINISH_COMMAND: Finish receiving a command */
  STATE_REPLY_FINISH_COMMAND,

  /* DEAD: Connection is in an unrecoverable error state, can only be closed */
  STATE_DEAD,

  /* CLOSED: Connection is closed */
  STATE_CLOSED,

};

/* These correspond to the external events in generator/generator. */
enum external_event {
  notify_read,
  notify_write,
  cmd_create,
  cmd_connect_sockaddr,
  cmd_connect_tcp,
  cmd_connect_command,
  cmd_connect_sa,
  cmd_connect_socket,
  cmd_issue,
};

/* State groups. */
enum state_group {
  GROUP_TOP,
  GROUP_CONNECT,
  GROUP_CONNECT_TCP,
  GROUP_CONNECT_COMMAND,
  GROUP_CONNECT_SA,
  GROUP_MAGIC,
  GROUP_OLDSTYLE,
  GROUP_NEWSTYLE,
  GROUP_NEWSTYLE_OPT_STARTTLS,
  GROUP_NEWSTYLE_OPT_EXTENDED_HEADERS,
  GROUP_NEWSTYLE_OPT_STRUCTURED_REPLY,
  GROUP_NEWSTYLE_OPT_META_CONTEXT,
  GROUP_NEWSTYLE_OPT_GO,
  GROUP_NEWSTYLE_OPT_EXPORT_NAME,
  GROUP_NEWSTYLE_OPT_LIST,
  GROUP_ISSUE_COMMAND,
  GROUP_REPLY,
  GROUP_REPLY_SIMPLE_REPLY,
  GROUP_REPLY_CHUNK_REPLY,
};

/* State transitions defined in states.c. */
extern int nbd_internal_enter_STATE_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_CONNECT_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_CONNECT_CONNECTING (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_CONNECT_TCP_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_CONNECT_TCP_CONNECT (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_CONNECT_TCP_CONNECTING (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_CONNECT_TCP_NEXT_ADDRESS (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_CONNECT_COMMAND_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_CONNECT_SA_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_MAGIC_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_MAGIC_RECV_MAGIC (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_MAGIC_CHECK_MAGIC (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_OLDSTYLE_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_OLDSTYLE_RECV_REMAINING (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_OLDSTYLE_CHECK (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_RECV_GFLAGS (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_CHECK_GFLAGS (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_SEND_CFLAGS (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_SEND (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_RECV_REPLY_PAYLOAD (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_CHECK_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_READ (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_WRITE (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_STARTTLS_TLS_HANDSHAKE_DONE (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_SEND (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_RECV_REPLY_PAYLOAD (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_EXTENDED_HEADERS_CHECK_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_SEND (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_RECV_REPLY_PAYLOAD (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_STRUCTURED_REPLY_CHECK_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAMELEN (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_EXPORTNAME (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_NRQUERIES (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_NEXT_QUERY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERYLEN (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_SEND_QUERY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_PREPARE_FOR_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_RECV_REPLY_PAYLOAD (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_META_CONTEXT_CHECK_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND_EXPORTNAMELEN (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND_EXPORT (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND_NRINFOS (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_SEND_INFO (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_RECV_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_RECV_REPLY_PAYLOAD (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_GO_CHECK_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_SEND_EXPORT (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_RECV_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_EXPORT_NAME_CHECK_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_SEND (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_RECV_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_RECV_REPLY_PAYLOAD (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_OPT_LIST_CHECK_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_PREPARE_OPT_ABORT (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_SEND_OPT_ABORT (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_SEND_OPTION_SHUTDOWN (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEWSTYLE_FINISHED (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_NEGOTIATING (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_READY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_ISSUE_COMMAND_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_ISSUE_COMMAND_SEND_REQUEST (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_ISSUE_COMMAND_PAUSE_SEND_REQUEST (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_ISSUE_COMMAND_PREPARE_WRITE_PAYLOAD (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_ISSUE_COMMAND_SEND_WRITE_PAYLOAD (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_ISSUE_COMMAND_PAUSE_WRITE_PAYLOAD (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_ISSUE_COMMAND_SEND_WRITE_SHUTDOWN (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_ISSUE_COMMAND_PAUSE_WRITE_SHUTDOWN (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_ISSUE_COMMAND_FINISH (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_RECV_REPLY (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_CHECK_REPLY_MAGIC (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_RECV_STRUCTURED_REMAINING (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_SIMPLE_REPLY_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_SIMPLE_REPLY_RECV_READ_PAYLOAD (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_START (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR_MESSAGE (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_ERROR_TAIL (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_DATA_DATA (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_OFFSET_HOLE (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_BS_HEADER (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RECV_BS_ENTRIES (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_RESYNC (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_CHUNK_REPLY_FINISH (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_REPLY_FINISH_COMMAND (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_DEAD (
             struct nbd_handle *h, bool *blocked
           );
extern int nbd_internal_enter_STATE_CLOSED (
             struct nbd_handle *h, bool *blocked
           );
