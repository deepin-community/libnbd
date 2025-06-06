#!/usr/bin/env bash
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

# Test zeroing of files using nbdkit sh plugin.

. ../tests/functions.sh

set -e
set -x

# FALLOC_FL_ZERO_RANGE support was added to fuse.ko in Linux 5.14.
requires_linux_kernel_version 5.14

requires_fuse
requires fallocate --version
requires $DD --version
requires $NBDKIT --version
requires $NBDKIT sh --version

# Difficult to arrange for this test to be run this test under
# valgrind, so don't bother.
if [ "x$LIBNBD_VALGRIND" = "x1" ]; then
    echo "$0: test skipped under valgrind"
    exit 77
fi

mp=test-zero.d
pidfile=test-zero.pid
out=test-zero.out
cleanup_fn fusermount3 -u $mp
cleanup_fn rm -rf $mp $pidfile $out

mkdir $mp

export DD mp pidfile out prog="$0"
$NBDKIT -U - \
        sh - \
        --run '
set -x

# Run nbdfuse and connect to the nbdkit server.
nbdfuse -P $pidfile $mp --unix $unixsocket &

# Wait for the pidfile to appear.
for i in {1..60}; do
    if test -f $pidfile; then
        break
    fi
    sleep 1
done
if ! test -f $pidfile; then
    echo "$prog: nbdfuse PID file $pidfile was not created"
    exit 1
fi

ls -al $mp

# Fully allocate the disk.
$DD if=/dev/zero of=$mp/nbd bs=512 count=2

# Explicitly zero the second sector.
fallocate -z -l 512 -o 512 $mp/nbd

# We have to explicitly unmount it here else nbdfuse will
# never exit and nbdkit will hang.
fusermount3 -u $mp
' <<'EOF'
# The nbdkit server script.
case "$1" in
  get_size) echo 1024 ;;
  can_write) ;;
  can_trim) ;;
  can_zero) ;;
  can_fast_zero) ;;
  pread) ;;
  pwrite) echo "$@" >> $out ;;
  trim) echo "$@" >> $out ;;
  zero) echo "$@" >> $out ;;
  *) exit 2 ;;
esac
EOF

# What commands were sent to nbdkit?
cat $out

grep 'pwrite  512 0' $out
grep 'pwrite  512 512' $out
grep 'zero  512 512' $out
