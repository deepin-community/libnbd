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

#ifndef LIBNBD_H
#define LIBNBD_H

/* This is the public interface to libnbd, a client library for
 * accessing Network Block Device (NBD) servers.
 *
 * Please read the libnbd(3) manual page to
 * find out how to use this library.
 */

#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined (__GNUC__)
#define LIBNBD_GCC_VERSION \
    (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

#ifndef LIBNBD_ATTRIBUTE_NONNULL
#if defined (__GNUC__) && LIBNBD_GCC_VERSION >= 120000 /* gcc >= 12.0 */
#define LIBNBD_ATTRIBUTE_NONNULL(...) \
  __attribute__ ((__nonnull__ (__VA_ARGS__)))
#else
#define LIBNBD_ATTRIBUTE_NONNULL(...)
#endif
#endif /* ! defined LIBNBD_ATTRIBUTE_NONNULL */

#if defined (__GNUC__) && LIBNBD_GCC_VERSION >= 120000 /* gcc >= 12.0 */
#define LIBNBD_ATTRIBUTE_ALLOC_DEALLOC(fn) \
  __attribute__ ((__malloc__, __malloc__ (fn, 1)))
#else
#define LIBNBD_ATTRIBUTE_ALLOC_DEALLOC(fn)
#endif

struct nbd_handle;

#define LIBNBD_TLS_DISABLE                       0
#define LIBNBD_TLS_ALLOW                         1
#define LIBNBD_TLS_REQUIRE                       2

#define LIBNBD_SIZE_MINIMUM                      0
#define LIBNBD_SIZE_PREFERRED                    1
#define LIBNBD_SIZE_MAXIMUM                      2
#define LIBNBD_SIZE_PAYLOAD                      3

#define LIBNBD_CMD_FLAG_FUA                      0x01U
#define LIBNBD_CMD_FLAG_NO_HOLE                  0x02U
#define LIBNBD_CMD_FLAG_DF                       0x04U
#define LIBNBD_CMD_FLAG_REQ_ONE                  0x08U
#define LIBNBD_CMD_FLAG_FAST_ZERO                0x10U
#define LIBNBD_CMD_FLAG_PAYLOAD_LEN              0x20U
#define LIBNBD_CMD_FLAG_MASK                     0x3fU

#define LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE     0x01U
#define LIBNBD_HANDSHAKE_FLAG_NO_ZEROES          0x02U
#define LIBNBD_HANDSHAKE_FLAG_MASK               0x03U

#define LIBNBD_STRICT_COMMANDS                   0x01U
#define LIBNBD_STRICT_FLAGS                      0x02U
#define LIBNBD_STRICT_BOUNDS                     0x04U
#define LIBNBD_STRICT_ZERO_SIZE                  0x08U
#define LIBNBD_STRICT_ALIGN                      0x10U
#define LIBNBD_STRICT_PAYLOAD                    0x20U
#define LIBNBD_STRICT_AUTO_FLAG                  0x40U
#define LIBNBD_STRICT_MASK                       0x7fU

#define LIBNBD_ALLOW_TRANSPORT_TCP               0x01U
#define LIBNBD_ALLOW_TRANSPORT_UNIX              0x02U
#define LIBNBD_ALLOW_TRANSPORT_VSOCK             0x04U
#define LIBNBD_ALLOW_TRANSPORT_SSH               0x08U
#define LIBNBD_ALLOW_TRANSPORT_MASK              0x0fU

#define LIBNBD_SHUTDOWN_ABANDON_PENDING          0x10000U
#define LIBNBD_SHUTDOWN_MASK                     0x10000U

#define LIBNBD_AIO_DIRECTION_READ                1
#define LIBNBD_AIO_DIRECTION_WRITE               2
#define LIBNBD_AIO_DIRECTION_BOTH                3
#define LIBNBD_READ_DATA                         1
#define LIBNBD_READ_HOLE                         2
#define LIBNBD_READ_ERROR                        3

extern void nbd_close (struct nbd_handle *h); /* h can be NULL */
#define LIBNBD_HAVE_NBD_CLOSE 1

extern struct nbd_handle *nbd_create (void)
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (nbd_close);
#define LIBNBD_HAVE_NBD_CREATE 1

extern const char *nbd_get_error (void);
#define LIBNBD_HAVE_NBD_GET_ERROR 1

extern int nbd_get_errno (void);
#define LIBNBD_HAVE_NBD_GET_ERRNO 1

/* This is used in the callback for nbd_block_status_64.
 */
typedef struct {
  uint64_t length;  /* Will not exceed INT64_MAX */
  uint64_t flags;
} nbd_extent;

/* These are used for callback parameters.  They are passed
 * by value not by reference.  See CALLBACKS in libnbd(3).
 */
typedef struct {
  int (*callback) (void *user_data, const void *subbuf, size_t count,
                   uint64_t offset, unsigned status, int *error);
  void *user_data;
  void (*free) (void *user_data);
} nbd_chunk_callback;
#define LIBNBD_HAVE_NBD_CHUNK_CALLBACK 1

typedef struct {
  int (*callback) (void *user_data, int *error);
  void *user_data;
  void (*free) (void *user_data);
} nbd_completion_callback;
#define LIBNBD_HAVE_NBD_COMPLETION_CALLBACK 1

typedef struct {
  int (*callback) (void *user_data, const char *context, const char *msg);
  void *user_data;
  void (*free) (void *user_data);
} nbd_debug_callback;
#define LIBNBD_HAVE_NBD_DEBUG_CALLBACK 1

typedef struct {
  int (*callback) (void *user_data, const char *metacontext,
                   uint64_t offset, uint32_t *entries, size_t nr_entries,
                   int *error);
  void *user_data;
  void (*free) (void *user_data);
} nbd_extent_callback;
#define LIBNBD_HAVE_NBD_EXTENT_CALLBACK 1

typedef struct {
  int (*callback) (void *user_data, const char *metacontext,
                   uint64_t offset, nbd_extent *entries, size_t nr_entries,
                   int *error);
  void *user_data;
  void (*free) (void *user_data);
} nbd_extent64_callback;
#define LIBNBD_HAVE_NBD_EXTENT64_CALLBACK 1

typedef struct {
  int (*callback) (void *user_data, const char *name,
                   const char *description);
  void *user_data;
  void (*free) (void *user_data);
} nbd_list_callback;
#define LIBNBD_HAVE_NBD_LIST_CALLBACK 1

typedef struct {
  int (*callback) (void *user_data, const char *name);
  void *user_data;
  void (*free) (void *user_data);
} nbd_context_callback;
#define LIBNBD_HAVE_NBD_CONTEXT_CALLBACK 1

/* Note NBD_NULL_* are only generated for callbacks which are
 * optional.  (See OClosure in the generator).
 */
#define NBD_NULL_COMPLETION ((nbd_completion_callback) { .callback = NULL })

extern int nbd_set_debug (
             struct nbd_handle *h, bool debug
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_DEBUG 1

extern int nbd_get_debug (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_DEBUG 1

extern int nbd_set_debug_callback (
             struct nbd_handle *h, nbd_debug_callback debug_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_DEBUG_CALLBACK 1

extern int nbd_clear_debug_callback (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CLEAR_DEBUG_CALLBACK 1

extern uint64_t nbd_stats_bytes_sent (
                  struct nbd_handle *h
                )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_STATS_BYTES_SENT 1

extern uint64_t nbd_stats_chunks_sent (
                  struct nbd_handle *h
                )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_STATS_CHUNKS_SENT 1

extern uint64_t nbd_stats_bytes_received (
                  struct nbd_handle *h
                )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_STATS_BYTES_RECEIVED 1

extern uint64_t nbd_stats_chunks_received (
                  struct nbd_handle *h
                )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_STATS_CHUNKS_RECEIVED 1

extern int nbd_set_handle_name (
             struct nbd_handle *h, const char *handle_name
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_SET_HANDLE_NAME 1

extern char * nbd_get_handle_name (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_HANDLE_NAME 1

extern uintptr_t nbd_set_private_data (
                   struct nbd_handle *h, uintptr_t private_data
                 )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_PRIVATE_DATA 1

extern uintptr_t nbd_get_private_data (
                   struct nbd_handle *h
                 )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_PRIVATE_DATA 1

extern int nbd_set_export_name (
             struct nbd_handle *h, const char *export_name
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_SET_EXPORT_NAME 1

extern char * nbd_get_export_name (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_EXPORT_NAME 1

extern int nbd_set_request_block_size (
             struct nbd_handle *h, bool request
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_REQUEST_BLOCK_SIZE 1

extern int nbd_get_request_block_size (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_REQUEST_BLOCK_SIZE 1

extern int nbd_set_full_info (
             struct nbd_handle *h, bool request
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_FULL_INFO 1

extern int nbd_get_full_info (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_FULL_INFO 1

extern char * nbd_get_canonical_export_name (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_CANONICAL_EXPORT_NAME 1

extern char * nbd_get_export_description (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_EXPORT_DESCRIPTION 1

extern int nbd_set_tls (
             struct nbd_handle *h, int tls
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_TLS 1

extern int nbd_get_tls (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_TLS 1

extern int nbd_get_tls_negotiated (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_TLS_NEGOTIATED 1

extern int nbd_set_tls_certificates (
             struct nbd_handle *h, const char *dir
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_SET_TLS_CERTIFICATES 1

extern int nbd_set_tls_verify_peer (
             struct nbd_handle *h, bool verify
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_TLS_VERIFY_PEER 1

extern int nbd_get_tls_verify_peer (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_TLS_VERIFY_PEER 1

extern int nbd_set_tls_username (
             struct nbd_handle *h, const char *username
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_SET_TLS_USERNAME 1

extern char * nbd_get_tls_username (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_TLS_USERNAME 1

extern int nbd_set_tls_hostname (
             struct nbd_handle *h, const char *hostname
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_SET_TLS_HOSTNAME 1

extern char * nbd_get_tls_hostname (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_TLS_HOSTNAME 1

extern int nbd_set_tls_psk_file (
             struct nbd_handle *h, const char *filename
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_SET_TLS_PSK_FILE 1

extern int nbd_set_request_extended_headers (
             struct nbd_handle *h, bool request
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_REQUEST_EXTENDED_HEADERS 1

extern int nbd_get_request_extended_headers (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_REQUEST_EXTENDED_HEADERS 1

extern int nbd_get_extended_headers_negotiated (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_EXTENDED_HEADERS_NEGOTIATED 1

extern int nbd_set_request_structured_replies (
             struct nbd_handle *h, bool request
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_REQUEST_STRUCTURED_REPLIES 1

extern int nbd_get_request_structured_replies (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_REQUEST_STRUCTURED_REPLIES 1

extern int nbd_get_structured_replies_negotiated (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_STRUCTURED_REPLIES_NEGOTIATED 1

extern int nbd_set_request_meta_context (
             struct nbd_handle *h, bool request
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_REQUEST_META_CONTEXT 1

extern int nbd_get_request_meta_context (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_REQUEST_META_CONTEXT 1

extern int nbd_set_handshake_flags (
             struct nbd_handle *h, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_HANDSHAKE_FLAGS 1

extern uint32_t nbd_get_handshake_flags (
                  struct nbd_handle *h
                )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_HANDSHAKE_FLAGS 1

extern int nbd_set_pread_initialize (
             struct nbd_handle *h, bool request
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_PREAD_INITIALIZE 1

extern int nbd_get_pread_initialize (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_PREAD_INITIALIZE 1

extern int nbd_set_strict_mode (
             struct nbd_handle *h, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_STRICT_MODE 1

extern uint32_t nbd_get_strict_mode (
                  struct nbd_handle *h
                )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_STRICT_MODE 1

extern int nbd_set_opt_mode (
             struct nbd_handle *h, bool enable
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_OPT_MODE 1

extern int nbd_get_opt_mode (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_OPT_MODE 1

extern int nbd_opt_go (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_OPT_GO 1

extern int nbd_opt_abort (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_OPT_ABORT 1

extern int nbd_opt_starttls (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_OPT_STARTTLS 1

extern int nbd_opt_extended_headers (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_OPT_EXTENDED_HEADERS 1

extern int nbd_opt_structured_reply (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_OPT_STRUCTURED_REPLY 1

extern int nbd_opt_list (
             struct nbd_handle *h, nbd_list_callback list_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_OPT_LIST 1

extern int nbd_opt_info (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_OPT_INFO 1

extern int nbd_opt_list_meta_context (
             struct nbd_handle *h, nbd_context_callback context_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_OPT_LIST_META_CONTEXT 1

extern int nbd_opt_list_meta_context_queries (
             struct nbd_handle *h, char **queries,
             nbd_context_callback context_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_OPT_LIST_META_CONTEXT_QUERIES 1

extern int nbd_opt_set_meta_context (
             struct nbd_handle *h, nbd_context_callback context_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_OPT_SET_META_CONTEXT 1

extern int nbd_opt_set_meta_context_queries (
             struct nbd_handle *h, char **queries,
             nbd_context_callback context_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_OPT_SET_META_CONTEXT_QUERIES 1

extern int nbd_add_meta_context (
             struct nbd_handle *h, const char *name
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_ADD_META_CONTEXT 1

extern ssize_t nbd_get_nr_meta_contexts (
                 struct nbd_handle *h
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_NR_META_CONTEXTS 1

extern char * nbd_get_meta_context (
                struct nbd_handle *h, size_t i
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_META_CONTEXT 1

extern int nbd_clear_meta_contexts (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CLEAR_META_CONTEXTS 1

extern int nbd_set_uri_allow_transports (
             struct nbd_handle *h, uint32_t mask
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_URI_ALLOW_TRANSPORTS 1

extern int nbd_set_uri_allow_tls (
             struct nbd_handle *h, int tls
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_URI_ALLOW_TLS 1

extern int nbd_set_uri_allow_local_file (
             struct nbd_handle *h, bool allow
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SET_URI_ALLOW_LOCAL_FILE 1

extern int nbd_connect_uri (
             struct nbd_handle *h, const char *uri
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_CONNECT_URI 1

extern int nbd_connect_unix (
             struct nbd_handle *h, const char *unixsocket
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_CONNECT_UNIX 1

extern int nbd_connect_vsock (
             struct nbd_handle *h, uint32_t cid, uint32_t port
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CONNECT_VSOCK 1

extern int nbd_connect_tcp (
             struct nbd_handle *h, const char *hostname, const char *port
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2, 3);
#define LIBNBD_HAVE_NBD_CONNECT_TCP 1

extern int nbd_connect_socket (
             struct nbd_handle *h, int sock
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CONNECT_SOCKET 1

extern int nbd_connect_command (
             struct nbd_handle *h, char **argv
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_CONNECT_COMMAND 1

extern int nbd_connect_systemd_socket_activation (
             struct nbd_handle *h, char **argv
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_CONNECT_SYSTEMD_SOCKET_ACTIVATION 1

extern int nbd_set_socket_activation_name (
             struct nbd_handle *h, const char *socket_name
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_SET_SOCKET_ACTIVATION_NAME 1

extern char * nbd_get_socket_activation_name (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_SOCKET_ACTIVATION_NAME 1

extern int nbd_is_read_only (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_IS_READ_ONLY 1

extern int nbd_can_flush (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CAN_FLUSH 1

extern int nbd_can_fua (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CAN_FUA 1

extern int nbd_is_rotational (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_IS_ROTATIONAL 1

extern int nbd_can_trim (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CAN_TRIM 1

extern int nbd_can_zero (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CAN_ZERO 1

extern int nbd_can_fast_zero (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CAN_FAST_ZERO 1

extern int nbd_can_block_status_payload (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CAN_BLOCK_STATUS_PAYLOAD 1

extern int nbd_can_df (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CAN_DF 1

extern int nbd_can_multi_conn (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CAN_MULTI_CONN 1

extern int nbd_can_cache (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CAN_CACHE 1

extern int nbd_can_meta_context (
             struct nbd_handle *h, const char *metacontext
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_CAN_META_CONTEXT 1

extern const char * nbd_get_protocol (
                      struct nbd_handle *h
                    )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_PROTOCOL 1

extern int64_t nbd_get_size (
                 struct nbd_handle *h
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_SIZE 1

extern int64_t nbd_get_block_size (
                 struct nbd_handle *h, int size_type
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_BLOCK_SIZE 1

extern int nbd_pread (
             struct nbd_handle *h, void *buf, size_t count, uint64_t offset,
             uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_PREAD 1

extern int nbd_pread_structured (
             struct nbd_handle *h, void *buf, size_t count, uint64_t offset,
             nbd_chunk_callback chunk_callback, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_PREAD_STRUCTURED 1

extern int nbd_pwrite (
             struct nbd_handle *h, const void *buf, size_t count,
             uint64_t offset, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_PWRITE 1

extern int nbd_shutdown (
             struct nbd_handle *h, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SHUTDOWN 1

extern int nbd_flush (
             struct nbd_handle *h, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_FLUSH 1

extern int nbd_trim (
             struct nbd_handle *h, uint64_t count, uint64_t offset,
             uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_TRIM 1

extern int nbd_cache (
             struct nbd_handle *h, uint64_t count, uint64_t offset,
             uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CACHE 1

extern int nbd_zero (
             struct nbd_handle *h, uint64_t count, uint64_t offset,
             uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_ZERO 1

extern int nbd_block_status (
             struct nbd_handle *h, uint64_t count, uint64_t offset,
             nbd_extent_callback extent_callback, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_BLOCK_STATUS 1

extern int nbd_block_status_64 (
             struct nbd_handle *h, uint64_t count, uint64_t offset,
             nbd_extent64_callback extent64_callback, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_BLOCK_STATUS_64 1

extern int nbd_block_status_filter (
             struct nbd_handle *h, uint64_t count, uint64_t offset,
             char **contexts, nbd_extent64_callback extent64_callback,
             uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 4);
#define LIBNBD_HAVE_NBD_BLOCK_STATUS_FILTER 1

extern int nbd_poll (
             struct nbd_handle *h, int timeout
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_POLL 1

extern int nbd_poll2 (
             struct nbd_handle *h, int fd, int timeout
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_POLL2 1

extern int nbd_aio_connect (
             struct nbd_handle *h, const struct sockaddr *addr,
             socklen_t addrlen
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_AIO_CONNECT 1

extern int nbd_aio_connect_uri (
             struct nbd_handle *h, const char *uri
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_AIO_CONNECT_URI 1

extern int nbd_aio_connect_unix (
             struct nbd_handle *h, const char *unixsocket
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_AIO_CONNECT_UNIX 1

extern int nbd_aio_connect_vsock (
             struct nbd_handle *h, uint32_t cid, uint32_t port
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_CONNECT_VSOCK 1

extern int nbd_aio_connect_tcp (
             struct nbd_handle *h, const char *hostname, const char *port
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2, 3);
#define LIBNBD_HAVE_NBD_AIO_CONNECT_TCP 1

extern int nbd_aio_connect_socket (
             struct nbd_handle *h, int sock
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_CONNECT_SOCKET 1

extern int nbd_aio_connect_command (
             struct nbd_handle *h, char **argv
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_AIO_CONNECT_COMMAND 1

extern int nbd_aio_connect_systemd_socket_activation (
             struct nbd_handle *h, char **argv
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_AIO_CONNECT_SYSTEMD_SOCKET_ACTIVATION 1

extern int nbd_aio_opt_go (
             struct nbd_handle *h,
             nbd_completion_callback completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_OPT_GO 1

extern int nbd_aio_opt_abort (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_OPT_ABORT 1

extern int nbd_aio_opt_starttls (
             struct nbd_handle *h,
             nbd_completion_callback completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_OPT_STARTTLS 1

extern int nbd_aio_opt_extended_headers (
             struct nbd_handle *h,
             nbd_completion_callback completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_OPT_EXTENDED_HEADERS 1

extern int nbd_aio_opt_structured_reply (
             struct nbd_handle *h,
             nbd_completion_callback completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_OPT_STRUCTURED_REPLY 1

extern int nbd_aio_opt_list (
             struct nbd_handle *h, nbd_list_callback list_callback,
             nbd_completion_callback completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_OPT_LIST 1

extern int nbd_aio_opt_info (
             struct nbd_handle *h,
             nbd_completion_callback completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_OPT_INFO 1

extern int nbd_aio_opt_list_meta_context (
             struct nbd_handle *h, nbd_context_callback context_callback,
             nbd_completion_callback completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_OPT_LIST_META_CONTEXT 1

extern int nbd_aio_opt_list_meta_context_queries (
             struct nbd_handle *h, char **queries,
             nbd_context_callback context_callback,
             nbd_completion_callback completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_AIO_OPT_LIST_META_CONTEXT_QUERIES 1

extern int nbd_aio_opt_set_meta_context (
             struct nbd_handle *h, nbd_context_callback context_callback,
             nbd_completion_callback completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_OPT_SET_META_CONTEXT 1

extern int nbd_aio_opt_set_meta_context_queries (
             struct nbd_handle *h, char **queries,
             nbd_context_callback context_callback,
             nbd_completion_callback completion_callback
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_AIO_OPT_SET_META_CONTEXT_QUERIES 1

extern int64_t nbd_aio_pread (
                 struct nbd_handle *h, void *buf, size_t count,
                 uint64_t offset,
                 nbd_completion_callback completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_AIO_PREAD 1

extern int64_t nbd_aio_pread_structured (
                 struct nbd_handle *h, void *buf, size_t count,
                 uint64_t offset, nbd_chunk_callback chunk_callback,
                 nbd_completion_callback completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_AIO_PREAD_STRUCTURED 1

extern int64_t nbd_aio_pwrite (
                 struct nbd_handle *h, const void *buf, size_t count,
                 uint64_t offset,
                 nbd_completion_callback completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_AIO_PWRITE 1

extern int nbd_aio_disconnect (
             struct nbd_handle *h, uint32_t flags
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_DISCONNECT 1

extern int64_t nbd_aio_flush (
                 struct nbd_handle *h,
                 nbd_completion_callback completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_FLUSH 1

extern int64_t nbd_aio_trim (
                 struct nbd_handle *h, uint64_t count, uint64_t offset,
                 nbd_completion_callback completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_TRIM 1

extern int64_t nbd_aio_cache (
                 struct nbd_handle *h, uint64_t count, uint64_t offset,
                 nbd_completion_callback completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_CACHE 1

extern int64_t nbd_aio_zero (
                 struct nbd_handle *h, uint64_t count, uint64_t offset,
                 nbd_completion_callback completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_ZERO 1

extern int64_t nbd_aio_block_status (
                 struct nbd_handle *h, uint64_t count, uint64_t offset,
                 nbd_extent_callback extent_callback,
                 nbd_completion_callback completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_BLOCK_STATUS 1

extern int64_t nbd_aio_block_status_64 (
                 struct nbd_handle *h, uint64_t count, uint64_t offset,
                 nbd_extent64_callback extent64_callback,
                 nbd_completion_callback completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_BLOCK_STATUS_64 1

extern int64_t nbd_aio_block_status_filter (
                 struct nbd_handle *h, uint64_t count, uint64_t offset,
                 char **contexts, nbd_extent64_callback extent64_callback,
                 nbd_completion_callback completion_callback,
                 uint32_t flags
               )
    LIBNBD_ATTRIBUTE_NONNULL (1, 4);
#define LIBNBD_HAVE_NBD_AIO_BLOCK_STATUS_FILTER 1

extern int nbd_aio_get_fd (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_GET_FD 1

extern unsigned nbd_aio_get_direction (
                  struct nbd_handle *h
                )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_GET_DIRECTION 1

extern int nbd_aio_notify_read (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_NOTIFY_READ 1

extern int nbd_aio_notify_write (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_NOTIFY_WRITE 1

extern int nbd_aio_is_created (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_IS_CREATED 1

extern int nbd_aio_is_connecting (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_IS_CONNECTING 1

extern int nbd_aio_is_negotiating (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_IS_NEGOTIATING 1

extern int nbd_aio_is_ready (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_IS_READY 1

extern int nbd_aio_is_processing (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_IS_PROCESSING 1

extern int nbd_aio_is_dead (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_IS_DEAD 1

extern int nbd_aio_is_closed (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_IS_CLOSED 1

extern int nbd_aio_command_completed (
             struct nbd_handle *h, uint64_t cookie
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_COMMAND_COMPLETED 1

extern int64_t nbd_aio_peek_command_completed (
                 struct nbd_handle *h
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_PEEK_COMMAND_COMPLETED 1

extern int nbd_aio_in_flight (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_AIO_IN_FLIGHT 1

extern const char * nbd_connection_state (
                      struct nbd_handle *h
                    )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_CONNECTION_STATE 1

extern const char * nbd_get_package_name (
                      struct nbd_handle *h
                    )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_PACKAGE_NAME 1

extern const char * nbd_get_version (
                      struct nbd_handle *h
                    )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_VERSION 1

extern int nbd_kill_subprocess (
             struct nbd_handle *h, int signum
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_KILL_SUBPROCESS 1

extern int64_t nbd_get_subprocess_pid (
                 struct nbd_handle *h
               )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_SUBPROCESS_PID 1

extern int nbd_supports_tls (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SUPPORTS_TLS 1

extern int nbd_supports_vsock (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SUPPORTS_VSOCK 1

extern int nbd_supports_uri (
             struct nbd_handle *h
           )
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_SUPPORTS_URI 1

extern char * nbd_get_uri (
                struct nbd_handle *h
              )
    LIBNBD_ATTRIBUTE_ALLOC_DEALLOC (__builtin_free)
    LIBNBD_ATTRIBUTE_NONNULL (1);
#define LIBNBD_HAVE_NBD_GET_URI 1

extern int nbd_is_uri (
             struct nbd_handle *h, const char *uri
           )
    LIBNBD_ATTRIBUTE_NONNULL (1, 2);
#define LIBNBD_HAVE_NBD_IS_URI 1

/* "base" namespace */
#define LIBNBD_NAMESPACE_BASE "base:"

/* "base" namespace contexts */
#define LIBNBD_CONTEXT_BASE_ALLOCATION "base:allocation"

/* "base:allocation" context related constants */
#define LIBNBD_STATE_HOLE                     1
#define LIBNBD_STATE_ZERO                     2

/* "qemu" namespace */
#define LIBNBD_NAMESPACE_QEMU "qemu:"

/* "qemu" namespace contexts */
#define LIBNBD_CONTEXT_QEMU_DIRTY_BITMAP "qemu:dirty-bitmap:"
#define LIBNBD_CONTEXT_QEMU_ALLOCATION_DEPTH "qemu:allocation-depth"

/* "qemu:dirty-bitmap:" context related constants */
#define LIBNBD_STATE_DIRTY                    1

#ifdef __cplusplus
}
#endif

#endif /* LIBNBD_H */
