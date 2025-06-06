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

open Ocaml_test_config

open Printf

let script = sprintf "%s/../../tests/meta-base-allocation.sh" srcdir

let entries = ref [||]
let f user_data metacontext offset e err =
  assert (user_data = 42);
  assert (!err = 0);
  if metacontext = "base:allocation" then
    entries := e;
  0

let () =
  let nbd = NBD.create () in
  NBD.add_meta_context nbd "base:allocation";
  NBD.connect_command nbd [nbdkit; "-s"; "--exit-with-parent"; "-v";
                           "sh"; script];

  NBD.block_status_64 nbd 65536_L 0_L (f 42);
  assert (!entries = [|  8192_L, 0_L;
                         8192_L, 1_L;
                        16384_L, 3_L;
                        16384_L, 2_L;
                        16384_L, 0_L |]);

  NBD.block_status_64 nbd 1024_L 32256_L (f 42);
  assert (!entries = [|   512_L, 3_L;
                        16384_L, 2_L |]);

  let flags = let open NBD.CMD_FLAG in [REQ_ONE] in
  NBD.block_status_64 nbd 1024_L 32256_L (f 42) ~flags;
  assert (!entries = [|   512_L, 3_L |])

let () = Gc.compact ()
