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

generator_built = \
	NBD.mli \
	NBD.ml \
	nbd-c.c \
	$(NULL)

EXTRA_DIST = \
	$(generator_built) \
	libnbd-ocaml.pod \
	$(NULL)

CLEANFILES += *.annot *.cmi *.cmo *.cmx *.o *.a *.so *.cma *.cmxa

man_MANS =
CLEANFILES += $(man_MANS)

if HAVE_POD

man_MANS += libnbd-ocaml.3

libnbd-ocaml.3: libnbd-ocaml.pod $(top_builddir)/podwrapper.pl
	$(PODWRAPPER) --section=3 --man $@ \
	    --html $(top_builddir)/html/$@.html \
	    $<

endif HAVE_POD

if HAVE_OCAML

OCAMLFLAGS = $(OCAML_FLAGS) $(OCAML_WARN_ERROR) -ccopt '$(CFLAGS)'
OCAMLPACKAGES = -package $(OCAMLFIND_PACKAGES) -I $(srcdir) -I $(builddir)

noinst_DATA = mlnbd.cma META
if HAVE_OCAMLOPT
noinst_DATA += mlnbd.cmxa
endif

# Build the C part into a library, so that automake handles the C
# compilation step for us.  Note that we don't directly use this
# library; we link with the object files that it generates.
noinst_LIBRARIES = libnbdocaml.a

# Both calls to ocamlmklib below will create 'libmlnbd.a' and if run
# at the same time, they will stomp on or corrupt each others copy.
# Hence we have to serialize the calls.

CLEANFILES += stamp-mlnbd

mlnbd.cma mlnbd.cmxa: stamp-mlnbd

nbd_cmm = NBD.cmo
if HAVE_OCAMLOPT
nbd_cmm += NBD.cmx
endif

stamp-mlnbd: libnbdocaml.a $(nbd_cmm)
	$(OCAMLFIND) mklib -o mlnbd \
	  $(OCAMLPACKAGES) \
	  -ldopt '$(LDFLAGS)' \
	  $(libnbdocaml_a_OBJECTS) NBD.cmo \
	  -L../lib/.libs -lnbd
if HAVE_OCAMLOPT
	$(OCAMLFIND) mklib -o mlnbd \
	  $(OCAMLPACKAGES) \
	  -ldopt '$(LDFLAGS)' \
	  $(libnbdocaml_a_OBJECTS) NBD.cmx \
	  -L../lib/.libs -lnbd
endif
	touch $@

libnbdocaml_a_CPPFLAGS = \
	-I$(top_builddir) -I$(OCAMLLIB) -I$(top_srcdir)/ocaml \
	-I$(top_srcdir)/include \
	-DCAML_NAME_SPACE \
	$(NULL)

libnbdocaml_a_CFLAGS = \
	$(WARNINGS_CFLAGS) \
	-fPIC \
	$(NULL)

libnbdocaml_a_SOURCES = \
	nbd-c.c \
	nbd-c.h \
	buffer.c \
	handle.c \
	helpers.c \
	$(NULL)

%.bc: %.cmo mlnbd.cma
	$(top_builddir)/libtool -dlopen $(top_builddir)/lib/.libs/libnbd.la --mode=execute \
        $(OCAMLFIND) ocamlc $(OCAMLFLAGS) -I . -package $(OCAMLFIND_PACKAGES) -linkpkg mlnbd.cma $< -o $@

if HAVE_OCAMLOPT
%.opt: %.cmx mlnbd.cmxa
	$(OCAMLFIND) ocamlopt $(OCAMLFLAGS) -cclib -L$(top_builddir)/lib/.libs -I . -package $(OCAMLFIND_PACKAGES) -linkpkg mlnbd.cmxa $< -o $@
endif

# Dependencies.
.depend: $(srcdir)/*.mli $(srcdir)/*.ml
	$(OCAMLFIND) ocamldep $(srcdir)/*.mli $(srcdir)/*.ml > $@
-include .depend

# Run ocamlfind to perform the install.  ocamlfind refuses to install
# the package if it is already installed, so remove the old package
# first.
data_hook_files = META *.so *.a *.cma *.cmi $(srcdir)/*.mli
if HAVE_OCAMLOPT
data_hook_files += *.cmx *.cmxa
endif

install-data-hook:
	mkdir -p $(DESTDIR)$(OCAMLLIB)
	mkdir -p $(DESTDIR)$(OCAMLLIB)/stublibs
	rm -rf $(DESTDIR)$(OCAMLLIB)/nbd
	rm -rf $(DESTDIR)$(OCAMLLIB)/stublibs/dllmlnbd.so*
	$(OCAMLFIND) install \
	  -ldconf ignore -destdir $(DESTDIR)$(OCAMLLIB) \
	  nbd \
	  $(data_hook_files)
	rm $(DESTDIR)$(OCAMLLIB)/nbd/libnbdocaml.a

# This "tricks" ocamlfind into allowing us to compile other OCaml
# programs against a locally compiled copy of the libnbd sources.
# ocamlfind needs to see a directory called ‘nbd’ which contains
# ‘META’.  The current directory is called ‘ocaml’, but if we make
# this symlink then we can create the required directory structure.
#
# Note if you just want to use this, make sure you use
# ‘../libnbd/run make’ in your other program and everything should
# just work.
CLEANFILES += nbd

all-local: nbd

nbd:
	rm -f $@
	$(LN_S) . $@

# OCaml documentation.

if HAVE_OCAMLDOC
ocamldoc_generated = \
	NBD.3 \
	NBD.ALLOW_TRANSPORT.3 \
	NBD.Buffer.3 \
	NBD.CMD_FLAG.3 \
	NBD.HANDSHAKE_FLAG.3 \
	NBD.SHUTDOWN.3 \
	NBD.SIZE.3 \
	NBD.STRICT.3 \
	NBD.TLS.3 \
	$(NULL)
man_MANS += $(ocamldoc_generated)

$(ocamldoc_generated): stamp-manpages
stamp-manpages: NBD.mli
	rm -f $@
	$(OCAMLFIND) ocamldoc $(OCAMLPACKAGES) -man -man-mini -man-suffix 3 $<
	touch $@
CLEANFILES += stamp-manpages
endif HAVE_OCAMLDOC

endif HAVE_OCAML
