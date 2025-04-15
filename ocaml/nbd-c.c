/* NBD client library in userspace
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
 */

#include <config.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <libnbd.h>

#include "nbd-c.h"

#include <caml/alloc.h>
#include <caml/bigarray.h>
#include <caml/callback.h>
#include <caml/fail.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>

#pragma GCC diagnostic ignored "-Wmissing-prototypes"

/* This is passed to *_wrapper as the user_data pointer
 * and freed in the free_user_data function below.
 */
struct user_data {
  value fnv;     /* Optional GC root pointing to OCaml function. */
  value bufv;    /* Optional GC root pointing to persistent buffer. */
};

static struct user_data *
alloc_user_data (void)
{
  struct user_data *data = calloc (1, sizeof *data);
  if (data == NULL)
    caml_raise_out_of_memory ();
  return data;
}

static void
free_user_data (void *user_data)
{
  struct user_data *data = user_data;

  if (data->fnv != 0)
    caml_remove_generational_global_root (&data->fnv);
  if (data->bufv != 0)
    caml_remove_generational_global_root (&data->bufv);
  free (data);
}

/* Wrapper for chunk callback. */
static int
chunk_wrapper_locked (void *user_data, const void *subbuf, size_t count,
                      uint64_t offset, unsigned status, int *error)
{
  CAMLparam0 ();
  CAMLlocal4 (subbufv, offsetv, statusv, errorv);
  CAMLlocal2 (exn, rv);
  const struct user_data *data = user_data;
  int r;
  value args[4];

  subbufv = caml_alloc_initialized_string (count, subbuf);
  offsetv = caml_copy_int64 (offset);
  statusv = Val_int (status);
  errorv = caml_alloc_tuple (1);
  Store_field (errorv, 0, Val_int (*error));
  args[0] = subbufv;
  args[1] = offsetv;
  args[2] = statusv;
  args[3] = errorv;
  rv = caml_callbackN_exn (data->fnv, 4, args);
  *error = Int_val (Field (errorv, 0));
  if (Is_exception_result (rv)) {
    nbd_internal_ocaml_exception_in_wrapper ("chunk", rv);
    CAMLreturnT (int, -1);
  }

  r = Int_val (rv);
  assert (r >= 0);
  CAMLreturnT (int, r);
}

static int
chunk_wrapper (void *user_data, const void *subbuf, size_t count,
               uint64_t offset, unsigned status, int *error)
{
  int ret = 0;

  caml_leave_blocking_section ();
  ret = chunk_wrapper_locked (user_data, subbuf, count, offset, status,
                              error);
  caml_enter_blocking_section ();
  return ret;
}

/* Wrapper for completion callback. */
static int
completion_wrapper_locked (void *user_data, int *error)
{
  CAMLparam0 ();
  CAMLlocal1 (errorv);
  CAMLlocal2 (exn, rv);
  const struct user_data *data = user_data;
  int r;
  value args[1];

  errorv = caml_alloc_tuple (1);
  Store_field (errorv, 0, Val_int (*error));
  args[0] = errorv;
  rv = caml_callbackN_exn (data->fnv, 1, args);
  *error = Int_val (Field (errorv, 0));
  if (Is_exception_result (rv)) {
    nbd_internal_ocaml_exception_in_wrapper ("completion", rv);
    CAMLreturnT (int, -1);
  }

  r = Int_val (rv);
  assert (r >= 0);
  CAMLreturnT (int, r);
}

static int
completion_wrapper (void *user_data, int *error)
{
  int ret = 0;

  caml_leave_blocking_section ();
  ret = completion_wrapper_locked (user_data, error);
  caml_enter_blocking_section ();
  return ret;
}

/* Wrapper for debug callback. */
static int
debug_wrapper_locked (void *user_data, const char *context, const char *msg)
{
  CAMLparam0 ();
  CAMLlocal2 (contextv, msgv);
  CAMLlocal2 (exn, rv);
  const struct user_data *data = user_data;
  int r;
  value args[2];

  contextv = caml_copy_string (context);
  msgv = caml_copy_string (msg);
  args[0] = contextv;
  args[1] = msgv;
  rv = caml_callbackN_exn (data->fnv, 2, args);
  if (Is_exception_result (rv)) {
    nbd_internal_ocaml_exception_in_wrapper ("debug", rv);
    CAMLreturnT (int, -1);
  }

  r = Int_val (rv);
  assert (r >= 0);
  CAMLreturnT (int, r);
}

static int
debug_wrapper (void *user_data, const char *context, const char *msg)
{
  int ret = 0;

  caml_leave_blocking_section ();
  ret = debug_wrapper_locked (user_data, context, msg);
  caml_enter_blocking_section ();
  return ret;
}

/* Wrapper for extent callback. */
static int
extent_wrapper_locked (void *user_data, const char *metacontext,
                       uint64_t offset, uint32_t *entries,
                       size_t nr_entries, int *error)
{
  CAMLparam0 ();
  CAMLlocal4 (metacontextv, offsetv, entriesv, errorv);
  CAMLlocal2 (exn, rv);
  const struct user_data *data = user_data;
  int r;
  value args[4];

  metacontextv = caml_copy_string (metacontext);
  offsetv = caml_copy_int64 (offset);
  entriesv = nbd_internal_ocaml_alloc_i64_from_u32_array (
               entries,
               nr_entries
             );
  errorv = caml_alloc_tuple (1);
  Store_field (errorv, 0, Val_int (*error));
  args[0] = metacontextv;
  args[1] = offsetv;
  args[2] = entriesv;
  args[3] = errorv;
  rv = caml_callbackN_exn (data->fnv, 4, args);
  *error = Int_val (Field (errorv, 0));
  if (Is_exception_result (rv)) {
    nbd_internal_ocaml_exception_in_wrapper ("extent", rv);
    CAMLreturnT (int, -1);
  }

  r = Int_val (rv);
  assert (r >= 0);
  CAMLreturnT (int, r);
}

static int
extent_wrapper (void *user_data, const char *metacontext, uint64_t offset,
                uint32_t *entries, size_t nr_entries, int *error)
{
  int ret = 0;

  caml_leave_blocking_section ();
  ret = extent_wrapper_locked (user_data, metacontext, offset, entries,
                               nr_entries, error);
  caml_enter_blocking_section ();
  return ret;
}

/* Wrapper for extent64 callback. */
static int
extent64_wrapper_locked (void *user_data, const char *metacontext,
                         uint64_t offset, nbd_extent *entries,
                         size_t nr_entries, int *error)
{
  CAMLparam0 ();
  CAMLlocal4 (metacontextv, offsetv, entriesv, errorv);
  CAMLlocal2 (exn, rv);
  const struct user_data *data = user_data;
  int r;
  value args[4];

  metacontextv = caml_copy_string (metacontext);
  offsetv = caml_copy_int64 (offset);
  entriesv = nbd_internal_ocaml_alloc_extent64_array (
               entries,
               nr_entries
             );
  errorv = caml_alloc_tuple (1);
  Store_field (errorv, 0, Val_int (*error));
  args[0] = metacontextv;
  args[1] = offsetv;
  args[2] = entriesv;
  args[3] = errorv;
  rv = caml_callbackN_exn (data->fnv, 4, args);
  *error = Int_val (Field (errorv, 0));
  if (Is_exception_result (rv)) {
    nbd_internal_ocaml_exception_in_wrapper ("extent64", rv);
    CAMLreturnT (int, -1);
  }

  r = Int_val (rv);
  assert (r >= 0);
  CAMLreturnT (int, r);
}

static int
extent64_wrapper (void *user_data, const char *metacontext, uint64_t offset,
                  nbd_extent *entries, size_t nr_entries, int *error)
{
  int ret = 0;

  caml_leave_blocking_section ();
  ret = extent64_wrapper_locked (user_data, metacontext, offset, entries,
                                 nr_entries, error);
  caml_enter_blocking_section ();
  return ret;
}

/* Wrapper for list callback. */
static int
list_wrapper_locked (void *user_data, const char *name,
                     const char *description)
{
  CAMLparam0 ();
  CAMLlocal2 (namev, descriptionv);
  CAMLlocal2 (exn, rv);
  const struct user_data *data = user_data;
  int r;
  value args[2];

  namev = caml_copy_string (name);
  descriptionv = caml_copy_string (description);
  args[0] = namev;
  args[1] = descriptionv;
  rv = caml_callbackN_exn (data->fnv, 2, args);
  if (Is_exception_result (rv)) {
    nbd_internal_ocaml_exception_in_wrapper ("list", rv);
    CAMLreturnT (int, -1);
  }

  r = Int_val (rv);
  assert (r >= 0);
  CAMLreturnT (int, r);
}

static int
list_wrapper (void *user_data, const char *name, const char *description)
{
  int ret = 0;

  caml_leave_blocking_section ();
  ret = list_wrapper_locked (user_data, name, description);
  caml_enter_blocking_section ();
  return ret;
}

/* Wrapper for context callback. */
static int
context_wrapper_locked (void *user_data, const char *name)
{
  CAMLparam0 ();
  CAMLlocal1 (namev);
  CAMLlocal2 (exn, rv);
  const struct user_data *data = user_data;
  int r;
  value args[1];

  namev = caml_copy_string (name);
  args[0] = namev;
  rv = caml_callbackN_exn (data->fnv, 1, args);
  if (Is_exception_result (rv)) {
    nbd_internal_ocaml_exception_in_wrapper ("context", rv);
    CAMLreturnT (int, -1);
  }

  r = Int_val (rv);
  assert (r >= 0);
  CAMLreturnT (int, r);
}

static int
context_wrapper (void *user_data, const char *name)
{
  int ret = 0;

  caml_leave_blocking_section ();
  ret = context_wrapper_locked (user_data, name);
  caml_enter_blocking_section ();
  return ret;
}

/* Convert OCaml TLS.t to int. */
static int
TLS_val (value v)
{
  /* NB: No allocation in this function, don't need to use
   * CAML* wrappers.
   */
  int r = 0;

  if (Is_long (v)) {
    /* Int_val (v) is the index of the enum in the type
     * (eg. v = 0 => enum = TLS.DISABLE).
     * Convert it to the C representation.
     */
    switch (Int_val (v)) {
    case 0: r = LIBNBD_TLS_DISABLE; break;
    case 1: r = LIBNBD_TLS_ALLOW; break;
    case 2: r = LIBNBD_TLS_REQUIRE; break;
    default: abort ();
    }
  }
  else
    r = Int_val (Field (v, 0)); /* UNKNOWN of int */

  return r;
}

/* Convert int to OCaml TLS.t. */
static value
Val_TLS (int i)
{
  CAMLparam0 ();
  CAMLlocal1 (rv);

  switch (i) {
  case LIBNBD_TLS_DISABLE: rv = Val_int (0); break;
  case LIBNBD_TLS_ALLOW: rv = Val_int (1); break;
  case LIBNBD_TLS_REQUIRE: rv = Val_int (2); break;
  default:
    rv = caml_alloc (1, 0); /* UNKNOWN of int */
    Store_field (rv, 0, Val_int (i));
  }

  CAMLreturn (rv);
}

/* Convert OCaml SIZE.t to int. */
static int
SIZE_val (value v)
{
  /* NB: No allocation in this function, don't need to use
   * CAML* wrappers.
   */
  int r = 0;

  if (Is_long (v)) {
    /* Int_val (v) is the index of the enum in the type
     * (eg. v = 0 => enum = SIZE.MINIMUM).
     * Convert it to the C representation.
     */
    switch (Int_val (v)) {
    case 0: r = LIBNBD_SIZE_MINIMUM; break;
    case 1: r = LIBNBD_SIZE_PREFERRED; break;
    case 2: r = LIBNBD_SIZE_MAXIMUM; break;
    case 3: r = LIBNBD_SIZE_PAYLOAD; break;
    default: abort ();
    }
  }
  else
    r = Int_val (Field (v, 0)); /* UNKNOWN of int */

  return r;
}

/* Convert OCaml CMD_FLAG.t list to uint32_t bitmask. */
static uint32_t
CMD_FLAG_val (value v)
{
  /* NB: No allocation in this function, don't need to use
   * CAML* wrappers.
   */
  value i;
  unsigned bit;
  uint32_t r = 0;

  for (; v != Val_emptylist; v = Field (v, 1)) {
    i = Field (v, 0);
    /* i contains either the index of the flag in the type,
     * or UNKNOWN of int containing the bit position.
     * (eg. i = 0 => flag = CMD_FLAG.FUA).
     * Convert it to the C representation.
     */
    if (Is_long (i)) {
      switch (Int_val (i)) {
      case 0: r |= LIBNBD_CMD_FLAG_FUA; break;
      case 1: r |= LIBNBD_CMD_FLAG_NO_HOLE; break;
      case 2: r |= LIBNBD_CMD_FLAG_DF; break;
      case 3: r |= LIBNBD_CMD_FLAG_REQ_ONE; break;
      case 4: r |= LIBNBD_CMD_FLAG_FAST_ZERO; break;
      case 5: r |= LIBNBD_CMD_FLAG_PAYLOAD_LEN; break;
      default: abort ();
      }
    }
    else {
      bit = Int_val (Field (i, 0)); /* UNKNOWN of int */
      if (bit > 31)
        caml_invalid_argument ("bitmask value out of range");
      else
        r |= 1u << bit;
    }
  }

  return r;
}

/* Convert OCaml HANDSHAKE_FLAG.t list to uint32_t bitmask. */
static uint32_t
HANDSHAKE_FLAG_val (value v)
{
  /* NB: No allocation in this function, don't need to use
   * CAML* wrappers.
   */
  value i;
  unsigned bit;
  uint32_t r = 0;

  for (; v != Val_emptylist; v = Field (v, 1)) {
    i = Field (v, 0);
    /* i contains either the index of the flag in the type,
     * or UNKNOWN of int containing the bit position.
     * (eg. i = 0 => flag = HANDSHAKE_FLAG.FIXED_NEWSTYLE).
     * Convert it to the C representation.
     */
    if (Is_long (i)) {
      switch (Int_val (i)) {
      case 0: r |= LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE; break;
      case 1: r |= LIBNBD_HANDSHAKE_FLAG_NO_ZEROES; break;
      default: abort ();
      }
    }
    else {
      bit = Int_val (Field (i, 0)); /* UNKNOWN of int */
      if (bit > 31)
        caml_invalid_argument ("bitmask value out of range");
      else
        r |= 1u << bit;
    }
  }

  return r;
}

/* Convert uint32_t bitmask to OCaml HANDSHAKE_FLAG.t list. */
static value
Val_HANDSHAKE_FLAG (unsigned flags)
{
  CAMLparam0 ();
  CAMLlocal3 (cdr, rv, v);
  int i;

  rv = Val_emptylist;
  for (i = 31; i >= 0; i--) {
    if (flags & (1 << i)) {
      switch (1 << i) {
      case LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE: v = Val_int (0); break;
      case LIBNBD_HANDSHAKE_FLAG_NO_ZEROES: v = Val_int (1); break;
      default:
        v = caml_alloc (1, 0); /* UNKNOWN of int */
        Store_field (v, 0, Val_int (i));
      }

      cdr = rv;
      rv = caml_alloc (2, 0);
      Store_field (rv, 0, v);
      Store_field (rv, 1, cdr);
    }
  }

  CAMLreturn (rv);
}

/* Convert OCaml STRICT.t list to uint32_t bitmask. */
static uint32_t
STRICT_val (value v)
{
  /* NB: No allocation in this function, don't need to use
   * CAML* wrappers.
   */
  value i;
  unsigned bit;
  uint32_t r = 0;

  for (; v != Val_emptylist; v = Field (v, 1)) {
    i = Field (v, 0);
    /* i contains either the index of the flag in the type,
     * or UNKNOWN of int containing the bit position.
     * (eg. i = 0 => flag = STRICT.COMMANDS).
     * Convert it to the C representation.
     */
    if (Is_long (i)) {
      switch (Int_val (i)) {
      case 0: r |= LIBNBD_STRICT_COMMANDS; break;
      case 1: r |= LIBNBD_STRICT_FLAGS; break;
      case 2: r |= LIBNBD_STRICT_BOUNDS; break;
      case 3: r |= LIBNBD_STRICT_ZERO_SIZE; break;
      case 4: r |= LIBNBD_STRICT_ALIGN; break;
      case 5: r |= LIBNBD_STRICT_PAYLOAD; break;
      case 6: r |= LIBNBD_STRICT_AUTO_FLAG; break;
      default: abort ();
      }
    }
    else {
      bit = Int_val (Field (i, 0)); /* UNKNOWN of int */
      if (bit > 31)
        caml_invalid_argument ("bitmask value out of range");
      else
        r |= 1u << bit;
    }
  }

  return r;
}

/* Convert uint32_t bitmask to OCaml STRICT.t list. */
static value
Val_STRICT (unsigned flags)
{
  CAMLparam0 ();
  CAMLlocal3 (cdr, rv, v);
  int i;

  rv = Val_emptylist;
  for (i = 31; i >= 0; i--) {
    if (flags & (1 << i)) {
      switch (1 << i) {
      case LIBNBD_STRICT_COMMANDS: v = Val_int (0); break;
      case LIBNBD_STRICT_FLAGS: v = Val_int (1); break;
      case LIBNBD_STRICT_BOUNDS: v = Val_int (2); break;
      case LIBNBD_STRICT_ZERO_SIZE: v = Val_int (3); break;
      case LIBNBD_STRICT_ALIGN: v = Val_int (4); break;
      case LIBNBD_STRICT_PAYLOAD: v = Val_int (5); break;
      case LIBNBD_STRICT_AUTO_FLAG: v = Val_int (6); break;
      default:
        v = caml_alloc (1, 0); /* UNKNOWN of int */
        Store_field (v, 0, Val_int (i));
      }

      cdr = rv;
      rv = caml_alloc (2, 0);
      Store_field (rv, 0, v);
      Store_field (rv, 1, cdr);
    }
  }

  CAMLreturn (rv);
}

/* Convert OCaml ALLOW_TRANSPORT.t list to uint32_t bitmask. */
static uint32_t
ALLOW_TRANSPORT_val (value v)
{
  /* NB: No allocation in this function, don't need to use
   * CAML* wrappers.
   */
  value i;
  unsigned bit;
  uint32_t r = 0;

  for (; v != Val_emptylist; v = Field (v, 1)) {
    i = Field (v, 0);
    /* i contains either the index of the flag in the type,
     * or UNKNOWN of int containing the bit position.
     * (eg. i = 0 => flag = ALLOW_TRANSPORT.TCP).
     * Convert it to the C representation.
     */
    if (Is_long (i)) {
      switch (Int_val (i)) {
      case 0: r |= LIBNBD_ALLOW_TRANSPORT_TCP; break;
      case 1: r |= LIBNBD_ALLOW_TRANSPORT_UNIX; break;
      case 2: r |= LIBNBD_ALLOW_TRANSPORT_VSOCK; break;
      case 3: r |= LIBNBD_ALLOW_TRANSPORT_SSH; break;
      default: abort ();
      }
    }
    else {
      bit = Int_val (Field (i, 0)); /* UNKNOWN of int */
      if (bit > 31)
        caml_invalid_argument ("bitmask value out of range");
      else
        r |= 1u << bit;
    }
  }

  return r;
}

/* Convert OCaml SHUTDOWN.t list to uint32_t bitmask. */
static uint32_t
SHUTDOWN_val (value v)
{
  /* NB: No allocation in this function, don't need to use
   * CAML* wrappers.
   */
  value i;
  unsigned bit;
  uint32_t r = 0;

  for (; v != Val_emptylist; v = Field (v, 1)) {
    i = Field (v, 0);
    /* i contains either the index of the flag in the type,
     * or UNKNOWN of int containing the bit position.
     * (eg. i = 0 => flag = SHUTDOWN.ABANDON_PENDING).
     * Convert it to the C representation.
     */
    if (Is_long (i)) {
      switch (Int_val (i)) {
      case 0: r |= LIBNBD_SHUTDOWN_ABANDON_PENDING; break;
      default: abort ();
      }
    }
    else {
      bit = Int_val (Field (i, 0)); /* UNKNOWN of int */
      if (bit > 31)
        caml_invalid_argument ("bitmask value out of range");
      else
        r |= 1u << bit;
    }
  }

  return r;
}

value
nbd_internal_ocaml_nbd_set_debug (value hv, value debugv)
{
  CAMLparam2 (hv, debugv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_debug"
    );

  bool debug = Bool_val (debugv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_debug (h, debug);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_debug (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_debug"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_get_debug (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_debug_callback (value hv, value debugv)
{
  CAMLparam2 (hv, debugv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_debug_callback"
    );

  nbd_debug_callback debug_callback;
  struct user_data *debug_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  debug_user_data->fnv = debugv;
  caml_register_generational_global_root (&debug_user_data->fnv);
  debug_callback.callback = debug_wrapper;
  debug_callback.user_data = debug_user_data;
  debug_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_debug_callback (h, debug_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_clear_debug_callback (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.clear_debug_callback"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_clear_debug_callback (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_stats_bytes_sent (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.stats_bytes_sent"
    );

  uint64_t r;

  caml_enter_blocking_section ();
  r =  nbd_stats_bytes_sent (h);
  caml_leave_blocking_section ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_stats_chunks_sent (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.stats_chunks_sent"
    );

  uint64_t r;

  caml_enter_blocking_section ();
  r =  nbd_stats_chunks_sent (h);
  caml_leave_blocking_section ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_stats_bytes_received (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.stats_bytes_received"
    );

  uint64_t r;

  caml_enter_blocking_section ();
  r =  nbd_stats_bytes_received (h);
  caml_leave_blocking_section ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_stats_chunks_received (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.stats_chunks_received"
    );

  uint64_t r;

  caml_enter_blocking_section ();
  r =  nbd_stats_chunks_received (h);
  caml_leave_blocking_section ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_handle_name (value hv, value handle_namev)
{
  CAMLparam2 (hv, handle_namev);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_handle_name"
    );

  const char *handle_name = String_val (handle_namev);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_handle_name (h, handle_name);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_handle_name (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_handle_name"
    );

  char * r;

  caml_enter_blocking_section ();
  r =  nbd_get_handle_name (h);
  caml_leave_blocking_section ();

  if (r == NULL)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_string (r);
  free (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_private_data (value hv, value private_datav)
{
  CAMLparam2 (hv, private_datav);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_private_data"
    );

  unsigned private_data = Int_val (private_datav);
  uintptr_t r;

  caml_enter_blocking_section ();
  r =  nbd_set_private_data (h, private_data);
  caml_leave_blocking_section ();

  rv = Val_int (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_private_data (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_private_data"
    );

  uintptr_t r;

  caml_enter_blocking_section ();
  r =  nbd_get_private_data (h);
  caml_leave_blocking_section ();

  rv = Val_int (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_export_name (value hv, value export_namev)
{
  CAMLparam2 (hv, export_namev);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_export_name"
    );

  const char *export_name = String_val (export_namev);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_export_name (h, export_name);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_export_name (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_export_name"
    );

  char * r;

  caml_enter_blocking_section ();
  r =  nbd_get_export_name (h);
  caml_leave_blocking_section ();

  if (r == NULL)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_string (r);
  free (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_request_block_size (value hv, value requestv)
{
  CAMLparam2 (hv, requestv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_request_block_size"
    );

  bool request = Bool_val (requestv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_request_block_size (h, request);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_request_block_size (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_request_block_size"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_get_request_block_size (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_full_info (value hv, value requestv)
{
  CAMLparam2 (hv, requestv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_full_info"
    );

  bool request = Bool_val (requestv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_full_info (h, request);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_full_info (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_full_info"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_get_full_info (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_canonical_export_name (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_canonical_export_name"
    );

  char * r;

  caml_enter_blocking_section ();
  r =  nbd_get_canonical_export_name (h);
  caml_leave_blocking_section ();

  if (r == NULL)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_string (r);
  free (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_export_description (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_export_description"
    );

  char * r;

  caml_enter_blocking_section ();
  r =  nbd_get_export_description (h);
  caml_leave_blocking_section ();

  if (r == NULL)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_string (r);
  free (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_tls (value hv, value tlsv)
{
  CAMLparam2 (hv, tlsv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_tls"
    );

  int tls = TLS_val (tlsv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_tls (h, tls);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_tls (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_tls"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_get_tls (h);
  caml_leave_blocking_section ();

  rv = Val_TLS (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_tls_negotiated (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_tls_negotiated"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_get_tls_negotiated (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_tls_certificates (value hv, value dirv)
{
  CAMLparam2 (hv, dirv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_tls_certificates"
    );

  const char *dir = String_val (dirv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_tls_certificates (h, dir);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_tls_verify_peer (value hv, value verifyv)
{
  CAMLparam2 (hv, verifyv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_tls_verify_peer"
    );

  bool verify = Bool_val (verifyv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_tls_verify_peer (h, verify);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_tls_verify_peer (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_tls_verify_peer"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_get_tls_verify_peer (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_tls_username (value hv, value usernamev)
{
  CAMLparam2 (hv, usernamev);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_tls_username"
    );

  const char *username = String_val (usernamev);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_tls_username (h, username);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_tls_username (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_tls_username"
    );

  char * r;

  caml_enter_blocking_section ();
  r =  nbd_get_tls_username (h);
  caml_leave_blocking_section ();

  if (r == NULL)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_string (r);
  free (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_tls_hostname (value hv, value hostnamev)
{
  CAMLparam2 (hv, hostnamev);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_tls_hostname"
    );

  const char *hostname = String_val (hostnamev);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_tls_hostname (h, hostname);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_tls_hostname (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_tls_hostname"
    );

  char * r;

  caml_enter_blocking_section ();
  r =  nbd_get_tls_hostname (h);
  caml_leave_blocking_section ();

  if (r == NULL)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_string (r);
  free (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_tls_psk_file (value hv, value filenamev)
{
  CAMLparam2 (hv, filenamev);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_tls_psk_file"
    );

  const char *filename = String_val (filenamev);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_tls_psk_file (h, filename);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_request_extended_headers (value hv,
                                                     value requestv)
{
  CAMLparam2 (hv, requestv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_request_extended_headers"
    );

  bool request = Bool_val (requestv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_request_extended_headers (h, request);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_request_extended_headers (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_request_extended_headers"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_get_request_extended_headers (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_extended_headers_negotiated (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_extended_headers_negotiated"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_get_extended_headers_negotiated (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_request_structured_replies (value hv,
                                                       value requestv)
{
  CAMLparam2 (hv, requestv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_request_structured_replies"
    );

  bool request = Bool_val (requestv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_request_structured_replies (h, request);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_request_structured_replies (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_request_structured_replies"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_get_request_structured_replies (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_structured_replies_negotiated (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_structured_replies_negotiated"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_get_structured_replies_negotiated (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_request_meta_context (value hv, value requestv)
{
  CAMLparam2 (hv, requestv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_request_meta_context"
    );

  bool request = Bool_val (requestv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_request_meta_context (h, request);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_request_meta_context (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_request_meta_context"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_get_request_meta_context (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_handshake_flags (value hv, value flagsv)
{
  CAMLparam2 (hv, flagsv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_handshake_flags"
    );

  uint32_t flags = HANDSHAKE_FLAG_val (flagsv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_handshake_flags (h, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_handshake_flags (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_handshake_flags"
    );

  uint32_t r;

  caml_enter_blocking_section ();
  r =  nbd_get_handshake_flags (h);
  caml_leave_blocking_section ();

  rv = Val_HANDSHAKE_FLAG (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_pread_initialize (value hv, value requestv)
{
  CAMLparam2 (hv, requestv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_pread_initialize"
    );

  bool request = Bool_val (requestv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_pread_initialize (h, request);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_pread_initialize (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_pread_initialize"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_get_pread_initialize (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_strict_mode (value hv, value flagsv)
{
  CAMLparam2 (hv, flagsv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_strict_mode"
    );

  uint32_t flags = STRICT_val (flagsv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_strict_mode (h, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_strict_mode (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_strict_mode"
    );

  uint32_t r;

  caml_enter_blocking_section ();
  r =  nbd_get_strict_mode (h);
  caml_leave_blocking_section ();

  rv = Val_STRICT (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_opt_mode (value hv, value enablev)
{
  CAMLparam2 (hv, enablev);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_opt_mode"
    );

  bool enable = Bool_val (enablev);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_opt_mode (h, enable);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_opt_mode (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_opt_mode"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_get_opt_mode (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_opt_go (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.opt_go"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_opt_go (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_opt_abort (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.opt_abort"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_opt_abort (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_opt_starttls (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.opt_starttls"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_opt_starttls (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_opt_extended_headers (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.opt_extended_headers"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_opt_extended_headers (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_opt_structured_reply (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.opt_structured_reply"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_opt_structured_reply (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_opt_list (value hv, value listv)
{
  CAMLparam2 (hv, listv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.opt_list"
    );

  nbd_list_callback list_callback;
  struct user_data *list_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  list_user_data->fnv = listv;
  caml_register_generational_global_root (&list_user_data->fnv);
  list_callback.callback = list_wrapper;
  list_callback.user_data = list_user_data;
  list_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_opt_list (h, list_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_int (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_opt_info (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.opt_info"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_opt_info (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_opt_list_meta_context (value hv, value contextv)
{
  CAMLparam2 (hv, contextv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.opt_list_meta_context"
    );

  nbd_context_callback context_callback;
  struct user_data *context_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  context_user_data->fnv = contextv;
  caml_register_generational_global_root (&context_user_data->fnv);
  context_callback.callback = context_wrapper;
  context_callback.user_data = context_user_data;
  context_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_opt_list_meta_context (h, context_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_int (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_opt_list_meta_context_queries (value hv,
                                                      value queriesv,
                                                      value contextv)
{
  CAMLparam3 (hv, queriesv, contextv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.opt_list_meta_context_queries"
    );

  char **queries = (char **)nbd_internal_ocaml_string_list (queriesv);
  nbd_context_callback context_callback;
  struct user_data *context_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  context_user_data->fnv = contextv;
  caml_register_generational_global_root (&context_user_data->fnv);
  context_callback.callback = context_wrapper;
  context_callback.user_data = context_user_data;
  context_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_opt_list_meta_context_queries (h, queries, context_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_int (r);
  free (queries);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_opt_set_meta_context (value hv, value contextv)
{
  CAMLparam2 (hv, contextv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.opt_set_meta_context"
    );

  nbd_context_callback context_callback;
  struct user_data *context_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  context_user_data->fnv = contextv;
  caml_register_generational_global_root (&context_user_data->fnv);
  context_callback.callback = context_wrapper;
  context_callback.user_data = context_user_data;
  context_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_opt_set_meta_context (h, context_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_int (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_opt_set_meta_context_queries (value hv,
                                                     value queriesv,
                                                     value contextv)
{
  CAMLparam3 (hv, queriesv, contextv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.opt_set_meta_context_queries"
    );

  char **queries = (char **)nbd_internal_ocaml_string_list (queriesv);
  nbd_context_callback context_callback;
  struct user_data *context_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  context_user_data->fnv = contextv;
  caml_register_generational_global_root (&context_user_data->fnv);
  context_callback.callback = context_wrapper;
  context_callback.user_data = context_user_data;
  context_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_opt_set_meta_context_queries (h, queries, context_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_int (r);
  free (queries);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_add_meta_context (value hv, value namev)
{
  CAMLparam2 (hv, namev);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.add_meta_context"
    );

  const char *name = String_val (namev);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_add_meta_context (h, name);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_nr_meta_contexts (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_nr_meta_contexts"
    );

  ssize_t r;

  caml_enter_blocking_section ();
  r =  nbd_get_nr_meta_contexts (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_int (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_meta_context (value hv, value iv)
{
  CAMLparam2 (hv, iv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_meta_context"
    );

  size_t i = Int_val (iv);
  char * r;

  caml_enter_blocking_section ();
  r =  nbd_get_meta_context (h, i);
  caml_leave_blocking_section ();

  if (r == NULL)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_string (r);
  free (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_clear_meta_contexts (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.clear_meta_contexts"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_clear_meta_contexts (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_uri_allow_transports (value hv, value maskv)
{
  CAMLparam2 (hv, maskv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_uri_allow_transports"
    );

  uint32_t mask = ALLOW_TRANSPORT_val (maskv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_uri_allow_transports (h, mask);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_uri_allow_tls (value hv, value tlsv)
{
  CAMLparam2 (hv, tlsv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_uri_allow_tls"
    );

  int tls = TLS_val (tlsv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_uri_allow_tls (h, tls);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_uri_allow_local_file (value hv, value allowv)
{
  CAMLparam2 (hv, allowv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_uri_allow_local_file"
    );

  bool allow = Bool_val (allowv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_uri_allow_local_file (h, allow);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_connect_uri (value hv, value uriv)
{
  CAMLparam2 (hv, uriv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.connect_uri"
    );

  const char *uri = String_val (uriv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_connect_uri (h, uri);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_connect_unix (value hv, value unixsocketv)
{
  CAMLparam2 (hv, unixsocketv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.connect_unix"
    );

  const char *unixsocket = String_val (unixsocketv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_connect_unix (h, unixsocket);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_connect_vsock (value hv, value cidv, value portv)
{
  CAMLparam3 (hv, cidv, portv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.connect_vsock"
    );

  int64_t cid64 = Int64_val (cidv);
  if (cid64 < 0 || (uint64_t)cid64 > UINT32_MAX)
    caml_invalid_argument ("'cid' out of range");
  uint32_t cid = (uint32_t)cid64;
  int64_t port64 = Int64_val (portv);
  if (port64 < 0 || (uint64_t)port64 > UINT32_MAX)
    caml_invalid_argument ("'port' out of range");
  uint32_t port = (uint32_t)port64;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_connect_vsock (h, cid, port);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_connect_tcp (value hv, value hostnamev, value portv)
{
  CAMLparam3 (hv, hostnamev, portv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.connect_tcp"
    );

  const char *hostname = String_val (hostnamev);
  const char *port = String_val (portv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_connect_tcp (h, hostname, port);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_connect_socket (value hv, value sockv)
{
  CAMLparam2 (hv, sockv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.connect_socket"
    );

  /* OCaml Unix.file_descr is just an int, at least on Unix. */
  int sock = Int_val (sockv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_connect_socket (h, sock);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_connect_command (value hv, value argvv)
{
  CAMLparam2 (hv, argvv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.connect_command"
    );

  char **argv = (char **)nbd_internal_ocaml_string_list (argvv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_connect_command (h, argv);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  free (argv);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_connect_systemd_socket_activation (value hv,
                                                          value argvv)
{
  CAMLparam2 (hv, argvv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.connect_systemd_socket_activation"
    );

  char **argv = (char **)nbd_internal_ocaml_string_list (argvv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_connect_systemd_socket_activation (h, argv);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  free (argv);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_set_socket_activation_name (value hv,
                                                   value socket_namev)
{
  CAMLparam2 (hv, socket_namev);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.set_socket_activation_name"
    );

  const char *socket_name = String_val (socket_namev);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_set_socket_activation_name (h, socket_name);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_socket_activation_name (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_socket_activation_name"
    );

  char * r;

  caml_enter_blocking_section ();
  r =  nbd_get_socket_activation_name (h);
  caml_leave_blocking_section ();

  if (r == NULL)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_string (r);
  free (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_is_read_only (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.is_read_only"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_is_read_only (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_can_flush (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.can_flush"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_can_flush (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_can_fua (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.can_fua"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_can_fua (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_is_rotational (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.is_rotational"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_is_rotational (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_can_trim (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.can_trim"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_can_trim (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_can_zero (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.can_zero"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_can_zero (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_can_fast_zero (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.can_fast_zero"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_can_fast_zero (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_can_block_status_payload (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.can_block_status_payload"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_can_block_status_payload (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_can_df (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.can_df"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_can_df (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_can_multi_conn (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.can_multi_conn"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_can_multi_conn (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_can_cache (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.can_cache"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_can_cache (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_can_meta_context (value hv, value metacontextv)
{
  CAMLparam2 (hv, metacontextv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.can_meta_context"
    );

  const char *metacontext = String_val (metacontextv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_can_meta_context (h, metacontext);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_protocol (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_protocol"
    );

  const char * r;

  caml_enter_blocking_section ();
  r =  nbd_get_protocol (h);
  caml_leave_blocking_section ();

  if (r == NULL)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_string (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_size (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_size"
    );

  int64_t r;

  caml_enter_blocking_section ();
  r =  nbd_get_size (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_block_size (value hv, value size_typev)
{
  CAMLparam2 (hv, size_typev);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_block_size"
    );

  int size_type = SIZE_val (size_typev);
  int64_t r;

  caml_enter_blocking_section ();
  r =  nbd_get_block_size (h, size_type);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_pread (value flagsv, value hv, value bufv,
                              value offsetv)
{
  CAMLparam4 (flagsv, hv, bufv, offsetv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.pread"
    );

  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  void *buf = Bytes_val (bufv);
  size_t count = caml_string_length (bufv);
  uint64_t offset = Int64_val (offsetv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_pread (h, buf, count, offset, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_pread_structured (value flagsv, value hv, value bufv,
                                         value offsetv, value chunkv)
{
  CAMLparam5 (flagsv, hv, bufv, offsetv, chunkv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.pread_structured"
    );

  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  void *buf = Bytes_val (bufv);
  size_t count = caml_string_length (bufv);
  uint64_t offset = Int64_val (offsetv);
  nbd_chunk_callback chunk_callback;
  struct user_data *chunk_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  chunk_user_data->fnv = chunkv;
  caml_register_generational_global_root (&chunk_user_data->fnv);
  chunk_callback.callback = chunk_wrapper;
  chunk_callback.user_data = chunk_user_data;
  chunk_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_pread_structured (h, buf, count, offset, chunk_callback, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_pwrite (value flagsv, value hv, value bufv,
                               value offsetv)
{
  CAMLparam4 (flagsv, hv, bufv, offsetv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.pwrite"
    );

  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  const void *buf = Bytes_val (bufv);
  size_t count = caml_string_length (bufv);
  uint64_t offset = Int64_val (offsetv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_pwrite (h, buf, count, offset, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_shutdown (value flagsv, value hv)
{
  CAMLparam2 (flagsv, hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.shutdown"
    );

  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of SHUTDOWN.t ] */
    flags = SHUTDOWN_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_shutdown (h, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_flush (value flagsv, value hv)
{
  CAMLparam2 (flagsv, hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.flush"
    );

  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_flush (h, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_trim (value flagsv, value hv, value countv,
                             value offsetv)
{
  CAMLparam4 (flagsv, hv, countv, offsetv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.trim"
    );

  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  uint64_t count = Int64_val (countv);
  uint64_t offset = Int64_val (offsetv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_trim (h, count, offset, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_cache (value flagsv, value hv, value countv,
                              value offsetv)
{
  CAMLparam4 (flagsv, hv, countv, offsetv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.cache"
    );

  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  uint64_t count = Int64_val (countv);
  uint64_t offset = Int64_val (offsetv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_cache (h, count, offset, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_zero (value flagsv, value hv, value countv,
                             value offsetv)
{
  CAMLparam4 (flagsv, hv, countv, offsetv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.zero"
    );

  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  uint64_t count = Int64_val (countv);
  uint64_t offset = Int64_val (offsetv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_zero (h, count, offset, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_block_status (value flagsv, value hv, value countv,
                                     value offsetv, value extentv)
{
  CAMLparam5 (flagsv, hv, countv, offsetv, extentv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.block_status"
    );

  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  uint64_t count = Int64_val (countv);
  uint64_t offset = Int64_val (offsetv);
  nbd_extent_callback extent_callback;
  struct user_data *extent_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  extent_user_data->fnv = extentv;
  caml_register_generational_global_root (&extent_user_data->fnv);
  extent_callback.callback = extent_wrapper;
  extent_callback.user_data = extent_user_data;
  extent_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_block_status (h, count, offset, extent_callback, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_block_status_64 (value flagsv, value hv,
                                        value countv, value offsetv,
                                        value extent64v)
{
  CAMLparam5 (flagsv, hv, countv, offsetv, extent64v);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.block_status_64"
    );

  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  uint64_t count = Int64_val (countv);
  uint64_t offset = Int64_val (offsetv);
  nbd_extent64_callback extent64_callback;
  struct user_data *extent64_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  extent64_user_data->fnv = extent64v;
  caml_register_generational_global_root (&extent64_user_data->fnv);
  extent64_callback.callback = extent64_wrapper;
  extent64_callback.user_data = extent64_user_data;
  extent64_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_block_status_64 (h, count, offset, extent64_callback, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_block_status_filter (value flagsv, value hv,
                                            value countv, value offsetv,
                                            value contextsv,
                                            value extent64v)
{
  CAMLparam5 (flagsv, hv, countv, offsetv, contextsv);
  CAMLxparam1 (extent64v);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.block_status_filter"
    );

  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  uint64_t count = Int64_val (countv);
  uint64_t offset = Int64_val (offsetv);
  char **contexts = (char **)nbd_internal_ocaml_string_list (contextsv);
  nbd_extent64_callback extent64_callback;
  struct user_data *extent64_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  extent64_user_data->fnv = extent64v;
  caml_register_generational_global_root (&extent64_user_data->fnv);
  extent64_callback.callback = extent64_wrapper;
  extent64_callback.user_data = extent64_user_data;
  extent64_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_block_status_filter (h, count, offset, contexts,
                                extent64_callback, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  free (contexts);
  CAMLreturn (rv);
}

/* Byte-code compat function because this method has > 5 parameters.
 */
value
nbd_internal_ocaml_nbd_block_status_filter_byte (value *argv, int argn)
{
  return nbd_internal_ocaml_nbd_block_status_filter (argv[0], argv[1],
                                                     argv[2], argv[3],
                                                     argv[4], argv[5]);
}

value
nbd_internal_ocaml_nbd_poll (value hv, value timeoutv)
{
  CAMLparam2 (hv, timeoutv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.poll"
    );

  int timeout = Int_val (timeoutv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_poll (h, timeout);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_int (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_poll2 (value hv, value fdv, value timeoutv)
{
  CAMLparam3 (hv, fdv, timeoutv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.poll2"
    );

  /* OCaml Unix.file_descr is just an int, at least on Unix. */
  int fd = Int_val (fdv);
  int timeout = Int_val (timeoutv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_poll2 (h, fd, timeout);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_int (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_connect (value hv, value addrv)
{
  CAMLparam2 (hv, addrv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_connect"
    );

  struct sockaddr_storage addr_storage;
  struct sockaddr *addr = (struct sockaddr *)&addr_storage;
  socklen_t addrlen;
  nbd_internal_unix_sockaddr_to_sa (addrv, &addr_storage, &addrlen);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_connect (h, addr, addrlen);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_connect_uri (value hv, value uriv)
{
  CAMLparam2 (hv, uriv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_connect_uri"
    );

  const char *uri = String_val (uriv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_connect_uri (h, uri);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_connect_unix (value hv, value unixsocketv)
{
  CAMLparam2 (hv, unixsocketv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_connect_unix"
    );

  const char *unixsocket = String_val (unixsocketv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_connect_unix (h, unixsocket);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_connect_vsock (value hv, value cidv, value portv)
{
  CAMLparam3 (hv, cidv, portv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_connect_vsock"
    );

  int64_t cid64 = Int64_val (cidv);
  if (cid64 < 0 || (uint64_t)cid64 > UINT32_MAX)
    caml_invalid_argument ("'cid' out of range");
  uint32_t cid = (uint32_t)cid64;
  int64_t port64 = Int64_val (portv);
  if (port64 < 0 || (uint64_t)port64 > UINT32_MAX)
    caml_invalid_argument ("'port' out of range");
  uint32_t port = (uint32_t)port64;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_connect_vsock (h, cid, port);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_connect_tcp (value hv, value hostnamev,
                                        value portv)
{
  CAMLparam3 (hv, hostnamev, portv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_connect_tcp"
    );

  const char *hostname = String_val (hostnamev);
  const char *port = String_val (portv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_connect_tcp (h, hostname, port);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_connect_socket (value hv, value sockv)
{
  CAMLparam2 (hv, sockv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_connect_socket"
    );

  /* OCaml Unix.file_descr is just an int, at least on Unix. */
  int sock = Int_val (sockv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_connect_socket (h, sock);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_connect_command (value hv, value argvv)
{
  CAMLparam2 (hv, argvv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_connect_command"
    );

  char **argv = (char **)nbd_internal_ocaml_string_list (argvv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_connect_command (h, argv);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  free (argv);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_connect_systemd_socket_activation (value hv,
                                                              value argvv)
{
  CAMLparam2 (hv, argvv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_connect_systemd_socket_activation"
    );

  char **argv = (char **)nbd_internal_ocaml_string_list (argvv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_connect_systemd_socket_activation (h, argv);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  free (argv);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_opt_go (value completionv, value hv)
{
  CAMLparam2 (completionv, hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_opt_go"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_opt_go (h, completion_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_opt_abort (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_opt_abort"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_opt_abort (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_opt_starttls (value completionv, value hv)
{
  CAMLparam2 (completionv, hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_opt_starttls"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_opt_starttls (h, completion_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_opt_extended_headers (value completionv,
                                                 value hv)
{
  CAMLparam2 (completionv, hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_opt_extended_headers"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_opt_extended_headers (h, completion_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_opt_structured_reply (value completionv,
                                                 value hv)
{
  CAMLparam2 (completionv, hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_opt_structured_reply"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_opt_structured_reply (h, completion_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_opt_list (value completionv, value hv,
                                     value listv)
{
  CAMLparam3 (completionv, hv, listv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_opt_list"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  nbd_list_callback list_callback;
  struct user_data *list_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  list_user_data->fnv = listv;
  caml_register_generational_global_root (&list_user_data->fnv);
  list_callback.callback = list_wrapper;
  list_callback.user_data = list_user_data;
  list_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_opt_list (h, list_callback, completion_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_opt_info (value completionv, value hv)
{
  CAMLparam2 (completionv, hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_opt_info"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_opt_info (h, completion_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_opt_list_meta_context (value completionv,
                                                  value hv, value contextv)
{
  CAMLparam3 (completionv, hv, contextv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_opt_list_meta_context"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  nbd_context_callback context_callback;
  struct user_data *context_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  context_user_data->fnv = contextv;
  caml_register_generational_global_root (&context_user_data->fnv);
  context_callback.callback = context_wrapper;
  context_callback.user_data = context_user_data;
  context_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_opt_list_meta_context (h, context_callback,
                                      completion_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_int (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_opt_list_meta_context_queries (value completionv,
                                                          value hv,
                                                          value queriesv,
                                                          value contextv)
{
  CAMLparam4 (completionv, hv, queriesv, contextv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_opt_list_meta_context_queries"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  char **queries = (char **)nbd_internal_ocaml_string_list (queriesv);
  nbd_context_callback context_callback;
  struct user_data *context_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  context_user_data->fnv = contextv;
  caml_register_generational_global_root (&context_user_data->fnv);
  context_callback.callback = context_wrapper;
  context_callback.user_data = context_user_data;
  context_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_opt_list_meta_context_queries (h, queries, context_callback,
                                              completion_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_int (r);
  free (queries);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_opt_set_meta_context (value completionv,
                                                 value hv, value contextv)
{
  CAMLparam3 (completionv, hv, contextv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_opt_set_meta_context"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  nbd_context_callback context_callback;
  struct user_data *context_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  context_user_data->fnv = contextv;
  caml_register_generational_global_root (&context_user_data->fnv);
  context_callback.callback = context_wrapper;
  context_callback.user_data = context_user_data;
  context_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_opt_set_meta_context (h, context_callback,
                                     completion_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_int (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_opt_set_meta_context_queries (value completionv,
                                                         value hv,
                                                         value queriesv,
                                                         value contextv)
{
  CAMLparam4 (completionv, hv, queriesv, contextv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_opt_set_meta_context_queries"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  char **queries = (char **)nbd_internal_ocaml_string_list (queriesv);
  nbd_context_callback context_callback;
  struct user_data *context_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  context_user_data->fnv = contextv;
  caml_register_generational_global_root (&context_user_data->fnv);
  context_callback.callback = context_wrapper;
  context_callback.user_data = context_user_data;
  context_callback.free = free_user_data;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_opt_set_meta_context_queries (h, queries, context_callback,
                                             completion_callback);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_int (r);
  free (queries);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_pread (value completionv, value flagsv, value hv,
                                  value bufv, value offsetv)
{
  CAMLparam5 (completionv, flagsv, hv, bufv, offsetv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_pread"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  struct caml_ba_array *buf_ba = Caml_ba_array_val (bufv);
  void *buf = (void *) buf_ba->data;
  size_t count = buf_ba->dim[0];
  uint64_t offset = Int64_val (offsetv);
  completion_user_data->bufv = bufv;
  caml_register_generational_global_root (&completion_user_data->bufv);
  int64_t r;

  caml_enter_blocking_section ();
  r =  nbd_aio_pread (h, buf, count, offset, completion_callback, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_pread_structured (value completionv,
                                             value flagsv, value hv,
                                             value bufv, value offsetv,
                                             value chunkv)
{
  CAMLparam5 (completionv, flagsv, hv, bufv, offsetv);
  CAMLxparam1 (chunkv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_pread_structured"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  struct caml_ba_array *buf_ba = Caml_ba_array_val (bufv);
  void *buf = (void *) buf_ba->data;
  size_t count = buf_ba->dim[0];
  uint64_t offset = Int64_val (offsetv);
  nbd_chunk_callback chunk_callback;
  struct user_data *chunk_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  chunk_user_data->fnv = chunkv;
  caml_register_generational_global_root (&chunk_user_data->fnv);
  chunk_callback.callback = chunk_wrapper;
  chunk_callback.user_data = chunk_user_data;
  chunk_callback.free = free_user_data;
  completion_user_data->bufv = bufv;
  caml_register_generational_global_root (&completion_user_data->bufv);
  int64_t r;

  caml_enter_blocking_section ();
  r =  nbd_aio_pread_structured (h, buf, count, offset, chunk_callback,
                                 completion_callback, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

/* Byte-code compat function because this method has > 5 parameters.
 */
value
nbd_internal_ocaml_nbd_aio_pread_structured_byte (value *argv, int argn)
{
  return nbd_internal_ocaml_nbd_aio_pread_structured (argv[0], argv[1],
                                                      argv[2], argv[3],
                                                      argv[4], argv[5]);
}

value
nbd_internal_ocaml_nbd_aio_pwrite (value completionv, value flagsv,
                                   value hv, value bufv, value offsetv)
{
  CAMLparam5 (completionv, flagsv, hv, bufv, offsetv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_pwrite"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  struct caml_ba_array *buf_ba = Caml_ba_array_val (bufv);
  const void *buf = (const void *) buf_ba->data;
  size_t count = buf_ba->dim[0];
  uint64_t offset = Int64_val (offsetv);
  completion_user_data->bufv = bufv;
  caml_register_generational_global_root (&completion_user_data->bufv);
  int64_t r;

  caml_enter_blocking_section ();
  r =  nbd_aio_pwrite (h, buf, count, offset, completion_callback, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_disconnect (value flagsv, value hv)
{
  CAMLparam2 (flagsv, hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_disconnect"
    );

  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_disconnect (h, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_flush (value completionv, value flagsv, value hv)
{
  CAMLparam3 (completionv, flagsv, hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_flush"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  int64_t r;

  caml_enter_blocking_section ();
  r =  nbd_aio_flush (h, completion_callback, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_trim (value completionv, value flagsv, value hv,
                                 value countv, value offsetv)
{
  CAMLparam5 (completionv, flagsv, hv, countv, offsetv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_trim"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  uint64_t count = Int64_val (countv);
  uint64_t offset = Int64_val (offsetv);
  int64_t r;

  caml_enter_blocking_section ();
  r =  nbd_aio_trim (h, count, offset, completion_callback, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_cache (value completionv, value flagsv, value hv,
                                  value countv, value offsetv)
{
  CAMLparam5 (completionv, flagsv, hv, countv, offsetv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_cache"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  uint64_t count = Int64_val (countv);
  uint64_t offset = Int64_val (offsetv);
  int64_t r;

  caml_enter_blocking_section ();
  r =  nbd_aio_cache (h, count, offset, completion_callback, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_zero (value completionv, value flagsv, value hv,
                                 value countv, value offsetv)
{
  CAMLparam5 (completionv, flagsv, hv, countv, offsetv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_zero"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  uint64_t count = Int64_val (countv);
  uint64_t offset = Int64_val (offsetv);
  int64_t r;

  caml_enter_blocking_section ();
  r =  nbd_aio_zero (h, count, offset, completion_callback, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_block_status (value completionv, value flagsv,
                                         value hv, value countv,
                                         value offsetv, value extentv)
{
  CAMLparam5 (completionv, flagsv, hv, countv, offsetv);
  CAMLxparam1 (extentv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_block_status"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  uint64_t count = Int64_val (countv);
  uint64_t offset = Int64_val (offsetv);
  nbd_extent_callback extent_callback;
  struct user_data *extent_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  extent_user_data->fnv = extentv;
  caml_register_generational_global_root (&extent_user_data->fnv);
  extent_callback.callback = extent_wrapper;
  extent_callback.user_data = extent_user_data;
  extent_callback.free = free_user_data;
  int64_t r;

  caml_enter_blocking_section ();
  r =  nbd_aio_block_status (h, count, offset, extent_callback,
                             completion_callback, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

/* Byte-code compat function because this method has > 5 parameters.
 */
value
nbd_internal_ocaml_nbd_aio_block_status_byte (value *argv, int argn)
{
  return nbd_internal_ocaml_nbd_aio_block_status (argv[0], argv[1], argv[2],
                                                  argv[3], argv[4], argv[5]);
}

value
nbd_internal_ocaml_nbd_aio_block_status_64 (value completionv, value flagsv,
                                            value hv, value countv,
                                            value offsetv, value extent64v)
{
  CAMLparam5 (completionv, flagsv, hv, countv, offsetv);
  CAMLxparam1 (extent64v);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_block_status_64"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  uint64_t count = Int64_val (countv);
  uint64_t offset = Int64_val (offsetv);
  nbd_extent64_callback extent64_callback;
  struct user_data *extent64_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  extent64_user_data->fnv = extent64v;
  caml_register_generational_global_root (&extent64_user_data->fnv);
  extent64_callback.callback = extent64_wrapper;
  extent64_callback.user_data = extent64_user_data;
  extent64_callback.free = free_user_data;
  int64_t r;

  caml_enter_blocking_section ();
  r =  nbd_aio_block_status_64 (h, count, offset, extent64_callback,
                                completion_callback, flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

/* Byte-code compat function because this method has > 5 parameters.
 */
value
nbd_internal_ocaml_nbd_aio_block_status_64_byte (value *argv, int argn)
{
  return nbd_internal_ocaml_nbd_aio_block_status_64 (argv[0], argv[1],
                                                     argv[2], argv[3],
                                                     argv[4], argv[5]);
}

value
nbd_internal_ocaml_nbd_aio_block_status_filter (value completionv,
                                                value flagsv, value hv,
                                                value countv, value offsetv,
                                                value contextsv,
                                                value extent64v)
{
  CAMLparam5 (completionv, flagsv, hv, countv, offsetv);
  CAMLxparam2 (contextsv, extent64v);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_block_status_filter"
    );

  nbd_completion_callback completion_callback = {0};
  struct user_data *completion_user_data = alloc_user_data ();
  if (completionv != Val_int (0)) { /* Some closure */
    /* The function may save a reference to the closure, so we
     * must treat it as a possible GC root.
     */
    completion_user_data->fnv = Field (completionv, 0);
    caml_register_generational_global_root (&completion_user_data->fnv);
    completion_callback.callback = completion_wrapper;
  }
  completion_callback.user_data = completion_user_data;
  completion_callback.free = free_user_data;
  uint32_t flags;
  if (flagsv != Val_int (0)) /* Some [ list of CMD_FLAG.t ] */
    flags = CMD_FLAG_val (Field (flagsv, 0));
  else /* None */
    flags = 0;
  uint64_t count = Int64_val (countv);
  uint64_t offset = Int64_val (offsetv);
  char **contexts = (char **)nbd_internal_ocaml_string_list (contextsv);
  nbd_extent64_callback extent64_callback;
  struct user_data *extent64_user_data = alloc_user_data ();
  /* The function may save a reference to the closure, so we
   * must treat it as a possible GC root.
   */
  extent64_user_data->fnv = extent64v;
  caml_register_generational_global_root (&extent64_user_data->fnv);
  extent64_callback.callback = extent64_wrapper;
  extent64_callback.user_data = extent64_user_data;
  extent64_callback.free = free_user_data;
  int64_t r;

  caml_enter_blocking_section ();
  r =  nbd_aio_block_status_filter (h, count, offset, contexts,
                                    extent64_callback, completion_callback,
                                    flags);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_int64 (r);
  free (contexts);
  CAMLreturn (rv);
}

/* Byte-code compat function because this method has > 5 parameters.
 */
value
nbd_internal_ocaml_nbd_aio_block_status_filter_byte (value *argv, int argn)
{
  return nbd_internal_ocaml_nbd_aio_block_status_filter (argv[0], argv[1],
                                                         argv[2], argv[3],
                                                         argv[4], argv[5],
                                                         argv[6]);
}

value
nbd_internal_ocaml_nbd_aio_get_fd (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_get_fd"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_get_fd (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_int (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_get_direction (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_get_direction"
    );

  unsigned r;

  caml_enter_blocking_section ();
  r =  nbd_aio_get_direction (h);
  caml_leave_blocking_section ();

  rv = Val_int (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_notify_read (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_notify_read"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_notify_read (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_notify_write (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_notify_write"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_notify_write (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_is_created (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_is_created"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_is_created (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_is_connecting (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_is_connecting"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_is_connecting (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_is_negotiating (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_is_negotiating"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_is_negotiating (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_is_ready (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_is_ready"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_is_ready (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_is_processing (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_is_processing"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_is_processing (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_is_dead (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_is_dead"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_is_dead (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_is_closed (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_is_closed"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_is_closed (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_command_completed (value hv, value cookiev)
{
  CAMLparam2 (hv, cookiev);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_command_completed"
    );

  uint64_t cookie = Int64_val (cookiev);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_command_completed (h, cookie);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_peek_command_completed (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_peek_command_completed"
    );

  int64_t r;

  caml_enter_blocking_section ();
  r =  nbd_aio_peek_command_completed (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_aio_in_flight (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.aio_in_flight"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_aio_in_flight (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_int (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_connection_state (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.connection_state"
    );

  const char * r;

  caml_enter_blocking_section ();
  r =  nbd_connection_state (h);
  caml_leave_blocking_section ();

  if (r == NULL)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_string (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_package_name (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_package_name"
    );

  const char * r;

  caml_enter_blocking_section ();
  r =  nbd_get_package_name (h);
  caml_leave_blocking_section ();

  if (r == NULL)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_string (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_version (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_version"
    );

  const char * r;

  caml_enter_blocking_section ();
  r =  nbd_get_version (h);
  caml_leave_blocking_section ();

  if (r == NULL)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_string (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_kill_subprocess (value hv, value signumv)
{
  CAMLparam2 (hv, signumv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.kill_subprocess"
    );

  int signum = Int_val (signumv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_kill_subprocess (h, signum);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_unit;
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_subprocess_pid (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_subprocess_pid"
    );

  int64_t r;

  caml_enter_blocking_section ();
  r =  nbd_get_subprocess_pid (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_int64 (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_supports_tls (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.supports_tls"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_supports_tls (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_supports_vsock (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.supports_vsock"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_supports_vsock (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_supports_uri (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.supports_uri"
    );

  int r;

  caml_enter_blocking_section ();
  r =  nbd_supports_uri (h);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_get_uri (value hv)
{
  CAMLparam1 (hv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.get_uri"
    );

  char * r;

  caml_enter_blocking_section ();
  r =  nbd_get_uri (h);
  caml_leave_blocking_section ();

  if (r == NULL)
    nbd_internal_ocaml_raise_error ();

  rv = caml_copy_string (r);
  free (r);
  CAMLreturn (rv);
}

value
nbd_internal_ocaml_nbd_is_uri (value hv, value uriv)
{
  CAMLparam2 (hv, uriv);
  CAMLlocal1 (rv);

  struct nbd_handle *h = NBD_val (hv);
  if (h == NULL)
    nbd_internal_ocaml_raise_closed (
      "NBD.is_uri"
    );

  const char *uri = String_val (uriv);
  int r;

  caml_enter_blocking_section ();
  r =  nbd_is_uri (h, uri);
  caml_leave_blocking_section ();

  if (r == -1)
    nbd_internal_ocaml_raise_error ();

  rv = Val_bool (r);
  CAMLreturn (rv);
}

