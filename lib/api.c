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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <assert.h>

#include <pthread.h>

/* GCC will remove NULL checks from this file for any parameter
 * annotated with attribute((nonnull)).  See RHBZ#1041336.  To
 * avoid this, disable the attribute when including libnbd.h.
 */
#define LIBNBD_ATTRIBUTE_NONNULL(...)

#include "libnbd.h"
#include "internal.h"

int
nbd_set_debug (
  struct nbd_handle *h, bool debug
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_debug");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: debug=%s",
           debug ? "true" : "false");
  }

  ret = nbd_unlocked_set_debug (h, debug);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_get_debug (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug_direct (h, "nbd_get_debug",
                  "enter:");
  }

  ret = nbd_unlocked_get_debug (h);

  if_debug (h) {
    debug_direct (h, "nbd_get_debug", "leave: ret=%d", ret);
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_set_debug_callback (
  struct nbd_handle *h, nbd_debug_callback debug_callback
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_debug_callback");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: debug=%s",
           "<fun>");
  }

  if (CALLBACK_IS_NULL (debug_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "debug");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_debug_callback (h, &debug_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (debug_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_clear_debug_callback (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_clear_debug_callback");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_clear_debug_callback (h);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

uint64_t
nbd_stats_bytes_sent (
  struct nbd_handle *h
)
{
  uint64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug_direct (h, "nbd_stats_bytes_sent",
                  "enter:");
  }

  ret = nbd_unlocked_stats_bytes_sent (h);

  if_debug (h) {
    debug_direct (h, "nbd_stats_bytes_sent", "leave: ret=%" PRIu64, ret);
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

uint64_t
nbd_stats_chunks_sent (
  struct nbd_handle *h
)
{
  uint64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug_direct (h, "nbd_stats_chunks_sent",
                  "enter:");
  }

  ret = nbd_unlocked_stats_chunks_sent (h);

  if_debug (h) {
    debug_direct (h, "nbd_stats_chunks_sent", "leave: ret=%" PRIu64, ret);
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

uint64_t
nbd_stats_bytes_received (
  struct nbd_handle *h
)
{
  uint64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug_direct (h, "nbd_stats_bytes_received",
                  "enter:");
  }

  ret = nbd_unlocked_stats_bytes_received (h);

  if_debug (h) {
    debug_direct (h, "nbd_stats_bytes_received", "leave: ret=%" PRIu64, ret);
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

uint64_t
nbd_stats_chunks_received (
  struct nbd_handle *h
)
{
  uint64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug_direct (h, "nbd_stats_chunks_received",
                  "enter:");
  }

  ret = nbd_unlocked_stats_chunks_received (h);

  if_debug (h) {
    debug_direct (h, "nbd_stats_chunks_received", "leave: ret=%" PRIu64,
                  ret);
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_set_handle_name (
  struct nbd_handle *h, const char *handle_name
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_handle_name");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *handle_name_printable =
        nbd_internal_printable_string (handle_name);
    debug (h,
           "enter: handle_name=%s",
           handle_name_printable ? handle_name_printable : "");
    free (handle_name_printable);
  }

  if (handle_name == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "handle_name");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_handle_name (h, handle_name);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

char *
nbd_get_handle_name (
  struct nbd_handle *h
)
{
  char * ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_handle_name");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_get_handle_name (h);

  if_debug (h) {
    if (ret == NULL)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      char *ret_printable =
          nbd_internal_printable_string (ret);
      debug (h, "leave: ret=%s", ret_printable ? ret_printable : "");
      free (ret_printable);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

uintptr_t
nbd_set_private_data (
  struct nbd_handle *h, uintptr_t private_data
)
{
  uintptr_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_set_private_data",
                  "enter: private_data=%"PRIuPTR"",
                  private_data);
  }

  ret = nbd_unlocked_set_private_data (h, private_data);

  if_debug (h) {
    debug_direct (h, "nbd_set_private_data", "leave: ret=%" PRIuPTR, ret);
  }

  return ret;
}

uintptr_t
nbd_get_private_data (
  struct nbd_handle *h
)
{
  uintptr_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_get_private_data",
                  "enter:");
  }

  ret = nbd_unlocked_get_private_data (h);

  if_debug (h) {
    debug_direct (h, "nbd_get_private_data", "leave: ret=%" PRIuPTR, ret);
  }

  return ret;
}

static inline bool
set_export_name_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state) ||
        nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created, or negotiating");
    return false;
  }
  return true;
}

int
nbd_set_export_name (
  struct nbd_handle *h, const char *export_name
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_export_name");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *export_name_printable =
        nbd_internal_printable_string (export_name);
    debug (h,
           "enter: export_name=%s",
           export_name_printable ? export_name_printable : "");
    free (export_name_printable);
  }

  p = set_export_name_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (export_name == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "export_name");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_export_name (h, export_name);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

char *
nbd_get_export_name (
  struct nbd_handle *h
)
{
  char * ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_export_name");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_get_export_name (h);

  if_debug (h) {
    if (ret == NULL)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      char *ret_printable =
          nbd_internal_printable_string (ret);
      debug (h, "leave: ret=%s", ret_printable ? ret_printable : "");
      free (ret_printable);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_request_block_size_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state) ||
        nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created, or negotiating");
    return false;
  }
  return true;
}

int
nbd_set_request_block_size (
  struct nbd_handle *h, bool request
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_request_block_size");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: request=%s",
           request ? "true" : "false");
  }

  p = set_request_block_size_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_request_block_size (h, request);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_get_request_block_size (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_request_block_size");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_get_request_block_size (h);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_full_info_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state) ||
        nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created, or negotiating");
    return false;
  }
  return true;
}

int
nbd_set_full_info (
  struct nbd_handle *h, bool request
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_full_info");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: request=%s",
           request ? "true" : "false");
  }

  p = set_full_info_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_full_info (h, request);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_get_full_info (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_full_info");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_get_full_info (h);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
get_canonical_export_name_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

char *
nbd_get_canonical_export_name (
  struct nbd_handle *h
)
{
  bool p;
  char * ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_canonical_export_name");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = get_canonical_export_name_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = NULL;
    goto out;
  }
  ret = nbd_unlocked_get_canonical_export_name (h);

 out:
  if_debug (h) {
    if (ret == NULL)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      char *ret_printable =
          nbd_internal_printable_string (ret);
      debug (h, "leave: ret=%s", ret_printable ? ret_printable : "");
      free (ret_printable);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
get_export_description_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

char *
nbd_get_export_description (
  struct nbd_handle *h
)
{
  bool p;
  char * ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_export_description");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = get_export_description_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = NULL;
    goto out;
  }
  ret = nbd_unlocked_get_export_description (h);

 out:
  if_debug (h) {
    if (ret == NULL)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      char *ret_printable =
          nbd_internal_printable_string (ret);
      debug (h, "leave: ret=%s", ret_printable ? ret_printable : "");
      free (ret_printable);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_tls_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_set_tls (
  struct nbd_handle *h, int tls
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_tls");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: tls=%d",
           tls);
  }

  p = set_tls_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  switch (tls) {
  case LIBNBD_TLS_DISABLE:
  case LIBNBD_TLS_ALLOW:
  case LIBNBD_TLS_REQUIRE:
    break;
  default:
    set_error (EINVAL, "%s: invalid value for parameter: %d",
               "tls", tls);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_tls (h, tls);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_get_tls (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug_direct (h, "nbd_get_tls",
                  "enter:");
  }

  ret = nbd_unlocked_get_tls (h);

  if_debug (h) {
    debug_direct (h, "nbd_get_tls", "leave: ret=%d", ret);
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
get_tls_negotiated_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_get_tls_negotiated (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_tls_negotiated");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = get_tls_negotiated_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_get_tls_negotiated (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_tls_certificates_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_set_tls_certificates (
  struct nbd_handle *h, const char *dir
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_tls_certificates");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *dir_printable =
        nbd_internal_printable_string (dir);
    debug (h,
           "enter: dir=%s",
           dir_printable ? dir_printable : "");
    free (dir_printable);
  }

  p = set_tls_certificates_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (dir == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "dir");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_tls_certificates (h, dir);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_tls_verify_peer_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_set_tls_verify_peer (
  struct nbd_handle *h, bool verify
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_tls_verify_peer");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: verify=%s",
           verify ? "true" : "false");
  }

  p = set_tls_verify_peer_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_tls_verify_peer (h, verify);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_get_tls_verify_peer (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug_direct (h, "nbd_get_tls_verify_peer",
                  "enter:");
  }

  ret = nbd_unlocked_get_tls_verify_peer (h);

  if_debug (h) {
    debug_direct (h, "nbd_get_tls_verify_peer", "leave: ret=%d", ret);
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_tls_username_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_set_tls_username (
  struct nbd_handle *h, const char *username
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_tls_username");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *username_printable =
        nbd_internal_printable_string (username);
    debug (h,
           "enter: username=%s",
           username_printable ? username_printable : "");
    free (username_printable);
  }

  p = set_tls_username_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (username == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "username");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_tls_username (h, username);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

char *
nbd_get_tls_username (
  struct nbd_handle *h
)
{
  char * ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_tls_username");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_get_tls_username (h);

  if_debug (h) {
    if (ret == NULL)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      char *ret_printable =
          nbd_internal_printable_string (ret);
      debug (h, "leave: ret=%s", ret_printable ? ret_printable : "");
      free (ret_printable);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_tls_hostname_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_set_tls_hostname (
  struct nbd_handle *h, const char *hostname
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_tls_hostname");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *hostname_printable =
        nbd_internal_printable_string (hostname);
    debug (h,
           "enter: hostname=%s",
           hostname_printable ? hostname_printable : "");
    free (hostname_printable);
  }

  p = set_tls_hostname_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (hostname == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "hostname");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_tls_hostname (h, hostname);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

char *
nbd_get_tls_hostname (
  struct nbd_handle *h
)
{
  char * ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_tls_hostname");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_get_tls_hostname (h);

  if_debug (h) {
    if (ret == NULL)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      char *ret_printable =
          nbd_internal_printable_string (ret);
      debug (h, "leave: ret=%s", ret_printable ? ret_printable : "");
      free (ret_printable);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_tls_psk_file_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_set_tls_psk_file (
  struct nbd_handle *h, const char *filename
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_tls_psk_file");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *filename_printable =
        nbd_internal_printable_string (filename);
    debug (h,
           "enter: filename=%s",
           filename_printable ? filename_printable : "");
    free (filename_printable);
  }

  p = set_tls_psk_file_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (filename == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "filename");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_tls_psk_file (h, filename);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_request_extended_headers_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_set_request_extended_headers (
  struct nbd_handle *h, bool request
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_request_extended_headers");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: request=%s",
           request ? "true" : "false");
  }

  p = set_request_extended_headers_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_request_extended_headers (h, request);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_get_request_extended_headers (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug_direct (h, "nbd_get_request_extended_headers",
                  "enter:");
  }

  ret = nbd_unlocked_get_request_extended_headers (h);

  if_debug (h) {
    debug_direct (h, "nbd_get_request_extended_headers", "leave: ret=%d",
                  ret);
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
get_extended_headers_negotiated_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_get_extended_headers_negotiated (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_extended_headers_negotiated");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = get_extended_headers_negotiated_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_get_extended_headers_negotiated (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_request_structured_replies_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_set_request_structured_replies (
  struct nbd_handle *h, bool request
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_request_structured_replies");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: request=%s",
           request ? "true" : "false");
  }

  p = set_request_structured_replies_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_request_structured_replies (h, request);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_get_request_structured_replies (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug_direct (h, "nbd_get_request_structured_replies",
                  "enter:");
  }

  ret = nbd_unlocked_get_request_structured_replies (h);

  if_debug (h) {
    debug_direct (h, "nbd_get_request_structured_replies", "leave: ret=%d",
                  ret);
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
get_structured_replies_negotiated_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_get_structured_replies_negotiated (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_structured_replies_negotiated");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = get_structured_replies_negotiated_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_get_structured_replies_negotiated (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_request_meta_context_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state) ||
        nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created, or negotiating");
    return false;
  }
  return true;
}

int
nbd_set_request_meta_context (
  struct nbd_handle *h, bool request
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_request_meta_context");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: request=%s",
           request ? "true" : "false");
  }

  p = set_request_meta_context_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_request_meta_context (h, request);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_get_request_meta_context (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_request_meta_context");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_get_request_meta_context (h);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_handshake_flags_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_set_handshake_flags (
  struct nbd_handle *h, uint32_t flags
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_handshake_flags");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: flags=0x%x",
           flags);
  }

  p = set_handshake_flags_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~LIBNBD_HANDSHAKE_FLAG_MASK) != 0)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_handshake_flags (h, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

uint32_t
nbd_get_handshake_flags (
  struct nbd_handle *h
)
{
  uint32_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug_direct (h, "nbd_get_handshake_flags",
                  "enter:");
  }

  ret = nbd_unlocked_get_handshake_flags (h);

  if_debug (h) {
    debug_direct (h, "nbd_get_handshake_flags", "leave: ret=%u", ret);
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_set_pread_initialize (
  struct nbd_handle *h, bool request
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_pread_initialize");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: request=%s",
           request ? "true" : "false");
  }

  ret = nbd_unlocked_set_pread_initialize (h, request);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_get_pread_initialize (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug_direct (h, "nbd_get_pread_initialize",
                  "enter:");
  }

  ret = nbd_unlocked_get_pread_initialize (h);

  if_debug (h) {
    debug_direct (h, "nbd_get_pread_initialize", "leave: ret=%d", ret);
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_set_strict_mode (
  struct nbd_handle *h, uint32_t flags
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_strict_mode");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: flags=0x%x",
           flags);
  }

  if (unlikely ((flags & ~LIBNBD_STRICT_MASK) != 0)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_strict_mode (h, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

uint32_t
nbd_get_strict_mode (
  struct nbd_handle *h
)
{
  uint32_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug_direct (h, "nbd_get_strict_mode",
                  "enter:");
  }

  ret = nbd_unlocked_get_strict_mode (h);

  if_debug (h) {
    debug_direct (h, "nbd_get_strict_mode", "leave: ret=%u", ret);
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_opt_mode_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_set_opt_mode (
  struct nbd_handle *h, bool enable
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_opt_mode");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: enable=%s",
           enable ? "true" : "false");
  }

  p = set_opt_mode_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_opt_mode (h, enable);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_get_opt_mode (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug_direct (h, "nbd_get_opt_mode",
                  "enter:");
  }

  ret = nbd_unlocked_get_opt_mode (h);

  if_debug (h) {
    debug_direct (h, "nbd_get_opt_mode", "leave: ret=%d", ret);
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
opt_go_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_opt_go (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_opt_go");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = opt_go_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_opt_go (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
opt_abort_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_opt_abort (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_opt_abort");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = opt_abort_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_opt_abort (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
opt_starttls_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_opt_starttls (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_opt_starttls");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = opt_starttls_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_opt_starttls (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
opt_extended_headers_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_opt_extended_headers (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_opt_extended_headers");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = opt_extended_headers_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_opt_extended_headers (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
opt_structured_reply_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_opt_structured_reply (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_opt_structured_reply");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = opt_structured_reply_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_opt_structured_reply (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
opt_list_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_opt_list (
  struct nbd_handle *h, nbd_list_callback list_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_opt_list");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: list=%s",
           "<fun>");
  }

  p = opt_list_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (list_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "list");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_opt_list (h, &list_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (list_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
opt_info_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_opt_info (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_opt_info");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = opt_info_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_opt_info (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
opt_list_meta_context_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_opt_list_meta_context (
  struct nbd_handle *h, nbd_context_callback context_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_opt_list_meta_context");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: context=%s",
           "<fun>");
  }

  p = opt_list_meta_context_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (context_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "context");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_opt_list_meta_context (h, &context_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (context_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
opt_list_meta_context_queries_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_opt_list_meta_context_queries (
  struct nbd_handle *h, char **queries,
  nbd_context_callback context_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_opt_list_meta_context_queries");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *queries_printable =
        nbd_internal_printable_string_list (queries);
    debug (h,
           "enter: queries=%s context=%s",
           queries_printable ? queries_printable : "", "<fun>");
    free (queries_printable);
  }

  p = opt_list_meta_context_queries_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (queries == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "queries");
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (context_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "context");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_opt_list_meta_context_queries (h, queries,
                                                    &context_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (context_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
opt_set_meta_context_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_opt_set_meta_context (
  struct nbd_handle *h, nbd_context_callback context_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_opt_set_meta_context");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: context=%s",
           "<fun>");
  }

  p = opt_set_meta_context_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (context_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "context");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_opt_set_meta_context (h, &context_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (context_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
opt_set_meta_context_queries_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_opt_set_meta_context_queries (
  struct nbd_handle *h, char **queries,
  nbd_context_callback context_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_opt_set_meta_context_queries");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *queries_printable =
        nbd_internal_printable_string_list (queries);
    debug (h,
           "enter: queries=%s context=%s",
           queries_printable ? queries_printable : "", "<fun>");
    free (queries_printable);
  }

  p = opt_set_meta_context_queries_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (queries == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "queries");
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (context_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "context");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_opt_set_meta_context_queries (h, queries,
                                                   &context_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (context_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
add_meta_context_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state) ||
        nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created, or negotiating");
    return false;
  }
  return true;
}

int
nbd_add_meta_context (
  struct nbd_handle *h, const char *name
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_add_meta_context");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *name_printable =
        nbd_internal_printable_string (name);
    debug (h,
           "enter: name=%s",
           name_printable ? name_printable : "");
    free (name_printable);
  }

  p = add_meta_context_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (name == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "name");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_add_meta_context (h, name);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

ssize_t
nbd_get_nr_meta_contexts (
  struct nbd_handle *h
)
{
  ssize_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_nr_meta_contexts");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_get_nr_meta_contexts (h);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%zd", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

char *
nbd_get_meta_context (
  struct nbd_handle *h, size_t i
)
{
  char * ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_meta_context");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: i=%zu",
           i);
  }

  ret = nbd_unlocked_get_meta_context (h, i);

  if_debug (h) {
    if (ret == NULL)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      char *ret_printable =
          nbd_internal_printable_string (ret);
      debug (h, "leave: ret=%s", ret_printable ? ret_printable : "");
      free (ret_printable);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
clear_meta_contexts_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state) ||
        nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created, or negotiating");
    return false;
  }
  return true;
}

int
nbd_clear_meta_contexts (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_clear_meta_contexts");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = clear_meta_contexts_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_clear_meta_contexts (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_uri_allow_transports_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_set_uri_allow_transports (
  struct nbd_handle *h, uint32_t mask
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_uri_allow_transports");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: mask=0x%x",
           mask);
  }

  p = set_uri_allow_transports_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (unlikely ((mask & ~LIBNBD_ALLOW_TRANSPORT_MASK) != 0)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "mask", mask);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_uri_allow_transports (h, mask);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_uri_allow_tls_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_set_uri_allow_tls (
  struct nbd_handle *h, int tls
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_uri_allow_tls");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: tls=%d",
           tls);
  }

  p = set_uri_allow_tls_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  switch (tls) {
  case LIBNBD_TLS_DISABLE:
  case LIBNBD_TLS_ALLOW:
  case LIBNBD_TLS_REQUIRE:
    break;
  default:
    set_error (EINVAL, "%s: invalid value for parameter: %d",
               "tls", tls);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_uri_allow_tls (h, tls);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_uri_allow_local_file_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_set_uri_allow_local_file (
  struct nbd_handle *h, bool allow
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_uri_allow_local_file");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: allow=%s",
           allow ? "true" : "false");
  }

  p = set_uri_allow_local_file_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_uri_allow_local_file (h, allow);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
connect_uri_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_connect_uri (
  struct nbd_handle *h, const char *uri
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_connect_uri");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *uri_printable =
        nbd_internal_printable_string (uri);
    debug (h,
           "enter: uri=%s",
           uri_printable ? uri_printable : "");
    free (uri_printable);
  }

  p = connect_uri_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (uri == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "uri");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_connect_uri (h, uri);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
connect_unix_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_connect_unix (
  struct nbd_handle *h, const char *unixsocket
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_connect_unix");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *unixsocket_printable =
        nbd_internal_printable_string (unixsocket);
    debug (h,
           "enter: unixsocket=%s",
           unixsocket_printable ? unixsocket_printable : "");
    free (unixsocket_printable);
  }

  p = connect_unix_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (unixsocket == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "unixsocket");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_connect_unix (h, unixsocket);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
connect_vsock_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_connect_vsock (
  struct nbd_handle *h, uint32_t cid, uint32_t port
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_connect_vsock");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: cid=%"PRIu32" port=%"PRIu32"",
           cid, port);
  }

  p = connect_vsock_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_connect_vsock (h, cid, port);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
connect_tcp_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_connect_tcp (
  struct nbd_handle *h, const char *hostname, const char *port
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_connect_tcp");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *hostname_printable =
        nbd_internal_printable_string (hostname);
    char *port_printable =
        nbd_internal_printable_string (port);
    debug (h,
           "enter: hostname=%s port=%s",
           hostname_printable ? hostname_printable : "",
           port_printable ? port_printable : "");
    free (hostname_printable);
    free (port_printable);
  }

  p = connect_tcp_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (hostname == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "hostname");
    ret = -1;
    goto out;
  }
  if (port == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "port");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_connect_tcp (h, hostname, port);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
connect_socket_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_connect_socket (
  struct nbd_handle *h, int sock
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_connect_socket");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: sock=%d",
           sock);
  }

  p = connect_socket_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_connect_socket (h, sock);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
connect_command_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_connect_command (
  struct nbd_handle *h, char **argv
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_connect_command");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *argv_printable =
        nbd_internal_printable_string_list (argv);
    debug (h,
           "enter: argv=%s",
           argv_printable ? argv_printable : "");
    free (argv_printable);
  }

  p = connect_command_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (argv == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "argv");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_connect_command (h, argv);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
connect_systemd_socket_activation_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_connect_systemd_socket_activation (
  struct nbd_handle *h, char **argv
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_connect_systemd_socket_activation");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *argv_printable =
        nbd_internal_printable_string_list (argv);
    debug (h,
           "enter: argv=%s",
           argv_printable ? argv_printable : "");
    free (argv_printable);
  }

  p = connect_systemd_socket_activation_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (argv == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "argv");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_connect_systemd_socket_activation (h, argv);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
set_socket_activation_name_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_set_socket_activation_name (
  struct nbd_handle *h, const char *socket_name
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_set_socket_activation_name");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *socket_name_printable =
        nbd_internal_printable_string (socket_name);
    debug (h,
           "enter: socket_name=%s",
           socket_name_printable ? socket_name_printable : "");
    free (socket_name_printable);
  }

  p = set_socket_activation_name_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (socket_name == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "socket_name");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_set_socket_activation_name (h, socket_name);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

char *
nbd_get_socket_activation_name (
  struct nbd_handle *h
)
{
  char * ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_socket_activation_name");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_get_socket_activation_name (h);

  if_debug (h) {
    if (ret == NULL)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      char *ret_printable =
          nbd_internal_printable_string (ret);
      debug (h, "leave: ret=%s", ret_printable ? ret_printable : "");
      free (ret_printable);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
is_read_only_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_is_read_only (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_is_read_only");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = is_read_only_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_is_read_only (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
can_flush_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_can_flush (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_can_flush");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = can_flush_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_can_flush (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
can_fua_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_can_fua (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_can_fua");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = can_fua_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_can_fua (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
is_rotational_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_is_rotational (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_is_rotational");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = is_rotational_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_is_rotational (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
can_trim_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_can_trim (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_can_trim");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = can_trim_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_can_trim (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
can_zero_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_can_zero (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_can_zero");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = can_zero_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_can_zero (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
can_fast_zero_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_can_fast_zero (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_can_fast_zero");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = can_fast_zero_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_can_fast_zero (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
can_block_status_payload_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_can_block_status_payload (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_can_block_status_payload");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = can_block_status_payload_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_can_block_status_payload (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
can_df_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_can_df (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_can_df");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = can_df_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_can_df (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
can_multi_conn_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_can_multi_conn (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_can_multi_conn");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = can_multi_conn_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_can_multi_conn (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
can_cache_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_can_cache (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_can_cache");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = can_cache_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_can_cache (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
can_meta_context_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int
nbd_can_meta_context (
  struct nbd_handle *h, const char *metacontext
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_can_meta_context");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *metacontext_printable =
        nbd_internal_printable_string (metacontext);
    debug (h,
           "enter: metacontext=%s",
           metacontext_printable ? metacontext_printable : "");
    free (metacontext_printable);
  }

  p = can_meta_context_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (metacontext == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "metacontext");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_can_meta_context (h, metacontext);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
get_protocol_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

const char *
nbd_get_protocol (
  struct nbd_handle *h
)
{
  bool p;
  const char * ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_protocol");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = get_protocol_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = NULL;
    goto out;
  }
  ret = nbd_unlocked_get_protocol (h);

 out:
  if_debug (h) {
    if (ret == NULL)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%s", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
get_size_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int64_t
nbd_get_size (
  struct nbd_handle *h
)
{
  bool p;
  int64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_size");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = get_size_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_get_size (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%" PRIi64, ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
get_block_size_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server, or shut down");
    return false;
  }
  return true;
}

int64_t
nbd_get_block_size (
  struct nbd_handle *h, int size_type
)
{
  bool p;
  int64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_block_size");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: size_type=%d",
           size_type);
  }

  p = get_block_size_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  switch (size_type) {
  case LIBNBD_SIZE_MINIMUM:
  case LIBNBD_SIZE_PREFERRED:
  case LIBNBD_SIZE_MAXIMUM:
  case LIBNBD_SIZE_PAYLOAD:
    break;
  default:
    set_error (EINVAL, "%s: invalid value for parameter: %d",
               "size_type", size_type);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_get_block_size (h, size_type);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%" PRIi64, ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
pread_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int
nbd_pread (
  struct nbd_handle *h, void *buf, size_t count, uint64_t offset,
  uint32_t flags
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_pread");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: buf=<buf> count=%zu offset=%"PRIu64" flags=0x%x",
           count, offset, flags);
  }

  if (h->pread_initialize)
    memset (buf, 0, count);
  p = pread_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (buf == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "buf");
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_pread (h, buf, count, offset, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
pread_structured_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int
nbd_pread_structured (
  struct nbd_handle *h, void *buf, size_t count, uint64_t offset,
  nbd_chunk_callback chunk_callback, uint32_t flags
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_pread_structured");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: buf=<buf> count=%zu offset=%"PRIu64" chunk=%s flags=0x%x",
           count, offset, "<fun>", flags);
  }

  if (h->pread_initialize)
    memset (buf, 0, count);
  p = pread_structured_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (buf == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "buf");
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (chunk_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "chunk");
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0x4) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_pread_structured (h, buf, count, offset,
                                       &chunk_callback, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (chunk_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
pwrite_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int
nbd_pwrite (
  struct nbd_handle *h, const void *buf, size_t count, uint64_t offset,
  uint32_t flags
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_pwrite");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *buf_printable =
        nbd_internal_printable_buffer (buf, count);
    debug (h,
           "enter: buf=\"%s\" count=%zu offset=%"PRIu64" flags=0x%x",
           buf_printable ? buf_printable : "", count, offset, flags);
    free (buf_printable);
  }

  p = pwrite_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (buf == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "buf");
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0x21) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_pwrite (h, buf, count, offset, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
shutdown_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating, or connected with the server");
    return false;
  }
  return true;
}

int
nbd_shutdown (
  struct nbd_handle *h, uint32_t flags
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_shutdown");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: flags=0x%x",
           flags);
  }

  p = shutdown_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~LIBNBD_SHUTDOWN_MASK) != 0)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_shutdown (h, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
flush_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int
nbd_flush (
  struct nbd_handle *h, uint32_t flags
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_flush");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: flags=0x%x",
           flags);
  }

  p = flush_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_flush (h, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
trim_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int
nbd_trim (
  struct nbd_handle *h, uint64_t count, uint64_t offset, uint32_t flags
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_trim");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: count=%"PRIu64" offset=%"PRIu64" flags=0x%x",
           count, offset, flags);
  }

  p = trim_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0x1) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_trim (h, count, offset, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
cache_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int
nbd_cache (
  struct nbd_handle *h, uint64_t count, uint64_t offset, uint32_t flags
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_cache");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: count=%"PRIu64" offset=%"PRIu64" flags=0x%x",
           count, offset, flags);
  }

  p = cache_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_cache (h, count, offset, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
zero_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int
nbd_zero (
  struct nbd_handle *h, uint64_t count, uint64_t offset, uint32_t flags
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_zero");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: count=%"PRIu64" offset=%"PRIu64" flags=0x%x",
           count, offset, flags);
  }

  p = zero_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0x13) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_zero (h, count, offset, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
block_status_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int
nbd_block_status (
  struct nbd_handle *h, uint64_t count, uint64_t offset,
  nbd_extent_callback extent_callback, uint32_t flags
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_block_status");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: count=%"PRIu64" offset=%"PRIu64" extent=%s flags=0x%x",
           count, offset, "<fun>", flags);
  }

  p = block_status_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (extent_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "extent");
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0x8) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_block_status (h, count, offset, &extent_callback,
                                   flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (extent_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
block_status_64_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int
nbd_block_status_64 (
  struct nbd_handle *h, uint64_t count, uint64_t offset,
  nbd_extent64_callback extent64_callback, uint32_t flags
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_block_status_64");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: count=%"PRIu64" offset=%"PRIu64" extent64=%s flags=0x%x",
           count, offset, "<fun>", flags);
  }

  p = block_status_64_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (extent64_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "extent64");
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0x8) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_block_status_64 (h, count, offset, &extent64_callback,
                                      flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (extent64_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
block_status_filter_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int
nbd_block_status_filter (
  struct nbd_handle *h, uint64_t count, uint64_t offset, char **contexts,
  nbd_extent64_callback extent64_callback, uint32_t flags
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_block_status_filter");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *contexts_printable =
        nbd_internal_printable_string_list (contexts);
    debug (h,
           "enter: count=%"PRIu64" offset=%"PRIu64" contexts=%s "
           "extent64=%s flags=0x%x",
           count, offset, contexts_printable ? contexts_printable : "",
           "<fun>", flags);
    free (contexts_printable);
  }

  p = block_status_filter_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (contexts == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "contexts");
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (extent64_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "extent64");
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0x28) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_block_status_filter (h, count, offset, contexts,
                                          &extent64_callback, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (extent64_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_poll (
  struct nbd_handle *h, int timeout
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_poll");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: timeout=%d",
           timeout);
  }

  ret = nbd_unlocked_poll (h, timeout);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_poll2 (
  struct nbd_handle *h, int fd, int timeout
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_poll2");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: fd=%d timeout=%d",
           fd, timeout);
  }

  ret = nbd_unlocked_poll2 (h, fd, timeout);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_connect_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_aio_connect (
  struct nbd_handle *h, const struct sockaddr *addr, socklen_t addrlen
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_connect");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: addr=<sockaddr> addrlen=%d",
           (int) addrlen);
  }

  p = aio_connect_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (addr == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "addr");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_connect (h, addr, addrlen);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_connect_uri_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_aio_connect_uri (
  struct nbd_handle *h, const char *uri
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_connect_uri");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *uri_printable =
        nbd_internal_printable_string (uri);
    debug (h,
           "enter: uri=%s",
           uri_printable ? uri_printable : "");
    free (uri_printable);
  }

  p = aio_connect_uri_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (uri == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "uri");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_connect_uri (h, uri);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_connect_unix_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_aio_connect_unix (
  struct nbd_handle *h, const char *unixsocket
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_connect_unix");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *unixsocket_printable =
        nbd_internal_printable_string (unixsocket);
    debug (h,
           "enter: unixsocket=%s",
           unixsocket_printable ? unixsocket_printable : "");
    free (unixsocket_printable);
  }

  p = aio_connect_unix_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (unixsocket == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "unixsocket");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_connect_unix (h, unixsocket);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_connect_vsock_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_aio_connect_vsock (
  struct nbd_handle *h, uint32_t cid, uint32_t port
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_connect_vsock");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: cid=%"PRIu32" port=%"PRIu32"",
           cid, port);
  }

  p = aio_connect_vsock_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_connect_vsock (h, cid, port);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_connect_tcp_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_aio_connect_tcp (
  struct nbd_handle *h, const char *hostname, const char *port
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_connect_tcp");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *hostname_printable =
        nbd_internal_printable_string (hostname);
    char *port_printable =
        nbd_internal_printable_string (port);
    debug (h,
           "enter: hostname=%s port=%s",
           hostname_printable ? hostname_printable : "",
           port_printable ? port_printable : "");
    free (hostname_printable);
    free (port_printable);
  }

  p = aio_connect_tcp_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (hostname == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "hostname");
    ret = -1;
    goto out;
  }
  if (port == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "port");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_connect_tcp (h, hostname, port);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_connect_socket_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_aio_connect_socket (
  struct nbd_handle *h, int sock
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_connect_socket");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: sock=%d",
           sock);
  }

  p = aio_connect_socket_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_connect_socket (h, sock);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_connect_command_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_aio_connect_command (
  struct nbd_handle *h, char **argv
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_connect_command");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *argv_printable =
        nbd_internal_printable_string_list (argv);
    debug (h,
           "enter: argv=%s",
           argv_printable ? argv_printable : "");
    free (argv_printable);
  }

  p = aio_connect_command_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (argv == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "argv");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_connect_command (h, argv);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_connect_systemd_socket_activation_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_created (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "newly created");
    return false;
  }
  return true;
}

int
nbd_aio_connect_systemd_socket_activation (
  struct nbd_handle *h, char **argv
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_connect_systemd_socket_activation");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *argv_printable =
        nbd_internal_printable_string_list (argv);
    debug (h,
           "enter: argv=%s",
           argv_printable ? argv_printable : "");
    free (argv_printable);
  }

  p = aio_connect_systemd_socket_activation_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (argv == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "argv");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_connect_systemd_socket_activation (h, argv);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_opt_go_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_aio_opt_go (
  struct nbd_handle *h, nbd_completion_callback completion_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_opt_go");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: completion=%s",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL");
  }

  p = aio_opt_go_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_opt_go (h, &completion_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_opt_abort_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_aio_opt_abort (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_opt_abort");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = aio_opt_abort_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_opt_abort (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_opt_starttls_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_aio_opt_starttls (
  struct nbd_handle *h, nbd_completion_callback completion_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_opt_starttls");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: completion=%s",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL");
  }

  p = aio_opt_starttls_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_opt_starttls (h, &completion_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_opt_extended_headers_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_aio_opt_extended_headers (
  struct nbd_handle *h, nbd_completion_callback completion_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_opt_extended_headers");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: completion=%s",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL");
  }

  p = aio_opt_extended_headers_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_opt_extended_headers (h, &completion_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_opt_structured_reply_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_aio_opt_structured_reply (
  struct nbd_handle *h, nbd_completion_callback completion_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_opt_structured_reply");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: completion=%s",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL");
  }

  p = aio_opt_structured_reply_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_opt_structured_reply (h, &completion_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_opt_list_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_aio_opt_list (
  struct nbd_handle *h, nbd_list_callback list_callback,
  nbd_completion_callback completion_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_opt_list");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: list=%s completion=%s",
           "<fun>",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL");
  }

  p = aio_opt_list_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (list_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "list");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_opt_list (h, &list_callback, &completion_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (list_callback);
  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_opt_info_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_aio_opt_info (
  struct nbd_handle *h, nbd_completion_callback completion_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_opt_info");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: completion=%s",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL");
  }

  p = aio_opt_info_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_opt_info (h, &completion_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_opt_list_meta_context_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_aio_opt_list_meta_context (
  struct nbd_handle *h, nbd_context_callback context_callback,
  nbd_completion_callback completion_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_opt_list_meta_context");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: context=%s completion=%s",
           "<fun>",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL");
  }

  p = aio_opt_list_meta_context_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (context_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "context");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_opt_list_meta_context (h, &context_callback,
                                                &completion_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (context_callback);
  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_opt_list_meta_context_queries_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_aio_opt_list_meta_context_queries (
  struct nbd_handle *h, char **queries,
  nbd_context_callback context_callback,
  nbd_completion_callback completion_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_opt_list_meta_context_queries");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *queries_printable =
        nbd_internal_printable_string_list (queries);
    debug (h,
           "enter: queries=%s context=%s completion=%s",
           queries_printable ? queries_printable : "", "<fun>",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL");
    free (queries_printable);
  }

  p = aio_opt_list_meta_context_queries_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (queries == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "queries");
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (context_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "context");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_opt_list_meta_context_queries (h, queries,
                                                        &context_callback,
                                                        &completion_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (context_callback);
  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_opt_set_meta_context_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_aio_opt_set_meta_context (
  struct nbd_handle *h, nbd_context_callback context_callback,
  nbd_completion_callback completion_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_opt_set_meta_context");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: context=%s completion=%s",
           "<fun>",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL");
  }

  p = aio_opt_set_meta_context_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (context_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "context");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_opt_set_meta_context (h, &context_callback,
                                               &completion_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (context_callback);
  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_opt_set_meta_context_queries_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_negotiating (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "negotiating");
    return false;
  }
  return true;
}

int
nbd_aio_opt_set_meta_context_queries (
  struct nbd_handle *h, char **queries,
  nbd_context_callback context_callback,
  nbd_completion_callback completion_callback
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_opt_set_meta_context_queries");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *queries_printable =
        nbd_internal_printable_string_list (queries);
    debug (h,
           "enter: queries=%s context=%s completion=%s",
           queries_printable ? queries_printable : "", "<fun>",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL");
    free (queries_printable);
  }

  p = aio_opt_set_meta_context_queries_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (queries == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "queries");
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (context_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "context");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_opt_set_meta_context_queries (h, queries,
                                                       &context_callback,
                                                       &completion_callback);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  FREE_CALLBACK (context_callback);
  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_pread_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int64_t
nbd_aio_pread (
  struct nbd_handle *h, void *buf, size_t count, uint64_t offset,
  nbd_completion_callback completion_callback, uint32_t flags
)
{
  bool p;
  int64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_pread");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: buf=<buf> count=%zu offset=%"PRIu64" completion=%s "
           "flags=0x%x",
           count, offset,
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL", flags);
  }

  if (h->pread_initialize)
    memset (buf, 0, count);
  p = aio_pread_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (buf == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "buf");
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_pread (h, buf, count, offset, &completion_callback,
                                flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%" PRIi64, ret);
    }
  }

  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_pread_structured_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int64_t
nbd_aio_pread_structured (
  struct nbd_handle *h, void *buf, size_t count, uint64_t offset,
  nbd_chunk_callback chunk_callback,
  nbd_completion_callback completion_callback, uint32_t flags
)
{
  bool p;
  int64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_pread_structured");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: buf=<buf> count=%zu offset=%"PRIu64" chunk=%s "
           "completion=%s flags=0x%x",
           count, offset, "<fun>",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL", flags);
  }

  if (h->pread_initialize)
    memset (buf, 0, count);
  p = aio_pread_structured_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (buf == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "buf");
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (chunk_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "chunk");
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0x4) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_pread_structured (h, buf, count, offset,
                                           &chunk_callback,
                                           &completion_callback, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%" PRIi64, ret);
    }
  }

  FREE_CALLBACK (chunk_callback);
  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_pwrite_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int64_t
nbd_aio_pwrite (
  struct nbd_handle *h, const void *buf, size_t count, uint64_t offset,
  nbd_completion_callback completion_callback, uint32_t flags
)
{
  bool p;
  int64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_pwrite");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *buf_printable =
        nbd_internal_printable_buffer (buf, count);
    debug (h,
           "enter: buf=\"%s\" count=%zu offset=%"PRIu64" completion=%s "
           "flags=0x%x",
           buf_printable ? buf_printable : "", count, offset,
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL", flags);
    free (buf_printable);
  }

  p = aio_pwrite_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (buf == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "buf");
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0x21) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_pwrite (h, buf, count, offset,
                                 &completion_callback, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%" PRIi64, ret);
    }
  }

  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_disconnect_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int
nbd_aio_disconnect (
  struct nbd_handle *h, uint32_t flags
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_disconnect");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: flags=0x%x",
           flags);
  }

  p = aio_disconnect_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_disconnect (h, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_flush_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int64_t
nbd_aio_flush (
  struct nbd_handle *h, nbd_completion_callback completion_callback,
  uint32_t flags
)
{
  bool p;
  int64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_flush");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: completion=%s flags=0x%x",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL", flags);
  }

  p = aio_flush_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_flush (h, &completion_callback, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%" PRIi64, ret);
    }
  }

  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_trim_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int64_t
nbd_aio_trim (
  struct nbd_handle *h, uint64_t count, uint64_t offset,
  nbd_completion_callback completion_callback, uint32_t flags
)
{
  bool p;
  int64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_trim");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: count=%"PRIu64" offset=%"PRIu64" completion=%s "
           "flags=0x%x",
           count, offset,
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL", flags);
  }

  p = aio_trim_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0x1) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_trim (h, count, offset, &completion_callback,
                               flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%" PRIi64, ret);
    }
  }

  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_cache_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int64_t
nbd_aio_cache (
  struct nbd_handle *h, uint64_t count, uint64_t offset,
  nbd_completion_callback completion_callback, uint32_t flags
)
{
  bool p;
  int64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_cache");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: count=%"PRIu64" offset=%"PRIu64" completion=%s "
           "flags=0x%x",
           count, offset,
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL", flags);
  }

  p = aio_cache_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_cache (h, count, offset, &completion_callback,
                                flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%" PRIi64, ret);
    }
  }

  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_zero_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int64_t
nbd_aio_zero (
  struct nbd_handle *h, uint64_t count, uint64_t offset,
  nbd_completion_callback completion_callback, uint32_t flags
)
{
  bool p;
  int64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_zero");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: count=%"PRIu64" offset=%"PRIu64" completion=%s "
           "flags=0x%x",
           count, offset,
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL", flags);
  }

  p = aio_zero_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0x13) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_zero (h, count, offset, &completion_callback,
                               flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%" PRIi64, ret);
    }
  }

  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_block_status_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int64_t
nbd_aio_block_status (
  struct nbd_handle *h, uint64_t count, uint64_t offset,
  nbd_extent_callback extent_callback,
  nbd_completion_callback completion_callback, uint32_t flags
)
{
  bool p;
  int64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_block_status");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: count=%"PRIu64" offset=%"PRIu64" extent=%s "
           "completion=%s flags=0x%x",
           count, offset, "<fun>",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL", flags);
  }

  p = aio_block_status_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (extent_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "extent");
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0x8) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_block_status (h, count, offset, &extent_callback,
                                       &completion_callback, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%" PRIi64, ret);
    }
  }

  FREE_CALLBACK (extent_callback);
  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_block_status_64_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int64_t
nbd_aio_block_status_64 (
  struct nbd_handle *h, uint64_t count, uint64_t offset,
  nbd_extent64_callback extent64_callback,
  nbd_completion_callback completion_callback, uint32_t flags
)
{
  bool p;
  int64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_block_status_64");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: count=%"PRIu64" offset=%"PRIu64" extent64=%s "
           "completion=%s flags=0x%x",
           count, offset, "<fun>",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL", flags);
  }

  p = aio_block_status_64_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (extent64_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "extent64");
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0x8) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_block_status_64 (h, count, offset,
                                          &extent64_callback,
                                          &completion_callback, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%" PRIi64, ret);
    }
  }

  FREE_CALLBACK (extent64_callback);
  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_block_status_filter_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server");
    return false;
  }
  return true;
}

int64_t
nbd_aio_block_status_filter (
  struct nbd_handle *h, uint64_t count, uint64_t offset, char **contexts,
  nbd_extent64_callback extent64_callback,
  nbd_completion_callback completion_callback, uint32_t flags
)
{
  bool p;
  int64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_block_status_filter");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *contexts_printable =
        nbd_internal_printable_string_list (contexts);
    debug (h,
           "enter: count=%"PRIu64" offset=%"PRIu64" contexts=%s "
           "extent64=%s completion=%s flags=0x%x",
           count, offset, contexts_printable ? contexts_printable : "",
           "<fun>",
           CALLBACK_IS_NULL (completion_callback) ? "<fun>" : "NULL", flags);
    free (contexts_printable);
  }

  p = aio_block_status_filter_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  if (contexts == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "contexts");
    ret = -1;
    goto out;
  }
  if (CALLBACK_IS_NULL (extent64_callback)) {
    set_error (EFAULT, "%s cannot be NULL", "extent64");
    ret = -1;
    goto out;
  }
  if (unlikely ((flags & ~0x28) != 0) &&
      ((h->strict & LIBNBD_STRICT_FLAGS) || flags > UINT16_MAX)) {
    set_error (EINVAL, "%s: invalid value for flag: 0x%x",
               "flags", flags);
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_block_status_filter (h, count, offset, contexts,
                                              &extent64_callback,
                                              &completion_callback, flags);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%" PRIi64, ret);
    }
  }

  FREE_CALLBACK (extent64_callback);
  FREE_CALLBACK (completion_callback);
  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_aio_get_fd (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_get_fd");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_aio_get_fd (h);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

unsigned
nbd_aio_get_direction (
  struct nbd_handle *h
)
{
  unsigned ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_aio_get_direction",
                  "enter:");
  }

  ret = nbd_unlocked_aio_get_direction (h);

  if_debug (h) {
    debug_direct (h, "nbd_aio_get_direction", "leave: ret=%u", ret);
  }

  return ret;
}

int
nbd_aio_notify_read (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_notify_read");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_aio_notify_read (h);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_aio_notify_write (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_notify_write");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_aio_notify_write (h);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_aio_is_created (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_aio_is_created",
                  "enter:");
  }

  ret = nbd_unlocked_aio_is_created (h);

  if_debug (h) {
    debug_direct (h, "nbd_aio_is_created", "leave: ret=%d", ret);
  }

  return ret;
}

int
nbd_aio_is_connecting (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_aio_is_connecting",
                  "enter:");
  }

  ret = nbd_unlocked_aio_is_connecting (h);

  if_debug (h) {
    debug_direct (h, "nbd_aio_is_connecting", "leave: ret=%d", ret);
  }

  return ret;
}

int
nbd_aio_is_negotiating (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_aio_is_negotiating",
                  "enter:");
  }

  ret = nbd_unlocked_aio_is_negotiating (h);

  if_debug (h) {
    debug_direct (h, "nbd_aio_is_negotiating", "leave: ret=%d", ret);
  }

  return ret;
}

int
nbd_aio_is_ready (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_aio_is_ready",
                  "enter:");
  }

  ret = nbd_unlocked_aio_is_ready (h);

  if_debug (h) {
    debug_direct (h, "nbd_aio_is_ready", "leave: ret=%d", ret);
  }

  return ret;
}

int
nbd_aio_is_processing (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_aio_is_processing",
                  "enter:");
  }

  ret = nbd_unlocked_aio_is_processing (h);

  if_debug (h) {
    debug_direct (h, "nbd_aio_is_processing", "leave: ret=%d", ret);
  }

  return ret;
}

int
nbd_aio_is_dead (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_aio_is_dead",
                  "enter:");
  }

  ret = nbd_unlocked_aio_is_dead (h);

  if_debug (h) {
    debug_direct (h, "nbd_aio_is_dead", "leave: ret=%d", ret);
  }

  return ret;
}

int
nbd_aio_is_closed (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_aio_is_closed",
                  "enter:");
  }

  ret = nbd_unlocked_aio_is_closed (h);

  if_debug (h) {
    debug_direct (h, "nbd_aio_is_closed", "leave: ret=%d", ret);
  }

  return ret;
}

int
nbd_aio_command_completed (
  struct nbd_handle *h, uint64_t cookie
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_command_completed");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: cookie=%"PRIu64"",
           cookie);
  }

  ret = nbd_unlocked_aio_command_completed (h, cookie);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int64_t
nbd_aio_peek_command_completed (
  struct nbd_handle *h
)
{
  int64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_peek_command_completed");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_aio_peek_command_completed (h);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%" PRIi64, ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

static inline bool
aio_in_flight_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state) ||
        nbd_internal_is_state_dead (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connected with the server, or shut down, or dead");
    return false;
  }
  return true;
}

int
nbd_aio_in_flight (
  struct nbd_handle *h
)
{
  bool p;
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_aio_in_flight");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = aio_in_flight_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_aio_in_flight (h);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

const char *
nbd_connection_state (
  struct nbd_handle *h
)
{
  const char * ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_connection_state");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_connection_state (h);

  if_debug (h) {
    if (ret == NULL)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%s", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

const char *
nbd_get_package_name (
  struct nbd_handle *h
)
{
  const char * ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_get_package_name",
                  "enter:");
  }

  ret = nbd_unlocked_get_package_name (h);

  if_debug (h) {
    debug_direct (h, "nbd_get_package_name", "leave: ret=%s", ret);
  }

  return ret;
}

const char *
nbd_get_version (
  struct nbd_handle *h
)
{
  const char * ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_get_version",
                  "enter:");
  }

  ret = nbd_unlocked_get_version (h);

  if_debug (h) {
    debug_direct (h, "nbd_get_version", "leave: ret=%s", ret);
  }

  return ret;
}

int
nbd_kill_subprocess (
  struct nbd_handle *h, int signum
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_kill_subprocess");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter: signum=%d",
           signum);
  }

  ret = nbd_unlocked_kill_subprocess (h, signum);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int64_t
nbd_get_subprocess_pid (
  struct nbd_handle *h
)
{
  int64_t ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_subprocess_pid");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  ret = nbd_unlocked_get_subprocess_pid (h);

  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%" PRIi64, ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_supports_tls (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_supports_tls",
                  "enter:");
  }

  ret = nbd_unlocked_supports_tls (h);

  if_debug (h) {
    debug_direct (h, "nbd_supports_tls", "leave: ret=%d", ret);
  }

  return ret;
}

int
nbd_supports_vsock (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_supports_vsock",
                  "enter:");
  }

  ret = nbd_unlocked_supports_vsock (h);

  if_debug (h) {
    debug_direct (h, "nbd_supports_vsock", "leave: ret=%d", ret);
  }

  return ret;
}

int
nbd_supports_uri (
  struct nbd_handle *h
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  /* This function must not call set_error. */
  if_debug (h) {
    debug_direct (h, "nbd_supports_uri",
                  "enter:");
  }

  ret = nbd_unlocked_supports_uri (h);

  if_debug (h) {
    debug_direct (h, "nbd_supports_uri", "leave: ret=%d", ret);
  }

  return ret;
}

static inline bool
get_uri_in_permitted_state (struct nbd_handle *h)
{
  const enum state state = get_public_state (h);

  if (!(nbd_internal_is_state_connecting (state) ||
        nbd_internal_is_state_negotiating (state) ||
        nbd_internal_is_state_ready (state) ||
        nbd_internal_is_state_processing (state) ||
        nbd_internal_is_state_closed (state) ||
        nbd_internal_is_state_dead (state))) {
    set_error (nbd_internal_is_state_created (state) ? ENOTCONN : EINVAL,
               "invalid state: %s: the handle must be %s",
               nbd_internal_state_short_string (state),
               "connecting, or negotiating, or connected with the server, "
               "or shut down, or dead");
    return false;
  }
  return true;
}

char *
nbd_get_uri (
  struct nbd_handle *h
)
{
  bool p;
  char * ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_get_uri");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    debug (h,
           "enter:");
  }

  p = get_uri_in_permitted_state (h);
  if (unlikely (!p)) {
    ret = NULL;
    goto out;
  }
  ret = nbd_unlocked_get_uri (h);

 out:
  if_debug (h) {
    if (ret == NULL)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      char *ret_printable =
          nbd_internal_printable_string (ret);
      debug (h, "leave: ret=%s", ret_printable ? ret_printable : "");
      free (ret_printable);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

int
nbd_is_uri (
  struct nbd_handle *h, const char *uri
)
{
  int ret;

  assert (h->magic == NBD_HANDLE_MAGIC);
  nbd_internal_set_error_context ("nbd_is_uri");

  pthread_mutex_lock (&h->lock);
  if_debug (h) {
    char *uri_printable =
        nbd_internal_printable_string (uri);
    debug (h,
           "enter: uri=%s",
           uri_printable ? uri_printable : "");
    free (uri_printable);
  }

  if (uri == NULL) {
    set_error (EFAULT, "%s cannot be NULL", "uri");
    ret = -1;
    goto out;
  }
  ret = nbd_unlocked_is_uri (h, uri);

 out:
  if_debug (h) {
    if (ret == -1)
      debug (h, "leave: error=\"%s\"", nbd_get_error ());
    else {
      debug (h, "leave: ret=%d", ret);
    }
  }

  if (h->public_state != get_next_state (h))
    h->public_state = get_next_state (h);
  pthread_mutex_unlock (&h->lock);
  return ret;
}

