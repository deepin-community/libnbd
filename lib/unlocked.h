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

#ifndef LIBNBD_UNLOCKED_H
#define LIBNBD_UNLOCKED_H

extern int nbd_unlocked_set_debug (
             struct nbd_handle *h, bool debug
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_get_debug (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_debug_callback (
             struct nbd_handle *h, nbd_debug_callback *debug_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_clear_debug_callback (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern uint64_t nbd_unlocked_stats_bytes_sent (
                  struct nbd_handle *h
                )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern uint64_t nbd_unlocked_stats_chunks_sent (
                  struct nbd_handle *h
                )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern uint64_t nbd_unlocked_stats_bytes_received (
                  struct nbd_handle *h
                )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern uint64_t nbd_unlocked_stats_chunks_received (
                  struct nbd_handle *h
                )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_handle_name (
             struct nbd_handle *h, const char *handle_name
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern char * nbd_unlocked_get_handle_name (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern uintptr_t nbd_unlocked_set_private_data (
                   struct nbd_handle *h, uintptr_t private_data
                 )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern uintptr_t nbd_unlocked_get_private_data (
                   struct nbd_handle *h
                 )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_export_name (
             struct nbd_handle *h, const char *export_name
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern char * nbd_unlocked_get_export_name (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_request_block_size (
             struct nbd_handle *h, bool request
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_get_request_block_size (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_full_info (
             struct nbd_handle *h, bool request
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_get_full_info (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern char * nbd_unlocked_get_canonical_export_name (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern char * nbd_unlocked_get_export_description (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_tls (
             struct nbd_handle *h, int tls
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_get_tls (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_get_tls_negotiated (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_tls_certificates (
             struct nbd_handle *h, const char *dir
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_set_tls_verify_peer (
             struct nbd_handle *h, bool verify
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_get_tls_verify_peer (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_tls_username (
             struct nbd_handle *h, const char *username
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern char * nbd_unlocked_get_tls_username (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_tls_hostname (
             struct nbd_handle *h, const char *hostname
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern char * nbd_unlocked_get_tls_hostname (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_tls_psk_file (
             struct nbd_handle *h, const char *filename
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_set_request_extended_headers (
             struct nbd_handle *h, bool request
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_get_request_extended_headers (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_get_extended_headers_negotiated (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_request_structured_replies (
             struct nbd_handle *h, bool request
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_get_request_structured_replies (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_get_structured_replies_negotiated (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_request_meta_context (
             struct nbd_handle *h, bool request
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_get_request_meta_context (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_handshake_flags (
             struct nbd_handle *h, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern uint32_t nbd_unlocked_get_handshake_flags (
                  struct nbd_handle *h
                )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_pread_initialize (
             struct nbd_handle *h, bool request
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_get_pread_initialize (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_strict_mode (
             struct nbd_handle *h, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern uint32_t nbd_unlocked_get_strict_mode (
                  struct nbd_handle *h
                )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_opt_mode (
             struct nbd_handle *h, bool enable
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_get_opt_mode (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_opt_go (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_opt_abort (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_opt_starttls (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_opt_extended_headers (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_opt_structured_reply (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_opt_list (
             struct nbd_handle *h, nbd_list_callback *list_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_opt_info (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_opt_list_meta_context (
             struct nbd_handle *h, nbd_context_callback *context_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_opt_list_meta_context_queries (
             struct nbd_handle *h, char **queries,
             nbd_context_callback *context_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_opt_set_meta_context (
             struct nbd_handle *h, nbd_context_callback *context_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_opt_set_meta_context_queries (
             struct nbd_handle *h, char **queries,
             nbd_context_callback *context_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_add_meta_context (
             struct nbd_handle *h, const char *name
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern ssize_t nbd_unlocked_get_nr_meta_contexts (
                 struct nbd_handle *h
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern char * nbd_unlocked_get_meta_context (
                struct nbd_handle *h, size_t i
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_clear_meta_contexts (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_uri_allow_transports (
             struct nbd_handle *h, uint32_t mask
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_uri_allow_tls (
             struct nbd_handle *h, int tls
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_set_uri_allow_local_file (
             struct nbd_handle *h, bool allow
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_connect_uri (
             struct nbd_handle *h, const char *uri
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_connect_unix (
             struct nbd_handle *h, const char *unixsocket
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_connect_vsock (
             struct nbd_handle *h, uint32_t cid, uint32_t port
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_connect_tcp (
             struct nbd_handle *h, const char *hostname, const char *port
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2, 3);
extern int nbd_unlocked_connect_socket (
             struct nbd_handle *h, int sock
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_connect_command (
             struct nbd_handle *h, char **argv
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_connect_systemd_socket_activation (
             struct nbd_handle *h, char **argv
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_set_socket_activation_name (
             struct nbd_handle *h, const char *socket_name
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern char * nbd_unlocked_get_socket_activation_name (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_is_read_only (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_can_flush (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_can_fua (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_is_rotational (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_can_trim (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_can_zero (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_can_fast_zero (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_can_block_status_payload (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_can_df (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_can_multi_conn (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_can_cache (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_can_meta_context (
             struct nbd_handle *h, const char *metacontext
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern const char * nbd_unlocked_get_protocol (
                      struct nbd_handle *h
                    )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int64_t nbd_unlocked_get_size (
                 struct nbd_handle *h
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int64_t nbd_unlocked_get_block_size (
                 struct nbd_handle *h, int size_type
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_pread (
             struct nbd_handle *h, void *buf, size_t count, uint64_t offset,
             uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_pread_structured (
             struct nbd_handle *h, void *buf, size_t count, uint64_t offset,
             nbd_chunk_callback *chunk_callback, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_pwrite (
             struct nbd_handle *h, const void *buf, size_t count,
             uint64_t offset, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_shutdown (
             struct nbd_handle *h, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_flush (
             struct nbd_handle *h, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_trim (
             struct nbd_handle *h, uint64_t count, uint64_t offset,
             uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_cache (
             struct nbd_handle *h, uint64_t count, uint64_t offset,
             uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_zero (
             struct nbd_handle *h, uint64_t count, uint64_t offset,
             uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_block_status (
             struct nbd_handle *h, uint64_t count, uint64_t offset,
             nbd_extent_callback *extent_callback, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_block_status_64 (
             struct nbd_handle *h, uint64_t count, uint64_t offset,
             nbd_extent64_callback *extent64_callback, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_block_status_filter (
             struct nbd_handle *h, uint64_t count, uint64_t offset,
             char **contexts, nbd_extent64_callback *extent64_callback,
             uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 4);
extern int nbd_unlocked_poll (
             struct nbd_handle *h, int timeout
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_poll2 (
             struct nbd_handle *h, int fd, int timeout
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_connect (
             struct nbd_handle *h, const struct sockaddr *addr,
             socklen_t addrlen
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_aio_connect_uri (
             struct nbd_handle *h, const char *uri
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_aio_connect_unix (
             struct nbd_handle *h, const char *unixsocket
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_aio_connect_vsock (
             struct nbd_handle *h, uint32_t cid, uint32_t port
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_connect_tcp (
             struct nbd_handle *h, const char *hostname, const char *port
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2, 3);
extern int nbd_unlocked_aio_connect_socket (
             struct nbd_handle *h, int sock
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_connect_command (
             struct nbd_handle *h, char **argv
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_aio_connect_systemd_socket_activation (
             struct nbd_handle *h, char **argv
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_aio_opt_go (
             struct nbd_handle *h,
             nbd_completion_callback *completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_opt_abort (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_opt_starttls (
             struct nbd_handle *h,
             nbd_completion_callback *completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_opt_extended_headers (
             struct nbd_handle *h,
             nbd_completion_callback *completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_opt_structured_reply (
             struct nbd_handle *h,
             nbd_completion_callback *completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_opt_list (
             struct nbd_handle *h, nbd_list_callback *list_callback,
             nbd_completion_callback *completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_opt_info (
             struct nbd_handle *h,
             nbd_completion_callback *completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_opt_list_meta_context (
             struct nbd_handle *h, nbd_context_callback *context_callback,
             nbd_completion_callback *completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_opt_list_meta_context_queries (
             struct nbd_handle *h, char **queries,
             nbd_context_callback *context_callback,
             nbd_completion_callback *completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_aio_opt_set_meta_context (
             struct nbd_handle *h, nbd_context_callback *context_callback,
             nbd_completion_callback *completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_opt_set_meta_context_queries (
             struct nbd_handle *h, char **queries,
             nbd_context_callback *context_callback,
             nbd_completion_callback *completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int64_t nbd_unlocked_aio_pread (
                 struct nbd_handle *h, void *buf, size_t count,
                 uint64_t offset,
                 nbd_completion_callback *completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int64_t nbd_unlocked_aio_pread_structured (
                 struct nbd_handle *h, void *buf, size_t count,
                 uint64_t offset, nbd_chunk_callback *chunk_callback,
                 nbd_completion_callback *completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int64_t nbd_unlocked_aio_pwrite (
                 struct nbd_handle *h, const void *buf, size_t count,
                 uint64_t offset,
                 nbd_completion_callback *completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
extern int nbd_unlocked_aio_disconnect (
             struct nbd_handle *h, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int64_t nbd_unlocked_aio_flush (
                 struct nbd_handle *h,
                 nbd_completion_callback *completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int64_t nbd_unlocked_aio_trim (
                 struct nbd_handle *h, uint64_t count, uint64_t offset,
                 nbd_completion_callback *completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int64_t nbd_unlocked_aio_cache (
                 struct nbd_handle *h, uint64_t count, uint64_t offset,
                 nbd_completion_callback *completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int64_t nbd_unlocked_aio_zero (
                 struct nbd_handle *h, uint64_t count, uint64_t offset,
                 nbd_completion_callback *completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int64_t nbd_unlocked_aio_block_status (
                 struct nbd_handle *h, uint64_t count, uint64_t offset,
                 nbd_extent_callback *extent_callback,
                 nbd_completion_callback *completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int64_t nbd_unlocked_aio_block_status_64 (
                 struct nbd_handle *h, uint64_t count, uint64_t offset,
                 nbd_extent64_callback *extent64_callback,
                 nbd_completion_callback *completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int64_t nbd_unlocked_aio_block_status_filter (
                 struct nbd_handle *h, uint64_t count, uint64_t offset,
                 char **contexts, nbd_extent64_callback *extent64_callback,
                 nbd_completion_callback *completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1, 4);
extern int nbd_unlocked_aio_get_fd (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern unsigned nbd_unlocked_aio_get_direction (
                  struct nbd_handle *h
                )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_notify_read (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_notify_write (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_is_created (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_is_connecting (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_is_negotiating (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_is_ready (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_is_processing (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_is_dead (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_is_closed (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_command_completed (
             struct nbd_handle *h, uint64_t cookie
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int64_t nbd_unlocked_aio_peek_command_completed (
                 struct nbd_handle *h
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_aio_in_flight (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern const char * nbd_unlocked_connection_state (
                      struct nbd_handle *h
                    )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern const char * nbd_unlocked_get_package_name (
                      struct nbd_handle *h
                    )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern const char * nbd_unlocked_get_version (
                      struct nbd_handle *h
                    )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_kill_subprocess (
             struct nbd_handle *h, int signum
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int64_t nbd_unlocked_get_subprocess_pid (
                 struct nbd_handle *h
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_supports_tls (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_supports_vsock (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_supports_uri (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern char * nbd_unlocked_get_uri (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
extern int nbd_unlocked_is_uri (
             struct nbd_handle *h, const char *uri
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);

#endif /* LIBNBD_UNLOCKED_H */
