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

info_sh_files = \
	info-can.sh \
	info-can-connect.sh \
	info-can-read.sh \
	info-can-zero.sh \
	info-cannot.sh \
	info-cmd-json.sh \
	info-cmd-map-totals.sh \
	info-cmd-size.sh \
	info-is-read-only.sh \
	info-list.sh \
	info-list-json.sh \
	info-list-qemu.sh \
	info-list-json-qemu.sh \
	info-list-uris.sh \
	info-json.sh \
	info-oldstyle.sh \
	info-packets.sh \
	info-null.sh \
	info-size.sh \
	info-text.sh \
	info-uri.sh \
	info-uri-json.sh \
	info-uri-nbds.sh \
	info-uri-quoting.sh \
	info-description.sh \
	info-description-qemu.sh \
	info-map-base-allocation.sh \
	info-map-base-allocation-json.sh \
	info-map-base-allocation-large.sh \
	info-map-base-allocation-weird.sh \
	info-map-base-allocation-zero.sh \
	info-map-qemu-dirty-bitmap.sh \
	info-map-qemu-allocation-depth.sh \
	info-map-totals.sh \
	info-map-totals-json.sh \
	info-atomic-output.sh \
	$(NULL)

EXTRA_DIST = \
	$(info_sh_files) \
	nbdinfo.pod \
	test-long-options.sh \
	test-short-options.sh \
	test-version.sh \
	$(NULL)

TESTS_ENVIRONMENT = \
	LIBNBD_DEBUG=1 \
	$(MALLOC_CHECKS) \
	EXPECTED_VERSION=$(VERSION) \
	$(NULL)
LOG_COMPILER = $(top_builddir)/run
TESTS =

nbdinfo_SOURCES = \
	nbdinfo.h \
	can.c \
	list.c \
	main.c \
	map.c \
	show.c \
	size.c \
	uri.c \
	utils.c \
	$(NULL)
nbdinfo_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common/include \
	-I$(top_srcdir)/common/utils \
	$(NULL)
nbdinfo_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	$(NULL)
nbdinfo_LDADD = \
	$(top_builddir)/common/utils/libutils.la \
	$(top_builddir)/lib/libnbd.la \
	$(NULL)

# This program depends on URI support; no point in installing it otherwise
if HAVE_LIBXML2

bin_PROGRAMS = nbdinfo

if HAVE_POD

man_MANS = \
	nbdinfo.1 \
	$(NULL)

nbdinfo.1: nbdinfo.pod $(top_builddir)/podwrapper.pl
	$(PODWRAPPER) --section=1 --man $@ \
	    --html $(top_builddir)/html/$@.html \
	    $<

endif HAVE_POD

TESTS += \
	test-long-options.sh \
	test-short-options.sh \
	test-version.sh \
	$(info_sh_files) \
	$(NULL)

check-valgrind:
	LIBNBD_VALGRIND=1 $(MAKE) check

endif HAVE_LIBXML2
