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

# Test --is read-only and --can write.

requires $NBDKIT --version
requires $NBDKIT null --version

$NBDKIT -U - -r null \
        --run '$VG nbdinfo --is read-only "nbd+unix:///?socket=$unixsocket"'
$NBDKIT -U - -r null \
        --run '! $VG nbdinfo --can write "nbd+unix:///?socket=$unixsocket"'
$NBDKIT -U - null \
        --run '$VG nbdinfo --can write "nbd+unix:///?socket=$unixsocket"'
$NBDKIT -U - null \
        --run '! $VG nbdinfo --is read-only "nbd+unix:///?socket=$unixsocket"'
