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

requires $NBDKIT --version
requires $NBDKIT -U - null --run 'test "$uri" != ""'
requires $TR --version

out=info-base-allocation.out
cleanup_fn rm -f $out
rm -f $out

# The sparse allocator used by nbdkit-data-plugin uses a 32K page
# size, and extents are always aligned with this.
$NBDKIT -U - data data='1 @131072 2' size=1M \
        --run '$VG nbdinfo --map "$uri"' > $out

cat $out

if [ "$($TR -s ' ' < $out)" != " 0 32768 0 data
 32768 98304 3 hole,zero
 131072 32768 0 data
 163840 884736 3 hole,zero" ]; then
    echo "$0: unexpected output from nbdinfo --map"
    exit 1
fi
