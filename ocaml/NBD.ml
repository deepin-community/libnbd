(* NBD client library in userspace
 * WARNING: THIS FILE IS GENERATED FROM
 * generator/generator
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
 *)

exception Error of string * Unix.error option
exception Closed of string
type cookie = int64
type extent = int64 * int64

(* Give the exceptions names so that they can be raised from the C code. *)
let () =
  Callback.register_exception "nbd_internal_ocaml_error" (Error ("", None));
  Callback.register_exception "nbd_internal_ocaml_closed" (Closed "")

module TLS = struct
  type t =
  | DISABLE
  | ALLOW
  | REQUIRE
  | UNKNOWN of int
end

module SIZE = struct
  type t =
  | MINIMUM
  | PREFERRED
  | MAXIMUM
  | PAYLOAD
  | UNKNOWN of int
end

module CMD_FLAG = struct
  type t =
  | FUA
  | NO_HOLE
  | DF
  | REQ_ONE
  | FAST_ZERO
  | PAYLOAD_LEN
  | UNKNOWN of int

  let mask = [
    FUA;
    NO_HOLE;
    DF;
    REQ_ONE;
    FAST_ZERO;
    PAYLOAD_LEN;
  ]
end

module HANDSHAKE_FLAG = struct
  type t =
  | FIXED_NEWSTYLE
  | NO_ZEROES
  | UNKNOWN of int

  let mask = [
    FIXED_NEWSTYLE;
    NO_ZEROES;
  ]
end

module STRICT = struct
  type t =
  | COMMANDS
  | FLAGS
  | BOUNDS
  | ZERO_SIZE
  | ALIGN
  | PAYLOAD
  | AUTO_FLAG
  | UNKNOWN of int

  let mask = [
    COMMANDS;
    FLAGS;
    BOUNDS;
    ZERO_SIZE;
    ALIGN;
    PAYLOAD;
    AUTO_FLAG;
  ]
end

module ALLOW_TRANSPORT = struct
  type t =
  | TCP
  | UNIX
  | VSOCK
  | SSH
  | UNKNOWN of int

  let mask = [
    TCP;
    UNIX;
    VSOCK;
    SSH;
  ]
end

module SHUTDOWN = struct
  type t =
  | ABANDON_PENDING
  | UNKNOWN of int

  let mask = [
    ABANDON_PENDING;
  ]
end

let aio_direction_read = 1_l
let aio_direction_write = 2_l
let aio_direction_both = 3_l
let read_data = 1_l
let read_hole = 2_l
let read_error = 3_l
let namespace_base = "base:"
let context_base_allocation = "base:allocation"
let state_hole = 1_l
let state_zero = 2_l
let namespace_qemu = "qemu:"
let context_qemu_dirty_bitmap = "qemu:dirty-bitmap:"
let state_dirty = 1_l
let context_qemu_allocation_depth = "qemu:allocation-depth"

module Buffer = struct
  type t =
    (char, Bigarray.int8_unsigned_elt, Bigarray.c_layout) Bigarray.Array1.t

  let alloc =
    Bigarray.Array1.create Bigarray.Char Bigarray.c_layout
  let size = Bigarray.Array1.dim

  external to_bytes : t -> bytes = "nbd_internal_ocaml_buffer_to_bytes"
  external to_string : t -> string = "nbd_internal_ocaml_buffer_to_bytes"
  external _of_bytes : bytes -> t -> unit =
    "nbd_internal_ocaml_buffer_of_bytes"
  external _of_string : string -> t -> unit =
    "nbd_internal_ocaml_buffer_of_bytes"
  let of_bytes b =
    let buf = alloc (Bytes.length b) in
    _of_bytes b buf;
    buf
  let of_string s =
    let buf = alloc (String.length s) in
    _of_string s buf;
    buf
end

external errno_of_unix_error : Unix.error -> int =
    "nbd_internal_code_of_unix_error" [@@noalloc]

type t

external _create : unit -> t = "nbd_internal_ocaml_nbd_create"
external close : t -> unit = "nbd_internal_ocaml_nbd_close"

let create () =
  let nbd = _create () in
  Gc.finalise close nbd;
  nbd

let with_handle f =
  let nbd = create () in
  try let r = f nbd in close nbd; r with exn -> close nbd; raise exn

external set_debug : t -> bool -> unit
    = "nbd_internal_ocaml_nbd_set_debug"
external get_debug : t -> bool
    = "nbd_internal_ocaml_nbd_get_debug"
external set_debug_callback : t -> (string -> string -> int) -> unit
    = "nbd_internal_ocaml_nbd_set_debug_callback"
external clear_debug_callback : t -> unit
    = "nbd_internal_ocaml_nbd_clear_debug_callback"
external stats_bytes_sent : t -> int64
    = "nbd_internal_ocaml_nbd_stats_bytes_sent"
external stats_chunks_sent : t -> int64
    = "nbd_internal_ocaml_nbd_stats_chunks_sent"
external stats_bytes_received : t -> int64
    = "nbd_internal_ocaml_nbd_stats_bytes_received"
external stats_chunks_received : t -> int64
    = "nbd_internal_ocaml_nbd_stats_chunks_received"
external set_handle_name : t -> string -> unit
    = "nbd_internal_ocaml_nbd_set_handle_name"
external get_handle_name : t -> string
    = "nbd_internal_ocaml_nbd_get_handle_name"
external set_private_data : t -> int -> int
    = "nbd_internal_ocaml_nbd_set_private_data"
external get_private_data : t -> int
    = "nbd_internal_ocaml_nbd_get_private_data"
external set_export_name : t -> string -> unit
    = "nbd_internal_ocaml_nbd_set_export_name"
external get_export_name : t -> string
    = "nbd_internal_ocaml_nbd_get_export_name"
external set_request_block_size : t -> bool -> unit
    = "nbd_internal_ocaml_nbd_set_request_block_size"
external get_request_block_size : t -> bool
    = "nbd_internal_ocaml_nbd_get_request_block_size"
external set_full_info : t -> bool -> unit
    = "nbd_internal_ocaml_nbd_set_full_info"
external get_full_info : t -> bool
    = "nbd_internal_ocaml_nbd_get_full_info"
external get_canonical_export_name : t -> string
    = "nbd_internal_ocaml_nbd_get_canonical_export_name"
external get_export_description : t -> string
    = "nbd_internal_ocaml_nbd_get_export_description"
external set_tls : t -> TLS.t -> unit
    = "nbd_internal_ocaml_nbd_set_tls"
external get_tls : t -> TLS.t
    = "nbd_internal_ocaml_nbd_get_tls"
external get_tls_negotiated : t -> bool
    = "nbd_internal_ocaml_nbd_get_tls_negotiated"
external set_tls_certificates : t -> string -> unit
    = "nbd_internal_ocaml_nbd_set_tls_certificates"
external set_tls_verify_peer : t -> bool -> unit
    = "nbd_internal_ocaml_nbd_set_tls_verify_peer"
external get_tls_verify_peer : t -> bool
    = "nbd_internal_ocaml_nbd_get_tls_verify_peer"
external set_tls_username : t -> string -> unit
    = "nbd_internal_ocaml_nbd_set_tls_username"
external get_tls_username : t -> string
    = "nbd_internal_ocaml_nbd_get_tls_username"
external set_tls_hostname : t -> string -> unit
    = "nbd_internal_ocaml_nbd_set_tls_hostname"
external get_tls_hostname : t -> string
    = "nbd_internal_ocaml_nbd_get_tls_hostname"
external set_tls_psk_file : t -> string -> unit
    = "nbd_internal_ocaml_nbd_set_tls_psk_file"
external set_request_extended_headers : t -> bool -> unit
    = "nbd_internal_ocaml_nbd_set_request_extended_headers"
external get_request_extended_headers : t -> bool
    = "nbd_internal_ocaml_nbd_get_request_extended_headers"
external get_extended_headers_negotiated : t -> bool
    = "nbd_internal_ocaml_nbd_get_extended_headers_negotiated"
external set_request_structured_replies : t -> bool -> unit
    = "nbd_internal_ocaml_nbd_set_request_structured_replies"
external get_request_structured_replies : t -> bool
    = "nbd_internal_ocaml_nbd_get_request_structured_replies"
external get_structured_replies_negotiated : t -> bool
    = "nbd_internal_ocaml_nbd_get_structured_replies_negotiated"
external set_request_meta_context : t -> bool -> unit
    = "nbd_internal_ocaml_nbd_set_request_meta_context"
external get_request_meta_context : t -> bool
    = "nbd_internal_ocaml_nbd_get_request_meta_context"
external set_handshake_flags : t -> HANDSHAKE_FLAG.t list -> unit
    = "nbd_internal_ocaml_nbd_set_handshake_flags"
external get_handshake_flags : t -> HANDSHAKE_FLAG.t list
    = "nbd_internal_ocaml_nbd_get_handshake_flags"
external set_pread_initialize : t -> bool -> unit
    = "nbd_internal_ocaml_nbd_set_pread_initialize"
external get_pread_initialize : t -> bool
    = "nbd_internal_ocaml_nbd_get_pread_initialize"
external set_strict_mode : t -> STRICT.t list -> unit
    = "nbd_internal_ocaml_nbd_set_strict_mode"
external get_strict_mode : t -> STRICT.t list
    = "nbd_internal_ocaml_nbd_get_strict_mode"
external set_opt_mode : t -> bool -> unit
    = "nbd_internal_ocaml_nbd_set_opt_mode"
external get_opt_mode : t -> bool
    = "nbd_internal_ocaml_nbd_get_opt_mode"
external opt_go : t -> unit
    = "nbd_internal_ocaml_nbd_opt_go"
external opt_abort : t -> unit
    = "nbd_internal_ocaml_nbd_opt_abort"
external opt_starttls : t -> bool
    = "nbd_internal_ocaml_nbd_opt_starttls"
external opt_extended_headers : t -> bool
    = "nbd_internal_ocaml_nbd_opt_extended_headers"
external opt_structured_reply : t -> bool
    = "nbd_internal_ocaml_nbd_opt_structured_reply"
external opt_list : t -> (string -> string -> int) -> int
    = "nbd_internal_ocaml_nbd_opt_list"
external opt_info : t -> unit
    = "nbd_internal_ocaml_nbd_opt_info"
external opt_list_meta_context : t -> (string -> int) -> int
    = "nbd_internal_ocaml_nbd_opt_list_meta_context"
external opt_list_meta_context_queries : t -> string list -> (string -> int) -> int
    = "nbd_internal_ocaml_nbd_opt_list_meta_context_queries"
external opt_set_meta_context : t -> (string -> int) -> int
    = "nbd_internal_ocaml_nbd_opt_set_meta_context"
external opt_set_meta_context_queries : t -> string list -> (string -> int) -> int
    = "nbd_internal_ocaml_nbd_opt_set_meta_context_queries"
external add_meta_context : t -> string -> unit
    = "nbd_internal_ocaml_nbd_add_meta_context"
external get_nr_meta_contexts : t -> int
    = "nbd_internal_ocaml_nbd_get_nr_meta_contexts"
external get_meta_context : t -> int -> string
    = "nbd_internal_ocaml_nbd_get_meta_context"
external clear_meta_contexts : t -> unit
    = "nbd_internal_ocaml_nbd_clear_meta_contexts"
external set_uri_allow_transports : t -> ALLOW_TRANSPORT.t list -> unit
    = "nbd_internal_ocaml_nbd_set_uri_allow_transports"
external set_uri_allow_tls : t -> TLS.t -> unit
    = "nbd_internal_ocaml_nbd_set_uri_allow_tls"
external set_uri_allow_local_file : t -> bool -> unit
    = "nbd_internal_ocaml_nbd_set_uri_allow_local_file"
external connect_uri : t -> string -> unit
    = "nbd_internal_ocaml_nbd_connect_uri"
external connect_unix : t -> string -> unit
    = "nbd_internal_ocaml_nbd_connect_unix"
external connect_vsock : t -> int64 (* uint32_t *) -> int64 (* uint32_t *) -> unit
    = "nbd_internal_ocaml_nbd_connect_vsock"
external connect_tcp : t -> string -> string -> unit
    = "nbd_internal_ocaml_nbd_connect_tcp"
external connect_socket : t -> Unix.file_descr -> unit
    = "nbd_internal_ocaml_nbd_connect_socket"
external connect_command : t -> string list -> unit
    = "nbd_internal_ocaml_nbd_connect_command"
external connect_systemd_socket_activation : t -> string list -> unit
    = "nbd_internal_ocaml_nbd_connect_systemd_socket_activation"
external set_socket_activation_name : t -> string -> unit
    = "nbd_internal_ocaml_nbd_set_socket_activation_name"
external get_socket_activation_name : t -> string
    = "nbd_internal_ocaml_nbd_get_socket_activation_name"
external is_read_only : t -> bool
    = "nbd_internal_ocaml_nbd_is_read_only"
external can_flush : t -> bool
    = "nbd_internal_ocaml_nbd_can_flush"
external can_fua : t -> bool
    = "nbd_internal_ocaml_nbd_can_fua"
external is_rotational : t -> bool
    = "nbd_internal_ocaml_nbd_is_rotational"
external can_trim : t -> bool
    = "nbd_internal_ocaml_nbd_can_trim"
external can_zero : t -> bool
    = "nbd_internal_ocaml_nbd_can_zero"
external can_fast_zero : t -> bool
    = "nbd_internal_ocaml_nbd_can_fast_zero"
external can_block_status_payload : t -> bool
    = "nbd_internal_ocaml_nbd_can_block_status_payload"
external can_df : t -> bool
    = "nbd_internal_ocaml_nbd_can_df"
external can_multi_conn : t -> bool
    = "nbd_internal_ocaml_nbd_can_multi_conn"
external can_cache : t -> bool
    = "nbd_internal_ocaml_nbd_can_cache"
external can_meta_context : t -> string -> bool
    = "nbd_internal_ocaml_nbd_can_meta_context"
external get_protocol : t -> string
    = "nbd_internal_ocaml_nbd_get_protocol"
external get_size : t -> int64
    = "nbd_internal_ocaml_nbd_get_size"
external get_block_size : t -> SIZE.t -> int64
    = "nbd_internal_ocaml_nbd_get_block_size"
external pread : ?flags:CMD_FLAG.t list -> t -> bytes -> int64 -> unit
    = "nbd_internal_ocaml_nbd_pread"
external pread_structured : ?flags:CMD_FLAG.t list -> t -> bytes -> int64 -> (bytes -> int64 -> int -> int ref -> int) -> unit
    = "nbd_internal_ocaml_nbd_pread_structured"
external pwrite : ?flags:CMD_FLAG.t list -> t -> bytes -> int64 -> unit
    = "nbd_internal_ocaml_nbd_pwrite"
external shutdown : ?flags:SHUTDOWN.t list -> t -> unit
    = "nbd_internal_ocaml_nbd_shutdown"
external flush : ?flags:CMD_FLAG.t list -> t -> unit
    = "nbd_internal_ocaml_nbd_flush"
external trim : ?flags:CMD_FLAG.t list -> t -> int64 -> int64 -> unit
    = "nbd_internal_ocaml_nbd_trim"
external cache : ?flags:CMD_FLAG.t list -> t -> int64 -> int64 -> unit
    = "nbd_internal_ocaml_nbd_cache"
external zero : ?flags:CMD_FLAG.t list -> t -> int64 -> int64 -> unit
    = "nbd_internal_ocaml_nbd_zero"
external block_status : ?flags:CMD_FLAG.t list -> t -> int64 -> int64 -> (string -> int64 -> int64 (* uint32_t *) array -> int ref -> int) -> unit
    = "nbd_internal_ocaml_nbd_block_status"
external block_status_64 : ?flags:CMD_FLAG.t list -> t -> int64 -> int64 -> (string -> int64 -> extent array -> int ref -> int) -> unit
    = "nbd_internal_ocaml_nbd_block_status_64"
external block_status_filter : ?flags:CMD_FLAG.t list -> t -> int64 -> int64 -> string list -> (string -> int64 -> extent array -> int ref -> int) -> unit
    = "nbd_internal_ocaml_nbd_block_status_filter_byte" "nbd_internal_ocaml_nbd_block_status_filter"
external poll : t -> int -> int
    = "nbd_internal_ocaml_nbd_poll"
external poll2 : t -> Unix.file_descr -> int -> int
    = "nbd_internal_ocaml_nbd_poll2"
external aio_connect : t -> Unix.sockaddr -> unit
    = "nbd_internal_ocaml_nbd_aio_connect"
external aio_connect_uri : t -> string -> unit
    = "nbd_internal_ocaml_nbd_aio_connect_uri"
external aio_connect_unix : t -> string -> unit
    = "nbd_internal_ocaml_nbd_aio_connect_unix"
external aio_connect_vsock : t -> int64 (* uint32_t *) -> int64 (* uint32_t *) -> unit
    = "nbd_internal_ocaml_nbd_aio_connect_vsock"
external aio_connect_tcp : t -> string -> string -> unit
    = "nbd_internal_ocaml_nbd_aio_connect_tcp"
external aio_connect_socket : t -> Unix.file_descr -> unit
    = "nbd_internal_ocaml_nbd_aio_connect_socket"
external aio_connect_command : t -> string list -> unit
    = "nbd_internal_ocaml_nbd_aio_connect_command"
external aio_connect_systemd_socket_activation : t -> string list -> unit
    = "nbd_internal_ocaml_nbd_aio_connect_systemd_socket_activation"
external aio_opt_go : ?completion:(int ref -> int) -> t -> unit
    = "nbd_internal_ocaml_nbd_aio_opt_go"
external aio_opt_abort : t -> unit
    = "nbd_internal_ocaml_nbd_aio_opt_abort"
external aio_opt_starttls : ?completion:(int ref -> int) -> t -> unit
    = "nbd_internal_ocaml_nbd_aio_opt_starttls"
external aio_opt_extended_headers : ?completion:(int ref -> int) -> t -> unit
    = "nbd_internal_ocaml_nbd_aio_opt_extended_headers"
external aio_opt_structured_reply : ?completion:(int ref -> int) -> t -> unit
    = "nbd_internal_ocaml_nbd_aio_opt_structured_reply"
external aio_opt_list : ?completion:(int ref -> int) -> t -> (string -> string -> int) -> unit
    = "nbd_internal_ocaml_nbd_aio_opt_list"
external aio_opt_info : ?completion:(int ref -> int) -> t -> unit
    = "nbd_internal_ocaml_nbd_aio_opt_info"
external aio_opt_list_meta_context : ?completion:(int ref -> int) -> t -> (string -> int) -> int
    = "nbd_internal_ocaml_nbd_aio_opt_list_meta_context"
external aio_opt_list_meta_context_queries : ?completion:(int ref -> int) -> t -> string list -> (string -> int) -> int
    = "nbd_internal_ocaml_nbd_aio_opt_list_meta_context_queries"
external aio_opt_set_meta_context : ?completion:(int ref -> int) -> t -> (string -> int) -> int
    = "nbd_internal_ocaml_nbd_aio_opt_set_meta_context"
external aio_opt_set_meta_context_queries : ?completion:(int ref -> int) -> t -> string list -> (string -> int) -> int
    = "nbd_internal_ocaml_nbd_aio_opt_set_meta_context_queries"
external aio_pread : ?completion:(int ref -> int) -> ?flags:CMD_FLAG.t list -> t -> Buffer.t -> int64 -> cookie
    = "nbd_internal_ocaml_nbd_aio_pread"
external aio_pread_structured : ?completion:(int ref -> int) -> ?flags:CMD_FLAG.t list -> t -> Buffer.t -> int64 -> (bytes -> int64 -> int -> int ref -> int) -> cookie
    = "nbd_internal_ocaml_nbd_aio_pread_structured_byte" "nbd_internal_ocaml_nbd_aio_pread_structured"
external aio_pwrite : ?completion:(int ref -> int) -> ?flags:CMD_FLAG.t list -> t -> Buffer.t -> int64 -> cookie
    = "nbd_internal_ocaml_nbd_aio_pwrite"
external aio_disconnect : ?flags:CMD_FLAG.t list -> t -> unit
    = "nbd_internal_ocaml_nbd_aio_disconnect"
external aio_flush : ?completion:(int ref -> int) -> ?flags:CMD_FLAG.t list -> t -> cookie
    = "nbd_internal_ocaml_nbd_aio_flush"
external aio_trim : ?completion:(int ref -> int) -> ?flags:CMD_FLAG.t list -> t -> int64 -> int64 -> cookie
    = "nbd_internal_ocaml_nbd_aio_trim"
external aio_cache : ?completion:(int ref -> int) -> ?flags:CMD_FLAG.t list -> t -> int64 -> int64 -> cookie
    = "nbd_internal_ocaml_nbd_aio_cache"
external aio_zero : ?completion:(int ref -> int) -> ?flags:CMD_FLAG.t list -> t -> int64 -> int64 -> cookie
    = "nbd_internal_ocaml_nbd_aio_zero"
external aio_block_status : ?completion:(int ref -> int) -> ?flags:CMD_FLAG.t list -> t -> int64 -> int64 -> (string -> int64 -> int64 (* uint32_t *) array -> int ref -> int) -> cookie
    = "nbd_internal_ocaml_nbd_aio_block_status_byte" "nbd_internal_ocaml_nbd_aio_block_status"
external aio_block_status_64 : ?completion:(int ref -> int) -> ?flags:CMD_FLAG.t list -> t -> int64 -> int64 -> (string -> int64 -> extent array -> int ref -> int) -> cookie
    = "nbd_internal_ocaml_nbd_aio_block_status_64_byte" "nbd_internal_ocaml_nbd_aio_block_status_64"
external aio_block_status_filter : ?completion:(int ref -> int) -> ?flags:CMD_FLAG.t list -> t -> int64 -> int64 -> string list -> (string -> int64 -> extent array -> int ref -> int) -> cookie
    = "nbd_internal_ocaml_nbd_aio_block_status_filter_byte" "nbd_internal_ocaml_nbd_aio_block_status_filter"
external aio_get_fd : t -> Unix.file_descr
    = "nbd_internal_ocaml_nbd_aio_get_fd"
external aio_get_direction : t -> int
    = "nbd_internal_ocaml_nbd_aio_get_direction"
external aio_notify_read : t -> unit
    = "nbd_internal_ocaml_nbd_aio_notify_read"
external aio_notify_write : t -> unit
    = "nbd_internal_ocaml_nbd_aio_notify_write"
external aio_is_created : t -> bool
    = "nbd_internal_ocaml_nbd_aio_is_created"
external aio_is_connecting : t -> bool
    = "nbd_internal_ocaml_nbd_aio_is_connecting"
external aio_is_negotiating : t -> bool
    = "nbd_internal_ocaml_nbd_aio_is_negotiating"
external aio_is_ready : t -> bool
    = "nbd_internal_ocaml_nbd_aio_is_ready"
external aio_is_processing : t -> bool
    = "nbd_internal_ocaml_nbd_aio_is_processing"
external aio_is_dead : t -> bool
    = "nbd_internal_ocaml_nbd_aio_is_dead"
external aio_is_closed : t -> bool
    = "nbd_internal_ocaml_nbd_aio_is_closed"
external aio_command_completed : t -> int64 -> bool
    = "nbd_internal_ocaml_nbd_aio_command_completed"
external aio_peek_command_completed : t -> int64
    = "nbd_internal_ocaml_nbd_aio_peek_command_completed"
external aio_in_flight : t -> int
    = "nbd_internal_ocaml_nbd_aio_in_flight"
external connection_state : t -> string
    = "nbd_internal_ocaml_nbd_connection_state"
external get_package_name : t -> string
    = "nbd_internal_ocaml_nbd_get_package_name"
external get_version : t -> string
    = "nbd_internal_ocaml_nbd_get_version"
external kill_subprocess : t -> int -> unit
    = "nbd_internal_ocaml_nbd_kill_subprocess"
external get_subprocess_pid : t -> int64
    = "nbd_internal_ocaml_nbd_get_subprocess_pid"
external supports_tls : t -> bool
    = "nbd_internal_ocaml_nbd_supports_tls"
external supports_vsock : t -> bool
    = "nbd_internal_ocaml_nbd_supports_vsock"
external supports_uri : t -> bool
    = "nbd_internal_ocaml_nbd_supports_uri"
external get_uri : t -> string
    = "nbd_internal_ocaml_nbd_get_uri"
external is_uri : t -> string -> bool
    = "nbd_internal_ocaml_nbd_is_uri"
