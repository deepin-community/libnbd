/* NBD client library in userspace
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

#ifndef LIBNBD_REQUIRES
#define LIBNBD_REQUIRES

extern void requires (const char *cmd);
extern void requires_not (const char *cmd);
extern void requires_not_exists (const char *filename);

/* Some specific tests using the requires() mechanism. */
extern void requires_qemu_nbd_tls_support (void);
extern void requires_qemu_nbd_tls_psk_support (void);
extern void requires_nbd_server_supports_inetd (void);
extern void requires_nbdkit_tls_verify_peer (void);
extern void requires_ssh_localhost (void);

#endif /* LIBNBD_REQUIRES */
