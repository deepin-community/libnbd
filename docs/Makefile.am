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

# This is populated in the generated file Makefile.inc
api_built =

# Our use of sinclude to bypass Automake is intentional; this file won't
# exist in a fresh git checkout until after the generator has run, but
# should already be present in any released tarball.  But, since automake
# can't see into this file, it did not hoist the resulting prerequisites
# prior to its 'all-am: Makefile $(MANS)' rule, which gets parsed before
# $(MANS) has grown in size, so we have to add a second all-am dependency.
sinclude $(srcdir)/Makefile.inc
all-am: $(api_built:%=%.3)

generator_built = \
	Makefile.inc \
	api-links.pod \
	api-flag-links.pod \
	$(api_built:%=%.pod) \
	$(NULL)

EXTRA_DIST = \
	$(generator_built) \
	libnbd.pod \
	libnbd-release-notes-1.2.pod \
	libnbd-release-notes-1.4.pod \
	libnbd-release-notes-1.6.pod \
	libnbd-release-notes-1.8.pod \
	libnbd-release-notes-1.10.pod \
	libnbd-release-notes-1.12.pod \
	libnbd-release-notes-1.14.pod \
	libnbd-release-notes-1.16.pod \
	libnbd-release-notes-1.18.pod \
	libnbd-release-notes-1.20.pod \
	libnbd-release-notes-1.22.pod \
	libnbd-security.pod \
	nbd_create.pod \
	nbd_close.3 \
	nbd_get_error.3 \
	nbd_get_errno.3 \
	$(NULL)

if HAVE_POD

# There are a handful of nroff files which are hand-written (such as
# nbd_close.3) but most are generated from POD.  List the generated
# ones here so we don't delete the hand-written ones in CLEANFILES.
manpages_with_pod_source = \
	libnbd.3 \
	libnbd-release-notes-1.2.1 \
	libnbd-release-notes-1.4.1 \
	libnbd-release-notes-1.6.1 \
	libnbd-release-notes-1.8.1 \
	libnbd-release-notes-1.10.1 \
	libnbd-release-notes-1.12.1 \
	libnbd-release-notes-1.14.1 \
	libnbd-release-notes-1.16.1 \
	libnbd-release-notes-1.18.1 \
	libnbd-release-notes-1.20.1 \
	libnbd-release-notes-1.22.1 \
	libnbd-security.3 \
	nbd_create.3 \
	$(api_built:%=%.3)

man_MANS = \
	$(manpages_with_pod_source) \
	nbd_close.3 \
	nbd_get_error.3 \
	nbd_get_errno.3 \
	$(NULL)
CLEANFILES += \
	$(manpages_with_pod_source) \
	$(NULL)

nbd_%.3: nbd_%.pod $(top_builddir)/podwrapper.pl
	$(PODWRAPPER) --section=3 --man $@ \
	    --html $(top_builddir)/html/$@.html \
	    $<

libnbd.3: libnbd.pod $(top_builddir)/podwrapper.pl \
		api-links.pod api-flag-links.pod
	$(PODWRAPPER) --section=3 --man $@ \
	    --insert $(srcdir)/api-links.pod:__API_LINKS__ \
	    --insert $(srcdir)/api-flag-links.pod:__API_FLAG_LINKS__ \
	    --html $(top_builddir)/html/$@.html \
	    $<

libnbd-release-notes-%.1: libnbd-release-notes-%.pod
	$(PODWRAPPER) --section=1 --man $@ \
	    --html $(top_builddir)/html/$@.html \
	    $<

libnbd-security.3: libnbd-security.pod
	$(PODWRAPPER) --section=3 --man $@ \
	    --html $(top_builddir)/html/$@.html \
	    $<

endif HAVE_POD
