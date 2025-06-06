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

EXTRA_DIST = LICENSE-FOR-EXAMPLES

noinst_PROGRAMS = \
	aio-connect-read \
	batched-read-write \
	connect-benchmark \
	connect-command \
	connect-uri \
	encryption \
	fetch-first-sector \
	get-size \
	list-exports \
	open-qcow2 \
	reads-and-writes \
	server-flags \
	strict-structured-reads \
	threaded-reads-and-writes \
	userfault-map \
	$(NULL)

aio_connect_read_SOURCES = \
	aio-connect-read.c \
	$(NULL)
aio_connect_read_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
aio_connect_read_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(NULL)
aio_connect_read_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(NULL)

batched_read_write_SOURCES = \
	batched-read-write.c \
	$(NULL)
batched_read_write_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
batched_read_write_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(NULL)
batched_read_write_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(NULL)

connect_benchmark_SOURCES = \
	connect-benchmark.c \
	$(NULL)
connect_benchmark_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
connect_benchmark_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(PTHREAD_CFLAGS) \
	$(NULL)
connect_benchmark_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(PTHREAD_LIBS) \
	$(NULL)

connect_command_SOURCES = \
	connect-command.c \
	$(NULL)
connect_command_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
connect_command_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(NULL)
connect_command_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(NULL)

connect_uri_SOURCES = \
	connect-uri.c \
	$(NULL)
connect_uri_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
connect_uri_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(NULL)
connect_uri_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(NULL)

encryption_SOURCES = \
	encryption.c \
	$(NULL)
encryption_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
encryption_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(NULL)
encryption_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(NULL)

fetch_first_sector_SOURCES = \
	fetch-first-sector.c \
	$(NULL)
fetch_first_sector_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
fetch_first_sector_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(NULL)
fetch_first_sector_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(NULL)

get_size_SOURCES = \
	get-size.c \
	$(NULL)
get_size_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
get_size_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(NULL)
get_size_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(NULL)

list_exports_SOURCES = \
	list-exports.c \
	$(NULL)
list_exports_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
list_exports_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(NULL)
list_exports_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(NULL)

open_qcow2_SOURCES = \
	open-qcow2.c \
	$(NULL)
open_qcow2_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
open_qcow2_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(NULL)
open_qcow2_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(NULL)

reads_and_writes_SOURCES = \
	reads-and-writes.c \
	$(NULL)
reads_and_writes_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
reads_and_writes_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(NULL)
reads_and_writes_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(NULL)

server_flags_SOURCES = \
	server-flags.c \
	$(NULL)
server_flags_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
server_flags_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(NULL)
server_flags_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(NULL)

strict_structured_reads_SOURCES = \
	strict-structured-reads.c \
	$(NULL)
strict_structured_reads_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
strict_structured_reads_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(NULL)
strict_structured_reads_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(NULL)

threaded_reads_and_writes_SOURCES = \
	threaded-reads-and-writes.c \
	$(NULL)
threaded_reads_and_writes_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
threaded_reads_and_writes_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(PTHREAD_CFLAGS) \
	$(NULL)
threaded_reads_and_writes_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(PTHREAD_LIBS) \
	$(NULL)

# This userfaultfd example only works on Linux >= 4.11.  However
# creating an autoconf conditional to express this is difficult, so on
# other platforms this will still compile but the program will just
# print an error.
userfault_map_SOURCES = \
	userfault-map.c \
	$(NULL)
userfault_map_CPPFLAGS = \
	-I$(top_srcdir) -I$(top_builddir) -I$(top_srcdir)/include \
	$(NULL)
userfault_map_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(PTHREAD_CFLAGS) \
	$(NULL)
userfault_map_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(PTHREAD_LIBS) \
	$(NULL)

if HAVE_GLIB
noinst_PROGRAMS += \
	glib-main-loop

glib_main_loop_SOURCES = \
	glib-main-loop.c \
	$(NULL)
glib_main_loop_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
glib_main_loop_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(NULL)
glib_main_loop_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(GLIB_LIBS) \
	$(NULL)
endif

if HAVE_LIBEV
noinst_PROGRAMS += \
	copy-libev

copy_libev_SOURCES = \
	copy-libev.c \
	$(NULL)
copy_libev_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(NULL)
copy_libev_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(LIBEV_CFLAGS) \
	$(NULL)
copy_libev_LDADD = \
	$(top_builddir)/lib/libnbd.la \
	$(LIBEV_LIBS) \
	$(NULL)
endif
