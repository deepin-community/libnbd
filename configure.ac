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

AC_INIT([libnbd],[1.22.0])

AC_CONFIG_MACRO_DIR([m4])
m4_ifdef([AC_USE_SYSTEM_EXTENSIONS],[],
         [m4_define([AC_USE_SYSTEM_EXTENSIONS],[])])
AC_USE_SYSTEM_EXTENSIONS
AC_SYS_LARGEFILE

dnl NB: Do not [quote] this parameter.
AM_INIT_AUTOMAKE(foreign subdir-objects)
LT_INIT

AC_CANONICAL_HOST

AC_PROG_LN_S
AC_PROG_SED

dnl Check for some commands used by the tests which should be the GNU
dnl coreutils variants.  On macOS these are prefixed with 'g' or 'gnu'.
AC_CHECK_PROGS([CMP],[gcmp cmp],[cmp])
AC_CHECK_PROGS([CUT],[gnucut cut],[cut])
AC_CHECK_PROGS([DD],[gdd dd],[dd])
AC_CHECK_PROGS([REALPATH],[grealpath realpath],[realpath])
AC_CHECK_PROGS([STAT],[gstat stat],[stat])
AC_CHECK_PROGS([TR],[gtr tr],[tr])
AC_CHECK_PROGS([TRUNCATE],[gtruncate truncate],[truncate])

dnl Bash must be at least version 4.  If it is too old, fail hard
dnl with a good diagnostic.  Note macOS ships an ancient version
dnl of bash (https://gitlab.com/nbdkit/nbdkit/-/issues/21)
bash=`which bash`
AC_MSG_CHECKING([for the major version of $bash])
bash_major=`bash -c 'echo ${BASH_VERSINFO:-0}'`
AC_MSG_RESULT([$bash_major])
AS_IF([test $bash_major -lt 4],
      [AC_MSG_ERROR([
Your bash shell ($bash) is older than version 4.

If you are using macOS, please install the latest version of bash
from homebrew or macports.

If you have installed the latest bash and this error is still
showing up, check that \$PATH is set correctly.
])])

dnl Check for basic C environment.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_CPP

AC_C_PROTOTYPES
test "x$U" != "x" && AC_MSG_ERROR([Compiler not ANSI compliant])

AM_PROG_CC_C_O

AX_PTHREAD

dnl Defines WORDS_BIGENDIAN on big endian platforms.
AC_C_BIGENDIAN

dnl Check for C++ (optional, we just use this to test the header
dnl can be included from C++ code).
AC_PROG_CXX

dnl The C++ compiler test is pretty useless because even if it fails
dnl it sets CXX=g++.  So test the compiler actually works.
AC_MSG_CHECKING([if the C++ compiler really really works])
AS_IF([$CXX --version >&AS_MESSAGE_LOG_FD 2>&1],[have_cxx=yes],[have_cxx=no])
AC_MSG_RESULT([$have_cxx])
AM_CONDITIONAL([HAVE_CXX], [test "$have_cxx" = "yes"])

AC_ARG_ENABLE([gcc-warnings],
    [AS_HELP_STRING([--enable-gcc-warnings],
                    [turn on lots of GCC warnings (for developers)])],
     [case $enableval in
      yes|no) ;;
      *)      AC_MSG_ERROR([bad value $enableval for gcc-warnings option]) ;;
      esac
      gcc_warnings=$enableval],
      [gcc_warnings=no]
)
if test "x$gcc_warnings" = "xyes"; then
    # Enable normal GCC warnings and a few more:
    #  - Warn about variable length arrays on stack.
    #  - Warn about large stack frames (since we may be used from threads).
    WARNINGS_CFLAGS="-Wall -Werror"
    AC_C_COMPILE_FLAGS([WARNINGS_CFLAGS],
                       [-Wvla -Wframe-larger-than=5000 -Wstack-usage=10000],
                       [$CFLAGS -Werror])
    AC_SUBST([WARNINGS_CFLAGS])
fi

dnl Check for __builtin_*_overflow.  We need the dummy parameters
dnl else detection doesn't work correctly for some reason.
AC_CHECK_DECLS([__builtin_add_overflow(int, int, int *),
                __builtin_mul_overflow(int, int, int *)],
                [], [], [])

dnl Check for __auto_type (GCC extension).
AC_MSG_CHECKING([if __auto_type is available in this compiler])
AC_COMPILE_IFELSE([
AC_LANG_SOURCE([[
static int
test (int a)
{
  __auto_type at = a;
  return at;
}
]])
    ],[
    AC_MSG_RESULT([yes])
    AC_DEFINE([HAVE_AUTO_TYPE],[1],[__auto_type is available])
    ],[
    AC_MSG_RESULT([no])
    ]
)

dnl Check sizeof long.
AC_CHECK_SIZEOF(long)

dnl Check for other headers, all optional.
AC_CHECK_HEADERS([\
        byteswap.h \
        endian.h \
        linux/fs.h \
        linux/userfaultfd.h \
        stdatomic.h \
        sys/disk.h \
        sys/disklabel.h \
        sys/endian.h \
        sys/ioctl.h \
        sys/syscall.h])

AC_CHECK_HEADERS([linux/vm_sockets.h sys/vsock.h], [], [], [[
  #include <sys/socket.h>
]])
AC_CHECK_TYPES([struct sockaddr_vm], [], [], [[
  #include <sys/socket.h>
  #ifdef HAVE_LINUX_VM_SOCKETS_H
  #include <linux/vm_sockets.h>
  #elif HAVE_SYS_VSOCK_H
  #include <sys/vsock.h>
  #endif
  #ifndef AF_VSOCK
  #error "no vsock support"
  #endif
]])

dnl Check for various libc functions, all optional.
dnl
dnl posix_fadvise helps to optimise linear reads and writes.
dnl
dnl When /proc/sys/kernel/core_pattern starts with a pipe (|) symbol, Linux
dnl ignores "ulimit -c" and (equivalent) setrlimit(RLIMIT_CORE) actions, for
dnl disabling core dumping. Only prctl() can be used then, for that purpose.
dnl
dnl strerrordesc_np (glibc only) is preferred over sys_errlist:
dnl https://lists.fedoraproject.org/archives/list/glibc@lists.fedoraproject.org/thread/WJHGG2OO7ABNAYICGA5WQZ2Q34Q2FEHU/
AC_CHECK_FUNCS([\
        posix_fadvise \
        posix_memalign \
        prctl \
        strcasestr \
        strerrordesc_np \
        valloc])

dnl Check for sys_errlist (optional).
AC_CHECK_DECLS([sys_errlist])

dnl Check for libdl/dlopen (optional - only used to test if the library
dnl can be used with libdl).
AC_CHECK_LIB([dl],[dlopen],[have_libdl=yes],[have_libdl=no])
AC_CHECK_HEADERS([dlfcn.h],[have_dlfcn=yes],[have_dlfcn=no])
AM_CONDITIONAL([HAVE_LIBDL],
               [test "x$have_libdl" = "xyes" && test "x$have_dlfcn" = "xyes"])

dnl Does this platform support libc_malloc_debug.so.0 (glibc >= 2.34)?
AC_MSG_CHECKING([if this is glibc >= 2.34])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <limits.h>
#if !defined(__GLIBC__) || __GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 34)
#error "not glibc 2.34"
#endif
        ]])],
   [if (LD_PRELOAD=libc_malloc_debug.so.0 /bin/true) 2>&1 | grep .; then
      is_glibc_234='missing suitable libc_malloc_debug.so.0'
    else
      is_glibc_234=yes
    fi], [is_glibc_234=no]
)
AC_MSG_RESULT([$is_glibc_234])
AM_CONDITIONAL([HAVE_GLIBC_234], [test "x$is_glibc_234" = "xyes"])

dnl Check for GnuTLS (optional, for TLS support).
AC_ARG_WITH([gnutls],
    [AS_HELP_STRING([--without-gnutls],
                    [disable use of gnutls @<:@default=check@:>@])],
    [],
    [with_gnutls=check])
AS_IF([test "$with_gnutls" != "no"],[
    PKG_CHECK_MODULES([GNUTLS], [gnutls >= 3.5.18], [
        printf "gnutls version is "; $PKG_CONFIG --modversion gnutls
        AC_SUBST([GNUTLS_CFLAGS])
        AC_SUBST([GNUTLS_LIBS])
        AC_DEFINE([HAVE_GNUTLS],[1],[gnutls found at compile time.])
    ], [
        AC_MSG_WARN([gnutls not found or < 3.5.18, TLS support will be disabled.])
    ])
])
AM_CONDITIONAL([HAVE_GNUTLS], [test "x$GNUTLS_LIBS" != "x"])

AS_IF([test "$GNUTLS_LIBS" != ""],[
    AC_MSG_CHECKING([for default TLS session priority string])
    AC_ARG_WITH([tls-priority],
        [AS_HELP_STRING([--with-tls-priority=...],
                        [default TLS session priority string @<:@default=NORMAL@:>@])],
        [tls_priority=$withval],
        [tls_priority=NORMAL])
    AC_MSG_RESULT([$tls_priority])
    AC_DEFINE_UNQUOTED([TLS_PRIORITY],["$tls_priority"],
                       [Default TLS session priority string])

    # Check for working <gnutls/socket.h>.  This is only needed for
    # gnutls 3.7.9 and no other version (unfortunately used by Debian
    # 12 and others).  In that release kTLS functionality was added to
    # <gnutls/socket.h> incorrectly, before being moved to
    # <gnutls/gnutls.h> in 3.8.0.  <gnutls/socket.h> breaks on
    # Windows, so we must make inclusion conditional.  When we move
    # the minimum version of gnutls >= 3.8.0 we can get rid of this.
    old_CFLAGS="$CFLAGS"
    CFLAGS="$GNUTLS_CFLAGS $CFLAGS"
    AC_CHECK_HEADERS([gnutls/socket.h])
    CFLAGS="$old_CFLAGS"

    # Check for APIs which may not be present.
    old_LIBS="$LIBS"
    LIBS="$GNUTLS_LIBS $LIBS"
    AC_CHECK_FUNCS([\
        gnutls_transport_is_ktls_enabled \
    ])
    LIBS="$old_LIBS"
])

dnl certtool (part of GnuTLS) for testing TLS with certificates.
dnl macOS has its own program called certtool and packages the
dnl GnuTLS tool as "gnutls-certtool".
AC_CHECK_PROGS([CERTTOOL],[gnutls-certtool certtool],[certtool])
AM_CONDITIONAL([HAVE_CERTTOOL], [test "x$CERTTOOL" != "x"])

dnl psktool (part of GnuTLS) for testing Pre-Shared Keys (PSK).
AC_CHECK_PROG([PSKTOOL], [psktool], [psktool])
AM_CONDITIONAL([HAVE_PSKTOOL], [test "x$PSKTOOL" != "x"])

dnl Check for libxml2 (optional, for NBD URI support)
AC_ARG_WITH([libxml2],
    [AS_HELP_STRING([--without-libxml2],
                    [disable use of libxml2 for URI support @<:@default=check@:>@])],
    [],
    [with_libxml2=check])
AS_IF([test "$with_libxml2" != "no"],[
    PKG_CHECK_MODULES([LIBXML2], [libxml-2.0], [
        printf "libxml2 version is "; $PKG_CONFIG --modversion libxml-2.0
        AC_SUBST([LIBXML2_CFLAGS])
        AC_SUBST([LIBXML2_LIBS])
        AC_DEFINE([HAVE_LIBXML2],[1],[libxml2 found at compile time.])
    ], [
        AC_MSG_WARN([libxml2 not found, NBD URI support will be disabled.])
    ])
])
AM_CONDITIONAL([HAVE_LIBXML2], [test "x$LIBXML2_LIBS" != "x"])

dnl nbdkit and some plugins are only needed to run the test suite.
dnl
dnl List the minimum version and plugins which are used by C code,
dnl because shell script tests can use 'requires' tests.
nbdkit_min_minor="12" ;# 1.12
nbdkit_plugins="file memory null pattern sh"
AC_CHECK_PROG([NBDKIT], [nbdkit], [nbdkit])
AS_IF([test "x$NBDKIT" != "x"], [
    have_nbdkit_features=yes

    AC_MSG_CHECKING([for nbdkit >= 1.$nbdkit_min_minor])
    nbdkit_minor="$(
        $NBDKIT --version | $SED 's/^nbdkit 1\.\(@<:@0-9@:>@*\)\..*/\1/'
    )"
    AS_IF([test $nbdkit_minor -ge $nbdkit_min_minor],[
        AC_MSG_RESULT([yes (1.$nbdkit_minor)])
        printf "nbdkit version is "; $NBDKIT --version
    ],[
        AC_MSG_RESULT([no (1.$nbdkit_minor)])
        AC_MSG_WARN([nbdkit is too old, some tests will be disabled])
        have_nbdkit_features=no
    ])

    for p in $nbdkit_plugins; do
        AC_MSG_CHECKING([for nbdkit $p plugin])
        AS_IF([$NBDKIT $p --version >&AS_MESSAGE_LOG_FD 2>&1], [
            AC_MSG_RESULT([yes])
        ],[
            AC_MSG_RESULT([no])
            AC_MSG_WARN([nbdkit $p plugin is missing, some tests will be disabled])
            have_nbdkit_features=no
        ])
    done

    AC_DEFINE_UNQUOTED([NBDKIT],["$NBDKIT"],
                       [The name of nbdkit server (optional)])

])
AM_CONDITIONAL([HAVE_NBDKIT],
    [test "x$NBDKIT" != "x" && test "x$have_nbdkit_features" = "xyes"])

dnl nbd-server, qemu-nbd and qemu-storage-daemon for interop testing.
AC_CHECK_PROG([NBD_SERVER], [nbd-server], [nbd-server])
AM_CONDITIONAL([HAVE_NBD_SERVER], [test "x$NBD_SERVER" != "x"])
AS_IF([test "x$NBD_SERVER" != "x"], [
  AC_DEFINE_UNQUOTED([NBD_SERVER],["$NBD_SERVER"],
                     [The name of nbd-server (optional)])
   printf "nbd-server version is "; $NBD_SERVER --version | head -1
])
AC_CHECK_PROG([QEMU_NBD], [qemu-nbd], [qemu-nbd])
AM_CONDITIONAL([HAVE_QEMU_NBD], [test "x$QEMU_NBD" != "x"])
AS_IF([test "x$QEMU_NBD" != "x"], [
  AC_DEFINE_UNQUOTED([QEMU_NBD],["$QEMU_NBD"],
                     [The name of qemu-nbd (optional)])
   printf "qemu-nbd version is "; $QEMU_NBD --version | head -1
])
AC_CHECK_PROG([QEMU_STORAGE_DAEMON],
               [qemu-storage-daemon], [qemu-storage-daemon])
AM_CONDITIONAL([HAVE_QEMU_STORAGE_DAEMON],
               [test "x$QEMU_STORAGE_DAEMON" != "x"])

dnl glib2 main loop for examples that interoperate with the glib main loop.
PKG_CHECK_MODULES([GLIB], [glib-2.0], [
    printf "glib2 version is "; $PKG_CONFIG --modversion glib-2.0
    AC_SUBST([GLIB_CFLAGS])
    AC_SUBST([GLIB_LIBS])
],[
    AC_MSG_WARN([glib2 not found, some examples will not be compiled])
])
AM_CONDITIONAL([HAVE_GLIB], [test "x$GLIB_LIBS" != "x"])

dnl libev support for examples that interoperate with libev event loop.
PKG_CHECK_MODULES([LIBEV], [libev], [
    printf "libev version is "; $PKG_CONFIG --modversion libev
    AC_SUBST([LIBEV_CFLAGS])
    AC_SUBST([LIBEV_LIBS])
],[
    dnl no pkg-config for libev, searching manually:
    AC_CHECK_HEADERS([ev.h], [
        AC_CHECK_LIB([ev], [ev_time], [
            AC_SUBST([LIBEV_LIBS], ["-lev"])
        ],
        [
            AC_MSG_WARN([libev not found, some examples will not be compiled])
        ])
    ],[
        AC_MSG_WARN([ev.h not found, some examples will not be compiled])
    ])
])

AS_IF([test "x$LIBEV_LIBS" != "x"], [
    old_CFLAGS="$CFLAGS"
    CFLAGS="$CFLAGS $LIBEV_CFLAGS -Werror=strict-aliasing -O2"
    AC_MSG_CHECKING([if the compiler is new enough for good aliasing rules])
    AC_COMPILE_IFELSE([
        AC_LANG_PROGRAM([
            #include <ev.h>

            static void cb (struct ev_loop *l, ev_timer *t, int e) { }
            static ev_timer timer;
        ], [
            ev_timer_init (&timer, cb, 0, .1);
        ])
    ], [
        AC_MSG_RESULT([yes])
    ], [
        AC_MSG_RESULT([no])
        LIBEV_CFLAGS="$LIBEV_CFLAGS -Wno-strict-aliasing"
    ])
    CFLAGS="$old_CFLAGS"
])
AM_CONDITIONAL([HAVE_LIBEV], [test "x$LIBEV_LIBS" != "x"])

dnl FUSE 3 is optional to build the nbdfuse program.
AC_ARG_ENABLE([fuse],
    AS_HELP_STRING([--disable-fuse], [disable FUSE (nbdfuse) support]),
    [],
    [enable_fuse=yes])
AS_IF([test "x$enable_fuse" != "xno"],[
    PKG_CHECK_MODULES([FUSE],[fuse3],[
        printf "fuse3 version is "; $PKG_CONFIG --modversion fuse3
        AC_SUBST([FUSE_CFLAGS])
        AC_SUBST([FUSE_LIBS])
        AC_DEFINE([HAVE_FUSE],[1],[Define to 1 if you have FUSE.])
    ],[
        enable_fuse=no
        AC_MSG_WARN([FUSE 3 library and headers are missing, so optional nbdfuse program won't be built])
    ])
])
AM_CONDITIONAL([HAVE_FUSE],[test "x$enable_fuse" != "xno"])

dnl libublksrv is optional to build the nbdublk program.
AC_ARG_ENABLE([ublk],
    AS_HELP_STRING([--disable-ublk], [disable ublk (nbdublk) support]),
    [],
    [enable_ublk=yes])
AS_IF([test "x$enable_ublk" != "xno"],[
    PKG_CHECK_MODULES([UBLKSRV],[ublksrv],[
        printf "ublksrv version is "; $PKG_CONFIG --modversion ublksrv
        AC_SUBST([UBLKSRV_CFLAGS])
        AC_SUBST([UBLKSRV_LIBS])
        AC_DEFINE([HAVE_UBLK],[1],[Define to 1 if you have ublk.])
    ],[
        enable_ublk=no
        AC_MSG_WARN([libublksrv (ublk server) library and headers are missing, so optional nbdublk program won't be built])
    ])
])
AM_CONDITIONAL([HAVE_UBLK],[test "x$enable_ublk" != "xno"])

dnl Check we have enough to run podwrapper.
AC_CHECK_PROG([PERL],[perl],[perl],[no])
AS_IF([test "x$PERL" != "xno"],[
    AC_MSG_CHECKING([if we have perl Pod::Man and Pod::Simple])
    AS_IF([$PERL -MPod::Man -MPod::Simple -e 1 >&AS_MESSAGE_LOG_FD 2>&1],[
        enable_pod=yes
    ],[
        enable_pod=no
    ])
    AC_MSG_RESULT([$enable_pod])
])
AM_CONDITIONAL([HAVE_POD],
               [test "x$PERL" != "xno" && test "x$enable_pod" = "xyes"])

dnl Define the path to the podwrapper program.
PODWRAPPER="$PERL $(pwd)/podwrapper.pl"
AC_SUBST([PODWRAPPER])

dnl Build the libFuzzer test.  See fuzzing/README.
AC_ARG_ENABLE([libfuzzer],
    [AS_HELP_STRING([--enable-libfuzzer],
                    [build the libFuzzer test (for developers)])],
    [],
    [enable_libfuzzer=no])
AM_CONDITIONAL([ENABLE_LIBFUZZER],[test "x$enable_libfuzzer" = "xyes"])

dnl Bash completion.
AC_ARG_WITH([bash-completions],
    [AS_HELP_STRING([--without-bash-completions], [disable installing bash completions @<:@default=check@:>@])])

AS_IF([test "x$with_bash_completions" != xno], [
    PKG_CHECK_MODULES([BASH_COMPLETION], [bash-completion >= 2.0], [
        bash_completion=yes
        AC_MSG_CHECKING([for bash-completions directory])
        m4_ifdef([PKG_CHECK_VAR],[
            PKG_CHECK_VAR(bashcompdir, [bash-completion], [completionsdir])
        ])
        AS_IF([test -z "$bashcompdir"], [
            bashcompdir="${sysconfdir}/bash_completion.d"
        ])
        AC_MSG_RESULT([$bashcompdir])
        AC_SUBST([bashcompdir])
    ],[
        bash_completion=no
        AC_MSG_WARN([bash-completion not installed])
    ])
])

AS_IF([test "x$bash_completion" = xno && test "x$with_bash_completions" = xyes], [
    AC_MSG_ERROR([bash-completions requested but required packages not found])
])

AM_CONDITIONAL([HAVE_BASH_COMPLETION],[test "x$bash_completion" = "xyes"])

dnl OCaml >= 4.05, required for running the generator when building
dnl from git, or optional unless you want to build the OCaml bindings.
AC_PROG_OCAML
AC_PROG_FINDLIB

dnl Flags we want to pass to every OCaml compiler call.
OCAML_WARN_ERROR="-warn-error +C+D+E+F+L+M+P+S+U+V+Y+Z+X+52-3"
AC_SUBST([OCAML_WARN_ERROR])
OCAML_FLAGS="-g -annot -safe-string"
AC_SUBST([OCAML_FLAGS])

dnl Note this only disables the OCaml bindings.
AC_ARG_ENABLE([ocaml],
    AS_HELP_STRING([--disable-ocaml], [disable OCaml language bindings]),
    [],
    [enable_ocaml=yes])
AM_CONDITIONAL([HAVE_OCAML],
    [test "x$OCAMLC" != "xno" && test "x$OCAMLFIND" != "xno" && \
     test "x$enable_ocaml" = "xyes"])
AM_CONDITIONAL([HAVE_OCAMLOPT],
    [test "x$OCAMLOPT" != "xno" && test "x$OCAMLFIND" != "xno" && \
     test "x$enable_ocaml" = "xyes"])
AM_CONDITIONAL([HAVE_OCAMLDOC],
    [test "x$OCAMLDOC" != "xno" && test "x$enable_ocaml" = "xyes"])

dnl HAVE_OCAMLC can be used to detect if we are able to run the
dnl generator.  We should NOT require ocamlfind for this.
AM_CONDITIONAL([HAVE_OCAMLC], [test "x$OCAMLC" != "xno"])

dnl OCaml if present should be >= 4.05
AS_IF([test "x$OCAMLC" != "xno" && test "x$OCAMLFIND" != "xno" && \
       test "x$enable_ocaml" = "xyes"],[
    ocaml_ver_str=4.05
    ocaml_min_major=4
    ocaml_min_minor=5
    AC_MSG_CHECKING([if OCaml version >= $ocaml_ver_str])
    ocaml_major="`echo $OCAMLVERSION | $AWK -F. '{print $1}'`"
    ocaml_minor="`echo $OCAMLVERSION | $AWK -F. '{print $2}' | $SED 's/^0//'`"
    AS_IF([test "$ocaml_major" -ge $((ocaml_min_major+1)) || ( test "$ocaml_major" -eq $ocaml_min_major && test "$ocaml_minor" -ge $ocaml_min_minor )],[
        AC_MSG_RESULT([yes ($ocaml_major, $ocaml_minor)])
    ],[
        AC_MSG_RESULT([no])
        AC_MSG_FAILURE([OCaml compiler is not new enough.  At least OCaml $ocaml_ver_str is required])
    ])

    dnl Which packages do we need to link to OCaml code?
    dnl In OCaml >= 5, bigarray is part of the stdlib.
    AC_MSG_CHECKING([what OCaml packages need to be linked in])
    AS_IF([test "$ocaml_major" -ge 5],[
        OCAMLFIND_PACKAGES="unix"
    ],[
        OCAMLFIND_PACKAGES="unix,bigarray"
    ])
    AC_MSG_RESULT([$OCAMLFIND_PACKAGES])
    AC_SUBST([OCAMLFIND_PACKAGES])
])

dnl Check if OCaml has caml_alloc_custom_mem (added in OCaml 4.08).
AS_IF([test "x$OCAMLC" != "xno" && test "x$OCAMLFIND" != "xno" && \
       test "x$enable_ocaml" = "xyes"],[
    AC_MSG_CHECKING([for caml_alloc_custom_mem])
    cat >conftest.c <<'EOF'
#include <caml/custom.h>
int main () { char *p = (void *)caml_alloc_custom_mem; return 0; }
EOF
    AS_IF([$OCAMLC conftest.c >&AS_MESSAGE_LOG_FD 2>&1],[
        AC_MSG_RESULT([yes])
        AC_DEFINE([HAVE_CAML_ALLOC_CUSTOM_MEM],[1],
                  [caml_alloc_custom_mem found at compile time.])
    ],[
        AC_MSG_RESULT([no])
    ])
    rm -f conftest.c conftest.o
])

dnl Check if OCaml has caml_alloc_initialized_string (added in OCaml 4.06).
AS_IF([test "x$OCAMLC" != "xno" && test "x$OCAMLFIND" != "xno" && \
       test "x$enable_ocaml" = "xyes"],[
    AC_MSG_CHECKING([for caml_alloc_initialized_string])
    cat >conftest.c <<'EOF'
#include <caml/alloc.h>
int main () { char *p = (void *)caml_alloc_initialized_string; return 0; }
EOF
    AS_IF([$OCAMLC conftest.c >&AS_MESSAGE_LOG_FD 2>&1],[
        AC_MSG_RESULT([yes])
        AC_DEFINE([HAVE_CAML_ALLOC_INITIALIZED_STRING],[1],
                  [caml_alloc_initialized_string found at compile time.])
    ],[
        AC_MSG_RESULT([no])
    ])
    rm -f conftest.c conftest.o
])

dnl Check if OCaml has caml_unix_get_sockaddr
dnl (added in OCaml 4.14, previously called 'get_sockaddr').
AS_IF([test "x$OCAMLC" != "xno" && test "x$OCAMLFIND" != "xno" && \
       test "x$enable_ocaml" = "xyes"],[
    AC_MSG_CHECKING([for caml_unix_get_sockaddr])
    cat >conftest.c <<'EOF'
#include <caml/mlvalues.h>
#include <caml/socketaddr.h>
int main () { char *p = (void *)caml_unix_get_sockaddr; return 0; }
EOF
    AS_IF([$OCAMLC conftest.c >&AS_MESSAGE_LOG_FD 2>&1],[
        AC_MSG_RESULT([yes])
        AC_DEFINE([HAVE_CAML_UNIX_GET_SOCKADDR],[1],
                  [caml_unix_get_sockaddr found at compile time.])
    ],[
        AC_MSG_RESULT([no])
    ])
    rm -f conftest.c conftest.o
])

dnl Python, optional for the Python bindings and shell.
PYTHON_PREFIX=
PYTHON_VERSION=
PYTHON_INSTALLDIR=

AC_ARG_ENABLE([python],
    AS_HELP_STRING([--disable-python], [disable Python 3 language bindings]),
    [],
    [enable_python=yes])
AS_IF([test "x$enable_python" != "xno"],[
    AC_CHECK_PROG([PYTHON],[python3],[python3],[no])

    if test "x$PYTHON" != "xno"; then
        AC_MSG_CHECKING([Python version])
        PYTHON_VERSION_MAJOR=`$PYTHON -c "import sys; print (sys.version_info@<:@0@:>@)"`
        PYTHON_VERSION_MINOR=`$PYTHON -c "import sys; print (sys.version_info@<:@1@:>@)"`
        PYTHON_VERSION="$PYTHON_VERSION_MAJOR.$PYTHON_VERSION_MINOR"
        AC_MSG_RESULT([$PYTHON_VERSION])

        # Debian: python-3.2.pc
        PKG_CHECK_MODULES([PYTHON], [python-"$PYTHON_VERSION"],[
            have_python_module=1
            AC_SUBST([PYTHON_CFLAGS])
            AC_SUBST([PYTHON_LIBS])
            AC_SUBST([PYTHON_VERSION])
            AC_DEFINE([HAVE_PYTHON],[1],[Python library found at compile time])
        ],[
            PKG_CHECK_MODULES([PYTHON], [python],[
                have_python_module=1
                AC_SUBST([PYTHON_CFLAGS])
                AC_SUBST([PYTHON_LIBS])
                AC_SUBST([PYTHON_VERSION])
                AC_DEFINE([HAVE_PYTHON],[1],[Python library found at compile time])
            ],[
                AC_MSG_WARN([python $PYTHON_VERSION not found])
            ])
        ])

        AC_MSG_CHECKING([Python prefix])
        PYTHON_PREFIX=`$PYTHON -c "import sys; print (sys.prefix)"`
        AC_MSG_RESULT([$PYTHON_PREFIX])

        AC_ARG_WITH([python-installdir],
                    [AS_HELP_STRING([--with-python-installdir=...],
                        [directory to install python modules @<:@default=check@:>@])],
                        [PYTHON_INSTALLDIR="$withval"
                         AC_MSG_NOTICE([Python install dir $PYTHON_INSTALLDIR])],
                        [PYTHON_INSTALLDIR=check])

        if test "x$PYTHON_INSTALLDIR" = "xcheck"; then
            PYTHON_INSTALLDIR=
            AC_MSG_CHECKING([for Python site-packages path])
            if test -z "$PYTHON_INSTALLDIR"; then
                PYTHON_INSTALLDIR=`$PYTHON -c "import sysconfig; \
                                               print (sysconfig.get_path('platlib'));"`
            fi
            AC_MSG_RESULT([$PYTHON_INSTALLDIR])
        fi

        AC_MSG_CHECKING([for Python extension suffix (PEP-3149)])
        if test -z "$PYTHON_EXT_SUFFIX"; then
            python_ext_suffix=`$PYTHON -c "import sysconfig; \
                                         print (sysconfig.get_config_var('EXT_SUFFIX') or sysconfig.get_config_var('SO'))"`
            PYTHON_EXT_SUFFIX=$python_ext_suffix
        fi
        AC_MSG_RESULT([$PYTHON_EXT_SUFFIX])
    fi

    AC_SUBST(PYTHON_PREFIX)
    AC_SUBST(PYTHON_VERSION)
    AC_SUBST(PYTHON_INSTALLDIR)
    AC_SUBST(PYTHON_EXT_SUFFIX)
])
AM_CONDITIONAL([HAVE_PYTHON],
    [test "x$PYTHON" != "xno" && test "x$have_python_module" = "x1" ])

dnl Use --enable-python-code-style to enable flake8 tests.
AC_ARG_ENABLE([python-code-style],
    [AS_HELP_STRING([--enable-python-code-style],
                    [enable Python code style (flake8) checks (for developers)])],
    [enable_pycodestyle=$enableval],
    [enable_pycodestyle=no])
AM_CONDITIONAL([ENABLE_PYCODESTYLE],[test "x$enable_pycodestyle" = "xyes"])

dnl Golang.
AC_ARG_ENABLE([golang],
    AS_HELP_STRING([--disable-golang], [disable Go language bindings]),
        [],
        [enable_golang=yes])
AS_IF([test "x$enable_golang" != "xno"],[
    AC_CHECK_PROG([GOLANG],[go],[go],[no])
    AC_CHECK_PROG([GOFMT],[gofmt],[gofmt],[no])
    AS_IF([test "x$GOLANG" != "xno"],[
        $GOLANG version ||:

        AC_MSG_CHECKING([for $GOLANG major version])
        [GOLANG_MAJOR_VERSION=`$GOLANG version | $SED 's/.*go\([[:digit:]]*\).*/\1/g'`]
        AC_MSG_RESULT([$GOLANG_MAJOR_VERSION])
        AC_SUBST([GOLANG_MAJOR_VERSION])
        AC_MSG_CHECKING([for $GOLANG minor version])
        [GOLANG_MINOR_VERSION=`$GOLANG version | $SED 's/.*go[[:digit:]]*\.\([[:digit:]]*\).*/\1/g'`]
        AC_MSG_RESULT([$GOLANG_MINOR_VERSION])
        AC_SUBST([GOLANG_MINOR_VERSION])

        AC_MSG_CHECKING([if $GOLANG is usable])
        AS_IF([ (
               cd $srcdir/golang/configure &&
               $GOLANG run . 2>&AS_MESSAGE_LOG_FD 1>&2 &&
               $GOLANG mod tidy 2>&AS_MESSAGE_LOG_FD 1>&2
               ) ],[
            AC_MSG_RESULT([yes])
        ],[
            AC_MSG_RESULT([no])
            AC_MSG_WARN([golang ($GOLANG) is installed but not usable])
            GOLANG=no
        ])
    ])
],[GOLANG=no])
AM_CONDITIONAL([HAVE_GOLANG],[test "x$GOLANG" != "xno"])

dnl Rust.
AC_ARG_ENABLE([rust],
    AS_HELP_STRING([--disable-rust], [disable Rust language bindings]),
        [],
        [enable_rust=yes])
AS_IF([test "x$enable_rust" != "xno"],[
    AC_CHECK_PROG([CARGO],[cargo],[cargo],[no])
    AS_IF([test "x$CARGO" != "xno"],[
        printf "cargo version is "; $CARGO --version
    ])
    AC_CHECK_PROG([RUSTFMT],[rustfmt],[rustfmt],[no])
    AS_IF([test "x$RUSTFMT" != "xno"],[
        printf "rustfmt version is "; $RUSTFMT --version
    ])
    AS_IF([test "x$CARGO" != "xno"],[
        AC_MSG_CHECKING([if $CARGO is usable])
        AS_IF([ (
              cd $srcdir/rust/cargo_test &&
               $CARGO test 2>&AS_MESSAGE_LOG_FD 1>&2 &&
               $CARGO doc 2>&AS_MESSAGE_LOG_FD 1>&2
               ) ],[
            AC_MSG_RESULT([yes])
        ],[
            AC_MSG_RESULT([no])
            AC_MSG_WARN([Rust ($CARGO) is installed but not usable])
            CARGO=no
        ])
    ])
],[CARGO=no])
AM_CONDITIONAL([HAVE_RUST],[test "x$CARGO" != "xno"])

AC_MSG_CHECKING([for how to mark DSO non-deletable at runtime])
NODELETE=
`$LD --help 2>&1 | grep -- "-z nodelete" >/dev/null` && \
    NODELETE="-Wl,-z -Wl,nodelete"
AC_MSG_RESULT([$NODELETE])
AC_SUBST([NODELETE])

AC_MSG_CHECKING([for how to set DSO symbol versions])
case $host_os in
  darwin*)
  VERSION_SCRIPT=
  ;;
  *)
  VERSION_SCRIPT="-Wl,--version-script=${srcdir}/libnbd.syms"
  ;;
esac
AC_MSG_RESULT([$VERSION_SCRIPT])
AC_SUBST([VERSION_SCRIPT])

dnl Produce output files.
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([podwrapper.pl],
                [chmod +x,-w podwrapper.pl])
AC_CONFIG_FILES([python/run-python-tests],
                [chmod +x,-w python/run-python-tests])
AC_CONFIG_FILES([run],
                [chmod +x,-w run])
AC_CONFIG_FILES([rust/run-tests.sh],
                [chmod +x,-w rust/run-tests.sh])
AC_CONFIG_FILES([sh/nbdsh],
                [chmod +x,-w sh/nbdsh])

AC_CONFIG_FILES([Makefile
                 bash-completion/Makefile
                 common/include/Makefile
                 common/utils/Makefile
                 copy/Makefile
                 docs/Makefile
                 dump/Makefile
                 examples/Makefile
                 fuse/Makefile
                 fuzzing/Makefile
                 generator/Makefile
                 generator/config.ml
                 golang/Makefile
                 golang/examples/Makefile
                 include/Makefile
                 info/Makefile
                 interop/Makefile
                 interop/nbd-server.conf
                 interop/nbd-server-tls.conf
                 lib/Makefile
                 lib/libnbd.pc
                 lib/local/libnbd.pc
                 ocaml/Makefile
                 ocaml/META
                 ocaml/examples/Makefile
                 ocaml/tests/Makefile
                 ocaml/tests/ocaml_test_config.ml
                 python/Makefile
                 rust/Makefile
                 sh/Makefile
                 tests/Makefile
                 tests/functions.sh
                 ublk/Makefile
                 valgrind/Makefile])

AC_OUTPUT

dnl Summary.
echo
echo
echo "----------------------------------------------------------------------"
echo "Thank you for downloading $PACKAGE_STRING"
echo
echo "This is how we have configured the optional components for you today:"
echo

print ()
{
    printf '    %.40s %s\n' \
        "$1 ........................................" "$2"
}

feature ()
{
    feat="$1"
    shift
    if "$@"; then
        print "$feat" "yes"
    else
        print "$feat" "no"
    fi
}

echo "Optional library features:"
echo
feature "TLS support"           test "x$HAVE_GNUTLS_TRUE" = "x"
feature "NBD URI support"       test "x$HAVE_LIBXML2_TRUE" = "x"
feature "AF_VSOCK support"      test "x$ac_cv_type_struct_sockaddr_vm" = "xyes"
feature "FUSE support"          test "x$HAVE_FUSE_TRUE" = "x"
feature "ublk support"          test "x$HAVE_UBLK_TRUE" = "x"
feature "Manual pages"          test "x$HAVE_POD_TRUE" = "x"
feature "Bash tab completion"   test "x$HAVE_BASH_COMPLETION_TRUE" = "x"

echo
echo "Language bindings:"
echo
feature "Go"                    test "x$HAVE_GOLANG_TRUE" = "x"
feature "OCaml"                 test "x$HAVE_OCAML_TRUE" = "x"
feature "Python"                test "x$HAVE_PYTHON_TRUE" = "x"
feature "Rust"                  test "x$HAVE_RUST_TRUE" = "x"

echo
echo "Server interoperability testing:"
echo
feature "qemu-nbd"              test "x$HAVE_QEMU_NBD_TRUE" = "x"
feature "qemu-storage-daemon"   test "x$HAVE_QEMU_STORAGE_DAEMON_TRUE" = "x"
feature "nbdkit"                test "x$HAVE_NBDKIT_TRUE" = "x"
feature "nbd-server"            test "x$HAVE_NBD_SERVER_TRUE" = "x"

echo
echo "Examples:"
echo
feature "glib example"          test "x$GLIB_LIBS" != "x"
feature "libev example"         test "x$LIBEV_LIBS" != "x"

echo
echo "If any optional component is configured ‘no’ when you expected ‘yes’"
echo "then you should check the preceding messages and README."
echo
echo "Please report bugs back to the mailing list:"
echo "https://lists.libguestfs.org"
echo
echo "Next you should type 'make' to build the package,"
echo "then 'make check' to run the tests."
