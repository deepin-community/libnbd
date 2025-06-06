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

# Set by functions.sh and used in the tests.
export abs_top_srcdir

requires nbdkit --version

# The -count=1 parameter is the "idiomatic way to bypass test caching".
# https://golang.org/doc/go1.10#test
# The -v option enables verbose output.
$GOLANG test -count=1 -v

# Run the only benchmarks with 10 milliseconds timeout to make sure they
# do not break by mistake, without overloading the CI. For performance
# testing run "go test" directly.
# The -run=- parameter is the way to match no test, assuming that no test or
# sub test contains "-" in the name.
$GOLANG test -run=- -bench=. -benchtime=10ms
