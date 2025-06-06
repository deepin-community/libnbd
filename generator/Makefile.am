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

CLEANFILES += *.annot *.cmi *.cmo generator

DISTCLEANFILES = stamp-generator

# Note: These are not source files.  They are parsed by the generator
# and output to lib/states.c.
states_code = \
	states.c \
	states-connect.c \
	states-connect-socket-activation.c \
	states-issue-command.c \
	states-magic.c \
	states-newstyle-opt-export-name.c \
	states-newstyle-opt-extended-headers.c \
	states-newstyle-opt-list.c \
	states-newstyle-opt-go.c \
	states-newstyle-opt-meta-context.c \
	states-newstyle-opt-starttls.c \
	states-newstyle-opt-structured-reply.c \
	states-newstyle.c \
	states-oldstyle.c \
	states-reply-chunk.c \
	states-reply-simple.c \
	states-reply.c \
	$(NULL)

# Some man pages contain example code and so must be rebuilt
# if that changes.
examples_code = $(wildcard $(top_srcdir)/examples/*.c)

# Source code for the generator.
sources = \
	config.mli \
	config.ml \
	utils.mli \
	utils.ml \
	state_machine.mli \
	state_machine.ml \
	API.mli \
	API.ml \
	state_machine_generator.mli \
	state_machine_generator.ml \
	C.mli \
	C.ml \
	docs.mli \
	docs.ml \
	Python.mli \
	Python.ml \
	OCaml.mli \
	OCaml.ml \
	GoLang.mli \
	GoLang.ml \
	RustSys.mli \
	RustSys.ml \
	Rust.mli \
	Rust.ml \
	generator.ml \
	$(NULL)

EXTRA_DIST = \
	README.state-machine.md \
	$(states_code) \
	$(sources) \
	$(NULL)

if HAVE_OCAMLC

stamp-generator: generator $(states_code) $(examples_code)
	rm -f $@
#	We must run this from the source directory since these
#	files are distributed in the tarball.
	cd $(top_srcdir) && \
	    OCAMLRUNPARAM=b $(abs_top_builddir)/generator/generator
	touch $@

generator: $(sources)
	$(OCAMLC) $(OCAML_FLAGS) $(OCAML_WARN_ERROR) -I $(srcdir) -I . \
	  -I +str str.cma -I +unix unix.cma $(sources) -o $@

else

stamp-generator:
	@rm -f $@
	@if [ -f $(top_srcdir)/lib/states.c ]; then            \
	    echo "No ocamlc (OCaml bytecode compiler), skipping the generator."; \
	    touch $@;                                          \
	else                                                   \
	    echo;                                              \
	    echo "        *** *** ***";                        \
	    echo;                                              \
	    echo "Generated files are missing from your build."; \
	    echo;                                              \
	    echo "Install the OCaml bytecode compiler (the program called ‘ocamlc’)"; \
	    echo "and rerun ./configure && make";              \
	    echo;                                              \
	    echo "OR build from the tarballs at https://download.libguestfs.org/libnbd/"; \
	    echo "which include generated files.";             \
	    echo;                                              \
	    echo "        *** *** ***";                        \
	    echo;                                              \
	    exit 1;                                            \
	fi

endif
