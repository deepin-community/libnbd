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

. ../tests/functions.sh

set -e
set -x

requires_root
requires_caps cap_sys_admin
requires $NBDKIT --exit-with-parent --version
requires test -r /sys/module/nbd
requires nbd-client --version
# /dev/nbd0 must not be in use.
requires_not nbd-client -c /dev/nbd0

pidfile=copy-nbd-to-block.pid
sock=$(mktemp -u /tmp/libnbd-test-copy.XXXXXX)
cleanup_fn rm -f $pidfile $sock
cleanup_fn nbd-client -d /dev/nbd0

# Run an nbdkit server to act as the backing for /dev/nbd0.
$NBDKIT --exit-with-parent -f -v -P $pidfile -U $sock memory 5M &
wait_for_pidfile $NBDKIT $pidfile

nbd-client -unix $sock /dev/nbd0 -b 512

$VG nbdcopy -v -- [ $NBDKIT --exit-with-parent -v pattern size=5M ] /dev/nbd0

# Check that /dev/nbd0 is still a block device and we didn't
# accidentally overwrite it with a regular file.
test -b /dev/nbd0
