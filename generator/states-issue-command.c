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

STATE_MACHINE {
 ISSUE_COMMAND.START:
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
    SET_NEXT_STATE_AND_BLOCK (%SEND_WRITE_SHUTDOWN);
  else if (h->wlen) {
    if (h->in_write_payload)
      SET_NEXT_STATE_AND_BLOCK (%SEND_WRITE_PAYLOAD);
    else
      SET_NEXT_STATE_AND_BLOCK (%SEND_REQUEST);
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
  SET_NEXT_STATE (%SEND_REQUEST);
  return 0;

 ISSUE_COMMAND.SEND_REQUEST:
  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (%.DEAD); return 0;
  case 0:  SET_NEXT_STATE (%PREPARE_WRITE_PAYLOAD);
  }
  return 0;

 ISSUE_COMMAND.PAUSE_SEND_REQUEST:
  assert (h->wlen);
  assert (h->cmds_to_issue != NULL);
  h->in_write_payload = false;
  SET_NEXT_STATE (%^REPLY.START);
  return 0;

 ISSUE_COMMAND.PREPARE_WRITE_PAYLOAD:
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
    SET_NEXT_STATE (%SEND_WRITE_PAYLOAD);
  }
  else if (cmd->type == NBD_CMD_DISC) {
    h->in_write_shutdown = true;
    SET_NEXT_STATE (%SEND_WRITE_SHUTDOWN);
  }
  else
    SET_NEXT_STATE (%FINISH);
  return 0;

 ISSUE_COMMAND.SEND_WRITE_PAYLOAD:
  switch (send_from_wbuf (h)) {
  case -1: SET_NEXT_STATE (%.DEAD); return 0;
  case 0:  SET_NEXT_STATE (%FINISH);
  }
  return 0;

 ISSUE_COMMAND.PAUSE_WRITE_PAYLOAD:
  assert (h->wlen);
  assert (h->cmds_to_issue != NULL);
  h->in_write_payload = true;
  SET_NEXT_STATE (%^REPLY.START);
  return 0;

 ISSUE_COMMAND.SEND_WRITE_SHUTDOWN:
  if (h->sock->ops->shut_writes (h, h->sock))
    SET_NEXT_STATE (%FINISH);
  return 0;

 ISSUE_COMMAND.PAUSE_WRITE_SHUTDOWN:
  assert (h->in_write_shutdown);
  SET_NEXT_STATE (%^REPLY.START);
  return 0;

 ISSUE_COMMAND.FINISH:
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
  SET_NEXT_STATE (%.READY);
  return 0;

} /* END STATE MACHINE */
