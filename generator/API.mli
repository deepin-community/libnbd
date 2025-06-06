(* hey emacs, this is OCaml code: -*- tuareg -*- *)
(* nbd client library in userspace: the API
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

type call = {
  args : arg list;         (** parameters (except handle) *)
  optargs : optarg list;   (** optional parameters (not optional in C) *)
  ret : ret;               (** return value *)
  shortdesc : string;      (** short description *)
  longdesc : string;       (** long description *)
  example : string option; (** path to example code (under top_srcdir) *)
  see_also : link list;    (** "SEE ALSO" section in man page *)
  (** List of permitted states for making this call.  [[]] = Any state. *)
  permitted_states : permitted_state list;
  (** Most functions must take a lock.  The only known exceptions are:
      - functions which return a constant (eg. [nbd_supports_uri])
      - functions which {b only} read from the atomic
        [get_public_state] and do nothing else with the handle. *)
  is_locked : bool;
  (** Most functions can call set_error.  For functions which are
      {b guaranteed} never to do that we can save a bit of time by
      setting this to false. *)
  may_set_error : bool;
  (** There are two types of asynchronous functions, those with a completion
      callback and those which change state when completed. This field tells
      if the function is asynchronous and in that case how one can check if
      it has completed. *)
  async_kind : async_kind option;
  (** A flag telling if the call may do something with the file descriptor.
      Some bindings needs exclusive access to the file descriptor and can not
      allow the user to call [aio_notify_read] or [aio_notify_write], neither
      directly nor indirectly from another call. So all calls that might trigger
      any of these functions to be called, including all synchronous commands
      like [pread] or [connect], should set this to [true]. *)
  modifies_fd : bool;
  (** The first stable version that the symbol appeared in, for
      example (1, 2) if the symbol was added in development cycle
      1.1.x and thus the first stable version was 1.2.  This is
      filled in by the generator, add new calls to the first_version
      table instead. *)
  mutable first_version : int * int;
}
and arg =
| Bool of string           (** bool *)
| BytesIn of string * string (** byte array + size passed in to the function *)
| BytesOut of string * string(** byte array + size specified by caller,
                                 written by the function *)
| BytesPersistIn of string * string (** same as above, but buffer persists *)
| BytesPersistOut of string * string
| Closure of closure       (** function pointer + void *opaque *)
| Enum of string * enum    (** enum/union type, int in C *)
| Extent64 of string       (** extent descriptor, with 63-bit size and
                               64-bit flags; struct nbd_extent in C,
                               tuple or pair in other languages *)
| Fd of string             (** file descriptor *)
| Flags of string * flags  (** flags, uint32_t in C *)
| Int of string            (** small int *)
| Int64 of string          (** 64 bit signed int *)
| Path of string           (** filename or path *)
| SizeT of string          (** like size_t, for counting array elements *)
| SockAddrAndLen of string * string (** struct sockaddr * + socklen_t *)
| String of string         (** string, cannot be NULL *)
| StringList of string     (** argv-style NULL-terminated array of strings *)
| UInt of string           (** small unsigned int *)
| UInt32 of string         (** 32 bit unsigned int *)
| UInt64 of string         (** 64 bit unsigned int *)
| UIntPtr of string        (** uintptr_t in C, same as UInt in non-C *)
and optarg =
| OClosure of closure      (** optional closure *)
| OFlags of string * flags * string list option (** optional flags, uint32_t
                                                    in C, and valid subset *)
and ret =
| RBool                    (** return a boolean, or error *)
| RStaticString            (** return a static string (must be located in
                               .text section), NULL for error *)
| RErr                     (** return 0 = ok, -1 = error *)
| RFd                      (** return a file descriptor, or error *)
| RInt                     (** return a small int, -1 = error *)
| RInt64                   (** 64 bit int, -1 = error *)
| RCookie                  (** 64 bit command cookie (>= 1), -1 = error *)
| RSizeT                   (** return a count like ssize_t, -1 = error *)
| RString                  (** return a newly allocated string,
                               caller frees, NULL for error *)
| RUInt                    (** return a bitmask, no error possible *)
| RUIntPtr                 (** uintptr_t in C, same as RUInt in non-C *)
| RUInt64                  (** 64 bit int, no error possible *)
| REnum of enum            (** return an enum, no error possible *)
| RFlags of flags          (** return bitmask of flags, no error possible *)
and closure = {
  cbname : string;         (** name of callback function *)
  cbargs : cbarg list;     (** all closures return int for now *)
}
and cbarg =
| CBArrayAndLen of arg * string (** array + number of entries *)
| CBBytesIn of string * string (** like BytesIn *)
| CBInt of string          (** like Int *)
| CBInt64 of string        (** like Int64 *)
| CBMutable of arg         (** mutable argument, eg. int* *)
| CBString of string       (** like String *)
| CBUInt of string         (** like UInt *)
| CBUInt64 of string       (** like UInt64 *)
and enum = {
  enum_prefix : string;    (** prefix of each enum variant *)
  enums : (string * int) list (** enum names and their values in C *)
}
and flags = {
  flag_prefix : string;    (** prefix of each flag name *)
  guard : string option;   (** additional gating for checking valid flags *)
  flags : (string * int) list; (** flag names and their values in C *)
  _unused : unit           (** silence warning 23 when using 'defaults with' *)
}
and permitted_state =
| Created                  (** can be called in the START state *)
| Connecting               (** can be called when connecting/handshaking *)
| Negotiating              (** can be called when handshaking in opt_mode *)
| Connected                (** when connected and READY or processing, but
                               not including CLOSED or DEAD *)
| Closed | Dead            (** can be called when the handle is
                               CLOSED or DEAD *)
and async_kind =
(** The asynchronous call has a completion callback. *)
| WithCompletionCallback
(** The asynchronous call is completed when the given handle call returns the
    given boolean value. Might for instance be ("aio_is_connected", false). *)
| ChangesState of string * bool
and link =
| Link of string           (** link to L<nbd_PAGE(3)> *)
| SectionLink of string    (** link to L<libnbd(3)/SECTION> *)
| MainPageLink             (** link to L<libnbd(3)> *)
| ExternalLink of string * int (** link to L<EXTERNAL(SECTION)> *)
| URLLink of string        (** link to L<URL> *)

val handle_calls : (string * call) list
(** List of API calls.  The string is the name of the call. *)

val first_version : (string * (int * int)) list
(** The first stable version that the symbol appeared in, for
    example (1, 2) if the symbol was added in development cycle
    1.1.x and thus the first stable version was 1.2. *)

val all_closures : closure list
val all_enums : enum list
val all_flags : flags list
val all_permitted_states : permitted_state list
val constants : (string * int) list
val metadata_namespaces : (string * (string * (string * int) list) list) list

val extract_links : string -> link list
(** Extract links from POD text. *)

val pod_of_link : link -> string
val verify_link : link -> unit
val sort_uniq_links : link list -> link list
