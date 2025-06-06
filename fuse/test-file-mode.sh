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

# Test nbdfuse file mode (added in nbdkit 1.24).

. ../tests/functions.sh

set -e
set -x

requires_fuse
requires $NBDKIT --exit-with-parent --version
requires $CMP --version
requires $DD --version

if ! test -r /dev/urandom; then
    echo "$0: test skipped: /dev/urandom not readable"
    exit 77
fi

pidfile=test-file-mode.pid
mp=test-file-mode
data=test-file-mode.data
cleanup_fn fusermount3 -u $mp
cleanup_fn rm -f $pidfile $mp $data

touch $mp
$VG nbdfuse -P $pidfile $mp \
            --command $NBDKIT -s --exit-with-parent memory size=10M &
wait_for_pidfile nbdfuse $pidfile

$DD if=/dev/urandom of=$data bs=1M count=10
# Use a weird block size when writing.  It's a bit pointless because
# something in the Linux/FUSE stack turns these into exact 4096 byte
# writes.
$DD if=$data of=$mp bs=65519 conv=nocreat,notrunc
$CMP $data $mp
