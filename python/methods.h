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

#ifndef LIBNBD_METHODS_H
#define LIBNBD_METHODS_H

#define PY_SSIZE_T_CLEAN 1
#include <Python.h>

#include <assert.h>

extern char **nbd_internal_py_get_string_list (PyObject *);
extern void nbd_internal_py_free_string_list (char **);
extern int nbd_internal_py_get_sockaddr (PyObject *,
    struct sockaddr_storage *, socklen_t *);
extern PyObject *nbd_internal_py_get_aio_view (PyObject *, int);
extern int nbd_internal_py_init_aio_buffer (PyObject *);
extern PyObject *nbd_internal_py_get_nbd_buffer_type (void);
extern PyObject *nbd_internal_py_wrap_errptr (int);
extern PyObject *nbd_internal_py_get_subview (PyObject *, const char *, size_t);

static inline struct nbd_handle *
get_handle (PyObject *obj)
{
  assert (obj);
  assert (obj != Py_None);
  return PyCapsule_GetPointer(obj, "nbd_handle");
}

/* nbd.Error exception. */
extern PyObject *nbd_internal_py_Error;

static inline void
raise_exception ()
{
  PyObject *args = Py_BuildValue ("si", nbd_get_error (), nbd_get_errno ());

  if (args != NULL) {
    PyErr_SetObject (nbd_internal_py_Error, args);
    Py_DECREF (args);
  }
}

extern PyObject *nbd_internal_py_create (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_close (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_display_version (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_alloc_aio_buffer (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_buffer_is_zero (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_debug (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_debug (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_debug_callback (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_clear_debug_callback (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_stats_bytes_sent (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_stats_chunks_sent (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_stats_bytes_received (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_stats_chunks_received (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_handle_name (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_handle_name (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_private_data (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_private_data (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_export_name (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_export_name (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_request_block_size (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_request_block_size (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_full_info (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_full_info (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_canonical_export_name (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_export_description (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_tls (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_tls (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_tls_negotiated (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_tls_certificates (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_tls_verify_peer (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_tls_verify_peer (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_tls_username (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_tls_username (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_tls_hostname (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_tls_hostname (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_tls_psk_file (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_request_extended_headers (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_request_extended_headers (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_extended_headers_negotiated (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_request_structured_replies (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_request_structured_replies (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_structured_replies_negotiated (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_request_meta_context (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_request_meta_context (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_handshake_flags (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_handshake_flags (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_pread_initialize (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_pread_initialize (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_strict_mode (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_strict_mode (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_opt_mode (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_opt_mode (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_opt_go (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_opt_abort (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_opt_starttls (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_opt_extended_headers (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_opt_structured_reply (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_opt_list (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_opt_info (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_opt_list_meta_context (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_opt_list_meta_context_queries (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_opt_set_meta_context (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_opt_set_meta_context_queries (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_add_meta_context (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_nr_meta_contexts (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_meta_context (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_clear_meta_contexts (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_uri_allow_transports (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_uri_allow_tls (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_uri_allow_local_file (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_connect_uri (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_connect_unix (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_connect_vsock (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_connect_tcp (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_connect_socket (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_connect_command (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_connect_systemd_socket_activation (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_set_socket_activation_name (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_socket_activation_name (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_is_read_only (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_can_flush (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_can_fua (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_is_rotational (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_can_trim (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_can_zero (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_can_fast_zero (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_can_block_status_payload (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_can_df (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_can_multi_conn (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_can_cache (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_can_meta_context (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_protocol (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_size (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_block_size (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_pread (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_pread_structured (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_pwrite (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_shutdown (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_flush (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_trim (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_cache (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_zero (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_block_status (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_block_status_64 (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_block_status_filter (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_poll (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_poll2 (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_connect (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_connect_uri (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_connect_unix (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_connect_vsock (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_connect_tcp (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_connect_socket (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_connect_command (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_connect_systemd_socket_activation (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_opt_go (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_opt_abort (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_opt_starttls (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_opt_extended_headers (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_opt_structured_reply (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_opt_list (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_opt_info (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_opt_list_meta_context (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_opt_list_meta_context_queries (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_opt_set_meta_context (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_opt_set_meta_context_queries (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_pread (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_pread_structured (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_pwrite (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_disconnect (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_flush (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_trim (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_cache (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_zero (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_block_status (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_block_status_64 (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_block_status_filter (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_get_fd (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_get_direction (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_notify_read (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_notify_write (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_is_created (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_is_connecting (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_is_negotiating (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_is_ready (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_is_processing (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_is_dead (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_is_closed (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_command_completed (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_peek_command_completed (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_aio_in_flight (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_connection_state (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_package_name (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_version (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_kill_subprocess (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_subprocess_pid (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_supports_tls (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_supports_vsock (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_supports_uri (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_get_uri (
                   PyObject *self, PyObject *args
                 );
extern PyObject *nbd_internal_py_is_uri (
                   PyObject *self, PyObject *args
                 );

#endif /* LIBNBD_METHODS_H */
