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

let messages = ref []
let f id context msg =
  assert (id = 42);
  messages := !messages @ [msg];
  0

let () =
  let nbd = NBD.create () in
  NBD.set_debug_callback nbd (f 42);

  NBD.connect_command nbd [nbdkit; "-s"; "--exit-with-parent"; "null"];
  NBD.shutdown nbd;

  print_endline "<<<";
  List.iter print_endline !messages;
  print_endline ">>>"

let () = Gc.compact ()
