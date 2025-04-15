# NBD client library in userspace
# WARNING: THIS FILE IS GENERATED FROM
# generator/generator
# ANY CHANGES YOU MAKE TO THIS FILE WILL BE LOST.
#
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

'''
Python bindings for libnbd

import nbd
h = nbd.NBD()
h.connect_tcp("localhost", "nbd")
buf = h.pread(512, 0)

Read the libnbd(3) man page to find out how to use the API.
'''

import contextlib
import libnbdmod

# Re-export Error exception as nbd.Error, adding some methods.
from libnbdmod import Error

Error.__doc__ = '''
Exception thrown when the underlying libnbd call fails.

This exception has three properties to query the error.  Use
the .string property to return a printable string containing
the error message.  Use the .errnum property for the associated
numeric error value (which may be 0 if the error did not
correspond to a system call failure), or the .errno property to
return a string containing the Python errno name if one is known
(which may be None if the numeric value does not correspond to
a known errno name).
'''

Error.string = property(lambda self: self.args[0])


def _errno(self):
    import errno
    try:
        return errno.errorcode[self.args[1]]
    except KeyError:
        return None


Error.errno = property(_errno)

Error.errnum = property(lambda self: self.args[1])


def _str(self):
    if self.errno:
        return "%s (%s)" % (self.string, self.errno)
    else:
        return "%s" % self.string


Error.__str__ = _str


class ClosedHandle(ValueError):
    '''This exception is thrown when any method is called on an
    nbd handle after you have called h.close() on the same handle.'''
    pass


@contextlib.contextmanager
def nbd():
    '''
    This is a context manager function.  Python will close the handle
    automatically even if the body throws an exception:

    with nbd.nbd() as h:
        # use the handle 'h'
    '''
    h = NBD()
    yield h
    h.close()


TLS_DISABLE = 0
TLS_ALLOW = 1
TLS_REQUIRE = 2

SIZE_MINIMUM = 0
SIZE_PREFERRED = 1
SIZE_MAXIMUM = 2
SIZE_PAYLOAD = 3

CMD_FLAG_FUA = 0x01
CMD_FLAG_NO_HOLE = 0x02
CMD_FLAG_DF = 0x04
CMD_FLAG_REQ_ONE = 0x08
CMD_FLAG_FAST_ZERO = 0x10
CMD_FLAG_PAYLOAD_LEN = 0x20
CMD_FLAG_MASK = 0x3f

HANDSHAKE_FLAG_FIXED_NEWSTYLE = 0x01
HANDSHAKE_FLAG_NO_ZEROES = 0x02
HANDSHAKE_FLAG_MASK = 0x03

STRICT_COMMANDS = 0x01
STRICT_FLAGS = 0x02
STRICT_BOUNDS = 0x04
STRICT_ZERO_SIZE = 0x08
STRICT_ALIGN = 0x10
STRICT_PAYLOAD = 0x20
STRICT_AUTO_FLAG = 0x40
STRICT_MASK = 0x7f

ALLOW_TRANSPORT_TCP = 0x01
ALLOW_TRANSPORT_UNIX = 0x02
ALLOW_TRANSPORT_VSOCK = 0x04
ALLOW_TRANSPORT_SSH = 0x08
ALLOW_TRANSPORT_MASK = 0x0f

SHUTDOWN_ABANDON_PENDING = 0x10000
SHUTDOWN_MASK = 0x10000

AIO_DIRECTION_READ = 1
AIO_DIRECTION_WRITE = 2
AIO_DIRECTION_BOTH = 3
READ_DATA = 1
READ_HOLE = 2
READ_ERROR = 3
NAMESPACE_BASE = "base:"
CONTEXT_BASE_ALLOCATION = "base:allocation"
STATE_HOLE = 1
STATE_ZERO = 2
NAMESPACE_QEMU = "qemu:"
CONTEXT_QEMU_DIRTY_BITMAP = "qemu:dirty-bitmap:"
STATE_DIRTY = 1
CONTEXT_QEMU_ALLOCATION_DEPTH = "qemu:allocation-depth"


class Buffer(object):
    '''Asynchronous I/O persistent buffer'''

    def __init__(self, len):
        '''Allocate an uninitialized AIO buffer used for nbd.aio_pread.'''
        self._o = libnbdmod.alloc_aio_buffer(len)

    @classmethod
    def from_buffer(cls, buf):
        '''Create an AIO buffer that shares an existing buffer-like object.

        Because the buffer is shared, changes to the original are visible
        to nbd.aio_pwrite, and changes in nbd.aio_pread are visible to the
        original.
        '''
        self = cls(0)
        # Ensure that buf is already buffer-like
        with memoryview(buf):
            self._o = buf
        self._init = True
        return self

    @classmethod
    def from_bytearray(cls, ba):
        '''Create an AIO buffer from a bytearray or other buffer-like object.

        If ba is not a buffer, it is tried as the parameter to the
        bytearray constructor.  Otherwise, ba is copied.  Either way, the
        resulting AIO buffer is independent from the original.
        '''
        return cls.from_buffer(bytearray(ba))

    def to_buffer(self):
        '''Return a shared view of the AIO buffer contents.

        This exposes the underlying buffer; changes to the buffer are
        visible to nbd.aio_pwrite, and changes from nbd.aio_pread are
        visible in the buffer.
        '''
        if not hasattr(self, '_init'):
            self._o = bytearray(len(self._o))
            self._init = True
        return self._o

    def to_bytearray(self):
        '''Copy an AIO buffer into a bytearray.

        This copies the contents of an AIO buffer to a new bytearray, which
        remains independent from the original.
        '''
        if not hasattr(self, '_init'):
            return bytearray(len(self._o))
        return bytearray(self._o)

    def size(self):
        '''Return the size of an AIO buffer.'''
        return len(self)

    def __len__(self):
        '''Return the size of an AIO buffer.'''
        return len(self._o)

    def is_zero(self, offset=0, size=-1):
        '''Returns true if and only if all bytes in the buffer are zeroes.

        Note that although a freshly allocated buffer is uninitialized,
        this will report it as all zeroes, as it will be force-initialized
        to zero before any code that can access the buffer's contents.

        By default this tests the whole buffer, but you can restrict
        the test to a sub-range of the buffer using the optional
        offset and size parameters.  If size = -1 then we check from
        offset to the end of the buffer.  If size = 0, the function
        always returns true.  If size > 0, we check the interval
        [offset..offset+size-1].
        '''
        return libnbdmod.aio_buffer_is_zero(self._o, offset, size,
                                            hasattr(self, '_init'))


class NBD(object):
    '''NBD handle'''

    def __init__(self):
        '''Create a new NBD handle.'''
        self._o = libnbdmod.create()

    def __del__(self):
        '''Close the NBD handle and underlying connection.'''
        if self._o:
            libnbdmod.close(self._o)
            self._o = None

    def _check_not_closed(self):
        if not self._o:
            raise ClosedHandle("libnbd: method called on closed handle")

    def close(self):
        '''Explicitly close the NBD handle and underlying connection.

        The handle is closed implicitly when its reference count goes
        to zero (eg. when it goes out of scope or the program ends).

        This call is only needed if you want to force the handle to
        close now.  After calling this, the program must not call
        any method on the handle (except the implicit call to
        __del__ which happens when the final reference is cleaned up).
        '''
        self._check_not_closed()
        libnbdmod.close(self._o)
        self._o = None

    def set_debug(self, debug):
        u'''▶ set or clear the debug flag

    Set or clear the debug flag. When debugging is enabled,
    debugging messages from the library are printed to
    stderr, unless a debugging callback has been defined too
    (see "nbd.set_debug_callback") in which case they are
    sent to that function. This flag defaults to false on
    newly created handles, except if "LIBNBD_DEBUG=1" is set
    in the environment in which case it defaults to true.
'''
        self._check_not_closed()
        return libnbdmod.set_debug(self._o, debug)

    def get_debug(self):
        u'''▶ return the state of the debug flag

    Return the state of the debug flag on this handle.
'''
        self._check_not_closed()
        return libnbdmod.get_debug(self._o)

    def set_debug_callback(self, debug):
        u'''▶ set the debug callback

    Set the debug callback. This function is called when the
    library emits debug messages, when debugging is enabled
    on a handle. The callback parameters are "user_data"
    passed to this function, the name of the libnbd function
    emitting the debug message ("context"), and the message
    itself ("msg"). If no debug callback is set on a handle
    then messages are printed on "stderr".

    The callback should not call "nbd_*" APIs on the same
    handle since it can be called while holding the handle
    lock and will cause a deadlock.
'''
        self._check_not_closed()
        return libnbdmod.set_debug_callback(self._o, debug)

    def clear_debug_callback(self):
        u'''▶ clear the debug callback

    Remove the debug callback if one was previously
    associated with the handle (with
    "nbd.set_debug_callback"). If no callback was associated
    this does nothing.
'''
        self._check_not_closed()
        return libnbdmod.clear_debug_callback(self._o)

    def stats_bytes_sent(self):
        u'''▶ statistics of bytes sent over connection so far

    Return the number of bytes that the client has sent to
    the server.

    This tracks the plaintext bytes utilized by the NBD
    protocol; it may differ from the number of bytes
    actually sent over the connection, particularly when TLS
    is in use.
'''
        self._check_not_closed()
        return libnbdmod.stats_bytes_sent(self._o)

    def stats_chunks_sent(self):
        u'''▶ statistics of chunks sent over connection so far

    Return the number of chunks that the client has sent to
    the server, where a chunk is a group of bytes delineated
    by a magic number that cannot be further subdivided
    without breaking the protocol.

    This number does not necessarily relate to the number of
    API calls made, nor to the number of TCP packets sent
    over the connection.
'''
        self._check_not_closed()
        return libnbdmod.stats_chunks_sent(self._o)

    def stats_bytes_received(self):
        u'''▶ statistics of bytes received over connection so far

    Return the number of bytes that the client has received
    from the server.

    This tracks the plaintext bytes utilized by the NBD
    protocol; it may differ from the number of bytes
    actually received over the connection, particularly when
    TLS is in use.
'''
        self._check_not_closed()
        return libnbdmod.stats_bytes_received(self._o)

    def stats_chunks_received(self):
        u'''▶ statistics of chunks received over connection so far

    Return the number of chunks that the client has received
    from the server, where a chunk is a group of bytes
    delineated by a magic number that cannot be further
    subdivided without breaking the protocol.

    This number does not necessarily relate to the number of
    API calls made, nor to the number of TCP packets
    received over the connection.
'''
        self._check_not_closed()
        return libnbdmod.stats_chunks_received(self._o)

    def set_handle_name(self, handle_name):
        u'''▶ set the handle name

    Handles have a name which is unique within the current
    process. The handle name is used in debug output.

    Handle names are normally generated automatically and
    have the form "nbd1", "nbd2", etc., but you can
    optionally use this call to give the handles a name
    which is meaningful for your application to make
    debugging output easier to understand.
'''
        self._check_not_closed()
        return libnbdmod.set_handle_name(self._o, handle_name)

    def get_handle_name(self):
        u'''▶ get the handle name

    Get the name of the handle. If it was previously set by
    calling "nbd.set_handle_name" then this returns the name
    that was set. Otherwise it will return a generic name
    like "nbd1", "nbd2", etc.
'''
        self._check_not_closed()
        return libnbdmod.get_handle_name(self._o)

    def set_private_data(self, private_data):
        u'''▶ set the per-handle private data

    Handles contain a private data field for applications to
    use for any purpose.

    When calling libnbd from C, the type of this field is
    "uintptr_t" so it can be used to store an unsigned
    integer or a pointer.

    In non-C bindings it can be used to store an unsigned
    integer.

    This function sets the value of this field and returns
    the old value (or 0 if it was not previously set).
'''
        self._check_not_closed()
        return libnbdmod.set_private_data(self._o, private_data)

    def get_private_data(self):
        u'''▶ get the per-handle private data

    Return the value of the private data field set
    previously by a call to "nbd.set_private_data" (or 0 if
    it was not previously set).
'''
        self._check_not_closed()
        return libnbdmod.get_private_data(self._o)

    def set_export_name(self, export_name):
        u'''▶ set the export name

    For servers which require an export name or can serve
    different content on different exports, set the
    "export_name" to connect to. The default is the empty
    string "".

    This is only relevant when connecting to servers using
    the newstyle protocol as the oldstyle protocol did not
    support export names. The NBD protocol limits export
    names to 4096 bytes, but servers may not support the
    full length. The encoding of export names is always
    UTF-8.

    When option mode is not in use, the export name must be
    set before beginning a connection. However, when
    "nbd.set_opt_mode" has enabled option mode, it is
    possible to change the export name prior to
    "nbd.opt_go". In particular, the use of "nbd.opt_list"
    during negotiation can be used to determine a name the
    server is likely to accept, and "nbd.opt_info" can be
    used to learn details about an export before connecting.

    This call may be skipped if using "nbd.connect_uri" to
    connect to a URI that includes an export name.
'''
        self._check_not_closed()
        return libnbdmod.set_export_name(self._o, export_name)

    def get_export_name(self):
        u'''▶ get the export name

    Get the export name associated with the handle. This is
    the name that libnbd requests; see
    "nbd.get_canonical_export_name" for determining if the
    server has a different canonical name for the given
    export (most common when requesting the default export
    name of an empty string "")
'''
        self._check_not_closed()
        return libnbdmod.get_export_name(self._o)

    def set_request_block_size(self, request):
        u'''▶ control whether NBD_OPT_GO requests block size

    By default, when connecting to an export, libnbd
    requests that the server report any block size
    restrictions. The NBD protocol states that a server may
    supply block sizes regardless of whether the client
    requests them, and libnbd will report those block sizes
    (see "nbd.get_block_size"); conversely, if a client does
    not request block sizes, the server may reject the
    connection instead of dealing with a client sending
    unaligned requests. This function makes it possible to
    test server behavior by emulating older clients.

    Note that even when block size is requested, the server
    is not obligated to provide any. Furthermore, if block
    sizes are provided (whether or not the client requested
    them), libnbd enforces alignment to those sizes unless
    "nbd.set_strict_mode" is used to bypass client-side
    safety checks.
'''
        self._check_not_closed()
        return libnbdmod.set_request_block_size(self._o, request)

    def get_request_block_size(self):
        u'''▶ see if NBD_OPT_GO requests block size

    Return the state of the block size request flag on this
    handle.
'''
        self._check_not_closed()
        return libnbdmod.get_request_block_size(self._o)

    def set_full_info(self, request):
        u'''▶ control whether NBD_OPT_GO requests extra details

    By default, when connecting to an export, libnbd only
    requests the details it needs to service data
    operations. The NBD protocol says that a server can
    supply optional information, such as a canonical name of
    the export (see "nbd.get_canonical_export_name") or a
    description of the export (see
    "nbd.get_export_description"), but that a hint from the
    client makes it more likely for this extra information
    to be provided. This function controls whether libnbd
    will provide that hint.

    Note that even when full info is requested, the server
    is not obligated to reply with all information that
    libnbd requested. Similarly, libnbd will ignore any
    optional server information that libnbd has not yet been
    taught to recognize. Furthermore, the hint to request
    block sizes is independently controlled via
    "nbd.set_request_block_size".
'''
        self._check_not_closed()
        return libnbdmod.set_full_info(self._o, request)

    def get_full_info(self):
        u'''▶ see if NBD_OPT_GO requests extra details

    Return the state of the full info request flag on this
    handle.
'''
        self._check_not_closed()
        return libnbdmod.get_full_info(self._o)

    def get_canonical_export_name(self):
        u'''▶ return the canonical export name, if the server has one

    The NBD protocol permits a server to report an optional
    canonical export name, which may differ from the
    client's request (as set by "nbd.set_export_name" or
    "nbd.connect_uri"). This function accesses any name
    returned by the server; it may be the same as the client
    request, but is more likely to differ when the client
    requested a connection to the default export name (an
    empty string "").

    Some servers are unlikely to report a canonical name
    unless the client specifically hinted about wanting it,
    via "nbd.set_full_info".
'''
        self._check_not_closed()
        return libnbdmod.get_canonical_export_name(self._o)

    def get_export_description(self):
        u'''▶ return the export description, if the server has one

    The NBD protocol permits a server to report an optional
    export description. This function reports any
    description returned by the server.

    Some servers are unlikely to report a description unless
    the client specifically hinted about wanting it, via
    "nbd.set_full_info". For qemu-nbd(8), a description is
    set with *-D*.
'''
        self._check_not_closed()
        return libnbdmod.get_export_description(self._o)

    def set_tls(self, tls):
        u'''▶ enable or require TLS (authentication and encryption)

    Enable or require TLS (authenticated and encrypted
    connections) to the NBD server. The possible settings
    are:

    "TLS_DISABLE"
        Disable TLS. (The default setting, unless using
        "nbd.connect_uri" with a URI that requires TLS).

        This setting is also necessary if you use
        "nbd.set_opt_mode" and want to interact in plaintext
        with a server that implements the NBD protocol's
        "SELECTIVETLS" mode, prior to enabling TLS with
        "nbd.opt_starttls". Most NBD servers with TLS
        support prefer the NBD protocol's "FORCEDTLS" mode,
        so this sort of manual interaction tends to be
        useful mainly during integration testing.

    "TLS_ALLOW"
        Enable TLS if possible.

        This option is insecure (or best effort) in that in
        some cases it will fall back to an unencrypted
        and/or unauthenticated connection if TLS could not
        be established. Use "TLS_REQUIRE" below if the
        connection must be encrypted.

        Some servers will drop the connection if TLS fails
        so fallback may not be possible.

    "TLS_REQUIRE"
        Require an encrypted and authenticated TLS
        connection. Always fail to connect if the connection
        is not encrypted and authenticated.

    As well as calling this you may also need to supply the
    path to the certificates directory
    ("nbd.set_tls_certificates"), the username
    ("nbd.set_tls_username") and/or the Pre-Shared Keys
    (PSK) file ("nbd.set_tls_psk_file"). For now, when using
    "nbd.connect_uri", any URI query parameters related to
    TLS are not handled automatically. Setting the level
    higher than zero will fail if libnbd was not compiled
    against gnutls; you can test whether this is the case
    with "nbd.supports_tls".
'''
        self._check_not_closed()
        return libnbdmod.set_tls(self._o, tls)

    def get_tls(self):
        u'''▶ get the TLS request setting

    Get the TLS request setting.

    Note: If you want to find out if TLS was actually
    negotiated on a particular connection use
    "nbd.get_tls_negotiated" instead.
'''
        self._check_not_closed()
        return libnbdmod.get_tls(self._o)

    def get_tls_negotiated(self):
        u'''▶ find out if TLS was negotiated on a connection

    After connecting you may call this to find out if the
    connection is using TLS.

    This is normally useful only if you set the TLS request
    mode to "TLS_ALLOW" (see "nbd.set_tls"), because in this
    mode we try to use TLS but fall back to unencrypted if
    it was not available. This function will tell you if TLS
    was negotiated or not.

    In "TLS_REQUIRE" mode (the most secure) the connection
    would have failed if TLS could not be negotiated. With
    "TLS_DISABLE" mode, TLS is not tried automatically; but
    if the NBD server uses the less-common "SELECTIVETLS"
    mode, this function reports whether a manual
    "nbd.opt_starttls" enabled TLS or if the connection is
    still plaintext.
'''
        self._check_not_closed()
        return libnbdmod.get_tls_negotiated(self._o)

    def set_tls_certificates(self, dir):
        u'''▶ set the path to the TLS certificates directory

    Set the path to the TLS certificates directory. If not
    set and TLS is used then a compiled in default is used.
    For root this is "/etc/pki/libnbd/". For non-root this
    is "$HOME/.pki/libnbd" and "$HOME/.config/pki/libnbd".
    If none of these directories can be found then the
    system trusted CAs are used.

    This function may be called regardless of whether TLS is
    supported, but will have no effect unless "nbd.set_tls"
    is also used to request or require TLS.
'''
        self._check_not_closed()
        return libnbdmod.set_tls_certificates(self._o, dir)

    def set_tls_verify_peer(self, verify):
        u'''▶ set whether we verify the identity of the server

    Set this flag to control whether libnbd will verify the
    identity of the server from the server's certificate and
    the certificate authority. This defaults to true when
    connecting to TCP servers using TLS certificate
    authentication, and false otherwise.

    This function may be called regardless of whether TLS is
    supported, but will have no effect unless "nbd.set_tls"
    is also used to request or require TLS.
'''
        self._check_not_closed()
        return libnbdmod.set_tls_verify_peer(self._o, verify)

    def get_tls_verify_peer(self):
        u'''▶ get whether we verify the identity of the server

    Get the verify peer flag.
'''
        self._check_not_closed()
        return libnbdmod.get_tls_verify_peer(self._o)

    def set_tls_username(self, username):
        u'''▶ set the TLS username

    Set the TLS client username. This is used if
    authenticating with PSK over TLS is enabled. If not set
    then the local username is used.

    This function may be called regardless of whether TLS is
    supported, but will have no effect unless "nbd.set_tls"
    is also used to request or require TLS.
'''
        self._check_not_closed()
        return libnbdmod.set_tls_username(self._o, username)

    def get_tls_username(self):
        u'''▶ get the current TLS username

    Get the current TLS username.
'''
        self._check_not_closed()
        return libnbdmod.get_tls_username(self._o)

    def set_tls_hostname(self, hostname):
        u'''▶ set the TLS hostname

    Set the TLS server hostname. This is used in preference
    to the hostname supplied when connecting (eg. through
    "nbd.connect_tcp"), or when there is no explicit
    hostname at all ("nbd.connect_unix"). It can be useful
    when you are connecting to a proxy which is forwarding
    the data to the final server, to specify the name of the
    final server so that the server's certificate can be
    checked.

    This function may be called regardless of whether TLS is
    supported, but will have no effect unless "nbd.set_tls"
    is also used to request or require TLS.
'''
        self._check_not_closed()
        return libnbdmod.set_tls_hostname(self._o, hostname)

    def get_tls_hostname(self):
        u'''▶ get the effective TLS hostname

    Get the TLS server hostname in effect. If not set, this
    returns the empty string (not an error).
'''
        self._check_not_closed()
        return libnbdmod.get_tls_hostname(self._o)

    def set_tls_psk_file(self, filename):
        u'''▶ set the TLS Pre-Shared Keys (PSK) filename

    Set the TLS Pre-Shared Keys (PSK) filename. This is used
    if trying to authenticate to the server using with a
    pre-shared key. There is no default so if this is not
    set then PSK authentication cannot be used to connect to
    the server.

    This function may be called regardless of whether TLS is
    supported, but will have no effect unless "nbd.set_tls"
    is also used to request or require TLS.
'''
        self._check_not_closed()
        return libnbdmod.set_tls_psk_file(self._o, filename)

    def set_request_extended_headers(self, request):
        u'''▶ control use of extended headers

    By default, libnbd tries to negotiate extended headers
    with the server, as this protocol extension permits the
    use of 64-bit zero, trim, and block status actions.
    However, for integration testing, it can be useful to
    clear this flag rather than find a way to alter the
    server to fail the negotiation request.

    For backwards compatibility, the setting of this knob is
    ignored if "nbd.set_request_structured_replies" is also
    set to false, since the use of extended headers implies
    structured replies.
'''
        self._check_not_closed()
        return libnbdmod.set_request_extended_headers(self._o, request)

    def get_request_extended_headers(self):
        u'''▶ see if extended headers are attempted

    Return the state of the request extended headers flag on
    this handle.

    Note: If you want to find out if extended headers were
    actually negotiated on a particular connection use
    "nbd.get_extended_headers_negotiated" instead.
'''
        self._check_not_closed()
        return libnbdmod.get_request_extended_headers(self._o)

    def get_extended_headers_negotiated(self):
        u'''▶ see if extended headers are in use

    After connecting you may call this to find out if the
    connection is using extended headers. Note that this
    setting is sticky; this can return true even after a
    second "nbd.opt_extended_headers" returns false because
    the server detected a duplicate request.

    When extended headers are not in use, commands are
    limited to a 32-bit length, even when the libnbd API
    uses a 64-bit parameter to express the length. But even
    when extended headers are supported, the server may
    enforce other limits, visible through
    "nbd.get_block_size".

    Note that when extended headers are negotiated, you
    should prefer the use of "nbd.block_status_64" instead
    of "nbd.block_status" if any of the meta contexts you
    requested via "nbd.add_meta_context" might return 64-bit
    status values; however, all of the well-known meta
    contexts covered by current "LIBNBD_CONTEXT_*" constants
    only return 32-bit status.
'''
        self._check_not_closed()
        return libnbdmod.get_extended_headers_negotiated(self._o)

    def set_request_structured_replies(self, request):
        u'''▶ control use of structured replies

    By default, libnbd tries to negotiate structured replies
    with the server, as this protocol extension must be in
    use before "nbd.can_meta_context" or "nbd.can_df" can
    return true. However, for integration testing, it can be
    useful to clear this flag rather than find a way to
    alter the server to fail the negotiation request. It is
    also useful to set this to false prior to using
    "nbd.set_opt_mode" if it is desired to control when to
    send "nbd.opt_structured_reply" during negotiation.

    Note that setting this knob to false also disables any
    automatic request for extended headers.
'''
        self._check_not_closed()
        return libnbdmod.set_request_structured_replies(self._o, request)

    def get_request_structured_replies(self):
        u'''▶ see if structured replies are attempted

    Return the state of the request structured replies flag
    on this handle.

    Note: If you want to find out if structured replies were
    actually negotiated on a particular connection use
    "nbd.get_structured_replies_negotiated" instead.
'''
        self._check_not_closed()
        return libnbdmod.get_request_structured_replies(self._o)

    def get_structured_replies_negotiated(self):
        u'''▶ see if structured replies are in use

    After connecting you may call this to find out if the
    connection is using structured replies. Note that this
    setting is sticky; this can return true even after a
    second "nbd.opt_structured_reply" returns false because
    the server detected a duplicate request.

    Note that if the connection negotiates extended headers,
    this function returns true (as extended headers imply
    structured replies) even if no explicit request for
    structured replies was attempted.
'''
        self._check_not_closed()
        return libnbdmod.get_structured_replies_negotiated(self._o)

    def set_request_meta_context(self, request):
        u'''▶ control whether connect automatically requests meta contexts

    This function controls whether the act of connecting to
    an export (all "nbd_connect_*" calls when
    "nbd.set_opt_mode" is false, or "nbd.opt_go" and
    "nbd.opt_info" when option mode is enabled) will also
    try to issue NBD_OPT_SET_META_CONTEXT when the server
    supports structured replies or extended headers and any
    contexts were registered by "nbd.add_meta_context". The
    default setting is true; however the extra step of
    negotiating meta contexts is not always desirable:
    performing both info and go on the same export works
    without needing to re-negotiate contexts on the second
    call; integration testing of other servers may benefit
    from manual invocation of "nbd.opt_set_meta_context" at
    other times in the negotiation sequence; and even when
    using just "nbd.opt_info", it can be faster to collect
    the server's results by relying on the callback function
    passed to "nbd.opt_list_meta_context" than a series of
    post-process calls to "nbd.can_meta_context".

    Note that this control has no effect if the server does
    not negotiate structured replies or extended headers, or
    if the client did not request any contexts via
    "nbd.add_meta_context". Setting this control to false
    may cause "nbd.block_status" to fail.
'''
        self._check_not_closed()
        return libnbdmod.set_request_meta_context(self._o, request)

    def get_request_meta_context(self):
        u'''▶ see if connect automatically requests meta contexts

    Return the state of the automatic meta context request
    flag on this handle.
'''
        self._check_not_closed()
        return libnbdmod.get_request_meta_context(self._o)

    def set_handshake_flags(self, flags):
        u'''▶ control use of handshake flags

    By default, libnbd tries to negotiate all possible
    handshake flags that are also supported by the server,
    since omitting a handshake flag can prevent the use of
    other functionality such as TLS encryption or structured
    replies. However, for integration testing, it can be
    useful to reduce the set of flags supported by the
    client to test that a particular server can handle
    various clients that were compliant to older versions of
    the NBD specification.

    The "flags" argument is a bitmask, including zero or
    more of the following handshake flags:

    "HANDSHAKE_FLAG_FIXED_NEWSTYLE" = 1
        The server gracefully handles unknown option
        requests from the client, rather than disconnecting.
        Without this flag, a client cannot safely request to
        use extensions such as TLS encryption or structured
        replies, as the request may cause an older server to
        drop the connection.

    "HANDSHAKE_FLAG_NO_ZEROES" = 2
        If the client is forced to use "NBD_OPT_EXPORT_NAME"
        instead of the preferred "NBD_OPT_GO", this flag
        allows the server to send fewer all-zero padding
        bytes over the connection.

    For convenience, the constant "HANDSHAKE_FLAG_MASK" is
    available to describe all flags supported by this build
    of libnbd. Future NBD extensions may add further flags,
    which in turn may be enabled by default in newer libnbd.
    As such, when attempting to disable only one specific
    bit, it is wiser to first call "nbd.get_handshake_flags"
    and modify that value, rather than blindly setting a
    constant value.
'''
        self._check_not_closed()
        return libnbdmod.set_handshake_flags(self._o, flags)

    def get_handshake_flags(self):
        u'''▶ see which handshake flags are supported

    Return the state of the handshake flags on this handle.
    When the handle has not yet completed a connection (see
    "nbd.aio_is_created"), this returns the flags that the
    client is willing to use, provided the server also
    advertises those flags. After the connection is ready
    (see "nbd.aio_is_ready"), this returns the flags that
    were actually agreed on between the server and client.
    If the NBD protocol defines new handshake flags, then
    the return value from a newer library version may
    include bits that were undefined at the time of
    compilation.
'''
        self._check_not_closed()
        return libnbdmod.get_handshake_flags(self._o)

    def set_pread_initialize(self, request):
        u'''▶ control whether libnbd pre-initializes read buffers

    By default, libnbd will pre-initialize the contents of a
    buffer passed to calls such as "nbd.pread" to all zeroes
    prior to checking for any other errors, so that even if
    a client application passed in an uninitialized buffer
    but fails to check for errors, it will not result in a
    potential security risk caused by an accidental leak of
    prior heap contents (see CVE-2022-0485 in
    libnbd-security(3) for an example of a security hole in
    an application built against an earlier version of
    libnbd that lacked consistent pre-initialization).
    However, for a client application that has audited that
    an uninitialized buffer is never dereferenced, or which
    performs its own pre-initialization, libnbd's
    sanitization efforts merely pessimize performance
    (although the time spent in pre-initialization may pale
    in comparison to time spent waiting on network packets).

    Calling this function with "request" set to false tells
    libnbd to skip the buffer initialization step in read
    commands.
'''
        self._check_not_closed()
        return libnbdmod.set_pread_initialize(self._o, request)

    def get_pread_initialize(self):
        u'''▶ see whether libnbd pre-initializes read buffers

    Return whether libnbd performs a pre-initialization of a
    buffer passed to "nbd.pread" and similar to all zeroes,
    as set by "nbd.set_pread_initialize".
'''
        self._check_not_closed()
        return libnbdmod.get_pread_initialize(self._o)

    def set_strict_mode(self, flags):
        u'''▶ control how strictly to follow NBD protocol

    By default, libnbd tries to detect requests that would
    trigger undefined behavior in the NBD protocol, and
    rejects them client side without causing any network
    traffic, rather than risking undefined server behavior.
    However, for integration testing, it can be handy to
    relax the strictness of libnbd, to coerce it into
    sending such requests over the network for testing the
    robustness of the server in dealing with such traffic.

    The "flags" argument is a bitmask, including zero or
    more of the following strictness flags:

    "STRICT_COMMANDS" = 0x1
        If set, this flag rejects client requests that do
        not comply with the set of advertised server flags
        (for example, attempting a write on a read-only
        server, or attempting to use "CMD_FLAG_FUA" when
        "nbd.can_fua" returned false). If clear, this flag
        relies on the server to reject unexpected commands.

    "STRICT_FLAGS" = 0x2
        If set, this flag rejects client requests that
        attempt to set a command flag not recognized by
        libnbd (those outside of "CMD_FLAG_MASK"), or a flag
        not normally associated with a command (such as
        using "CMD_FLAG_FUA" on a read command). If clear,
        all flags are sent on to the server, even if sending
        such a flag may cause the server to change its reply
        in a manner that confuses libnbd, perhaps causing
        deadlock or ending the connection.

        Flags that are known by libnbd as associated with a
        given command (such as "CMD_FLAG_DF" for
        "nbd.pread_structured" gated by "nbd.can_df") are
        controlled by "STRICT_COMMANDS" instead; and
        "CMD_FLAG_PAYLOAD_LEN" is managed automatically by
        libnbd unless "STRICT_AUTO_FLAG" is disabled.

        Note that the NBD protocol only supports 16 bits of
        command flags, even though the libnbd API uses
        "uint32_t"; bits outside of the range permitted by
        the protocol are always a client-side error.

    "STRICT_BOUNDS" = 0x4
        If set, this flag rejects client requests that would
        exceed the export bounds without sending any traffic
        to the server. If clear, this flag relies on the
        server to detect out-of-bounds requests.

    "STRICT_ZERO_SIZE" = 0x8
        If set, this flag rejects client requests with
        length 0. If clear, this permits zero-length
        requests to the server, which may produce undefined
        results.

    "STRICT_ALIGN" = 0x10
        If set, and the server provided minimum block sizes
        (see "SIZE_MINIMUM" for "nbd.get_block_size"), this
        flag rejects client requests that do not have length
        and offset aligned to the server's minimum
        requirements. If clear, unaligned requests are sent
        to the server, where it is up to the server whether
        to honor or reject the request.

    "STRICT_PAYLOAD" = 0x20
        If set, the client refuses to send a command to the
        server with more than libnbd's outgoing payload
        maximum (see "SIZE_PAYLOAD" for
        "nbd.get_block_size"), whether or not the server
        advertised a block size maximum. If clear, oversize
        requests up to 64MiB may be attempted, although
        requests larger than 32MiB are liable to cause some
        servers to disconnect.

    "STRICT_AUTO_FLAG" = 0x40
        If set, commands that accept the
        "CMD_FLAG_PAYLOAD_LEN" flag (such as "nbd.pwrite"
        and nbd_block_status_filter(3)) ignore the presence
        or absence of that flag from the caller, instead
        sending the value over the wire that matches the
        server's expectations based on whether extended
        headers were negotiated when the connection was
        made. If clear, the caller takes on the
        responsibility for whether the payload length flag
        is set or clear during the affected command, which
        can be useful during integration testing but is more
        likely to lead to undefined behavior.

    For convenience, the constant "STRICT_MASK" is available
    to describe all strictness flags supported by this build
    of libnbd. Future versions of libnbd may add further
    flags, which are likely to be enabled by default for
    additional client-side filtering. As such, when
    attempting to relax only one specific bit while keeping
    remaining checks at the client side, it is wiser to
    first call "nbd.get_strict_mode" and modify that value,
    rather than blindly setting a constant value.
'''
        self._check_not_closed()
        return libnbdmod.set_strict_mode(self._o, flags)

    def get_strict_mode(self):
        u'''▶ see which strictness flags are in effect

    Return flags indicating which protocol strictness items
    are being enforced locally by libnbd rather than the
    server. The return value from a newer library version
    may include bits that were undefined at the time of
    compilation.
'''
        self._check_not_closed()
        return libnbdmod.get_strict_mode(self._o)

    def set_opt_mode(self, enable):
        u'''▶ control option mode, for pausing during option negotiation

    Set this flag to true in order to request that a
    connection command "nbd_connect_*" will pause for
    negotiation options rather than proceeding all the way
    to the ready state, when communicating with a newstyle
    server. This setting has no effect when connecting to an
    oldstyle server.

    Note that libnbd defaults to attempting
    "NBD_OPT_STARTTLS", "NBD_OPT_EXTENDED_HEADERS", and
    "NBD_OPT_STRUCTURED_REPLY" before letting you control
    remaining negotiation steps; if you need control over
    these steps as well, first set "nbd.set_tls" to
    "TLS_DISABLE", and "nbd.set_request_extended_headers" or
    "nbd.set_request_structured_replies" to false, before
    starting the connection attempt.

    When option mode is enabled, you have fine-grained
    control over which options are negotiated, compared to
    the default of the server negotiating everything on your
    behalf using settings made before starting the
    connection. To leave the mode and proceed on to the
    ready state, you must use "nbd.opt_go" successfully; a
    failed "nbd.opt_go" returns to the negotiating state to
    allow a change of export name before trying again. You
    may also use "nbd.opt_abort" or "nbd.shutdown" to end
    the connection without finishing negotiation.
'''
        self._check_not_closed()
        return libnbdmod.set_opt_mode(self._o, enable)

    def get_opt_mode(self):
        u'''▶ return whether option mode was enabled

    Return true if option negotiation mode was enabled on
    this handle.
'''
        self._check_not_closed()
        return libnbdmod.get_opt_mode(self._o)

    def opt_go(self):
        u'''▶ end negotiation and move on to using an export

    Request that the server finish negotiation and move on
    to serving the export previously specified by the most
    recent "nbd.set_export_name" or "nbd.connect_uri". This
    can only be used if "nbd.set_opt_mode" enabled option
    mode.

    By default, libnbd will automatically request all meta
    contexts registered by "nbd.add_meta_context" as part of
    this call; but this can be suppressed with
    "nbd.set_request_meta_context", particularly if
    "nbd.opt_set_meta_context" was used earlier in the
    negotiation sequence.

    If this fails, the server may still be in negotiation,
    where it is possible to attempt another option such as a
    different export name; although older servers will
    instead have killed the connection.
'''
        self._check_not_closed()
        return libnbdmod.opt_go(self._o)

    def opt_abort(self):
        u'''▶ end negotiation and close the connection

    Request that the server finish negotiation, gracefully
    if possible, then close the connection. This can only be
    used if "nbd.set_opt_mode" enabled option mode.
'''
        self._check_not_closed()
        return libnbdmod.opt_abort(self._o)

    def opt_starttls(self):
        u'''▶ request the server to initiate TLS

    Request that the server initiate a secure TLS
    connection, by sending "NBD_OPT_STARTTLS". This can only
    be used if "nbd.set_opt_mode" enabled option mode;
    furthermore, if you use "nbd.set_tls" to request
    anything other than the default of "TLS_DISABLE", then
    libnbd will have already attempted a TLS connection
    prior to allowing you control over option negotiation.
    This command is disabled if "nbd.supports_tls" reports
    false.

    This function is mainly useful for integration testing
    of corner cases in server handling; in particular,
    misuse of this function when coupled with a server that
    is not careful about resetting stateful commands such as
    "nbd.opt_structured_reply" could result in a security
    hole (see CVE-2021-3716 against nbdkit, for example).
    Thus, when security is a concern, you should instead
    prefer to use "nbd.set_tls" with "TLS_REQUIRE" and let
    libnbd negotiate TLS automatically.

    This function returns true if the server replies with
    success, false if the server replies with an error, and
    fails only if the server does not reply (such as for a
    loss of connection, which can include when the server
    rejects credentials supplied during the TLS handshake).
    Note that the NBD protocol documents that requesting TLS
    after it is already enabled is a client error; most
    servers will gracefully fail a second request, but that
    does not downgrade a TLS session that has already been
    established, as reported by "nbd.get_tls_negotiated".
'''
        self._check_not_closed()
        return libnbdmod.opt_starttls(self._o)

    def opt_extended_headers(self):
        u'''▶ request the server to enable extended headers

    Request that the server use extended headers, by sending
    "NBD_OPT_EXTENDED_HEADERS". This can only be used if
    "nbd.set_opt_mode" enabled option mode; furthermore,
    libnbd defaults to automatically requesting this unless
    you use "nbd.set_request_extended_headers" or
    "nbd.set_request_structured_replies" prior to
    connecting. This function is mainly useful for
    integration testing of corner cases in server handling.

    This function returns true if the server replies with
    success, false if the server replies with an error, and
    fails only if the server does not reply (such as for a
    loss of connection). Note that some servers fail a
    second request as redundant; libnbd assumes that once
    one request has succeeded, then extended headers are
    supported (as visible by
    "nbd.get_extended_headers_negotiated") regardless if
    later calls to this function return false. If this
    function returns true, the use of structured replies is
    implied.
'''
        self._check_not_closed()
        return libnbdmod.opt_extended_headers(self._o)

    def opt_structured_reply(self):
        u'''▶ request the server to enable structured replies

    Request that the server use structured replies, by
    sending "NBD_OPT_STRUCTURED_REPLY". This can only be
    used if "nbd.set_opt_mode" enabled option mode;
    furthermore, libnbd defaults to automatically requesting
    this unless you use "nbd.set_request_structured_replies"
    prior to connecting. This function is mainly useful for
    integration testing of corner cases in server handling.

    This function returns true if the server replies with
    success, false if the server replies with an error, and
    fails only if the server does not reply (such as for a
    loss of connection). Note that some servers fail a
    second request as redundant; libnbd assumes that once
    one request has succeeded, then structured replies are
    supported (as visible by
    "nbd.get_structured_replies_negotiated") regardless if
    later calls to this function return false. Similarly, a
    server may fail this request if extended headers are
    already negotiated, since extended headers take
    priority.
'''
        self._check_not_closed()
        return libnbdmod.opt_structured_reply(self._o)

    def opt_list(self, list):
        u'''▶ request the server to list all exports during negotiation

    Request that the server list all exports that it
    supports. This can only be used if "nbd.set_opt_mode"
    enabled option mode.

    The "list" function is called once per advertised
    export, with any "user_data" passed to this function,
    and with "name" and "description" supplied by the
    server. Many servers omit descriptions, in which case
    "description" will be an empty string. Remember that it
    is not safe to call "nbd.set_export_name" from within
    the context of the callback function; rather, your code
    must copy any "name" needed for later use after this
    function completes. At present, the return value of the
    callback is ignored, although a return of -1 should be
    avoided.

    For convenience, when this function succeeds, it returns
    the number of exports that were advertised by the
    server.

    Not all servers understand this request, and even when
    it is understood, the server might intentionally send an
    empty list to avoid being an information leak, may
    encounter a failure after delivering partial results, or
    may refuse to answer more than one query per connection
    in the interest of avoiding negotiation that does not
    resolve. Thus, this function may succeed even when no
    exports are reported, or may fail but have a non-empty
    list. Likewise, the NBD protocol does not specify an
    upper bound for the number of exports that might be
    advertised, so client code should be aware that a server
    may send a lengthy list.

    For nbd-server(1) you will need to allow clients to make
    list requests by adding "allowlist=true" to the
    "[generic]" section of /etc/nbd-server/config. For
    qemu-nbd(8), a description is set with *-D*.
'''
        self._check_not_closed()
        return libnbdmod.opt_list(self._o, list)

    def opt_info(self):
        u'''▶ request the server for information about an export

    Request that the server supply information about the
    export name previously specified by the most recent
    "nbd.set_export_name" or "nbd.connect_uri". This can
    only be used if "nbd.set_opt_mode" enabled option mode.

    If successful, functions like "nbd.is_read_only" and
    "nbd.get_size" will report details about that export. If
    "nbd.set_request_meta_context" is set (the default) and
    structured replies or extended headers were negotiated,
    it is also valid to use "nbd.can_meta_context" after
    this call. However, it may be more efficient to clear
    that setting and manually utilize
    "nbd.opt_list_meta_context" with its callback approach,
    for learning which contexts an export supports. In
    general, if "nbd.opt_go" is called next, that call will
    likely succeed with the details remaining the same,
    although this is not guaranteed by all servers.

    Not all servers understand this request, and even when
    it is understood, the server might fail the request even
    when a corresponding "nbd.opt_go" would succeed.
'''
        self._check_not_closed()
        return libnbdmod.opt_info(self._o)

    def opt_list_meta_context(self, context):
        u'''▶ list available meta contexts, using implicit query list

    Request that the server list available meta contexts
    associated with the export previously specified by the
    most recent "nbd.set_export_name" or "nbd.connect_uri",
    and with a list of queries from prior calls to
    "nbd.add_meta_context" (see
    "nbd.opt_list_meta_context_queries" if you want to
    supply an explicit query list instead). This can only be
    used if "nbd.set_opt_mode" enabled option mode.

    The NBD protocol allows a client to decide how many
    queries to ask the server. Rather than taking that list
    of queries as a parameter to this function, libnbd
    reuses the current list of requested meta contexts as
    set by "nbd.add_meta_context"; you can use
    "nbd.clear_meta_contexts" to set up a different list of
    queries. When the list is empty, a server will typically
    reply with all contexts that it supports; when the list
    is non-empty, the server will reply only with supported
    contexts that match the client's request. Note that a
    reply by the server might be encoded to represent
    several feasible contexts within one string, rather than
    multiple strings per actual context name that would
    actually succeed during "nbd.opt_go"; so it is still
    necessary to use "nbd.can_meta_context" after connecting
    to see which contexts are actually supported.

    The "context" function is called once per server reply,
    with any "user_data" passed to this function, and with
    "name" supplied by the server. Remember that it is not
    safe to call "nbd.add_meta_context" from within the
    context of the callback function; rather, your code must
    copy any "name" needed for later use after this function
    completes. At present, the return value of the callback
    is ignored, although a return of -1 should be avoided.

    For convenience, when this function succeeds, it returns
    the number of replies returned by the server.

    Not all servers understand this request, and even when
    it is understood, the server might intentionally send an
    empty list because it does not support the requested
    context, or may encounter a failure after delivering
    partial results. Thus, this function may succeed even
    when no contexts are reported, or may fail but have a
    non-empty list. Likewise, the NBD protocol does not
    specify an upper bound for the number of replies that
    might be advertised, so client code should be aware that
    a server may send a lengthy list.
'''
        self._check_not_closed()
        return libnbdmod.opt_list_meta_context(self._o, context)

    def opt_list_meta_context_queries(self, queries, context):
        u'''▶ list available meta contexts, using explicit query list

    Request that the server list available meta contexts
    associated with the export previously specified by the
    most recent "nbd.set_export_name" or "nbd.connect_uri",
    and with an explicit list of queries provided as a
    parameter (see "nbd.opt_list_meta_context" if you want
    to reuse an implicit query list instead). This can only
    be used if "nbd.set_opt_mode" enabled option mode.

    The NBD protocol allows a client to decide how many
    queries to ask the server. For this function, the list
    is explicit in the "queries" parameter. When the list is
    empty, a server will typically reply with all contexts
    that it supports; when the list is non-empty, the server
    will reply only with supported contexts that match the
    client's request. Note that a reply by the server might
    be encoded to represent several feasible contexts within
    one string, rather than multiple strings per actual
    context name that would actually succeed during
    "nbd.opt_go"; so it is still necessary to use
    "nbd.can_meta_context" after connecting to see which
    contexts are actually supported.

    The "context" function is called once per server reply,
    with any "user_data" passed to this function, and with
    "name" supplied by the server. Remember that it is not
    safe to call "nbd.add_meta_context" from within the
    context of the callback function; rather, your code must
    copy any "name" needed for later use after this function
    completes. At present, the return value of the callback
    is ignored, although a return of -1 should be avoided.

    For convenience, when this function succeeds, it returns
    the number of replies returned by the server.

    Not all servers understand this request, and even when
    it is understood, the server might intentionally send an
    empty list because it does not support the requested
    context, or may encounter a failure after delivering
    partial results. Thus, this function may succeed even
    when no contexts are reported, or may fail but have a
    non-empty list. Likewise, the NBD protocol does not
    specify an upper bound for the number of replies that
    might be advertised, so client code should be aware that
    a server may send a lengthy list.
'''
        self._check_not_closed()
        return libnbdmod.opt_list_meta_context_queries(self._o, queries,
                                                       context)

    def opt_set_meta_context(self, context):
        u'''▶ select specific meta contexts, using implicit query list

    Request that the server supply all recognized meta
    contexts registered through prior calls to
    "nbd.add_meta_context", in conjunction with the export
    previously specified by the most recent
    "nbd.set_export_name" or "nbd.connect_uri". This can
    only be used if "nbd.set_opt_mode" enabled option mode.
    Normally, this function is redundant, as "nbd.opt_go"
    automatically does the same task if structured replies
    or extended headers have already been negotiated. But
    manual control over meta context requests can be useful
    for fine-grained testing of how a server handles unusual
    negotiation sequences. Often, use of this function is
    coupled with "nbd.set_request_meta_context" to bypass
    the automatic context request normally performed by
    "nbd.opt_go".

    The NBD protocol allows a client to decide how many
    queries to ask the server. Rather than taking that list
    of queries as a parameter to this function, libnbd
    reuses the current list of requested meta contexts as
    set by "nbd.add_meta_context"; you can use
    "nbd.clear_meta_contexts" to set up a different list of
    queries (see "nbd.opt_set_meta_context_queries" to pass
    an explicit list of contexts instead). Since this
    function is primarily designed for testing servers,
    libnbd does not prevent the use of this function on an
    empty list or when "nbd.set_request_structured_replies"
    has disabled structured replies, in order to see how a
    server behaves.

    The "context" function is called once per server reply,
    with any "user_data" passed to this function, and with
    "name" supplied by the server. Additionally, each server
    name will remain visible through "nbd.can_meta_context"
    until the next attempt at "nbd.set_export_name" or
    "nbd.opt_set_meta_context", as well as "nbd.opt_go" or
    "nbd.opt_info" that trigger an automatic meta context
    request. Remember that it is not safe to call any
    "nbd_*" APIs from within the context of the callback
    function. At present, the return value of the callback
    is ignored, although a return of -1 should be avoided.

    For convenience, when this function succeeds, it returns
    the number of replies returned by the server.

    Not all servers understand this request, and even when
    it is understood, the server might intentionally send an
    empty list because it does not support the requested
    context, or may encounter a failure after delivering
    partial results. Thus, this function may succeed even
    when no contexts are reported, or may fail but have a
    non-empty list.
'''
        self._check_not_closed()
        return libnbdmod.opt_set_meta_context(self._o, context)

    def opt_set_meta_context_queries(self, queries, context):
        u'''▶ select specific meta contexts, using explicit query list

    Request that the server supply all recognized meta
    contexts passed in through "queries", in conjunction
    with the export previously specified by the most recent
    "nbd.set_export_name" or "nbd.connect_uri". This can
    only be used if "nbd.set_opt_mode" enabled option mode.
    Normally, this function is redundant, as "nbd.opt_go"
    automatically does the same task if structured replies
    or extended headers have already been negotiated. But
    manual control over meta context requests can be useful
    for fine-grained testing of how a server handles unusual
    negotiation sequences. Often, use of this function is
    coupled with "nbd.set_request_meta_context" to bypass
    the automatic context request normally performed by
    "nbd.opt_go".

    The NBD protocol allows a client to decide how many
    queries to ask the server. This function takes an
    explicit list of queries; to instead reuse an implicit
    list, see "nbd.opt_set_meta_context". Since this
    function is primarily designed for testing servers,
    libnbd does not prevent the use of this function on an
    empty list or when "nbd.set_request_structured_replies"
    has disabled structured replies, in order to see how a
    server behaves.

    The "context" function is called once per server reply,
    with any "user_data" passed to this function, and with
    "name" supplied by the server. Additionally, each server
    name will remain visible through "nbd.can_meta_context"
    until the next attempt at "nbd.set_export_name" or
    "nbd.opt_set_meta_context", as well as "nbd.opt_go" or
    "nbd.opt_info" that trigger an automatic meta context
    request. Remember that it is not safe to call any
    "nbd_*" APIs from within the context of the callback
    function. At present, the return value of the callback
    is ignored, although a return of -1 should be avoided.

    For convenience, when this function succeeds, it returns
    the number of replies returned by the server.

    Not all servers understand this request, and even when
    it is understood, the server might intentionally send an
    empty list because it does not support the requested
    context, or may encounter a failure after delivering
    partial results. Thus, this function may succeed even
    when no contexts are reported, or may fail but have a
    non-empty list.
'''
        self._check_not_closed()
        return libnbdmod.opt_set_meta_context_queries(self._o, queries,
                                                      context)

    def add_meta_context(self, name):
        u'''▶ ask server to negotiate metadata context

    During connection libnbd can negotiate zero or more
    metadata contexts with the server. Metadata contexts are
    features (such as "base:allocation") which describe
    information returned by the "nbd.block_status_64"
    command (for "base:allocation" this is whether blocks of
    data are allocated, zero or sparse).

    This call adds one metadata context to the list to be
    negotiated. You can call it as many times as needed. The
    list is initially empty when the handle is created; you
    can check the contents of the list with
    "nbd.get_nr_meta_contexts" and "nbd.get_meta_context",
    or clear it with "nbd.clear_meta_contexts".

    The NBD protocol limits meta context names to 4096
    bytes, but servers may not support the full length. The
    encoding of meta context names is always UTF-8.

    Not all servers support all metadata contexts. To learn
    if a context was actually negotiated, call
    "nbd.can_meta_context" after connecting.

    The single parameter is the name of the metadata
    context, for example "CONTEXT_BASE_ALLOCATION".
    <libnbd.h> includes defined constants beginning with
    "CONTEXT_" for some well-known contexts, but you are
    free to pass in other contexts.

    Other metadata contexts are server-specific, but include
    "qemu:dirty-bitmap:..." and "qemu:allocation-depth" for
    qemu-nbd (see qemu-nbd *-B* and *-A* options).
'''
        self._check_not_closed()
        return libnbdmod.add_meta_context(self._o, name)

    def get_nr_meta_contexts(self):
        u'''▶ return the current number of requested meta contexts

    During connection libnbd can negotiate zero or more
    metadata contexts with the server. Metadata contexts are
    features (such as "base:allocation") which describe
    information returned by the "nbd.block_status_64"
    command (for "base:allocation" this is whether blocks of
    data are allocated, zero or sparse).

    This command returns how many meta contexts have been
    added to the list to request from the server via
    "nbd.add_meta_context". The server is not obligated to
    honor all of the requests; to see what it actually
    supports, see "nbd.can_meta_context".
'''
        self._check_not_closed()
        return libnbdmod.get_nr_meta_contexts(self._o)

    def get_meta_context(self, i):
        u'''▶ return the i'th meta context request

    During connection libnbd can negotiate zero or more
    metadata contexts with the server. Metadata contexts are
    features (such as "base:allocation") which describe
    information returned by the "nbd.block_status_64"
    command (for "base:allocation" this is whether blocks of
    data are allocated, zero or sparse).

    This command returns the i'th meta context request, as
    added by "nbd.add_meta_context", and bounded by
    "nbd.get_nr_meta_contexts".
'''
        self._check_not_closed()
        return libnbdmod.get_meta_context(self._o, i)

    def clear_meta_contexts(self):
        u'''▶ reset the list of requested meta contexts

    During connection libnbd can negotiate zero or more
    metadata contexts with the server. Metadata contexts are
    features (such as "base:allocation") which describe
    information returned by the "nbd.block_status_64"
    command (for "base:allocation" this is whether blocks of
    data are allocated, zero or sparse).

    This command resets the list of meta contexts to request
    back to an empty list, for re-population by further use
    of "nbd.add_meta_context". It is primarily useful when
    option negotiation mode is selected (see
    "nbd.set_opt_mode"), for altering the list of attempted
    contexts between subsequent export queries.
'''
        self._check_not_closed()
        return libnbdmod.clear_meta_contexts(self._o)

    def set_uri_allow_transports(self, mask):
        u'''▶ set the allowed transports in NBD URIs

    Set which transports are allowed to appear in NBD URIs.
    The default is to allow any transport.

    The "mask" parameter may contain any of the following
    flags ORed together:

    "ALLOW_TRANSPORT_TCP" = 0x1
    "ALLOW_TRANSPORT_UNIX" = 0x2
    "ALLOW_TRANSPORT_VSOCK" = 0x4
    "ALLOW_TRANSPORT_SSH" = 0x8

    For convenience, the constant "ALLOW_TRANSPORT_MASK" is
    available to describe all transports recognized by this
    build of libnbd. A future version of the library may add
    new flags.
'''
        self._check_not_closed()
        return libnbdmod.set_uri_allow_transports(self._o, mask)

    def set_uri_allow_tls(self, tls):
        u'''▶ set the allowed TLS settings in NBD URIs

    Set which TLS settings are allowed to appear in NBD
    URIs. The default is to allow either non-TLS or TLS
    URIs.

    The "tls" parameter can be:

    "TLS_DISABLE"
        TLS URIs are not permitted, ie. a URI such as
        "nbds://..." will be rejected.

    "TLS_ALLOW"
        This is the default. TLS may be used or not,
        depending on whether the URI uses "nbds" or "nbd".

    "TLS_REQUIRE"
        TLS URIs are required. All URIs must use "nbds".
'''
        self._check_not_closed()
        return libnbdmod.set_uri_allow_tls(self._o, tls)

    def set_uri_allow_local_file(self, allow):
        u'''▶ set the allowed transports in NBD URIs

    Allow NBD URIs to reference local files. This is
    *disabled* by default.

    Currently this setting only controls whether the
    "tls-psk-file" parameter in NBD URIs is allowed.
'''
        self._check_not_closed()
        return libnbdmod.set_uri_allow_local_file(self._o, allow)

    def connect_uri(self, uri):
        u'''▶ connect to NBD URI

    Connect (synchronously) to an NBD server and export by
    specifying the NBD URI. NBD URIs are a standard way to
    specify a network block device endpoint, using a syntax
    like "nbd://example.com" which is convenient, well
    defined and future proof.

    This call works by parsing the URI parameter and calling
    "nbd.set_export_name" and "nbd.set_tls" and other calls
    as needed, followed by "nbd.connect_tcp",
    "nbd.connect_unix" or "nbd.connect_vsock".

    This call returns when the connection has been made. By
    default, this proceeds all the way to transmission
    phase, but "nbd.set_opt_mode" can be used for manual
    control over option negotiation performed before
    transmission phase.

  Example URIs supported
    "nbd://example.com"
        Connect over TCP, unencrypted, to "example.com" port
        10809.

    "nbds://example.com"
        Connect over TCP with TLS, to "example.com" port
        10809. If the server does not support TLS then this
        will fail.

    "nbd+unix:///foo?socket=/tmp/nbd.sock"
        Connect over the Unix domain socket /tmp/nbd.sock to
        an NBD server running locally. The export name is
        set to "foo" (note without any leading "/"
        character).

    "nbds+unix://alice@/?socket=/tmp/nbd.sock&tls-certificat
    es=certs"
        Connect over a Unix domain socket, enabling TLS and
        setting the path to a directory containing
        certificates and keys.

    "nbd+vsock:///"
        In this scenario libnbd is running in a virtual
        machine. Connect over "AF_VSOCK" to an NBD server
        running on the hypervisor.

    "nbd+ssh://server/"
        Connect to remote "server" using Secure Shell, and
        tunnel NBD to an NBD server listening on port 10809.

  Supported URI formats
    The following schemes are supported in the current
    version of libnbd:

    "nbd:"
        Connect over TCP without using TLS.

    "nbds:"
        Connect over TCP. TLS is required and the connection
        will fail if the server does not support TLS.

    "nbd+unix:"
    "nbds+unix:"
        Connect over a Unix domain socket, without or with
        TLS respectively. The "socket" parameter is
        required.

    "nbd+vsock:"
    "nbds+vsock:"
        Connect over the "AF_VSOCK" transport, without or
        with TLS respectively. You can use
        "nbd.supports_vsock" to see if this build of libnbd
        supports "AF_VSOCK".

    "nbd+ssh:"
    "nbds+ssh:"
        *Experimental*

        Tunnel NBD over a Secure Shell connection. This
        requires that ssh(1) is installed locally, and that
        nc(1) (from the nmap project) is installed on the
        remote server.

    The authority part of the URI
    ("[username@][servername][:port]") is parsed depending
    on the transport. For TCP it specifies the server to
    connect to and optional port number. For "+unix" it
    should not be present. For "+vsock" the server name is
    the numeric CID (eg. 2 to connect to the host), and the
    optional port number may be present. For "+ssh" the
    Secure Shell server and optional port. If the "username"
    is present it is used for TLS authentication.

    For all transports, an export name may be present,
    parsed in accordance with the NBD URI specification.

    Finally the query part of the URI can contain:

    socket=SOCKET
        Specifies the Unix domain socket to connect on. Must
        be present for the "+unix" transport, optional for
        "+ssh", and must not be present for the other
        transports.

    tls-certificates=DIR
        Set the certificates directory. See
        "nbd.set_tls_certificates". Note this is not allowed
        by default - see next section.

    tls-psk-file=PSKFILE
        Set the PSK file. See "nbd.set_tls_psk_file". Note
        this is not allowed by default - see next section.

    tls-hostname="SERVER"
        Set the TLS hostname. See "nbd.set_tls_hostname".

    tls-verify-peer=false
        Do not verify the server certificate. See
        "nbd.set_tls_verify_peer". The default is "true".

  Disable URI features
    For security reasons you might want to disable certain
    URI features. Pre-filtering URIs is error-prone and
    should not be attempted. Instead use the libnbd APIs
    below to control what can appear in URIs. Note you must
    call these functions on the same handle before calling
    "nbd.connect_uri" or "nbd.aio_connect_uri".

    TCP, Unix domain socket, "AF_VSOCK" or SSH transports
        Default: all allowed

        To select which transports are allowed call
        "nbd.set_uri_allow_transports".

    TLS Default: both non-TLS and TLS connections allowed

        To force TLS off or on in URIs call
        "nbd.set_uri_allow_tls".

    Connect to Unix domain socket in the local filesystem
        Default: allowed

        To prevent this you must disable the "+unix"
        transport using "nbd.set_uri_allow_transports".

    Read from local files
        Default: denied

        To allow URIs to contain references to local files
        (eg. for parameters like "tls-psk-file") call
        "nbd.set_uri_allow_local_file".

  Overriding the export name
    It is possible to override the export name portion of a
    URI by using "nbd.set_opt_mode" to enable option mode,
    then using "nbd.set_export_name" and "nbd.opt_go" as
    part of subsequent negotiation.

  Optional features
    This call will fail if libnbd was not compiled with
    libxml2; you can test whether this is the case with
    "nbd.supports_uri".

    Support for URIs that require TLS will fail if libnbd
    was not compiled with gnutls; you can test whether this
    is the case with "nbd.supports_tls".

  Constructing a URI from an existing connection
    See "nbd.get_uri".

  See if a string is an NBD URI
    See "nbd.is_uri".

  Differences from qemu and glib parsing of NBD URIs
    qemu(1) also supports NBD URIs and has a separate URI
    parser. In qemu ≤ 9.0 this was done using their own
    parser. In qemu ≥ 9.1 this is done using glib "g_uri"
    functions. The current (glib-based) parser does not
    parse the export name part of the URI in exactly the
    same way as libnbd, which may cause URIs to work in
    libnbd but not in qemu or *vice versa*. Only URIs using
    exportnames should be affected. For details see
    <https://gitlab.com/qemu-project/qemu/-/issues/2584>.

  Limitations on vsock port numbers
    The vsock(7) protocol allows 32 bit unsigned ports,
    reserving ports 0, 1 and 2 for special purposes. In
    Linux, ports < 1024 are reserved for privileged
    processes.

    libxml2 (used to parse the URI) imposes additional
    restrictions. libxml2 < 2.9 limited port numbers to
    99,999,999. libxml2 ≥ 2.9 limits port numbers to ≤
    0x7fff_ffff (31 bits).
'''
        self._check_not_closed()
        return libnbdmod.connect_uri(self._o, uri)

    def connect_unix(self, unixsocket):
        u'''▶ connect to NBD server over a Unix domain socket

    Connect (synchronously) over the named Unix domain
    socket ("unixsocket") to an NBD server running on the
    same machine.

    This call returns when the connection has been made. By
    default, this proceeds all the way to transmission
    phase, but "nbd.set_opt_mode" can be used for manual
    control over option negotiation performed before
    transmission phase.
'''
        self._check_not_closed()
        return libnbdmod.connect_unix(self._o, unixsocket)

    def connect_vsock(self, cid, port):
        u'''▶ connect to NBD server over AF_VSOCK protocol

    Connect (synchronously) over the "AF_VSOCK" protocol
    from a virtual machine to an NBD server, usually running
    on the host. The "cid" and "port" parameters specify the
    server address. Usually "cid" should be 2 (to connect to
    the host), and "port" might be 10809 or another port
    number assigned to you by the host administrator.

    Not all systems support "AF_VSOCK"; to determine if
    libnbd was built on a system with vsock support, see
    "nbd.supports_vsock".

    This call returns when the connection has been made. By
    default, this proceeds all the way to transmission
    phase, but "nbd.set_opt_mode" can be used for manual
    control over option negotiation performed before
    transmission phase.
'''
        self._check_not_closed()
        return libnbdmod.connect_vsock(self._o, cid, port)

    def connect_tcp(self, hostname, port):
        u'''▶ connect to NBD server over a TCP port

    Connect (synchronously) to the NBD server listening on
    "hostname:port". The "port" may be a port name such as
    "nbd", or it may be a port number as a string such as
    "10809".

    This call returns when the connection has been made. By
    default, this proceeds all the way to transmission
    phase, but "nbd.set_opt_mode" can be used for manual
    control over option negotiation performed before
    transmission phase.
'''
        self._check_not_closed()
        return libnbdmod.connect_tcp(self._o, hostname, port)

    def connect_socket(self, sock):
        u'''▶ connect directly to a connected socket

    Pass a connected socket "sock" through which libnbd will
    talk to the NBD server.

    The caller is responsible for creating and connecting
    this socket by some method, before passing it to libnbd.

    If this call returns without error then socket ownership
    is passed to libnbd. Libnbd will close the socket when
    the handle is closed. The caller must not use the socket
    in any way.

    This call returns when the connection has been made. By
    default, this proceeds all the way to transmission
    phase, but "nbd.set_opt_mode" can be used for manual
    control over option negotiation performed before
    transmission phase.
'''
        self._check_not_closed()
        return libnbdmod.connect_socket(self._o, sock)

    def connect_command(self, argv):
        u'''▶ connect to NBD server command

    Run the command as a subprocess and connect to it over
    stdin/stdout. This is for use with NBD servers which can
    behave like inetd clients, such as nbdkit(1) using the
    *-s*/*--single* flag, and nbd-server(1) with port number
    set to 0.

    To run qemu-nbd(1), use
    "nbd.connect_systemd_socket_activation" instead.

  Subprocess
    Libnbd will fork the "argv" command and pass the NBD
    socket to it using file descriptors 0 and 1
    (stdin/stdout):

     ┌─────────┬─────────┐    ┌────────────────┐
     │ program │ libnbd  │    │   NBD server   │
     │         │         │    │       (argv)   │
     │         │ socket ╍╍╍╍╍╍╍╍▶ stdin/stdout │
     └─────────┴─────────┘    └────────────────┘

    When the NBD handle is closed the server subprocess is
    killed.

    This call returns when the connection has been made. By
    default, this proceeds all the way to transmission
    phase, but "nbd.set_opt_mode" can be used for manual
    control over option negotiation performed before
    transmission phase.
'''
        self._check_not_closed()
        return libnbdmod.connect_command(self._o, argv)

    def connect_systemd_socket_activation(self, argv):
        u'''▶ connect using systemd socket activation

    Run the command as a subprocess and connect to it using
    systemd socket activation.

    This is especially useful for running qemu-nbd(1) as a
    subprocess of libnbd, for example to use it to open
    qcow2 files.

    To run nbdkit as a subprocess, this function can be
    used, or "nbd.connect_command".

    To run nbd-server(1) as a subprocess, this function
    cannot be used, you must use "nbd.connect_command".

  Socket activation
    Libnbd will fork the "argv" command and pass an NBD
    socket to it using special "LISTEN_*" environment
    variables (as defined by the systemd socket activation
    protocol).

     ┌─────────┬─────────┐    ┌───────────────┐
     │ program │ libnbd  │    │  qemu-nbd or  │
     │         │         │    │  other server │
     │         │ socket ╍╍╍╍╍╍╍╍▶             │
     └─────────┴─────────┘    └───────────────┘

    When the NBD handle is closed the server subprocess is
    killed.

   Socket name
    The socket activation protocol lets you optionally give
    the socket a name. If used, the name is passed to the
    NBD server using the "LISTEN_FDNAMES" environment
    variable. To provide a socket name, call
    "nbd.set_socket_activation_name" before calling the
    connect function.

    This call returns when the connection has been made. By
    default, this proceeds all the way to transmission
    phase, but "nbd.set_opt_mode" can be used for manual
    control over option negotiation performed before
    transmission phase.
'''
        self._check_not_closed()
        return libnbdmod.connect_systemd_socket_activation(self._o, argv)

    def set_socket_activation_name(self, socket_name):
        u'''▶ set the socket activation name

    When running an NBD server using
    "nbd.connect_systemd_socket_activation" you can
    optionally name the socket. Call this function before
    connecting to the server.

    Some servers such as qemu-storage-daemon(1) can use this
    information to associate the socket with a name used on
    the command line, but most servers will ignore it. The
    name is passed through the "LISTEN_FDNAMES" environment
    variable.

    The parameter "socket_name" can be a short alphanumeric
    string. If it is set to the empty string (also the
    default when the handle is created) then the name
    "unknown" will be seen by the server.
'''
        self._check_not_closed()
        return libnbdmod.set_socket_activation_name(self._o, socket_name)

    def get_socket_activation_name(self):
        u'''▶ get the socket activation name

    Return the socket name used when you call
    "nbd.connect_systemd_socket_activation" on the same
    handle. By default this will return the empty string
    meaning that the server will see the name "unknown".
'''
        self._check_not_closed()
        return libnbdmod.get_socket_activation_name(self._o)

    def is_read_only(self):
        u'''▶ is the NBD export read-only?

    Returns true if the NBD export is read-only; writes and
    write-like operations will fail.

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.is_read_only(self._o)

    def can_flush(self):
        u'''▶ does the server support the flush command?

    Returns true if the server supports the flush command
    (see "nbd.flush", "nbd.aio_flush"). Returns false if the
    server does not.

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.can_flush(self._o)

    def can_fua(self):
        u'''▶ does the server support the FUA flag?

    Returns true if the server supports the FUA flag on
    certain commands (see "nbd.pwrite").

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.can_fua(self._o)

    def is_rotational(self):
        u'''▶ is the NBD disk rotational (like a disk)?

    Returns true if the disk exposed over NBD is rotational
    (like a traditional floppy or hard disk). Returns false
    if the disk has no penalty for random access (like an
    SSD or RAM disk).

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.is_rotational(self._o)

    def can_trim(self):
        u'''▶ does the server support the trim command?

    Returns true if the server supports the trim command
    (see "nbd.trim", "nbd.aio_trim"). Returns false if the
    server does not.

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.can_trim(self._o)

    def can_zero(self):
        u'''▶ does the server support the zero command?

    Returns true if the server supports the zero command
    (see "nbd.zero", "nbd.aio_zero"). Returns false if the
    server does not.

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.can_zero(self._o)

    def can_fast_zero(self):
        u'''▶ does the server support the fast zero flag?

    Returns true if the server supports the use of the
    "CMD_FLAG_FAST_ZERO" flag to the zero command (see
    "nbd.zero", "nbd.aio_zero"). Returns false if the server
    does not.

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.can_fast_zero(self._o)

    def can_block_status_payload(self):
        u'''▶ does the server support the block status payload flag?

    Returns true if the server supports the use of the
    "CMD_FLAG_PAYLOAD_LEN" flag to allow filtering of the
    block status command (see "nbd.block_status_filter").
    Returns false if the server does not. Note that this
    will never return true if
    "nbd.get_extended_headers_negotiated" is false.

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.can_block_status_payload(self._o)

    def can_df(self):
        u'''▶ does the server support the don't fragment flag to pread?

    Returns true if the server supports structured reads
    with an ability to request a non-fragmented read (see
    "nbd.pread_structured", "nbd.aio_pread_structured").
    Returns false if the server either lacks structured
    reads or if it does not support a non-fragmented read
    request.

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.can_df(self._o)

    def can_multi_conn(self):
        u'''▶ does the server support multi-conn?

    Returns true if the server supports multi-conn. Returns
    false if the server does not.

    It is not safe to open multiple handles connecting to
    the same server if you will write to the server and the
    server does not advertise multi-conn support. The safe
    way to check for this is to open one connection, check
    this flag is true, then open further connections as
    required.

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.can_multi_conn(self._o)

    def can_cache(self):
        u'''▶ does the server support the cache command?

    Returns true if the server supports the cache command
    (see "nbd.cache", "nbd.aio_cache"). Returns false if the
    server does not.

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.can_cache(self._o)

    def can_meta_context(self, metacontext):
        u'''▶ does the server support a specific meta context?

    Returns true if the server supports the given meta
    context (see "nbd.add_meta_context"). Returns false if
    the server does not. It is possible for this command to
    fail if meta contexts were requested but there is a
    missing or failed attempt at NBD_OPT_SET_META_CONTEXT
    during option negotiation.

    If the server supports block status filtering (see
    "nbd.can_block_status_payload", this function must
    return true for any filter name passed to
    "nbd.block_status_filter".

    The single parameter is the name of the metadata
    context, for example "CONTEXT_BASE_ALLOCATION".
    <libnbd.h> includes defined constants for well-known
    namespace contexts beginning with "CONTEXT_", but you
    are free to pass in other contexts.

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.can_meta_context(self._o, metacontext)

    def get_protocol(self):
        u'''▶ return the NBD protocol variant

    Return the NBD protocol variant in use on the
    connection. At the moment this returns one of the
    strings "oldstyle", "newstyle" or "newstyle-fixed".
    Other strings might be returned in the future. Most
    modern NBD servers use "newstyle-fixed".

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.get_protocol(self._o)

    def get_size(self):
        u'''▶ return the export size

    Returns the size in bytes of the NBD export.

    Note that this call fails with "EOVERFLOW" for an
    unlikely server that advertises a size which cannot fit
    in a 64-bit signed integer.

    nbdinfo(1) *--size* option is a way to access this API
    from shell scripts.

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.get_size(self._o)

    def get_block_size(self, size_type):
        u'''▶ return a specific server block size constraint

    Returns a specific block size constraint advertised by
    the server. If zero is returned it means the server did
    not advertise a constraint.

    Constraints are hints. Servers differ in their behaviour
    as to whether they enforce constraints or not.

    The "size_type" parameter selects which constraint to
    read. It can be one of:

    "SIZE_MINIMUM" = 0
        If non-zero, this will be a power of 2 between 1 and
        64k; any client request that is not aligned in
        length or offset to this size is likely to fail with
        "EINVAL". The image size will generally also be a
        multiple of this value (if not, the final few bytes
        are inaccessible while obeying alignment
        constraints).

        If zero (meaning no information was returned by the
        server), it is safest to assume a minimum block size
        of 512, although many servers support a minimum
        block size of 1.

        If the server provides a constraint, then libnbd
        defaults to honoring that constraint client-side
        unless "STRICT_ALIGN" is cleared in
        nbd_set_strict_mode(3).

    "SIZE_PREFERRED" = 1
        If non-zero, this is a power of 2 representing the
        preferred size for efficient I/O. Smaller requests
        may incur overhead such as read-modify-write cycles
        that will not be present when using I/O that is a
        multiple of this value. This value may be larger
        than the size of the export.

        If zero (meaning no information was returned by the
        server), using 4k as a preferred block size tends to
        give decent performance.

    "SIZE_MAXIMUM" = 2
        If non-zero, this represents the maximum length that
        the server is willing to handle during "nbd.pread"
        or "nbd.pwrite". Other functions like "nbd.zero" may
        still be able to use larger sizes. Note that this
        function returns what the server advertised, but
        libnbd itself imposes a maximum of 64M.

        If zero (meaning no information was returned by the
        server), some NBD servers will abruptly disconnect
        if a transaction sends or receives more than 32M of
        data.

    "SIZE_PAYLOAD" = 3
        This value is not advertised by the server, but
        rather represents the maximum outgoing payload size
        for a given connection that libnbd will enforce
        unless "STRICT_PAYLOAD" is cleared in
        nbd_set_strict_mode(3). It is always non-zero: never
        smaller than 1M, never larger than 64M, and matches
        "SIZE_MAXIMUM" when possible.

    Future NBD extensions may result in additional
    "size_type" values. Note that by default, libnbd
    requests all available block sizes, but that a server
    may differ in what sizes it chooses to report if
    "nbd.set_request_block_size" alters whether the client
    requests sizes.

    This call does not block, because it returns data that
    is saved in the handle from the NBD protocol handshake.
'''
        self._check_not_closed()
        return libnbdmod.get_block_size(self._o, size_type)

    def pread(self, count, offset, flags=0):
        u'''▶ read from the NBD server

    Issue a read command to the NBD server for the range
    starting at "offset" and ending at "offset" + "count" -
    1. NBD can only read all or nothing using this call. The
    call returns when the data has been read fully into
    "buf" or there is an error. See also
    "nbd.pread_structured", if finer visibility is required
    into the server's replies, or if you want to use
    "CMD_FLAG_DF".

    Note that libnbd currently enforces a maximum read
    buffer of 64MiB, even if the server would permit a
    larger buffer in a single transaction; attempts to
    exceed this will result in an "ERANGE" error. The server
    may enforce a smaller limit, which can be learned with
    "nbd.get_block_size".

    The "flags" parameter must be 0 for now (it exists for
    future NBD protocol extensions).

    Note that if this command fails, and
    "nbd.get_pread_initialize" returns true, then libnbd
    sanitized "buf", but it is unspecified whether the
    contents of "buf" will read as zero or as partial
    results from the server. If "nbd.get_pread_initialize"
    returns false, then libnbd did not sanitize "buf", and
    the contents are undefined on failure.

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.pread(self._o, count, offset, flags)

    def pread_structured(self, count, offset, chunk, flags=0):
        u'''▶ read from the NBD server

    Issue a read command to the NBD server for the range
    starting at "offset" and ending at "offset" + "count" -
    1. The server's response may be subdivided into chunks
    which may arrive out of order before reassembly into the
    original buffer; the "chunk" callback is used for
    notification after each chunk arrives, and may perform
    additional sanity checking on the server's reply. The
    callback cannot call "nbd_*" APIs on the same handle
    since it holds the handle lock and will cause a
    deadlock. If the callback returns -1, and no earlier
    error has been detected, then the overall read command
    will fail with any non-zero value stored into the
    callback's "error" parameter (with a default of
    "EPROTO"); but any further chunks will still invoke the
    callback.

    The "chunk" function is called once per chunk of data
    received, with the "user_data" passed to this function.
    The "subbuf" and "count" parameters represent the subset
    of the original buffer which has just been populated by
    results from the server (in C, "subbuf" always points
    within the original "buf"; but this guarantee may not
    extend to other language bindings). The "offset"
    parameter represents the absolute offset at which
    "subbuf" begins within the image (note that this is not
    the relative offset of "subbuf" within the original
    buffer "buf"). Changes to "error" on output are ignored
    unless the callback fails. The input meaning of the
    "error" parameter is controlled by the "status"
    parameter, which is one of

    "READ_DATA" = 1
        "subbuf" was populated with "count" bytes of data.
        On input, "error" contains the errno value of any
        earlier detected error, or zero.

    "READ_HOLE" = 2
        "subbuf" represents a hole, and contains "count" NUL
        bytes. On input, "error" contains the errno value of
        any earlier detected error, or zero.

    "READ_ERROR" = 3
        "count" is 0, so "subbuf" is unusable. On input,
        "error" contains the errno value reported by the
        server as occurring while reading that "offset",
        regardless if any earlier error has been detected.

    Future NBD extensions may permit other values for
    "status", but those will not be returned to a client
    that has not opted in to requesting such extensions. If
    the server is non-compliant, it is possible for the
    "chunk" function to be called more times than you expect
    or with "count" 0 for "READ_DATA" or "READ_HOLE". It is
    also possible that the "chunk" function is not called at
    all (in particular, "READ_ERROR" is used only when an
    error is associated with a particular offset, and not
    when the server reports a generic error), but you are
    guaranteed that the callback was called at least once if
    the overall read succeeds. Libnbd does not validate that
    the server obeyed the requirement that a read call must
    not have overlapping chunks and must not succeed without
    enough chunks to cover the entire request.

    Note that libnbd currently enforces a maximum read
    buffer of 64MiB, even if the server would permit a
    larger buffer in a single transaction; attempts to
    exceed this will result in an "ERANGE" error. The server
    may enforce a smaller limit, which can be learned with
    "nbd.get_block_size".

    The "flags" parameter may be 0 for no flags, or may
    contain "CMD_FLAG_DF" meaning that the server should not
    reply with more than one fragment (if that is supported
    - some servers cannot do this, see "nbd.can_df"). Libnbd
    does not validate that the server actually obeys the
    flag.

    Note that if this command fails, and
    "nbd.get_pread_initialize" returns true, then libnbd
    sanitized "buf", but it is unspecified whether the
    contents of "buf" will read as zero or as partial
    results from the server. If "nbd.get_pread_initialize"
    returns false, then libnbd did not sanitize "buf", and
    the contents are undefined on failure.

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.pread_structured(self._o, count, offset, chunk,
                                          flags)

    def pwrite(self, buf, offset, flags=0):
        u'''▶ write to the NBD server

    Issue a write command to the NBD server, writing the
    data in "buf" to the range starting at "offset" and
    ending at "offset" + "count" - 1. NBD can only write all
    or nothing using this call. The call returns when the
    command has been acknowledged by the server, or there is
    an error. Note this will generally return an error if
    "nbd.is_read_only" is true.

    Note that libnbd defaults to enforcing a maximum write
    buffer of the lesser of 64MiB or any maximum payload
    size advertised by the server; attempts to exceed this
    will generally result in a client-side "ERANGE" error,
    rather than a server-side disconnection. The actual
    limit can be learned with "nbd.get_block_size".

    The "flags" parameter may be 0 for no flags, or may
    contain "CMD_FLAG_FUA" meaning that the server should
    not return until the data has been committed to
    permanent storage (if that is supported - some servers
    cannot do this, see "nbd.can_fua"). For convenience,
    unless nbd_set_strict_flags(3) was used to disable
    "STRICT_AUTO_FLAG", libnbd ignores the presence or
    absence of the flag "CMD_FLAG_PAYLOAD_LEN" in "flags",
    while correctly using the flag over the wire according
    to whether extended headers were negotiated.

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.pwrite(self._o, buf, offset, flags)

    def shutdown(self, flags=0):
        u'''▶ disconnect from the NBD server

    Issue the disconnect command to the NBD server. This is
    a nice way to tell the server we are going away, but
    from the client's point of view has no advantage over
    abruptly closing the connection (see "nbd.close").

    This function works whether or not the handle is ready
    for transmission of commands. If more fine-grained
    control is needed, see "nbd.aio_opt_abort" and
    "nbd.aio_disconnect".

    The "flags" argument is a bitmask, including zero or
    more of the following shutdown flags:

    "SHUTDOWN_ABANDON_PENDING" = 0x10000
        If there are any pending requests which have not yet
        been sent to the server (see "nbd.aio_in_flight"),
        abandon them without sending them to the server,
        rather than the usual practice of issuing those
        commands before informing the server of the intent
        to disconnect.

    For convenience, the constant "SHUTDOWN_MASK" is
    available to describe all shutdown flags recognized by
    this build of libnbd. A future version of the library
    may add new flags.
'''
        self._check_not_closed()
        return libnbdmod.shutdown(self._o, flags)

    def flush(self, flags=0):
        u'''▶ send flush command to the NBD server

    Issue the flush command to the NBD server. The function
    should return when all write commands which have
    completed have been committed to permanent storage on
    the server. Note this will generally return an error if
    "nbd.can_flush" is false.

    The "flags" parameter must be 0 for now (it exists for
    future NBD protocol extensions).

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.flush(self._o, flags)

    def trim(self, count, offset, flags=0):
        u'''▶ send trim command to the NBD server

    Issue a trim command to the NBD server, which if
    supported by the server causes a hole to be punched in
    the backing store starting at "offset" and ending at
    "offset" + "count" - 1. The call returns when the
    command has been acknowledged by the server, or there is
    an error. Note this will generally return an error if
    "nbd.can_trim" is false or "nbd.is_read_only" is true.

    Note that not all servers can support a "count" of 4GiB
    or larger; "nbd.get_extended_headers_negotiated"
    indicates which servers will parse a request larger than
    32 bits. The NBD protocol does not yet have a way for a
    client to learn if the server will enforce an even
    smaller maximum trim size, although a future extension
    may add a constraint visible in "nbd.get_block_size".

    The "flags" parameter may be 0 for no flags, or may
    contain "CMD_FLAG_FUA" meaning that the server should
    not return until the data has been committed to
    permanent storage (if that is supported - some servers
    cannot do this, see "nbd.can_fua").

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.trim(self._o, count, offset, flags)

    def cache(self, count, offset, flags=0):
        u'''▶ send cache (prefetch) command to the NBD server

    Issue the cache (prefetch) command to the NBD server,
    which if supported by the server causes data to be
    prefetched into faster storage by the server, speeding
    up a subsequent "nbd.pread" call. The server can also
    silently ignore this command. Note this will generally
    return an error if "nbd.can_cache" is false.

    Note that not all servers can support a "count" of 4GiB
    or larger; "nbd.get_extended_headers_negotiated"
    indicates which servers will parse a request larger than
    32 bits. The NBD protocol does not yet have a way for a
    client to learn if the server will enforce an even
    smaller maximum cache size, although a future extension
    may add a constraint visible in "nbd.get_block_size".

    The "flags" parameter must be 0 for now (it exists for
    future NBD protocol extensions).

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.cache(self._o, count, offset, flags)

    def zero(self, count, offset, flags=0):
        u'''▶ send write zeroes command to the NBD server

    Issue a write zeroes command to the NBD server, which if
    supported by the server causes a zeroes to be written
    efficiently starting at "offset" and ending at "offset"
    + "count" - 1. The call returns when the command has
    been acknowledged by the server, or there is an error.
    Note this will generally return an error if
    "nbd.can_zero" is false or "nbd.is_read_only" is true.

    Note that not all servers can support a "count" of 4GiB
    or larger; "nbd.get_extended_headers_negotiated"
    indicates which servers will parse a request larger than
    32 bits. The NBD protocol does not yet have a way for a
    client to learn if the server will enforce an even
    smaller maximum zero size, although a future extension
    may add a constraint visible in "nbd.get_block_size".
    Also, some servers may permit a larger zero request only
    when the "CMD_FLAG_FAST_ZERO" is in use.

    The "flags" parameter may be 0 for no flags, or may
    contain "CMD_FLAG_FUA" meaning that the server should
    not return until the data has been committed to
    permanent storage (if that is supported - some servers
    cannot do this, see "nbd.can_fua"), "CMD_FLAG_NO_HOLE"
    meaning that the server should favor writing actual
    allocated zeroes over punching a hole, and/or
    "CMD_FLAG_FAST_ZERO" meaning that the server must fail
    quickly if writing zeroes is no faster than a normal
    write (if that is supported - some servers cannot do
    this, see "nbd.can_fast_zero").

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.zero(self._o, count, offset, flags)

    def block_status(self, count, offset, extent, flags=0):
        u'''▶ send block status command, with 32-bit callback

    Issue the block status command to the NBD server. If
    supported by the server, this causes metadata context
    information about blocks beginning from the specified
    offset to be returned. The "count" parameter is a hint:
    the server may choose to return less status, or the
    final block may extend beyond the requested range. If
    multiple contexts are supported, the number of blocks
    and cumulative length of those blocks need not be
    identical between contexts.

    Note that not all servers can support a "count" of 4GiB
    or larger; "nbd.get_extended_headers_negotiated"
    indicates which servers will parse a request larger than
    32 bits. The NBD protocol does not yet have a way for a
    client to learn if the server will enforce an even
    smaller maximum block status size, although a future
    extension may add a constraint visible in
    "nbd.get_block_size". Furthermore, this function is
    inherently limited to 32-bit values. If the server
    replies with a larger extent, the length of that extent
    will be truncated to just below 32 bits and any further
    extents from the server will be ignored. If the server
    replies with a status value larger than 32 bits (only
    possible when extended headers are in use), the callback
    function will be passed an "EOVERFLOW" error. To get the
    full extent information from a server that supports
    64-bit extents, you must use "nbd.block_status_64".

    Depending on which metadata contexts were enabled before
    connecting (see "nbd.add_meta_context") and which are
    supported by the server (see "nbd.can_meta_context")
    this call returns information about extents by calling
    back to the "extent" function. The callback cannot call
    "nbd_*" APIs on the same handle since it holds the
    handle lock and will cause a deadlock. If the callback
    returns -1, and no earlier error has been detected, then
    the overall block status command will fail with any
    non-zero value stored into the callback's "error"
    parameter (with a default of "EPROTO"); but any further
    contexts will still invoke the callback.

    The "extent" function is called once per type of
    metadata available, with the "user_data" passed to this
    function. The "metacontext" parameter is a string such
    as "base:allocation". The "entries" array is an array of
    pairs of integers with the first entry in each pair
    being the length (in bytes) of the block and the second
    entry being a status/flags field which is specific to
    the metadata context. The number of pairs passed to the
    function is "nr_entries/2". The NBD protocol document in
    the section about "NBD_REPLY_TYPE_BLOCK_STATUS"
    describes the meaning of this array; for contexts known
    to libnbd, <libnbd.h> contains constants beginning with
    "STATE_" that may help decipher the values. On entry to
    the callback, the "error" parameter contains the errno
    value of any previously detected error, but even if an
    earlier error was detected, the current "metacontext"
    and "entries" are valid.

    It is possible for the extent function to be called more
    times than you expect (if the server is buggy), so
    always check the "metacontext" field to ensure you are
    receiving the data you expect. It is also possible that
    the extent function is not called at all, even for
    metadata contexts that you requested. This indicates
    either that the server doesn't support the context or
    for some other reason cannot return the data.

    The "flags" parameter may be 0 for no flags, or may
    contain "CMD_FLAG_REQ_ONE" meaning that the server
    should return only one extent per metadata context where
    that extent does not exceed "count" bytes; however,
    libnbd does not validate that the server obeyed the
    flag.

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.block_status(self._o, count, offset, extent, flags)

    def block_status_64(self, count, offset, extent64, flags=0):
        u'''▶ send block status command, with 64-bit callback

    Issue the block status command to the NBD server. If
    supported by the server, this causes metadata context
    information about blocks beginning from the specified
    offset to be returned. The "count" parameter is a hint:
    the server may choose to return less status, or the
    final block may extend beyond the requested range. When
    multiple contexts are supported, the number of blocks
    and cumulative length of those blocks need not be
    identical between contexts; this command generally
    returns the status of all negotiated contexts, while
    some servers also support a filtered request (see
    "nbd.can_block_status_payload",
    "nbd.block_status_filter").

    Note that not all servers can support a "count" of 4GiB
    or larger; "nbd.get_extended_headers_negotiated"
    indicates which servers will parse a request larger than
    32 bits. The NBD protocol does not yet have a way for a
    client to learn if the server will enforce an even
    smaller maximum block status size, although a future
    extension may add a constraint visible in
    "nbd.get_block_size".

    Depending on which metadata contexts were enabled before
    connecting (see "nbd.add_meta_context") and which are
    supported by the server (see "nbd.can_meta_context")
    this call returns information about extents by calling
    back to the "extent64" function. The callback cannot
    call "nbd_*" APIs on the same handle since it holds the
    handle lock and will cause a deadlock. If the callback
    returns -1, and no earlier error has been detected, then
    the overall block status command will fail with any
    non-zero value stored into the callback's "error"
    parameter (with a default of "EPROTO"); but any further
    contexts will still invoke the callback.

    The "extent64" function is called once per type of
    metadata available, with the "user_data" passed to this
    function. The "metacontext" parameter is a string such
    as "base:allocation". The "entries" array is an array of
    nbd_extent structs, containing length (in bytes) of the
    block and a status/flags field which is specific to the
    metadata context. The number of array entries passed to
    the function is "nr_entries". The NBD protocol document
    in the section about "NBD_REPLY_TYPE_BLOCK_STATUS"
    describes the meaning of this array; for contexts known
    to libnbd, <libnbd.h> contains constants beginning with
    "STATE_" that may help decipher the values. On entry to
    the callback, the "error" parameter contains the errno
    value of any previously detected error.

    It is possible for the extent function to be called more
    times than you expect (if the server is buggy), so
    always check the "metacontext" field to ensure you are
    receiving the data you expect. It is also possible that
    the extent function is not called at all, even for
    metadata contexts that you requested. This indicates
    either that the server doesn't support the context or
    for some other reason cannot return the data.

    The "flags" parameter may be 0 for no flags, or may
    contain "CMD_FLAG_REQ_ONE" meaning that the server
    should return only one extent per metadata context where
    that extent does not exceed "count" bytes; however,
    libnbd does not validate that the server obeyed the
    flag.

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.block_status_64(self._o, count, offset, extent64,
                                         flags)

    def block_status_filter(self, count, offset, contexts, extent64,
                            flags=0):
        u'''▶ send filtered block status command, with 64-bit callback

    Issue a filtered block status command to the NBD server.
    If supported by the server (see
    "nbd.can_block_status_payload"), this causes metadata
    context information about blocks beginning from the
    specified offset to be returned, and with the result
    limited to just the contexts specified in "filter". Note
    that all strings in "filter" must be supported by
    "nbd.can_meta_context".

    All other parameters to this function have the same
    semantics as in "nbd.block_status_64"; except that for
    convenience, unless <nbd_set_strict_flags(3)> was used
    to disable "STRICT_AUTO_FLAG", libnbd ignores the
    presence or absence of the flag "CMD_FLAG_PAYLOAD_LEN"
    in "flags", while correctly using the flag over the
    wire.

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.block_status_filter(self._o, count, offset,
                                             contexts, extent64, flags)

    def poll(self, timeout):
        u'''▶ poll the handle once

    This is a simple implementation of poll(2) which is used
    internally by synchronous API calls. On success, it
    returns 0 if the "timeout" (in milliseconds) occurs, or
    1 if the poll completed and the state machine
    progressed. Set "timeout" to -1 to block indefinitely
    (but be careful that eventual action is actually
    expected - for example, if the connection is established
    but there are no commands in flight, using an infinite
    timeout will permanently block).

    This function is mainly useful as an example of how you
    might integrate libnbd with your own main loop, rather
    than being intended as something you would use.
'''
        self._check_not_closed()
        return libnbdmod.poll(self._o, timeout)

    def poll2(self, fd, timeout):
        u'''▶ poll the handle once, with fd

    This is the same as "nbd.poll", but an additional file
    descriptor parameter is passed. The additional fd is
    also polled (using "POLLIN"). One use for this is to
    wait for an eventfd(2).
'''
        self._check_not_closed()
        return libnbdmod.poll2(self._o, fd, timeout)

    def aio_connect(self, addr):
        u'''▶ connect to the NBD server

    Begin connecting to the NBD server. The "addr" and
    "addrlen" parameters specify the address of the socket
    to connect to.

    You can check if the connection attempt is still
    underway by calling "nbd.aio_is_connecting". If
    "nbd.set_opt_mode" is enabled, the connection is ready
    for manual option negotiation once
    "nbd.aio_is_negotiating" returns true; otherwise, the
    connection attempt will include the NBD handshake, and
    is ready for use once "nbd.aio_is_ready" returns true.
'''
        self._check_not_closed()
        return libnbdmod.aio_connect(self._o, addr)

    def aio_connect_uri(self, uri):
        u'''▶ connect to an NBD URI

    Begin connecting to the NBD URI "uri". Parameters behave
    as documented in "nbd.connect_uri".

    You can check if the connection attempt is still
    underway by calling "nbd.aio_is_connecting". If
    "nbd.set_opt_mode" is enabled, the connection is ready
    for manual option negotiation once
    "nbd.aio_is_negotiating" returns true; otherwise, the
    connection attempt will include the NBD handshake, and
    is ready for use once "nbd.aio_is_ready" returns true.
'''
        self._check_not_closed()
        return libnbdmod.aio_connect_uri(self._o, uri)

    def aio_connect_unix(self, unixsocket):
        u'''▶ connect to the NBD server over a Unix domain socket

    Begin connecting to the NBD server over Unix domain
    socket ("unixsocket"). Parameters behave as documented
    in "nbd.connect_unix".

    You can check if the connection attempt is still
    underway by calling "nbd.aio_is_connecting". If
    "nbd.set_opt_mode" is enabled, the connection is ready
    for manual option negotiation once
    "nbd.aio_is_negotiating" returns true; otherwise, the
    connection attempt will include the NBD handshake, and
    is ready for use once "nbd.aio_is_ready" returns true.
'''
        self._check_not_closed()
        return libnbdmod.aio_connect_unix(self._o, unixsocket)

    def aio_connect_vsock(self, cid, port):
        u'''▶ connect to the NBD server over AF_VSOCK socket

    Begin connecting to the NBD server over the "AF_VSOCK"
    protocol to the server "cid:port". Parameters behave as
    documented in "nbd.connect_vsock".

    You can check if the connection attempt is still
    underway by calling "nbd.aio_is_connecting". If
    "nbd.set_opt_mode" is enabled, the connection is ready
    for manual option negotiation once
    "nbd.aio_is_negotiating" returns true; otherwise, the
    connection attempt will include the NBD handshake, and
    is ready for use once "nbd.aio_is_ready" returns true.
'''
        self._check_not_closed()
        return libnbdmod.aio_connect_vsock(self._o, cid, port)

    def aio_connect_tcp(self, hostname, port):
        u'''▶ connect to the NBD server over a TCP port

    Begin connecting to the NBD server listening on
    "hostname:port". Parameters behave as documented in
    "nbd.connect_tcp".

    You can check if the connection attempt is still
    underway by calling "nbd.aio_is_connecting". If
    "nbd.set_opt_mode" is enabled, the connection is ready
    for manual option negotiation once
    "nbd.aio_is_negotiating" returns true; otherwise, the
    connection attempt will include the NBD handshake, and
    is ready for use once "nbd.aio_is_ready" returns true.
'''
        self._check_not_closed()
        return libnbdmod.aio_connect_tcp(self._o, hostname, port)

    def aio_connect_socket(self, sock):
        u'''▶ connect directly to a connected socket

    Begin connecting to the connected socket "fd".
    Parameters behave as documented in "nbd.connect_socket".

    You can check if the connection attempt is still
    underway by calling "nbd.aio_is_connecting". If
    "nbd.set_opt_mode" is enabled, the connection is ready
    for manual option negotiation once
    "nbd.aio_is_negotiating" returns true; otherwise, the
    connection attempt will include the NBD handshake, and
    is ready for use once "nbd.aio_is_ready" returns true.
'''
        self._check_not_closed()
        return libnbdmod.aio_connect_socket(self._o, sock)

    def aio_connect_command(self, argv):
        u'''▶ connect to the NBD server

    Run the command as a subprocess and begin connecting to
    it over stdin/stdout. Parameters behave as documented in
    "nbd.connect_command".

    You can check if the connection attempt is still
    underway by calling "nbd.aio_is_connecting". If
    "nbd.set_opt_mode" is enabled, the connection is ready
    for manual option negotiation once
    "nbd.aio_is_negotiating" returns true; otherwise, the
    connection attempt will include the NBD handshake, and
    is ready for use once "nbd.aio_is_ready" returns true.
'''
        self._check_not_closed()
        return libnbdmod.aio_connect_command(self._o, argv)

    def aio_connect_systemd_socket_activation(self, argv):
        u'''▶ connect using systemd socket activation

    Run the command as a subprocess and begin connecting to
    it using systemd socket activation. Parameters behave as
    documented in "nbd.connect_systemd_socket_activation".

    You can check if the connection attempt is still
    underway by calling "nbd.aio_is_connecting". If
    "nbd.set_opt_mode" is enabled, the connection is ready
    for manual option negotiation once
    "nbd.aio_is_negotiating" returns true; otherwise, the
    connection attempt will include the NBD handshake, and
    is ready for use once "nbd.aio_is_ready" returns true.
'''
        self._check_not_closed()
        return libnbdmod.aio_connect_systemd_socket_activation(self._o,
                                                               argv)

    def aio_opt_go(self, completion=None):
        u'''▶ end negotiation and move on to using an export

    Request that the server finish negotiation and move on
    to serving the export previously specified by the most
    recent "nbd.set_export_name" or "nbd.connect_uri". This
    can only be used if "nbd.set_opt_mode" enabled option
    mode.

    To determine when the request completes, wait for
    "nbd.aio_is_connecting" to return false. Or supply the
    optional "completion_callback" which will be invoked as
    described in "Completion callbacks" in libnbd(3), except
    that it is automatically retired regardless of return
    value. Note that directly detecting whether the server
    returns an error (as is done by the return value of the
    synchronous counterpart) is only possible with a
    completion callback; however it is also possible to
    indirectly detect an error when "nbd.aio_is_negotiating"
    returns true.
'''
        self._check_not_closed()
        return libnbdmod.aio_opt_go(self._o, completion)

    def aio_opt_abort(self):
        u'''▶ end negotiation and close the connection

    Request that the server finish negotiation, gracefully
    if possible, then close the connection. This can only be
    used if "nbd.set_opt_mode" enabled option mode.

    To determine when the request completes, wait for
    "nbd.aio_is_connecting" to return false.
'''
        self._check_not_closed()
        return libnbdmod.aio_opt_abort(self._o)

    def aio_opt_starttls(self, completion=None):
        u'''▶ request the server to initiate TLS

    Request that the server initiate a secure TLS
    connection, by sending "NBD_OPT_STARTTLS". This behaves
    like the synchronous counterpart "nbd.opt_starttls",
    except that it does not wait for the server's response.

    To determine when the request completes, wait for
    "nbd.aio_is_connecting" to return false. Or supply the
    optional "completion_callback" which will be invoked as
    described in "Completion callbacks" in libnbd(3), except
    that it is automatically retired regardless of return
    value. Note that detecting whether the server returns an
    error (as is done by the return value of the synchronous
    counterpart) is only possible with a completion
    callback.
'''
        self._check_not_closed()
        return libnbdmod.aio_opt_starttls(self._o, completion)

    def aio_opt_extended_headers(self, completion=None):
        u'''▶ request the server to enable extended headers

    Request that the server use extended headers, by sending
    "NBD_OPT_EXTENDED_HEADERS". This behaves like the
    synchronous counterpart "nbd.opt_extended_headers",
    except that it does not wait for the server's response.

    To determine when the request completes, wait for
    "nbd.aio_is_connecting" to return false. Or supply the
    optional "completion_callback" which will be invoked as
    described in "Completion callbacks" in libnbd(3), except
    that it is automatically retired regardless of return
    value. Note that detecting whether the server returns an
    error (as is done by the return value of the synchronous
    counterpart) is only possible with a completion
    callback.
'''
        self._check_not_closed()
        return libnbdmod.aio_opt_extended_headers(self._o, completion)

    def aio_opt_structured_reply(self, completion=None):
        u'''▶ request the server to enable structured replies

    Request that the server use structured replies, by
    sending "NBD_OPT_STRUCTURED_REPLY". This behaves like
    the synchronous counterpart "nbd.opt_structured_reply",
    except that it does not wait for the server's response.

    To determine when the request completes, wait for
    "nbd.aio_is_connecting" to return false. Or supply the
    optional "completion_callback" which will be invoked as
    described in "Completion callbacks" in libnbd(3), except
    that it is automatically retired regardless of return
    value. Note that detecting whether the server returns an
    error (as is done by the return value of the synchronous
    counterpart) is only possible with a completion
    callback.
'''
        self._check_not_closed()
        return libnbdmod.aio_opt_structured_reply(self._o, completion)

    def aio_opt_list(self, list, completion=None):
        u'''▶ request the server to list all exports during negotiation

    Request that the server list all exports that it
    supports. This can only be used if "nbd.set_opt_mode"
    enabled option mode.

    To determine when the request completes, wait for
    "nbd.aio_is_connecting" to return false. Or supply the
    optional "completion_callback" which will be invoked as
    described in "Completion callbacks" in libnbd(3), except
    that it is automatically retired regardless of return
    value. Note that detecting whether the server returns an
    error (as is done by the return value of the synchronous
    counterpart) is only possible with a completion
    callback.
'''
        self._check_not_closed()
        return libnbdmod.aio_opt_list(self._o, list, completion)

    def aio_opt_info(self, completion=None):
        u'''▶ request the server for information about an export

    Request that the server supply information about the
    export name previously specified by the most recent
    "nbd.set_export_name" or "nbd.connect_uri". This can
    only be used if "nbd.set_opt_mode" enabled option mode.

    To determine when the request completes, wait for
    "nbd.aio_is_connecting" to return false. Or supply the
    optional "completion_callback" which will be invoked as
    described in "Completion callbacks" in libnbd(3), except
    that it is automatically retired regardless of return
    value. Note that detecting whether the server returns an
    error (as is done by the return value of the synchronous
    counterpart) is only possible with a completion
    callback.
'''
        self._check_not_closed()
        return libnbdmod.aio_opt_info(self._o, completion)

    def aio_opt_list_meta_context(self, context, completion=None):
        u'''▶ request list of available meta contexts, using implicit query

    Request that the server list available meta contexts
    associated with the export previously specified by the
    most recent "nbd.set_export_name" or "nbd.connect_uri",
    and with a list of queries from prior calls to
    "nbd.add_meta_context" (see
    "nbd.aio_opt_list_meta_context_queries" if you want to
    supply an explicit query list instead). This can only be
    used if "nbd.set_opt_mode" enabled option mode.

    To determine when the request completes, wait for
    "nbd.aio_is_connecting" to return false. Or supply the
    optional "completion_callback" which will be invoked as
    described in "Completion callbacks" in libnbd(3), except
    that it is automatically retired regardless of return
    value. Note that detecting whether the server returns an
    error (as is done by the return value of the synchronous
    counterpart) is only possible with a completion
    callback.
'''
        self._check_not_closed()
        return libnbdmod.aio_opt_list_meta_context(self._o, context,
                                                   completion)

    def aio_opt_list_meta_context_queries(self, queries, context,
                                          completion=None):
        u'''▶ request list of available meta contexts, using explicit query

    Request that the server list available meta contexts
    associated with the export previously specified by the
    most recent "nbd.set_export_name" or "nbd.connect_uri",
    and with an explicit list of queries provided as a
    parameter (see "nbd.aio_opt_list_meta_context" if you
    want to reuse an implicit query list instead). This can
    only be used if "nbd.set_opt_mode" enabled option mode.

    To determine when the request completes, wait for
    "nbd.aio_is_connecting" to return false. Or supply the
    optional "completion_callback" which will be invoked as
    described in "Completion callbacks" in libnbd(3), except
    that it is automatically retired regardless of return
    value. Note that detecting whether the server returns an
    error (as is done by the return value of the synchronous
    counterpart) is only possible with a completion
    callback.
'''
        self._check_not_closed()
        return libnbdmod.aio_opt_list_meta_context_queries(self._o, queries,
                                                           context,
                                                           completion)

    def aio_opt_set_meta_context(self, context, completion=None):
        u'''▶ select specific meta contexts, with implicit query list

    Request that the server supply all recognized meta
    contexts registered through prior calls to
    "nbd.add_meta_context", in conjunction with the export
    previously specified by the most recent
    "nbd.set_export_name" or "nbd.connect_uri". This can
    only be used if "nbd.set_opt_mode" enabled option mode.
    Normally, this function is redundant, as "nbd.opt_go"
    automatically does the same task if structured replies
    or extended headers have already been negotiated. But
    manual control over meta context requests can be useful
    for fine-grained testing of how a server handles unusual
    negotiation sequences. Often, use of this function is
    coupled with "nbd.set_request_meta_context" to bypass
    the automatic context request normally performed by
    "nbd.opt_go".

    To determine when the request completes, wait for
    "nbd.aio_is_connecting" to return false. Or supply the
    optional "completion_callback" which will be invoked as
    described in "Completion callbacks" in libnbd(3), except
    that it is automatically retired regardless of return
    value. Note that detecting whether the server returns an
    error (as is done by the return value of the synchronous
    counterpart) is only possible with a completion
    callback.
'''
        self._check_not_closed()
        return libnbdmod.aio_opt_set_meta_context(self._o, context,
                                                  completion)

    def aio_opt_set_meta_context_queries(self, queries, context,
                                         completion=None):
        u'''▶ select specific meta contexts, with explicit query list

    Request that the server supply all recognized meta
    contexts passed in through "queries", in conjunction
    with the export previously specified by the most recent
    "nbd.set_export_name" or "nbd.connect_uri". This can
    only be used if "nbd.set_opt_mode" enabled option mode.
    Normally, this function is redundant, as "nbd.opt_go"
    automatically does the same task if structured replies
    or extended headers have already been negotiated. But
    manual control over meta context requests can be useful
    for fine-grained testing of how a server handles unusual
    negotiation sequences. Often, use of this function is
    coupled with "nbd.set_request_meta_context" to bypass
    the automatic context request normally performed by
    "nbd.opt_go".

    To determine when the request completes, wait for
    "nbd.aio_is_connecting" to return false. Or supply the
    optional "completion_callback" which will be invoked as
    described in "Completion callbacks" in libnbd(3), except
    that it is automatically retired regardless of return
    value. Note that detecting whether the server returns an
    error (as is done by the return value of the synchronous
    counterpart) is only possible with a completion
    callback.
'''
        self._check_not_closed()
        return libnbdmod.aio_opt_set_meta_context_queries(self._o, queries,
                                                          context,
                                                          completion)

    def aio_pread(self, buf, offset, completion=None, flags=0):
        u'''▶ read from the NBD server

    Issue a read command to the NBD server.

    To check if the command completed, call
    "nbd.aio_command_completed". Or supply the optional
    "completion_callback" which will be invoked as described
    in "Completion callbacks" in libnbd(3).

    Note that you must ensure "buf" is valid until the
    command has completed. Furthermore, if the "error"
    parameter to "completion_callback" is set or if
    "nbd.aio_command_completed" reports failure, and if
    "nbd.get_pread_initialize" returns true, then libnbd
    sanitized "buf", but it is unspecified whether the
    contents of "buf" will read as zero or as partial
    results from the server. If "nbd.get_pread_initialize"
    returns false, then libnbd did not sanitize "buf", and
    the contents are undefined on failure.

    Other parameters behave as documented in "nbd.pread".

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.aio_pread(self._o, buf, offset, completion, flags)

    def aio_pread_structured(self, buf, offset, chunk, completion=None,
                             flags=0):
        u'''▶ read from the NBD server

    Issue a read command to the NBD server.

    To check if the command completed, call
    "nbd.aio_command_completed". Or supply the optional
    "completion_callback" which will be invoked as described
    in "Completion callbacks" in libnbd(3).

    Note that you must ensure "buf" is valid until the
    command has completed. Furthermore, if the "error"
    parameter to "completion_callback" is set or if
    "nbd.aio_command_completed" reports failure, and if
    "nbd.get_pread_initialize" returns true, then libnbd
    sanitized "buf", but it is unspecified whether the
    contents of "buf" will read as zero or as partial
    results from the server. If "nbd.get_pread_initialize"
    returns false, then libnbd did not sanitize "buf", and
    the contents are undefined on failure.

    Other parameters behave as documented in
    "nbd.pread_structured".

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.aio_pread_structured(self._o, buf, offset, chunk,
                                              completion, flags)

    def aio_pwrite(self, buf, offset, completion=None, flags=0):
        u'''▶ write to the NBD server

    Issue a write command to the NBD server.

    To check if the command completed, call
    "nbd.aio_command_completed". Or supply the optional
    "completion_callback" which will be invoked as described
    in "Completion callbacks" in libnbd(3).

    Note that you must ensure "buf" is valid until the
    command has completed. Other parameters behave as
    documented in "nbd.pwrite".

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.aio_pwrite(self._o, buf, offset, completion, flags)

    def aio_disconnect(self, flags=0):
        u'''▶ disconnect from the NBD server

    Issue the disconnect command to the NBD server. This is
    not a normal command because NBD servers are not obliged
    to send a reply. Instead you should wait for
    "nbd.aio_is_closed" to become true on the connection.
    Once this command is issued, you cannot issue any
    further commands.

    Although libnbd does not prevent you from issuing this
    command while still waiting on the replies to previous
    commands, the NBD protocol recommends that you wait
    until there are no other commands in flight (see
    "nbd.aio_in_flight"), to give the server a better chance
    at a clean shutdown.

    The "flags" parameter must be 0 for now (it exists for
    future NBD protocol extensions). There is no direct
    synchronous counterpart; however, "nbd.shutdown" will
    call this function if appropriate.
'''
        self._check_not_closed()
        return libnbdmod.aio_disconnect(self._o, flags)

    def aio_flush(self, completion=None, flags=0):
        u'''▶ send flush command to the NBD server

    Issue the flush command to the NBD server.

    To check if the command completed, call
    "nbd.aio_command_completed". Or supply the optional
    "completion_callback" which will be invoked as described
    in "Completion callbacks" in libnbd(3).

    Other parameters behave as documented in "nbd.flush".

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.aio_flush(self._o, completion, flags)

    def aio_trim(self, count, offset, completion=None, flags=0):
        u'''▶ send trim command to the NBD server

    Issue a trim command to the NBD server.

    To check if the command completed, call
    "nbd.aio_command_completed". Or supply the optional
    "completion_callback" which will be invoked as described
    in "Completion callbacks" in libnbd(3).

    Other parameters behave as documented in "nbd.trim".

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.aio_trim(self._o, count, offset, completion, flags)

    def aio_cache(self, count, offset, completion=None, flags=0):
        u'''▶ send cache (prefetch) command to the NBD server

    Issue the cache (prefetch) command to the NBD server.

    To check if the command completed, call
    "nbd.aio_command_completed". Or supply the optional
    "completion_callback" which will be invoked as described
    in "Completion callbacks" in libnbd(3).

    Other parameters behave as documented in "nbd.cache".

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.aio_cache(self._o, count, offset, completion,
                                   flags)

    def aio_zero(self, count, offset, completion=None, flags=0):
        u'''▶ send write zeroes command to the NBD server

    Issue a write zeroes command to the NBD server.

    To check if the command completed, call
    "nbd.aio_command_completed". Or supply the optional
    "completion_callback" which will be invoked as described
    in "Completion callbacks" in libnbd(3).

    Other parameters behave as documented in "nbd.zero".

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.aio_zero(self._o, count, offset, completion, flags)

    def aio_block_status(self, count, offset, extent, completion=None,
                         flags=0):
        u'''▶ send block status command, with 32-bit callback

    Send the block status command to the NBD server.

    To check if the command completed, call
    "nbd.aio_command_completed". Or supply the optional
    "completion_callback" which will be invoked as described
    in "Completion callbacks" in libnbd(3).

    Other parameters behave as documented in
    "nbd.block_status".

    This function is inherently limited to 32-bit values. If
    the server replies with a larger extent, the length of
    that extent will be truncated to just below 32 bits and
    any further extents from the server will be ignored. If
    the server replies with a status value larger than 32
    bits (only possible when extended headers are in use),
    the callback function will be passed an "EOVERFLOW"
    error. To get the full extent information from a server
    that supports 64-bit extents, you must use
    "nbd.aio_block_status_64".

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.aio_block_status(self._o, count, offset, extent,
                                          completion, flags)

    def aio_block_status_64(self, count, offset, extent64, completion=None,
                            flags=0):
        u'''▶ send block status command, with 64-bit callback

    Send the block status command to the NBD server.

    To check if the command completed, call
    "nbd.aio_command_completed". Or supply the optional
    "completion_callback" which will be invoked as described
    in "Completion callbacks" in libnbd(3).

    Other parameters behave as documented in
    "nbd.block_status_64".

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.aio_block_status_64(self._o, count, offset,
                                             extent64, completion, flags)

    def aio_block_status_filter(self, count, offset, contexts, extent64,
                                completion=None, flags=0):
        u'''▶ send filtered block status command to the NBD server

    Send a filtered block status command to the NBD server.

    To check if the command completed, call
    "nbd.aio_command_completed". Or supply the optional
    "completion_callback" which will be invoked as described
    in "Completion callbacks" in libnbd(3).

    Other parameters behave as documented in
    "nbd.block_status_filter".

    By default, libnbd will reject attempts to use this
    function with parameters that are likely to result in
    server failure, such as requesting an unknown command
    flag. The "nbd.set_strict_mode" function can be used to
    alter which scenarios should await a server reply rather
    than failing fast.
'''
        self._check_not_closed()
        return libnbdmod.aio_block_status_filter(self._o, count, offset,
                                                 contexts, extent64,
                                                 completion, flags)

    def aio_get_fd(self):
        u'''▶ return file descriptor associated with this connection

    Return the underlying file descriptor associated with
    this connection. You can use this to check if the file
    descriptor is ready for reading or writing and call
    "nbd.aio_notify_read" or "nbd.aio_notify_write". See
    also "nbd.aio_get_direction". Do not do anything else
    with the file descriptor.
'''
        self._check_not_closed()
        return libnbdmod.aio_get_fd(self._o)

    def aio_get_direction(self):
        u'''▶ return the read or write direction

    Return the current direction of this connection, which
    means whether we are next expecting to read data from
    the server, write data to the server, or both. It
    returns

    0   We are not expected to interact with the server file
        descriptor from the current state. It is not worth
        attempting to use poll(2); if the connection is not
        dead, then state machine progress must instead come
        from some other means such as "nbd.aio_connect".

    "AIO_DIRECTION_READ" = 1
        We are expected next to read from the server. If
        using poll(2) you would set "events = POLLIN". If
        "revents" returns "POLLIN" or "POLLHUP" you would
        then call "nbd.aio_notify_read".

        Note that once libnbd reaches "nbd.aio_is_ready",
        this direction is returned even when there are no
        commands in flight (see "nbd.aio_in_flight"). In a
        single-threaded use of libnbd, it is not worth
        polling until after issuing a command, as otherwise
        the server will never wake up the poll. In a
        multi-threaded scenario, you can have one thread
        begin a polling loop prior to any commands, but any
        other thread that issues a command will need a way
        to kick the polling thread out of poll in case
        issuing the command changes the needed polling
        direction. Possible ways to do this include polling
        for activity on a pipe-to-self, or using
        pthread_kill(3) to send a signal that is masked
        except during ppoll(2).

    "AIO_DIRECTION_WRITE" = 2
        We are expected next to write to the server. If
        using poll(2) you would set "events = POLLOUT". If
        "revents" returns "POLLOUT" you would then call
        "nbd.aio_notify_write".

    "AIO_DIRECTION_BOTH" = 3
        We are expected next to either read or write to the
        server. If using poll(2) you would set "events =
        POLLIN|POLLOUT". If only one of "POLLIN" or
        "POLLOUT" is returned, then see above. However, if
        both are returned, it is better to call only
        "nbd.aio_notify_read", as processing the server's
        reply may change the state of the connection and
        invalidate the need to write more commands.
'''
        self._check_not_closed()
        return libnbdmod.aio_get_direction(self._o)

    def aio_notify_read(self):
        u'''▶ notify that the connection is readable

    Send notification to the state machine that the
    connection is readable. Typically this is called after
    your main loop has detected that the file descriptor
    associated with this connection is readable.
'''
        self._check_not_closed()
        return libnbdmod.aio_notify_read(self._o)

    def aio_notify_write(self):
        u'''▶ notify that the connection is writable

    Send notification to the state machine that the
    connection is writable. Typically this is called after
    your main loop has detected that the file descriptor
    associated with this connection is writable.
'''
        self._check_not_closed()
        return libnbdmod.aio_notify_write(self._o)

    def aio_is_created(self):
        u'''▶ check if the connection has just been created

    Return true if this connection has just been created.
    This is the state before the handle has started
    connecting to a server. In this state the handle can
    start to be connected by calling functions such as
    "nbd.aio_connect".
'''
        self._check_not_closed()
        return libnbdmod.aio_is_created(self._o)

    def aio_is_connecting(self):
        u'''▶ check if the connection is connecting or handshaking

    Return true if this connection is connecting to the
    server or in the process of handshaking and negotiating
    options which happens before the handle becomes ready to
    issue commands (see "nbd.aio_is_ready").
'''
        self._check_not_closed()
        return libnbdmod.aio_is_connecting(self._o)

    def aio_is_negotiating(self):
        u'''▶ check if connection is ready to send handshake option

    Return true if this connection is ready to start another
    option negotiation command while handshaking with the
    server. An option command will move back to the
    connecting state (see "nbd.aio_is_connecting"). Note
    that this state cannot be reached unless requested by
    "nbd.set_opt_mode", and even then it only works with
    newstyle servers; an oldstyle server will skip straight
    to "nbd.aio_is_ready".
'''
        self._check_not_closed()
        return libnbdmod.aio_is_negotiating(self._o)

    def aio_is_ready(self):
        u'''▶ check if the connection is in the ready state

    Return true if this connection is connected to the NBD
    server, the handshake has completed, and the connection
    is idle or waiting for a reply. In this state the handle
    is ready to issue commands.
'''
        self._check_not_closed()
        return libnbdmod.aio_is_ready(self._o)

    def aio_is_processing(self):
        u'''▶ check if the connection is processing a command

    Return true if this connection is connected to the NBD
    server, the handshake has completed, and the connection
    is processing commands (either writing out a request or
    reading a reply).

    Note the ready state ("nbd.aio_is_ready") is not
    included. In the ready state commands may be *in flight*
    (the *server* is processing them), but libnbd is not
    processing them.
'''
        self._check_not_closed()
        return libnbdmod.aio_is_processing(self._o)

    def aio_is_dead(self):
        u'''▶ check if the connection is dead

    Return true if the connection has encountered a fatal
    error and is dead. In this state the handle may only be
    closed. There is no way to recover a handle from the
    dead state.
'''
        self._check_not_closed()
        return libnbdmod.aio_is_dead(self._o)

    def aio_is_closed(self):
        u'''▶ check if the connection is closed

    Return true if the connection has closed. There is no
    way to reconnect a closed connection. Instead you must
    close the whole handle.
'''
        self._check_not_closed()
        return libnbdmod.aio_is_closed(self._o)

    def aio_command_completed(self, cookie):
        u'''▶ check if the command completed

    Return true if the command completed. If this function
    returns true then the command was successful and it has
    been retired. Return false if the command is still in
    flight. This can also fail with an error in case the
    command failed (in this case the command is also
    retired). A command is retired either via this command,
    or by using a completion callback which returns 1.

    The "cookie" parameter is the positive unique 64 bit
    cookie for the command, as returned by a call such as
    "nbd.aio_pread".
'''
        self._check_not_closed()
        return libnbdmod.aio_command_completed(self._o, cookie)

    def aio_peek_command_completed(self):
        u'''▶ check if any command has completed

    Return the unique positive 64 bit cookie of the first
    non-retired but completed command, 0 if there are
    in-flight commands but none of them are awaiting
    retirement, or -1 on error including when there are no
    in-flight commands. Any cookie returned by this function
    must still be passed to "nbd.aio_command_completed" to
    actually retire the command and learn whether the
    command was successful.
'''
        self._check_not_closed()
        return libnbdmod.aio_peek_command_completed(self._o)

    def aio_in_flight(self):
        u'''▶ check how many aio commands are still in flight

    Return the number of in-flight aio commands that are
    still awaiting a response from the server before they
    can be retired. If this returns a non-zero value when
    requesting a disconnect from the server (see
    "nbd.aio_disconnect" and "nbd.shutdown"), libnbd does
    not try to wait for those commands to complete
    gracefully; if the server strands commands while
    shutting down, "nbd.aio_command_completed" will report
    those commands as failed with a status of "ENOTCONN".
'''
        self._check_not_closed()
        return libnbdmod.aio_in_flight(self._o)

    def connection_state(self):
        u'''▶ return string describing the state of the connection

    Returns a descriptive string for the state of the
    connection. This can be used for debugging or
    troubleshooting, but you should not rely on the state of
    connections since it may change in future versions.
'''
        self._check_not_closed()
        return libnbdmod.connection_state(self._o)

    def get_package_name(self):
        u'''▶ return the name of the library

    Returns the name of the library, always "libnbd" unless
    the library was modified with another name at compile
    time.
'''
        self._check_not_closed()
        return libnbdmod.get_package_name(self._o)

    def get_version(self):
        u'''▶ return the version of the library

    Return the version of libnbd. This is returned as a
    string in the form "major.minor.release" where each of
    major, minor and release is a small positive integer.
    For example:

         minor
           ↓
        "1.0.3"
         ↑   ↑
     major   release

    major = 0
        The major number was 0 for the early experimental
        versions of libnbd where we still had an unstable
        API.

    major = 1
        The major number is 1 for the versions of libnbd
        with a long-term stable API and ABI. It is not
        anticipated that major will be any number other than
        1.

    minor = 0, 2, ... (even)
        The minor number is even for stable releases.

    minor = 1, 3, ... (odd)
        The minor number is odd for development versions.
        Note that new APIs added in a development version
        remain experimental and subject to change in that
        branch until they appear in a stable release.

    release
        The release number is incremented for each release
        along a particular branch.
'''
        self._check_not_closed()
        return libnbdmod.get_version(self._o)

    def kill_subprocess(self, signum):
        u'''▶ kill server running as a subprocess

    This call may be used to kill the server running as a
    subprocess that was previously created using
    "nbd.connect_command". You do not need to use this call.
    It is only needed if the server does not exit when the
    socket is closed.

    The "signum" parameter is the optional signal number to
    send (see signal(7)). If "signum" is 0 then "SIGTERM" is
    sent.
'''
        self._check_not_closed()
        return libnbdmod.kill_subprocess(self._o, signum)

    def get_subprocess_pid(self):
        u'''▶ get the process ID of the subprocess

    For connections which create a subprocess such as
    "nbd.connect_command", this returns the process ID (PID)
    of the subprocess. This is only supported on some
    platforms.

    This is mainly useful in debugging cases. For example,
    this could be used to learn where to attach gdb(1) to
    diagnose a crash in the NBD server subprocess.
'''
        self._check_not_closed()
        return libnbdmod.get_subprocess_pid(self._o)

    def supports_tls(self):
        u'''▶ true if libnbd was compiled with support for TLS

    Returns true if libnbd was compiled with gnutls which is
    required to support TLS encryption, or false if not.
'''
        self._check_not_closed()
        return libnbdmod.supports_tls(self._o)

    def supports_vsock(self):
        u'''▶ true if libnbd was compiled with support for AF_VSOCK

    Returns true if libnbd was compiled with support for the
    "AF_VSOCK" family of sockets, or false if not.

    Note that on the Linux operating system, this returns
    true if there is compile-time support, but you may still
    need runtime support for some aspects of AF_VSOCK usage;
    for example, use of "VMADDR_CID_LOCAL" as the server
    name requires that the *vsock_loopback* kernel module is
    loaded.
'''
        self._check_not_closed()
        return libnbdmod.supports_vsock(self._o)

    def supports_uri(self):
        u'''▶ true if libnbd was compiled with support for NBD URIs

    Returns true if libnbd was compiled with libxml2 which
    is required to support NBD URIs, or false if not.
'''
        self._check_not_closed()
        return libnbdmod.supports_uri(self._o)

    def get_uri(self):
        u'''▶ construct an NBD URI for a connection

    This makes a best effort attempt to construct an NBD URI
    which could be used to connect back to the same server
    (using "nbd.connect_uri").

    In some cases there is not enough information in the
    handle to successfully create a URI (eg. if you
    connected with "nbd.connect_socket"). In such cases the
    call returns "NULL" and further diagnostic information
    is available via "nbd.get_errno" and "nbd.get_error" as
    usual.

    Even if a URI is returned it is not guaranteed to work,
    and it may not be optimal.

    nbdinfo(1) *--uri* option is a way to access this API
    from shell scripts.
'''
        self._check_not_closed()
        return libnbdmod.get_uri(self._o)

    def is_uri(self, uri):
        u'''▶ detect if a string could be an NBD URI

    Detect if the parameter "uri" could be an NBD URI or
    not. The function returns true if "uri" is likely to be
    an NBD URI, or false if not.

    This can be used to write programs that take either a
    URI or something else like a filename as a parameter.
    nbdcopy(1) is one such program.

    The current test is heuristic. In particular it *does
    not* guarantee that "nbd.connect_uri" will work.
'''
        self._check_not_closed()
        return libnbdmod.is_uri(self._o, uri)


package_name = NBD().get_package_name()
__version__ = NBD().get_version()

if __name__ == "__main__":
    import nbdsh

    nbdsh.shell()
