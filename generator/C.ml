(* hey emacs, this is OCaml code: -*- tuareg -*- *)
(* nbd client library in userspace: generate the C API and documentation
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

open Printf

open API
open Utils

type closure_style = Direct | AddressOf | Pointer
type parens_style = NoParens | ParensSameLine | ParensNewLineWithIndent of int

let generate_lib_libnbd_syms () =
  generate_header HashStyle;

  (* Sort and group the calls by first_version, and emit them in order. *)
  let cmp (_, {first_version = a}) (_, {first_version = b}) = compare a b in
  let calls = List.sort cmp handle_calls in
  let extract ((_, {first_version}) as call) = first_version, call in
  let calls = List.map extract calls in
  let calls = group_by calls in

  let prev = ref None in
  List.iter (
    fun ((major, minor), calls) ->
      pr "# Public symbols in libnbd %d.%d:\n" major minor;
      pr "LIBNBD_%d.%d {\n" major minor;
      pr "  global:\n";
      if (major, minor) = (1, 0) then (
        pr "    nbd_create;\n";
        pr "    nbd_close;\n";
        pr "    nbd_get_errno;\n";
        pr "    nbd_get_error;\n"
      );
      List.iter (fun (name, _) -> pr "    nbd_%s;\n" name) calls;
      (match !prev with
       | None ->
          pr "  # Everything else is hidden.\n";
          pr "  local: *;\n";
       | Some _ -> ()
      );
      pr "}";
      (match !prev with
       | None -> ()
       | Some (old_major, old_minor) ->
          pr " LIBNBD_%d.%d" old_major old_minor
      );
      pr ";\n";
      pr "\n";
      prev := Some (major, minor)
  ) calls

let errcode_of_ret =
  function
  | RBool | RErr | RFd | RInt | RInt64 | RCookie | RSizeT -> Some "-1"
  | RStaticString | RString -> Some "NULL"
  | RUInt | RUIntPtr | RUInt64 | REnum _
  | RFlags _ -> None (* errors not possible *)

let type_of_ret =
  function
  | RBool | RErr | RFd | RInt | REnum _ -> "int"
  | RInt64 | RCookie -> "int64_t"
  | RSizeT -> "ssize_t"
  | RStaticString -> "const char *"
  | RString -> "char *"
  | RUInt -> "unsigned"
  | RUIntPtr -> "uintptr_t"
  | RUInt64 -> "uint64_t"
  | RFlags _ -> "uint32_t"

let name_of_arg =
  function
  | Bool n -> [n]
  | BytesIn (n, len) -> [n; len]
  | BytesOut (n, len) -> [n; len]
  | BytesPersistIn (n, len) -> [n; len]
  | BytesPersistOut (n, len) -> [n; len]
  | Closure { cbname } ->
     [ sprintf "%s_callback" cbname; sprintf "%s_user_data" cbname ]
  | Enum (n, _) -> [n]
  | Extent64 _ -> assert false (* only used in extent64_closure *)
  | Fd n -> [n]
  | Flags (n, _) -> [n]
  | Int n -> [n]
  | Int64 n -> [n]
  | Path n -> [n]
  | SizeT n -> [n]
  | SockAddrAndLen (n, len) -> [n; len]
  | String n -> [n]
  | StringList n -> [n]
  | UInt n -> [n]
  | UInt32 n -> [n]
  | UInt64 n -> [n]
  | UIntPtr n -> [n]

let name_of_optarg =
  function
  | OClosure { cbname } -> cbname
  | OFlags (n, _, _) -> n

(* Map function arguments to __attribute__((nonnull)) annotations.
 * Because a single arg may map to multiple C parameters this
 * returns a list.  Note: true => add attribute nonnull.
 *)
let arg_attr_nonnull =
  function
  (* BytesIn/Out are passed using a non-null pointer, and size_t *)
  | BytesIn _ | BytesOut _
  | BytesPersistIn _ | BytesPersistOut _ -> [ true; false ]
  (* sockaddr is also non-null pointer, and length *)
  | SockAddrAndLen (n, len) -> [ true; false ]
  (* strings should be marked as non-null *)
  | Path _ | String _ -> [ true ]
  (* list of strings should be marked as non-null *)
  | StringList n -> [ true ]
  (* numeric and other non-pointer types are not able to be null *)
  | Bool _ | Closure _ | Enum _ | Fd _ | Flags _
  | Int _ | Int64 _ | SizeT _
  | UInt _ | UInt32 _ | UInt64 _ | UIntPtr _ -> [ false ]
  | Extent64 _ -> assert false (* only used in extent64_closure *)

let optarg_attr_nonnull (OClosure _ | OFlags _) = [ false ]

let rec print_arg_list ?(wrap = false) ?maxcol ?handle ?types
          ?(parens = ParensSameLine) ?closure_style args optargs =
  (match parens with
   | NoParens -> ()
   | ParensSameLine -> pr "("
   | ParensNewLineWithIndent indent -> pr "(\n%s" (spaces (indent + 2))
  );
  if wrap then
    pr_wrap ?maxcol ','
      (fun () -> print_arg_list' ?handle ?types ?closure_style args optargs)
  else
    print_arg_list' ?handle ?types ?closure_style args optargs;
  (match parens with
   | NoParens -> ()
   | ParensSameLine -> pr ")"
   | ParensNewLineWithIndent indent -> pr "\n%s)" (spaces indent)
  )

and print_arg_list' ?(handle = false) ?(types = true) ?(closure_style = Direct)
          args optargs =
  let comma = ref false in
  if handle then (
    comma := true;
    if types then pr "struct nbd_handle *";
    pr "h"
  );
  List.iter (
    fun arg ->
      if !comma then pr ", ";
      comma := true;
      match arg with
      | Bool n ->
         if types then pr "bool ";
         pr "%s" n
      | BytesIn (n, len)
      | BytesPersistIn (n, len) ->
         if types then pr "const void *";
         pr "%s, " n;
         if types then pr "size_t ";
         pr "%s" len
      | BytesOut (n, len)
      | BytesPersistOut (n, len) ->
         if types then pr "void *";
         pr "%s, " n;
         if types then pr "size_t ";
         pr "%s" len
      | Closure { cbname; cbargs } ->
         if types then pr "nbd_%s_callback " cbname;
         let mark = match closure_style with
           | Direct -> ""
           | AddressOf -> "&"
           | Pointer -> "*" in
         pr "%s%s_callback" mark cbname
      | Enum (n, _) ->
         if types then pr "int ";
         pr "%s" n
      | Extent64 _ -> assert false (* only used in extent64_closure *)
      | Flags (n, _) ->
         if types then pr "uint32_t ";
         pr "%s" n
      | Fd n | Int n ->
         if types then pr "int ";
         pr "%s" n
      | Int64 n ->
         if types then pr "int64_t ";
         pr "%s" n
      | SizeT n ->
         if types then pr "size_t ";
         pr "%s" n
      | Path n
      | String n ->
         if types then pr "const char *";
         pr "%s" n
      | StringList n ->
         if types then pr "char **";
         pr "%s" n
      | SockAddrAndLen (n, len) ->
         if types then pr "const struct sockaddr *";
         pr "%s, " n;
         if types then pr "socklen_t ";
         pr "%s" len
      | UInt n ->
         if types then pr "unsigned ";
         pr "%s" n
      | UInt32 n ->
         if types then pr "uint32_t ";
         pr "%s" n
      | UInt64 n ->
         if types then pr "uint64_t ";
         pr "%s" n
      | UIntPtr n ->
         if types then pr "uintptr_t ";
         pr "%s" n
  ) args;
  List.iter (
    fun optarg ->
      if !comma then pr ", ";
      comma := true;
      match optarg with
      | OClosure { cbname; cbargs } ->
         if types then pr "nbd_%s_callback " cbname;
         let mark = match closure_style with
           | Direct -> ""
           | AddressOf -> "&"
           | Pointer -> "*" in
         pr "%s%s_callback" mark cbname
      | OFlags (n, _, _) ->
         if types then pr "uint32_t ";
         pr "%s" n
  ) optargs

let print_call ?wrap ?maxcol ?closure_style name args optargs ret =
  pr "%s " (type_of_ret ret);
  let designator_column = output_column () in
  pr "nbd_%s " name;
  print_arg_list ~handle:true ?wrap ?maxcol
      ~parens:(ParensNewLineWithIndent designator_column) ?closure_style args
      optargs

let print_fndecl ?wrap ?closure_style name args optargs ret =
  pr "extern ";
  print_call ?wrap ?closure_style name args optargs ret;

  (* For RString, note that the caller must free() the argument.
   * Since this is used in a public header, we must use gcc's spelling
   * of __builtin_free in case bare free is defined as a macro.
   *)
  (match ret with
   | RString ->
      pr "\n    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)";
   | _ -> ()
  );

  (* Output __attribute__((nonnull)) for the function parameters:
   * eg. struct nbd_handle *, int, char *
   *     => [ true, false, true ]
   *     => LIBNBD_ATTRIBUTE_NONNULL (1, 3)
   *     => __attribute__ ((nonnull (1, 3)))
   *)
  let nns : bool list =
    [ true ] (* struct nbd_handle * *)
    @ List.flatten (List.map arg_attr_nonnull args)
    @ List.flatten (List.map optarg_attr_nonnull optargs) in
  let nns = List.mapi (fun i b -> (i+1, b)) nns in
  let nns = filter_map (fun (i, b) -> if b then Some i else None) nns in
  let nns : string list = List.map string_of_int nns in
  pr "\n    LIBNBD_ATTRIBUTE_NONNULL (%s);\n" (String.concat ", " nns)

let rec print_cbarg_list ?(wrap = false) ?maxcol ?types ?(parens = true)
          cbargs =
  if parens then pr "(";
  if wrap then
    pr_wrap ?maxcol ','
      (fun () -> print_cbarg_list' ?types cbargs)
  else
    print_cbarg_list' ?types cbargs;
  if parens then pr ")"

and print_cbarg_list' ?(types = true) cbargs =
  if types then pr "void *";
  pr "user_data";

  List.iter (
    fun cbarg ->
      pr ", ";
      match cbarg with
      | CBArrayAndLen (UInt32 n, len) ->
         if types then pr "uint32_t *";
         pr "%s, " n;
         if types then pr "size_t ";
         pr "%s" len
      | CBArrayAndLen (Extent64 n, len) ->
         if types then pr "nbd_extent *";
         pr "%s, " n;
         if types then pr "size_t ";
         pr "%s" len
      | CBArrayAndLen _ -> assert false
      | CBBytesIn (n, len) ->
         if types then pr "const void *";
         pr "%s, " n;
         if types then pr "size_t ";
         pr "%s" len
      | CBInt n ->
         if types then pr "int ";
         pr "%s" n
      | CBInt64 n ->
         if types then pr "int64_t ";
         pr "%s" n
      | CBMutable (Int n) ->
         if types then pr "int *";
         pr "%s" n
      | CBMutable arg -> assert false
      | CBString n ->
         if types then pr "const char *";
         pr "%s" n
      | CBUInt n ->
         if types then pr "unsigned ";
         pr "%s" n
      | CBUInt64 n ->
         if types then pr "uint64_t ";
         pr "%s" n
  ) cbargs

(* Callback structs/typedefs in <libnbd.h> *)
let print_closure_structs () =
  pr "/* These are used for callback parameters.  They are passed\n";
  pr " * by value not by reference.  See CALLBACKS in libnbd(3).\n";
  pr " */\n";
  List.iter (
    fun { cbname; cbargs } ->
      pr "typedef struct {\n";
      pr "  int (*callback) ";
      print_cbarg_list ~wrap:true cbargs;
      pr ";\n";
      pr "  void *user_data;\n";
      pr "  void (*free) (void *user_data);\n";
      pr "} nbd_%s_callback;\n" cbname;
      pr "#define LIBNBD_HAVE_NBD_%s_CALLBACK 1\n"
        (String.uppercase_ascii cbname);
      pr "\n"
  ) all_closures;

  pr "/* Note NBD_NULL_* are only generated for callbacks which are\n";
  pr " * optional.  (See OClosure in the generator).\n";
  pr " */\n";
  let oclosures =
    let optargs = List.map (fun (_, { optargs }) -> optargs) handle_calls in
    let optargs = List.flatten optargs in
    let optargs =
      filter_map (function OClosure cb -> Some cb | _ -> None) optargs in
    List.sort_uniq compare optargs in
  List.iter (
    fun { cbname } ->
      pr "#define NBD_NULL_%s ((nbd_%s_callback) { .callback = NULL })\n"
        (String.uppercase_ascii cbname) cbname;
  ) oclosures;
  pr "\n"

let print_fndecl_and_define ?wrap name args optargs ret =
  let name_upper = String.uppercase_ascii name in
  print_fndecl ?wrap name args optargs ret;
  pr "#define LIBNBD_HAVE_NBD_%s 1\n" name_upper;
  pr "\n"

let print_ns_ctxt ns ns_upper ctxt =
  let ctxt_macro = macro_name ctxt in
  let ctxt_upper = String.uppercase_ascii ctxt_macro in
  pr "#define LIBNBD_CONTEXT_%s_%s \"%s:%s\"\n"
    ns_upper ctxt_upper ns ctxt

let print_ns_ctxt_bits ns ctxt consts =
  if consts <> [] then (
    pr "\n";
    pr "/* \"%s:%s\" context related constants */\n" ns ctxt;
    List.iter (fun (n, i) -> pr "#define LIBNBD_%-30s %d\n" n i) consts
  )

let print_ns ns ctxts =
  let ns_upper = String.uppercase_ascii ns in
  pr "/* \"%s\" namespace */\n" ns;
  pr "#define LIBNBD_NAMESPACE_%s \"%s:\"\n" ns_upper ns;
  pr "\n";
  pr "/* \"%s\" namespace contexts */\n" ns;
  List.iter (
    fun (ctxt, _) -> print_ns_ctxt ns ns_upper ctxt
  ) ctxts;
  List.iter (
    fun (ctxt, consts) -> print_ns_ctxt_bits ns ctxt consts
  ) ctxts;
  pr "\n"

let generate_include_libnbd_h () =
  generate_header CStyle;

  pr "#ifndef LIBNBD_H\n";
  pr "#define LIBNBD_H\n";
  pr "\n";
  pr "/* This is the public interface to libnbd, a client library for\n";
  pr " * accessing Network Block Device (NBD) servers.\n";
  pr " *\n";
  pr " * Please read the libnbd(3) manual page to\n";
  pr " * find out how to use this library.\n";
  pr " */\n";
  pr "\n";
  pr "#include <stdbool.h>\n";
  pr "#include <stdint.h>\n";
  pr "#include <sys/socket.h>\n";
  pr "\n";
  pr "#ifdef __cplusplus\n";
  pr "extern \"C\" {\n";
  pr "#endif\n";
  pr "\n";
  pr "#if defined (__GNUC__)\n";
  pr "#define LIBNBD_GCC_VERSION \\\n";
  pr "    (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)\n";
  pr "#endif\n";
  pr "\n";
  pr "#ifndef LIBNBD_ATTRIBUTE_NONNULL\n";
  pr "#if defined (__GNUC__) && LIBNBD_GCC_VERSION >= 120000 /* gcc >= 12.0 */\n";
  pr "#define LIBNBD_ATTRIBUTE_NONNULL(...) \\\n";
  pr "  __attribute__ ((__nonnull__ (__VA_ARGS__)))\n";
  pr "#else\n";
  pr "#define LIBNBD_ATTRIBUTE_NONNULL(...)\n";
  pr "#endif\n";
  pr "#endif /* ! defined LIBNBD_ATTRIBUTE_NONNULL */\n";
  pr "\n";
  pr "#if defined (__GNUC__) && LIBNBD_GCC_VERSION >= 120000 /* gcc >= 12.0 */\n";
  pr "#define LIBNBD_ATTRIBUTE_ALLOC_DEALLOC(fn) \\\n";
  pr "  __attribute__ ((__malloc__, __malloc__ (fn, 1)))\n";
  pr "#else\n";
  pr "#define LIBNBD_ATTRIBUTE_ALLOC_DEALLOC(fn)\n";
  pr "#endif\n";
  pr "\n";
  pr "struct nbd_handle;\n";
  pr "\n";
  List.iter (
    fun { enum_prefix; enums } ->
      List.iter (
        fun (enum, i) ->
          let enum = sprintf "LIBNBD_%s_%s" enum_prefix enum in
          pr "#define %-40s %d\n" enum i
      ) enums;
      pr "\n"
  ) all_enums;
  List.iter (
    fun { flag_prefix; flags } ->
      let mask = ref 0 in
      List.iter (
        fun (flag, i) ->
          let flag = sprintf "LIBNBD_%s_%s" flag_prefix flag in
          pr "#define %-40s 0x%02xU\n" flag i;
          mask := !mask lor i
      ) flags;
      let flag = sprintf "LIBNBD_%s_MASK" flag_prefix in
      pr "#define %-40s 0x%02xU\n" flag !mask;
      pr "\n"
  ) all_flags;
  List.iter (
    fun (n, i) ->
      let n = sprintf "LIBNBD_%s" n in
      pr "#define %-40s %d\n" n i
  ) constants;
  pr "\n";
  pr "extern void nbd_close (struct nbd_handle *h); /* h can be NULL */\n";
  pr "#define LIBNBD_HAVE_NBD_CLOSE 1\n";
  pr "\n";
  pr "extern struct nbd_handle *nbd_create (void)\n";
  pr "    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (nbd_close);\n";
  pr "#define LIBNBD_HAVE_NBD_CREATE 1\n";
  pr "\n";
  pr "extern const char *nbd_get_error (void);\n";
  pr "#define LIBNBD_HAVE_NBD_GET_ERROR 1\n";
  pr "\n";
  pr "extern int nbd_get_errno (void);\n";
  pr "#define LIBNBD_HAVE_NBD_GET_ERRNO 1\n";
  pr "\n";
  pr "/* This is used in the callback for nbd_block_status_64.\n";
  pr " */\n";
  pr "typedef struct {\n";
  pr "  uint64_t length;  /* Will not exceed INT64_MAX */\n";
  pr "  uint64_t flags;\n";
  pr "} nbd_extent;\n";
  pr "\n";
  print_closure_structs ();
  List.iter (
    fun (name, { args; optargs; ret }) ->
      print_fndecl_and_define ~wrap:true name args optargs ret
  ) handle_calls;
  List.iter (
    fun (ns, ctxts) -> print_ns ns ctxts
  ) metadata_namespaces;
  pr "#ifdef __cplusplus\n";
  pr "}\n";
  pr "#endif\n";
  pr "\n";
  pr "#endif /* LIBNBD_H */\n"

let generate_lib_unlocked_h () =
  generate_header CStyle;

  pr "#ifndef LIBNBD_UNLOCKED_H\n";
  pr "#define LIBNBD_UNLOCKED_H\n";
  pr "\n";
  List.iter (
    fun (name, { args; optargs; ret }) ->
    print_fndecl ~wrap:true ~closure_style:Pointer ("unlocked_" ^ name)
      args optargs ret
  ) handle_calls;
  pr "\n";
  pr "#endif /* LIBNBD_UNLOCKED_H */\n"

let permitted_state_text ?(fold=false) permitted_states =
  assert (permitted_states <> []);
  let sep = if fold then ", or\n" else ", or " in
  String.concat sep
    (List.map (
         function
         | Created -> "newly created"
         | Connecting -> "connecting"
         | Negotiating -> "negotiating"
         | Connected -> "connected with the server"
         | Closed -> "shut down"
         | Dead -> "dead"
       ) permitted_states
    )

(* Generate wrappers around each API call which are a place to
 * grab the thread mutex (lock) and do logging.
 *)
let generate_lib_api_c () =
  (* Print the wrapper added around all API calls. *)
  let rec print_wrapper (name, {args; optargs; ret; permitted_states;
                                is_locked; may_set_error}) =
    if permitted_states <> [] then (
      pr "static inline bool\n";
      pr "%s_in_permitted_state (struct nbd_handle *h)\n" name;
      pr "{\n";
      pr "  const enum state state = get_public_state (h);\n";
      pr "\n";
      let or_nl_indent = " ||\n        " in
      let tests =
        List.map (
          function
          | Created -> "nbd_internal_is_state_created (state)"
          | Connecting -> "nbd_internal_is_state_connecting (state)"
          | Negotiating -> "nbd_internal_is_state_negotiating (state)"
          | Connected ->
              "nbd_internal_is_state_ready (state)" ^
              or_nl_indent ^
              "nbd_internal_is_state_processing (state)"
          | Closed -> "nbd_internal_is_state_closed (state)"
          | Dead -> "nbd_internal_is_state_dead (state)"
        ) permitted_states in
      pr "  if (!(%s)) {\n" (String.concat or_nl_indent tests);
      pr "    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,\n";
      pr "               \"invalid state: %%s: the handle must be %%s\",\n";
      pr "               nbd_internal_state_short_string (state),\n";
      pr "               \"";
      pr_wrap_cstr (fun () -> pr "%s" (permitted_state_text permitted_states));
      pr "\");\n";
      pr "    return false;\n";
      pr "  }\n";
      pr "  return true;\n";
      pr "}\n";
      pr "\n"
    );

    let need_out_label = ref false in

    let ret_c_type = type_of_ret ret and errcode = errcode_of_ret ret in
    pr "%s\n" ret_c_type;
    pr "nbd_%s " name;
    print_arg_list ~wrap:true ~handle:true ~parens:(ParensNewLineWithIndent 0)
      args optargs;
    pr "\n";
    pr "{\n";
    if permitted_states <> [] then
      pr "  bool p;\n";
    pr "  %s ret;\n" ret_c_type;
    pr "\n";
    pr "  assert (h->magic == NBD_HANDLE_MAGIC);\n";
    if may_set_error then (
      pr "  nbd_internal_set_error_context (\"nbd_%s\");\n" name;
      pr "\n";
    )
    else
      pr "  /* This function must not call set_error. */\n";

    (* Lock the handle. *)
    if is_locked then
      pr "  pthread_mutex_lock (&h->lock);\n";
    print_trace_enter name args optargs may_set_error;
    pr "\n";

    (* Sanitize read buffers before any error is possible. *)
    List.iter (
      function
      | BytesOut (n, count)
      | BytesPersistOut (n, count) ->
         pr "  if (h->pread_initialize)\n";
         pr "    memset (%s, 0, %s);\n" n count
      | _ -> ()
    ) args;

    (* Check current state is permitted for this call. *)
    if permitted_states <> [] then (
      let value = match errcode with
        | Some value -> value
        | None -> assert false in
      pr "  p = %s_in_permitted_state (h);\n" name;
      pr "  if (unlikely (!p)) {\n";
      pr "    ret = %s;\n" value;
      pr "    goto out;\n";
      pr "  }\n";
      need_out_label := true
    );

    (* Check parameters are valid. *)
    let print_flags_check n { flag_prefix; flags; guard } subset =
      let value = match errcode with
        | Some value -> value
        | None -> assert false in
      let mask = match subset with
        | Some [] -> "0"
        | Some subset ->
           let v = ref 0 in
           List.iter (
             fun (flag, i) ->
               if List.mem flag subset then v := !v lor i
           ) flags;
           sprintf "0x%x" !v
        | None -> "LIBNBD_" ^ flag_prefix ^ "_MASK" in
      let guard = match guard with
        | Some value -> " &&\n      " ^ value
        | None -> "" in
      pr "  if (unlikely ((%s & ~%s) != 0)%s) {\n" n mask guard;
      pr "    set_error (EINVAL, \"%%s: invalid value for flag: 0x%%x\",\n";
      pr "               \"%s\", %s);\n" n n;
      pr "    ret = %s;\n" value;
      pr "    goto out;\n";
      pr "  }\n";
      need_out_label := true
    in
    List.iter (
      function
      | Closure { cbname } ->
         let value = match errcode with
           | Some value -> value
           | None -> assert false in
         pr "  if (CALLBACK_IS_NULL (%s_callback)) {\n" cbname;
         pr "    set_error (EFAULT, \"%%s cannot be NULL\", \"%s\");\n" cbname;
         pr "    ret = %s;\n" value;
         pr "    goto out;\n";
         pr "  }\n";
         need_out_label := true
      | Enum (n, { enum_prefix; enums }) ->
         let value = match errcode with
           | Some value -> value
           | None -> assert false in
         pr "  switch (%s) {\n" n;
         List.iter (
           fun (enum, _) ->
             pr "  case LIBNBD_%s_%s:\n" enum_prefix enum
         ) enums;
         pr "    break;\n";
         pr "  default:\n";
         pr "    set_error (EINVAL, \"%%s: invalid value for parameter: %%d\",\n";
         pr "               \"%s\", %s);\n" n n;
         pr "    ret = %s;\n" value;
         pr "    goto out;\n";
         pr "  }\n";
         need_out_label := true
      | Flags (n, flags) ->
         print_flags_check n flags None
      | BytesIn (n, _) | BytesOut (n, _)
      | BytesPersistIn (n, _) | BytesPersistOut (n, _)
      | SockAddrAndLen (n, _)
      | Path n
      | String n
      | StringList n ->
         let value = match errcode with
           | Some value -> value
           | None -> assert false in
         pr "  if (%s == NULL) {\n" n;
         pr "    set_error (EFAULT, \"%%s cannot be NULL\", \"%s\");\n" n;
         pr "    ret = %s;\n" value;
         pr "    goto out;\n";
         pr "  }\n";
         need_out_label := true
      | _ -> ()
    ) args;
    List.iter (
      function
      | OClosure _ -> ()
      | OFlags (n, flags, subset) ->
         print_flags_check n flags subset
    ) optargs;

    (* Make the call. *)
    pr "  ret = nbd_unlocked_%s " name;
    print_arg_list ~wrap:true ~types:false ~handle:true
      ~closure_style:AddressOf args optargs;
    pr ";\n";
    pr "\n";
    if !need_out_label then
      pr " out:\n";
    print_trace_leave name ret may_set_error;
    pr "\n";
    (* Finish any closures not transferred to state machine. *)
    List.iter (
      function
      | Closure { cbname } ->
         pr "  FREE_CALLBACK (%s_callback);\n" cbname
      | _ -> ()
    ) args;
    List.iter (
      function
      | OClosure { cbname } ->
         pr "  FREE_CALLBACK (%s_callback);\n" cbname
      | OFlags _ -> ()
    ) optargs;
    if is_locked then (
      pr "  if (h->public_state != get_next_state (h))\n";
      pr "    h->public_state = get_next_state (h);\n";
      pr "  pthread_mutex_unlock (&h->lock);\n"
    );
    pr "  return ret;\n";
    pr "}\n";
    pr "\n"

  (* Print the trace when we enter a call with debugging enabled. *)
  and print_trace_enter name args optargs may_set_error =
    pr "  if_debug (h) {\n";
    List.iter (
      function
      | BytesIn (n, count)
      | BytesPersistIn (n, count) ->
         pr "    char *%s_printable =\n" n;
         pr "        nbd_internal_printable_buffer (%s, %s);\n" n count
      | Path n
      | String n ->
         pr "    char *%s_printable =\n" n;
         pr "        nbd_internal_printable_string (%s);\n" n
      | StringList n ->
         pr "    char *%s_printable =\n" n;
         pr "        nbd_internal_printable_string_list (%s);\n" n
      | BytesOut _ | BytesPersistOut _
      | Bool _ | Closure _ | Enum _ | Flags _ | Fd _ | Int _
      | Int64 _ | SizeT _
      | SockAddrAndLen _ | UInt _ | UInt32 _ | UInt64 _ | UIntPtr _ -> ()
      | Extent64 _ -> assert false (* only used in extent64_closure *)
    ) args;
    let indent =
      if may_set_error then (
        pr "    debug (h,\n";
        spaces 11
      )
      else (
        pr "    debug_direct (h, \"nbd_%s\",\n" name;
        spaces 18
      ) in
    pr "%s\"" indent;
    let print_format_string () =
      pr "enter:";
      List.iter (
        function
        | Bool n -> pr " %s=%%s" n
        | BytesOut (n, count)
        | BytesPersistOut (n, count) -> pr " %s=<buf> %s=%%zu" n count
        | BytesIn (n, count)
        | BytesPersistIn (n, count) ->
           pr " %s=\\\"%%s\\\" %s=%%zu" n count
        | Closure { cbname } -> pr " %s=%%s" cbname
        | Enum (n, _) -> pr " %s=%%d" n
        | Extent64 _ -> assert false (* only used in extent64_closure *)
        | Flags (n, _) -> pr " %s=0x%%x" n
        | Fd n | Int n -> pr " %s=%%d" n
        | Int64 n -> pr " %s=%%\"PRIi64\"" n
        | SizeT n -> pr " %s=%%zu" n
        | SockAddrAndLen (n, len) -> pr " %s=<sockaddr> %s=%%d" n len
        | Path n
        | String n -> pr " %s=%%s" n
        | StringList n -> pr " %s=%%s" n
        | UInt n -> pr " %s=%%u" n
        | UInt32 n -> pr " %s=%%\"PRIu32\"" n
        | UInt64 n -> pr " %s=%%\"PRIu64\"" n
        | UIntPtr n -> pr " %s=%%\"PRIuPTR\"" n
      ) args;
      List.iter (
        function
        | OClosure { cbname } -> pr " %s=%%s" cbname
        | OFlags (n, _, _) -> pr " %s=0x%%x" n
      ) optargs
    in
    pr_wrap_cstr print_format_string;
    pr "\"";
    let num_args = List.length args
    and num_optargs = List.length optargs in
    let num_allargs = num_args + num_optargs in
    if num_allargs > 0 then
      pr ",\n%s" indent;
    let print_args () =
      List.iteri (
        fun i arg ->
          (match arg with
           | Bool n -> pr "%s ? \"true\" : \"false\"" n
           | BytesOut (n, count)
           | BytesPersistOut (n, count) -> pr "%s" count
           | BytesIn (n, count)
           | BytesPersistIn (n, count) ->
              pr "%s_printable ? %s_printable : \"\", %s" n n count
           | Closure _ -> pr "\"<fun>\""
           | Enum (n, _) -> pr "%s" n
           | Extent64 _ -> assert false (* only used in extent64_closure *)
           | Flags (n, _) -> pr "%s" n
           | Fd n | Int n | Int64 n | SizeT n -> pr "%s" n
           | SockAddrAndLen (_, len) -> pr "(int) %s" len
           | Path n | String n | StringList n ->
              pr "%s_printable ? %s_printable : \"\"" n n
           | UInt n | UInt32 n | UInt64 n | UIntPtr n -> pr "%s" n
          );
          if i < num_allargs - 1 then
            pr ", "
      ) args;
      List.iteri (
        fun i optarg ->
          (match optarg with
           | OClosure { cbname } ->
              pr "CALLBACK_IS_NULL (%s_callback) ? \"<fun>\" : \"NULL\"" cbname
           | OFlags (n, _, _) -> pr "%s" n
          );
          if num_args + i < num_allargs - 1 then
            pr ", "
      ) optargs
    in
    pr_wrap ',' print_args;
    pr ");\n";
    List.iter (
      function
      | BytesIn (n, _)
      | BytesPersistIn (n, _)
      | Path n
      | String n
      | StringList n ->
         pr "    free (%s_printable);\n" n
      | BytesOut _ | BytesPersistOut _
      | Bool _ | Closure _ | Enum _ | Flags _ | Fd _ | Int _
      | Int64 _ | SizeT _
      | SockAddrAndLen _ | UInt _ | UInt32 _ | UInt64 _ | UIntPtr _ -> ()
      | Extent64 _ -> assert false (* only used in extent64_closure *)
    ) args;
    pr "  }\n"
  (* Print the trace when we leave a call with debugging enabled. *)
  and print_trace_leave name ret may_set_error =
    pr "  if_debug (h) {\n";
    if may_set_error then (
      (match errcode_of_ret ret with
       | Some r ->
          pr "    if (ret == %s)\n" r;
       | None -> assert false
      );
      pr "      debug (h, \"leave: error=\\\"%%s\\\"\", nbd_get_error ());\n";
      pr "    else {\n";
      (match ret with
       | RString ->
          pr "      char *ret_printable =\n";
          pr "          nbd_internal_printable_string (ret);\n"
       | _ -> ()
      );
      pr "      debug ("
    )
    else
      pr "    debug_direct (";
    let print_args () =
      if may_set_error then
        pr "h, \"leave: ret="
      else
        pr "h, \"nbd_%s\", \"leave: ret=" name;
      (match ret with
       | RBool | RErr | RFd | RInt | REnum _ -> pr "%%d\", ret"
       | RInt64 | RCookie -> pr "%%\" PRIi64, ret"
       | RSizeT -> pr "%%zd\", ret"
       | RString -> pr "%%s\", ret_printable ? ret_printable : \"\""
       | RStaticString -> pr "%%s\", ret"
       | RUInt | RFlags _ -> pr "%%u\", ret"
       | RUIntPtr -> pr "%%\" PRIuPTR, ret"
       | RUInt64 -> pr "%%\" PRIu64, ret"
      )
    in
    pr_wrap ',' print_args;
    pr ");\n";
    if may_set_error then (
      (match ret with
       | RString -> pr "      free (ret_printable);\n"
       | _ -> ()
      );
      pr "    }\n";
    );
    pr "  }\n"
  in

  generate_header CStyle;

  pr "#include <config.h>\n";
  pr "\n";
  pr "#include <stdio.h>\n";
  pr "#include <stdlib.h>\n";
  pr "#include <stdint.h>\n";
  pr "#include <inttypes.h>\n";
  pr "#include <errno.h>\n";
  pr "#include <assert.h>\n";
  pr "\n";
  pr "#include <pthread.h>\n";
  pr "\n";
  pr "/* GCC will remove NULL checks from this file for any parameter\n";
  pr " * annotated with attribute((nonnull)).  See RHBZ#1041336.  To\n";
  pr " * avoid this, disable the attribute when including libnbd.h.\n";
  pr " */\n";
  pr "#define LIBNBD_ATTRIBUTE_NONNULL(...)\n";
  pr "\n";
  pr "#include \"libnbd.h\"\n";
  pr "#include \"internal.h\"\n";
  pr "\n";
  List.iter print_wrapper handle_calls
