(* hey emacs, this is OCaml code: -*- tuareg -*- *)
(* libnbd OCaml test case
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

let () =
  NBD.with_handle (
    fun nbd ->
      let name = NBD.get_export_name nbd in
      assert (name = "");
      let info = NBD.get_full_info nbd in
      assert (not info);
      let tls = NBD.get_tls nbd in
      assert (tls = NBD.TLS.DISABLE);
      let eh = NBD.get_request_extended_headers nbd in
      assert (eh = true);
      let sr = NBD.get_request_structured_replies nbd in
      assert sr;
      let meta = NBD.get_request_meta_context nbd in
      assert meta;
      let bs = NBD.get_request_block_size nbd in
      assert bs;
      let init = NBD.get_pread_initialize nbd in
      assert init;
      let flags = NBD.get_handshake_flags nbd in
      assert (flags = NBD.HANDSHAKE_FLAG.mask);
      let opt = NBD.get_opt_mode nbd in
      assert (not opt)
  )

let () = Gc.compact ()
