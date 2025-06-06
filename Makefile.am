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

include $(top_srcdir)/common-rules.mk

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	.dir-locals.el \
	.editorconfig \
	.gitattributes \
	.gitignore \
	.ocamlformat \
	contrib/libnbd.m4 \
	html/pod.css \
	README.md \
	rustfmt.toml \
	scripts/git.orderfile \
	SECURITY \
	$(NULL)

CLEANFILES += m4/*~

SUBDIRS = \
	generator \
	include \
	common/include \
	common/utils \
	lib \
	docs \
	examples \
	valgrind \
	. \
	tests \
	python \
	sh \
	info \
	copy \
	dump \
	fuse \
	ublk \
	ocaml \
	ocaml/examples \
	ocaml/tests \
	golang \
	golang/examples \
	rust \
	interop \
	fuzzing \
	bash-completion \
	$(NULL)

noinst_SCRIPTS = run

# Check no files are missing from EXTRA_DIST rules, and that all
# generated files have been included in the tarball.  (Note you must
# have done 'make dist')
maintainer-check-extra-dist:
	@tar ztf $(PACKAGE_NAME)-$(VERSION).tar.gz | sort | \
	    sed 's,^$(PACKAGE_NAME)-$(VERSION)/,,' > tarfiles
	@git ls-files | \
      grep -v \
	    -e '^\.cirrus.yml' \
	    -e '^\.gitlab-ci.yml' \
	    -e '^ci/' | \
	    sort > gitfiles
	@comm -13 tarfiles gitfiles > comm-out
	@echo Checking for differences between EXTRA_DIST and git ...
	@cat comm-out
	@[ ! -s comm-out ]
	@rm tarfiles gitfiles comm-out
	@echo PASS: EXTRA_DIST tests

check-valgrind: all
	@for d in tests info copy fuse ocaml/tests interop; do \
	    $(MAKE) -C $$d check-valgrind || exit 1; \
	done

bench: all
	@for d in common/utils; do \
	    $(MAKE) -C $$d bench || exit 1; \
	done

check-root:
	@for d in copy; do \
	    $(MAKE) -C $$d check-root || exit 1; \
	done

#----------------------------------------------------------------------
# Maintainers only!

# Commit everything in the current directory and set the commit
# message to the current version number.
maintainer-commit:
	git commit -a -m "Version $(VERSION)."

# Tag HEAD with the current version.
maintainer-tag:
	git tag -a v$(VERSION) -m "Version $(VERSION)." -f

# Build golang distribution file.  This is unpacked on the webserver
# under libguestfs.org/libnbd/golang/
maintainer-golang-dist:
	cd golang && ./make-dist.sh
