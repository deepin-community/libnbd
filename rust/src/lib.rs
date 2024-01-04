// nbd client library in userspace
// Copyright Tage Johansson
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#![deny(warnings)]

#[cfg(feature = "tokio")]
mod async_bindings;
#[cfg(feature = "tokio")]
mod async_handle;
mod bindings;
mod error;
mod handle;
pub mod types;
#[allow(unused)]
mod utils;
#[cfg(feature = "tokio")]
pub use async_bindings::*;
#[cfg(feature = "tokio")]
pub use async_handle::{AsyncHandle, SharedResult};
pub use bindings::*;
pub use error::{Error, ErrorKind, FatalErrorKind, Result};
pub use handle::Handle;
pub(crate) use libnbd_sys as sys;
