# nbd client library in userspace
# Copyright Red Hat
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

include $(top_srcdir)/subdir-rules.mk

EXTRA_DIST = \
	dirty-bitmap.sh \
	interop-qemu-storage-daemon.sh \
	interop-qemu-block-size.sh \
	block-status-64.sh \
	list-exports-nbd-config \
	list-exports-test-dir/disk1 \
	list-exports-test-dir/disk2 \
	structured-read.sh \
	opt-extended-headers.sh \
	block-status-payload.sh \
	strict-mode-auto-flag.sh \
	$(NULL)

TESTS_ENVIRONMENT = \
	LIBNBD_DEBUG=1 \
	$(MALLOC_CHECKS) \
	$(NULL)
LOG_COMPILER = $(top_builddir)/run
check_PROGRAMS =
TESTS =

# XXX automake subdir-objects is broken, use a workaround instead.
BUILT_SOURCES = requires.c
requires.c: $(top_srcdir)/tests/requires.c
	rm -f $@
	$(LN_S) $(top_srcdir)/tests/$@ $@

# Common flags.
# Note there is no such thing as "AM_LDADD".
AM_CPPFLAGS = \
	-I$(top_srcdir)/common/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/tests \
	$(NULL)
AM_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(GNUTLS_CFLAGS) \
	$(NULL)

#----------------------------------------------------------------------
# Test interoperability with nbd-server.

if HAVE_NBD_SERVER

check_PROGRAMS += \
	interop-nbd-server \
	interop-nbd-server-tls \
	list-exports-nbd-server \
	$(NULL)
TESTS += \
	interop-nbd-server \
	interop-nbd-server-tls \
	list-exports-nbd-server \
	$(NULL)

interop_nbd_server_SOURCES = \
	interop.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
# nbd-server on Alpine 3.20, 3.21 is broken.
# It hangs after printing:
# (process:26936): GLib-CRITICAL **: 12:35:37.011: g_thread_pool_push: assertion 'real' failed
interop_nbd_server_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DREQUIRES=' requires_nbd_server_supports_inetd (); requires_not_exists ("/etc/alpine-release"); ' \
	-DSERVER=\"$(NBD_SERVER)\" \
	-DSERVER_PARAMS='"-d", "-C", "nbd-server.conf", "0", TMPFILE' \
	-DEXPORT_NAME='""' \
	$(NULL)
interop_nbd_server_LDADD = $(top_builddir)/lib/libnbd.la

interop_nbd_server_tls_SOURCES = \
	interop.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
interop_nbd_server_tls_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DREQUIRES=' requires ("test -d $(abs_top_builddir)/tests/pki"); requires_nbd_server_supports_inetd (); ' \
	-DSERVER=\"$(NBD_SERVER)\" \
	-DSERVER_PARAMS='"-d", "-C", "nbd-server-tls.conf", "0", TMPFILE' \
	-DEXPORT_NAME='""' \
	-DCERTS='"../tests/pki"' \
	-DTLS_MODE=LIBNBD_TLS_REQUIRE \
	$(NULL)
interop_nbd_server_tls_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(GNUTLS_LIBS) \
	$(NULL)

list_exports_nbd_server_SOURCES = \
	list-exports.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
list_exports_nbd_server_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DREQUIRES=' requires_nbd_server_supports_inetd (); ' \
	-DSERVER=\"$(NBD_SERVER)\" \
	-DSERVER_PARAMS='"-C", "$(srcdir)/list-exports-nbd-config", "-d", "0"' \
	-DEXPORTS='"disk1", "disk2"' \
	-DDESCRIPTIONS='"", ""' \
	$(NULL)
list_exports_nbd_server_LDADD = $(top_builddir)/lib/libnbd.la

endif HAVE_NBD_SERVER

#----------------------------------------------------------------------
# Test interoperability with qemu-nbd.

if HAVE_QEMU_NBD

check_PROGRAMS += \
	interop-qemu-nbd \
	interop-qemu-nbd-tls-certs \
	interop-qemu-nbd-tls-psk \
	list-exports-qemu-nbd \
	socket-activation-qemu-nbd \
	dirty-bitmap \
	block-status-64 \
	structured-read \
	opt-extended-headers \
	block-status-payload \
	$(NULL)
TESTS += \
	interop-qemu-nbd \
	interop-qemu-nbd-tls-certs \
	interop-qemu-nbd-tls-psk \
	list-exports-qemu-nbd \
	socket-activation-qemu-nbd \
	dirty-bitmap.sh \
	block-status-64.sh \
	structured-read.sh \
	interop-qemu-block-size.sh \
	opt-extended-headers.sh \
	block-status-payload.sh \
	strict-mode-auto-flag.sh \
	$(NULL)

interop_qemu_nbd_SOURCES = \
	interop.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
interop_qemu_nbd_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DSOCKET_ACTIVATION=1 \
	-DSERVER=\"$(QEMU_NBD)\" \
	-DSERVER_PARAMS='"-f", "raw", "-x", "/", TMPFILE' \
	-DEXPORT_NAME='"/"' \
	$(NULL)
interop_qemu_nbd_LDADD = $(top_builddir)/lib/libnbd.la

# qemu-nbd requires absolute path to dir
interop_qemu_nbd_tls_certs_SOURCES = \
	interop.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
interop_qemu_nbd_tls_certs_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DREQUIRES=' requires ("test -d $(abs_top_builddir)/tests/pki"); requires_qemu_nbd_tls_support (); ' \
	-DSOCKET_ACTIVATION=1 \
	-DSERVER=\"$(QEMU_NBD)\" \
	-DSERVER_PARAMS='"--object", "tls-creds-x509,id=tls0,endpoint=server,dir=$(abs_top_builddir)/tests/pki", "--tls-creds", "tls0", "-f", "raw", "-x", "/", TMPFILE' \
	-DEXPORT_NAME='"/"' \
	-DCERTS='"../tests/pki"' \
	-DTLS_MODE=LIBNBD_TLS_REQUIRE \
	$(NULL)
interop_qemu_nbd_tls_certs_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(GNUTLS_LIBS) \
	$(NULL)

# qemu-nbd requires absolute path to dir
interop_qemu_nbd_tls_psk_SOURCES = \
	interop.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
interop_qemu_nbd_tls_psk_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DREQUIRES=' requires ("test -f $(abs_top_builddir)/tests/keys.psk"); requires_qemu_nbd_tls_support (); requires_qemu_nbd_tls_psk_support (); ' \
	-DSOCKET_ACTIVATION=1 \
	-DSERVER=\"$(QEMU_NBD)\" \
	-DSERVER_PARAMS='"--object", "tls-creds-psk,id=tls0,endpoint=server,dir=$(abs_top_builddir)/tests", "--tls-creds", "tls0", "-f", "raw", "-x", "/", TMPFILE' \
	-DEXPORT_NAME='"/"' \
	-DPSK='"../tests/keys.psk"' \
	-DTLS_MODE=LIBNBD_TLS_REQUIRE \
	$(NULL)
interop_qemu_nbd_tls_psk_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(GNUTLS_LIBS) \
	$(NULL)

list_exports_qemu_nbd_SOURCES = \
	list-exports.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
list_exports_qemu_nbd_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DSOCKET_ACTIVATION=1 \
	-DNEEDS_TMPFILE=1 \
	-DSERVER=\"$(QEMU_NBD)\" \
	-DSERVER_PARAMS='"-f", "raw", "-x", "testing", "-D", "data", TMPFILE' \
	-DEXPORTS='"testing"' \
	-DDESCRIPTIONS='"data"' \
	$(NULL)
list_exports_qemu_nbd_LDADD = $(top_builddir)/lib/libnbd.la

socket_activation_qemu_nbd_SOURCES = socket-activation.c
socket_activation_qemu_nbd_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DSERVER=\"$(QEMU_NBD)\" \
	-DSERVER_PARAMS='"-f", "raw", "-x", "", tmpfile' \
	$(NULL)
socket_activation_qemu_nbd_LDADD = $(top_builddir)/lib/libnbd.la

dirty_bitmap_SOURCES = dirty-bitmap.c
dirty_bitmap_LDADD = $(top_builddir)/lib/libnbd.la

block_status_64_SOURCES = block-status-64.c
block_status_64_LDADD = $(top_builddir)/lib/libnbd.la

structured_read_SOURCES = structured-read.c
structured_read_LDADD = $(top_builddir)/lib/libnbd.la

opt_extended_headers_SOURCES = opt-extended-headers.c
opt_extended_headers_LDADD = $(top_builddir)/lib/libnbd.la

block_status_payload_SOURCES = block-status-payload.c
block_status_payload_LDADD = $(top_builddir)/lib/libnbd.la

endif HAVE_QEMU_NBD

#----------------------------------------------------------------------
# Test interoperability with qemu-storage-daemon.
#
# We treat qemu-storage-daemon as effectively the same as qemu-nbd
# (above) since they use the same code.  But there is a single test to
# make sure we can use it.

if HAVE_QEMU_STORAGE_DAEMON
# It was too complicated to extend interop.c to support q-s-d (since
# it doesn't support stdin/stdio or systemd socket activation) so use
# this shell script instead.
TESTS += interop-qemu-storage-daemon.sh
endif HAVE_QEMU_STORAGE_DAEMON

#----------------------------------------------------------------------
# Test interoperability with nbdkit.

if HAVE_NBDKIT

check_PROGRAMS += \
	interop-nbdkit \
	interop-nbdkit-tls-certs \
	interop-nbdkit-tls-certs-allow-enabled \
	interop-nbdkit-tls-certs-allow-fallback \
	interop-nbdkit-tls-certs-bad-CA \
	interop-nbdkit-tls-certs-bad-hostname \
	interop-nbdkit-tls-psk \
	interop-nbdkit-tls-psk-allow-enabled \
	interop-nbdkit-tls-psk-allow-fallback \
	socket-activation-nbdkit \
	list-exports-nbdkit \
	$(NULL)
TESTS += \
	interop-nbdkit \
	interop-nbdkit-tls-certs \
	interop-nbdkit-tls-certs-allow-enabled \
	interop-nbdkit-tls-certs-allow-fallback \
	interop-nbdkit-tls-certs-bad-CA \
	interop-nbdkit-tls-certs-bad-hostname \
	interop-nbdkit-tls-psk \
	interop-nbdkit-tls-psk-allow-enabled \
	interop-nbdkit-tls-psk-allow-fallback \
	socket-activation-nbdkit \
	list-exports-nbdkit \
	$(NULL)

interop_nbdkit_SOURCES = \
	interop.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
interop_nbdkit_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DSERVER=\"$(NBDKIT)\" \
	-DSERVER_PARAMS='"-s", "--exit-with-parent", "file", TMPFILE' \
	$(NULL)
interop_nbdkit_LDADD = $(top_builddir)/lib/libnbd.la

interop_nbdkit_tls_certs_SOURCES = \
	interop.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
interop_nbdkit_tls_certs_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DREQUIRES=' requires ("test -d ../tests/pki"); ' \
	-DSERVER=\"$(NBDKIT)\" \
	-DSERVER_PARAMS='"--tls=require", "--tls-certificates=../tests/pki", "-s", "--exit-with-parent", "file", TMPFILE' \
	-DCERTS='"../tests/pki"' \
	-DTLS_MODE=LIBNBD_TLS_REQUIRE \
	$(NULL)
interop_nbdkit_tls_certs_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(GNUTLS_LIBS) \
	$(NULL)

interop_nbdkit_tls_certs_allow_enabled_SOURCES = \
	interop.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
interop_nbdkit_tls_certs_allow_enabled_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DREQUIRES=' requires ("test -d ../tests/pki"); ' \
	-DSERVER=\"$(NBDKIT)\" \
	-DSERVER_PARAMS='"--tls=require", "--tls-certificates=../tests/pki", "-s", "--exit-with-parent", "file", TMPFILE' \
	-DCERTS='"../tests/pki"' \
	-DTLS_MODE=LIBNBD_TLS_ALLOW \
	$(NULL)
interop_nbdkit_tls_certs_allow_enabled_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(GNUTLS_LIBS) \
	$(NULL)

interop_nbdkit_tls_certs_allow_fallback_SOURCES = \
	interop.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
interop_nbdkit_tls_certs_allow_fallback_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DREQUIRES=' requires ("test -d ../tests/pki"); ' \
	-DSERVER=\"$(NBDKIT)\" \
	-DSERVER_PARAMS='"--tls=off", "-s", "--exit-with-parent", "file", TMPFILE' \
	-DCERTS='"../tests/pki"' \
	-DTLS_MODE=LIBNBD_TLS_ALLOW \
	-DTLS_FALLBACK=1 \
	$(NULL)
interop_nbdkit_tls_certs_allow_fallback_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(GNUTLS_LIBS) \
	$(NULL)

# In this test, nbdkit offers a server certificate signed by our CA in
# the tests/pki directory, but we deliberately tell libnbd to test
# against the system CA (-DCERTS=NULL).  This is expected to fail the
# connection with the error:
# libnbd: debug: nbd1: nbd_connect_command: handle dead: nbd_connect_command: gnutls_handshake: Error in the certificate verification. (15/1)
interop_nbdkit_tls_certs_bad_CA_SOURCES = \
	interop.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
interop_nbdkit_tls_certs_bad_CA_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DREQUIRES=' requires ("test -d ../tests/pki"); ' \
	-DSERVER=\"$(NBDKIT)\" \
	-DSERVER_PARAMS='"--tls=require", "--tls-certificates=../tests/pki", "-s", "--exit-with-parent", "null"' \
	-DCERTS=NULL \
	-DTLS_MODE=LIBNBD_TLS_REQUIRE \
	-DEXPECT_FAIL=1 \
	$(NULL)
interop_nbdkit_tls_certs_bad_CA_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(GNUTLS_LIBS) \
	$(NULL)

# This test is expected to fail because even though the CA is correct,
# we override tls_hostname with a bogus server name that won't match
# the server's certificate.
interop_nbdkit_tls_certs_bad_hostname_SOURCES = \
	interop.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
interop_nbdkit_tls_certs_bad_hostname_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DREQUIRES=' requires ("test -d ../tests/pki"); ' \
	-DSERVER=\"$(NBDKIT)\" \
	-DSERVER_PARAMS='"--tls=require", "--tls-certificates=../tests/pki", "-s", "--exit-with-parent", "null"' \
	-DCERTS='"../tests/pki"' \
	-DTLS_MODE=LIBNBD_TLS_REQUIRE \
	-DTLS_HOSTNAME='"this.is.bad.example.com"' \
	-DEXPECT_FAIL=1 \
	$(NULL)
interop_nbdkit_tls_certs_bad_hostname_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(GNUTLS_LIBS) \
	$(NULL)

interop_nbdkit_tls_psk_SOURCES = \
	interop.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
interop_nbdkit_tls_psk_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DREQUIRES=' requires ("test -f ../tests/keys.psk"); ' \
	-DSERVER=\"$(NBDKIT)\" \
	-DSERVER_PARAMS='"--tls=require", "--tls-psk=../tests/keys.psk", "-s", "--exit-with-parent", "file", TMPFILE' \
	-DPSK='"../tests/keys.psk"' \
	-DTLS_MODE=LIBNBD_TLS_REQUIRE \
	$(NULL)
interop_nbdkit_tls_psk_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(GNUTLS_LIBS) \
	$(NULL)

interop_nbdkit_tls_psk_allow_enabled_SOURCES = \
	interop.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
interop_nbdkit_tls_psk_allow_enabled_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DREQUIRES=' requires ("test -f ../tests/keys.psk"); ' \
	-DSERVER=\"$(NBDKIT)\" \
	-DSERVER_PARAMS='"--tls=require", "--tls-psk=../tests/keys.psk", "-s", "--exit-with-parent", "file", TMPFILE' \
	-DPSK='"../tests/keys.psk"' \
	-DTLS_MODE=LIBNBD_TLS_ALLOW \
	$(NULL)
interop_nbdkit_tls_psk_allow_enabled_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(GNUTLS_LIBS) \
	$(NULL)

interop_nbdkit_tls_psk_allow_fallback_SOURCES = \
	interop.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
interop_nbdkit_tls_psk_allow_fallback_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DREQUIRES=' requires ("test -f ../tests/keys.psk"); ' \
	-DSERVER=\"$(NBDKIT)\" \
	-DSERVER_PARAMS='"--tls=off", "-s", "--exit-with-parent", "file", TMPFILE' \
	-DPSK='"../tests/keys.psk"' \
	-DTLS_MODE=LIBNBD_TLS_ALLOW \
	-DTLS_FALLBACK=1 \
	$(NULL)
interop_nbdkit_tls_psk_allow_fallback_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(GNUTLS_LIBS) \
	$(NULL)

socket_activation_nbdkit_SOURCES = socket-activation.c
socket_activation_nbdkit_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DSERVER=\"$(NBDKIT)\" \
	-DSERVER_PARAMS='"file", tmpfile' \
	$(NULL)
socket_activation_nbdkit_LDADD = $(top_builddir)/lib/libnbd.la

# This test requires nbdkit >= 1.22, checked at runtime.
list_exports_nbdkit_SOURCES = \
	list-exports.c \
	requires.c \
	../tests/requires.h \
	$(NULL)
list_exports_nbdkit_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-DREQUIRES=' requires ("$(NBDKIT) file --version"); requires ("minor=`$(NBDKIT) --dump-config | grep ^version_minor | cut -d= -f2`; echo $$minor; test $$minor -ge 22"); ' \
	-DSERVER=\"$(NBDKIT)\" \
	-DSERVER_PARAMS='"-s", "--exit-with-parent", "file", "dir=$(srcdir)/list-exports-test-dir"' \
	-DEXPORTS='"disk1", "disk2"' \
	-DDESCRIPTIONS='"", ""' \
	$(NULL)
list_exports_nbdkit_LDADD = $(top_builddir)/lib/libnbd.la

endif HAVE_NBDKIT

check-valgrind:
	LIBNBD_VALGRIND=1 $(MAKE) check
