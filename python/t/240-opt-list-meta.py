# libnbd Python bindings
# Copyright Red Hat
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

import nbd
import os

nbdkit = os.getenv("NBDKIT", "nbdkit")

count = 0
seen = False


def f(user_data, name):
    global count
    global seen
    assert user_data == 42
    count = count + 1
    if name == nbd.CONTEXT_BASE_ALLOCATION:
        seen = True


def must_fail(f, *args, **kwds):
    try:
        f(*args, **kwds)
        assert False
    except nbd.Error:
        pass


# Get into negotiating state.
h = nbd.NBD()
h.set_opt_mode(True)
h.connect_command([nbdkit, "-s", "--exit-with-parent", "-v",
                   "memory", "size=1M"])

# First pass: empty query should give at least "base:allocation".
count = 0
seen = False
r = h.opt_list_meta_context(lambda *args: f(42, *args))
assert r == count
assert r >= 1
assert seen is True
max = count

# Second pass: bogus query has no response.
count = 0
seen = False
h.add_meta_context("x-nosuch:")
r = h.opt_list_meta_context(lambda *args: f(42, *args))
assert r == 0
assert r == count
assert seen is False

# Third pass: specific query should have one match.
count = 0
seen = False
h.add_meta_context(nbd.CONTEXT_BASE_ALLOCATION)
assert h.get_nr_meta_contexts() == 2
assert h.get_meta_context(1) == nbd.CONTEXT_BASE_ALLOCATION
r = h.opt_list_meta_context(lambda *args: f(42, *args))
assert r == 1
assert count == 1
assert seen is True

# Fourth pass: opt_list_meta_context is stateless, so it should
# not wipe status learned during opt_info
count = 0
seen = False
must_fail(h.can_meta_context, nbd.CONTEXT_BASE_ALLOCATION)
must_fail(h.get_size)
h.opt_info()
assert h.get_size() == 1024*1024
assert h.can_meta_context(nbd.CONTEXT_BASE_ALLOCATION) is True
h.clear_meta_contexts()
h.add_meta_context("x-nosuch:")
r = h.opt_list_meta_context(lambda *args: f(42, *args))
assert r == 0
assert count == 0
assert seen is False
assert h.get_size() == 1048576
assert h.can_meta_context(nbd.CONTEXT_BASE_ALLOCATION) is True

# Final pass: "base:" query should get at least "base:allocation"
count = 0
seen = False
h.add_meta_context("base:")
r = h.opt_list_meta_context(lambda *args: f(42, *args))
assert r >= 1
assert r <= max
assert r == count
assert seen is True

h.opt_abort()

# Repeat but this time without structured replies. Deal gracefully
# with older servers that don't allow the attempt.
h = nbd.NBD()
h.set_opt_mode(True)
h.set_request_structured_replies(False)
h.connect_command([nbdkit, "-s", "--exit-with-parent", "-v",
                   "memory", "size=1M"])
bytes = h.stats_bytes_sent()

try:
    count = 0
    seen = False
    r = h.opt_list_meta_context(lambda *args: f(42, *args))
    assert r == count
    assert r >= 1
    assert seen is True
except nbd.Error as ex:
    assert h.stats_bytes_sent() > bytes
    print("ignoring failure from old server: %s" % ex.string)

# Now enable structured replies, and a retry should pass.
r = h.opt_structured_reply()
assert r is True

count = 0
seen = False
r = h.opt_list_meta_context(lambda *args: f(42, *args))
assert r == count
assert r >= 1
assert seen is True

h.opt_abort()
