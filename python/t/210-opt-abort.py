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
import errno
import os

nbdkit = os.getenv("NBDKIT", "nbdkit")

h = nbd.NBD()
h.set_opt_mode(True)
h.connect_command([nbdkit, "-s", "--exit-with-parent", "-v", "null"])
assert h.get_protocol() == "newstyle-fixed"
assert h.get_structured_replies_negotiated()
h.opt_abort()
assert h.aio_is_closed()
try:
    h.get_size()
    assert False
except nbd.Error as ex:
    assert ex.errnum == errno.EINVAL
