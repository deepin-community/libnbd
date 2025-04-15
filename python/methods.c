/* NBD client library in userspace
 * WARNING: THIS FILE IS GENERATED FROM
 * generator/generator
 * ANY CHANGES YOU MAKE TO THIS FILE WILL BE LOST.
 *
 * Copyright Red Hat
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#define PY_SSIZE_T_CLEAN 1
#include <Python.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <libnbd.h>

#include <methods.h>

/* This is passed to *_wrapper as the user_data pointer
 * and freed in the free_user_data function below.
 */
struct user_data {
  PyObject *fn;    /* Optional pointer to Python function. */
  PyObject *view;  /* Optional PyMemoryView of persistent buffer. */
};

static struct user_data *
alloc_user_data (void)
{
  struct user_data *data = calloc (1, sizeof *data);
  if (data == NULL) {
    PyErr_NoMemory ();
    return NULL;
  }
  return data;
}

static void
free_user_data (void *user_data)
{
  struct user_data *data = user_data;

  if (data) {
    PyGILState_STATE py_save = PyGILState_Ensure ();
    Py_XDECREF (data->fn);
    Py_XDECREF (data->view);
    PyGILState_Release (py_save);
    free (data);
  }
}

/* Wrapper for chunk callback. */
static int
chunk_wrapper (void *user_data, const void *subbuf, size_t count,
               uint64_t offset, unsigned status, int *error)
{
  const struct user_data *data = user_data;
  int ret = -1;

  PyGILState_STATE py_save = PyGILState_Ensure ();
  PyObject *py_args, *py_ret;
  PyObject *py_subbuf = NULL;
  PyObject *py_error = NULL;

  py_subbuf = nbd_internal_py_get_subview (data->view, subbuf, count);
  if (!py_subbuf) { PyErr_PrintEx (0); goto out; }
  py_error = nbd_internal_py_wrap_errptr (*error);
  if (!py_error) { PyErr_PrintEx (0); goto out; }

  py_args = Py_BuildValue ("(OKIO)", py_subbuf, offset, status, py_error);
  if (!py_args) { PyErr_PrintEx (0); goto out; }

  py_ret = PyObject_CallObject (data->fn, py_args);

  Py_DECREF (py_args);

  if (py_ret != NULL) {
    if (PyLong_Check (py_ret))
      ret = PyLong_AsLong (py_ret);
    else
      /* If it's not a long, just assume it's 0. */
      ret = 0;
    Py_DECREF (py_ret);
  }
  else {
    /* Special case failed assertions to be fatal. */
    if (PyErr_ExceptionMatches (PyExc_AssertionError)) {
      PyErr_Print ();
      abort ();
    }
    PyErr_PrintEx (0); /* print exception */
  };

 out:
  Py_XDECREF (py_subbuf);
  if (py_error) {
    PyObject *py_error_ret = PyObject_GetAttrString (py_error, "value");
    *error = PyLong_AsLong (py_error_ret);
    Py_DECREF (py_error_ret);
    Py_DECREF (py_error);
  }
  PyGILState_Release (py_save);
  return ret;
}

/* Wrapper for completion callback. */
static int
completion_wrapper (void *user_data, int *error)
{
  const struct user_data *data = user_data;
  int ret = -1;

  PyGILState_STATE py_save = PyGILState_Ensure ();
  PyObject *py_args, *py_ret;
  PyObject *py_error = NULL;

  py_error = nbd_internal_py_wrap_errptr (*error);
  if (!py_error) { PyErr_PrintEx (0); goto out; }

  py_args = Py_BuildValue ("(O)", py_error);
  if (!py_args) { PyErr_PrintEx (0); goto out; }

  py_ret = PyObject_CallObject (data->fn, py_args);

  Py_DECREF (py_args);

  if (py_ret != NULL) {
    if (PyLong_Check (py_ret))
      ret = PyLong_AsLong (py_ret);
    else
      /* If it's not a long, just assume it's 0. */
      ret = 0;
    Py_DECREF (py_ret);
  }
  else {
    /* Special case failed assertions to be fatal. */
    if (PyErr_ExceptionMatches (PyExc_AssertionError)) {
      PyErr_Print ();
      abort ();
    }
    PyErr_PrintEx (0); /* print exception */
  };

 out:
  if (py_error) {
    PyObject *py_error_ret = PyObject_GetAttrString (py_error, "value");
    *error = PyLong_AsLong (py_error_ret);
    Py_DECREF (py_error_ret);
    Py_DECREF (py_error);
  }
  PyGILState_Release (py_save);
  return ret;
}

/* Wrapper for debug callback. */
static int
debug_wrapper (void *user_data, const char *context, const char *msg)
{
  const struct user_data *data = user_data;
  int ret = -1;

  PyGILState_STATE py_save = PyGILState_Ensure ();
  PyObject *py_args, *py_ret;


  py_args = Py_BuildValue ("(ss)", context, msg);
  if (!py_args) { PyErr_PrintEx (0); goto out; }

  py_ret = PyObject_CallObject (data->fn, py_args);

  Py_DECREF (py_args);

  if (py_ret != NULL) {
    if (PyLong_Check (py_ret))
      ret = PyLong_AsLong (py_ret);
    else
      /* If it's not a long, just assume it's 0. */
      ret = 0;
    Py_DECREF (py_ret);
  }
  else {
    /* Special case failed assertions to be fatal. */
    if (PyErr_ExceptionMatches (PyExc_AssertionError)) {
      PyErr_Print ();
      abort ();
    }
    PyErr_PrintEx (0); /* print exception */
  };

 out:
  PyGILState_Release (py_save);
  return ret;
}

/* Wrapper for extent callback. */
static int
extent_wrapper (void *user_data, const char *metacontext, uint64_t offset,
                uint32_t *entries, size_t nr_entries, int *error)
{
  const struct user_data *data = user_data;
  int ret = -1;

  PyGILState_STATE py_save = PyGILState_Ensure ();
  PyObject *py_args, *py_ret;
  PyObject *py_entries = NULL;
  PyObject *py_error = NULL;

  py_entries = PyList_New (nr_entries);
  if (!py_entries) { PyErr_PrintEx (0); goto out; }
  size_t i_entries;
  for (i_entries = 0; i_entries < nr_entries; ++i_entries) {
    PyObject *py_e_entries = PyLong_FromUnsignedLong (entries[i_entries]);
    if (!py_e_entries) { PyErr_PrintEx (0); goto out; }
    PyList_SET_ITEM (py_entries, i_entries, py_e_entries);
  }
  py_error = nbd_internal_py_wrap_errptr (*error);
  if (!py_error) { PyErr_PrintEx (0); goto out; }

  py_args = Py_BuildValue ("(sKOO)", metacontext, offset, py_entries,
                           py_error);
  if (!py_args) { PyErr_PrintEx (0); goto out; }

  py_ret = PyObject_CallObject (data->fn, py_args);

  Py_DECREF (py_args);

  if (py_ret != NULL) {
    if (PyLong_Check (py_ret))
      ret = PyLong_AsLong (py_ret);
    else
      /* If it's not a long, just assume it's 0. */
      ret = 0;
    Py_DECREF (py_ret);
  }
  else {
    /* Special case failed assertions to be fatal. */
    if (PyErr_ExceptionMatches (PyExc_AssertionError)) {
      PyErr_Print ();
      abort ();
    }
    PyErr_PrintEx (0); /* print exception */
  };

 out:
  Py_XDECREF (py_entries);
  if (py_error) {
    PyObject *py_error_ret = PyObject_GetAttrString (py_error, "value");
    *error = PyLong_AsLong (py_error_ret);
    Py_DECREF (py_error_ret);
    Py_DECREF (py_error);
  }
  PyGILState_Release (py_save);
  return ret;
}

/* Wrapper for extent64 callback. */
static int
extent64_wrapper (void *user_data, const char *metacontext, uint64_t offset,
                  nbd_extent *entries, size_t nr_entries, int *error)
{
  const struct user_data *data = user_data;
  int ret = -1;

  PyGILState_STATE py_save = PyGILState_Ensure ();
  PyObject *py_args, *py_ret;
  PyObject *py_entries = NULL;
  PyObject *py_error = NULL;

  py_entries = PyList_New (nr_entries);
  if (!py_entries) { PyErr_PrintEx (0); goto out; }
  size_t i_entries;
  for (i_entries = 0; i_entries < nr_entries; ++i_entries) {
    PyObject *py_e_entries = Py_BuildValue ("KK", entries[i_entries].length,
                                            entries[i_entries].flags);
    if (!py_e_entries) { PyErr_PrintEx (0); goto out; }
    PyList_SET_ITEM (py_entries, i_entries, py_e_entries);
  }
  py_error = nbd_internal_py_wrap_errptr (*error);
  if (!py_error) { PyErr_PrintEx (0); goto out; }

  py_args = Py_BuildValue ("(sKOO)", metacontext, offset, py_entries,
                           py_error);
  if (!py_args) { PyErr_PrintEx (0); goto out; }

  py_ret = PyObject_CallObject (data->fn, py_args);

  Py_DECREF (py_args);

  if (py_ret != NULL) {
    if (PyLong_Check (py_ret))
      ret = PyLong_AsLong (py_ret);
    else
      /* If it's not a long, just assume it's 0. */
      ret = 0;
    Py_DECREF (py_ret);
  }
  else {
    /* Special case failed assertions to be fatal. */
    if (PyErr_ExceptionMatches (PyExc_AssertionError)) {
      PyErr_Print ();
      abort ();
    }
    PyErr_PrintEx (0); /* print exception */
  };

 out:
  Py_XDECREF (py_entries);
  if (py_error) {
    PyObject *py_error_ret = PyObject_GetAttrString (py_error, "value");
    *error = PyLong_AsLong (py_error_ret);
    Py_DECREF (py_error_ret);
    Py_DECREF (py_error);
  }
  PyGILState_Release (py_save);
  return ret;
}

/* Wrapper for list callback. */
static int
list_wrapper (void *user_data, const char *name, const char *description)
{
  const struct user_data *data = user_data;
  int ret = -1;

  PyGILState_STATE py_save = PyGILState_Ensure ();
  PyObject *py_args, *py_ret;


  py_args = Py_BuildValue ("(ss)", name, description);
  if (!py_args) { PyErr_PrintEx (0); goto out; }

  py_ret = PyObject_CallObject (data->fn, py_args);

  Py_DECREF (py_args);

  if (py_ret != NULL) {
    if (PyLong_Check (py_ret))
      ret = PyLong_AsLong (py_ret);
    else
      /* If it's not a long, just assume it's 0. */
      ret = 0;
    Py_DECREF (py_ret);
  }
  else {
    /* Special case failed assertions to be fatal. */
    if (PyErr_ExceptionMatches (PyExc_AssertionError)) {
      PyErr_Print ();
      abort ();
    }
    PyErr_PrintEx (0); /* print exception */
  };

 out:
  PyGILState_Release (py_save);
  return ret;
}

/* Wrapper for context callback. */
static int
context_wrapper (void *user_data, const char *name)
{
  const struct user_data *data = user_data;
  int ret = -1;

  PyGILState_STATE py_save = PyGILState_Ensure ();
  PyObject *py_args, *py_ret;


  py_args = Py_BuildValue ("(s)", name);
  if (!py_args) { PyErr_PrintEx (0); goto out; }

  py_ret = PyObject_CallObject (data->fn, py_args);

  Py_DECREF (py_args);

  if (py_ret != NULL) {
    if (PyLong_Check (py_ret))
      ret = PyLong_AsLong (py_ret);
    else
      /* If it's not a long, just assume it's 0. */
      ret = 0;
    Py_DECREF (py_ret);
  }
  else {
    /* Special case failed assertions to be fatal. */
    if (PyErr_ExceptionMatches (PyExc_AssertionError)) {
      PyErr_Print ();
      abort ();
    }
    PyErr_PrintEx (0); /* print exception */
  };

 out:
  PyGILState_Release (py_save);
  return ret;
}

PyObject *
nbd_internal_py_set_debug (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int debug;

  if (!PyArg_ParseTuple (args, "Op:nbd_set_debug", &py_h, &debug))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_debug (h, debug);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_debug (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_debug", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_debug (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_debug_callback (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  struct user_data *debug_user_data = NULL;
  PyObject *py_debug_fn;
  nbd_debug_callback debug = { .callback = debug_wrapper,
                               .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OO:nbd_set_debug_callback", &py_h,
                         &py_debug_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  debug.user_data = debug_user_data = alloc_user_data ();
  if (debug_user_data == NULL) goto out;
  if (!PyCallable_Check (py_debug_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter debug is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_debug_fn);
  debug_user_data->fn = py_debug_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_debug_callback (h, debug);
  Py_END_ALLOW_THREADS;
  debug_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  free_user_data (debug_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_clear_debug_callback (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_clear_debug_callback", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_clear_debug_callback (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_stats_bytes_sent (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  uint64_t ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_stats_bytes_sent", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_stats_bytes_sent (h);
  Py_END_ALLOW_THREADS;
  py_ret = PyLong_FromLongLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_stats_chunks_sent (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  uint64_t ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_stats_chunks_sent", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_stats_chunks_sent (h);
  Py_END_ALLOW_THREADS;
  py_ret = PyLong_FromLongLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_stats_bytes_received (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  uint64_t ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_stats_bytes_received", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_stats_bytes_received (h);
  Py_END_ALLOW_THREADS;
  py_ret = PyLong_FromLongLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_stats_chunks_received (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  uint64_t ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_stats_chunks_received", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_stats_chunks_received (h);
  Py_END_ALLOW_THREADS;
  py_ret = PyLong_FromLongLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_handle_name (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  const char *handle_name;

  if (!PyArg_ParseTuple (args, "Os:nbd_set_handle_name", &py_h,
                         &handle_name))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_handle_name (h, handle_name);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_handle_name (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  char * ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_handle_name", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_handle_name (h);
  Py_END_ALLOW_THREADS;
  if (ret == NULL) {
    raise_exception ();
    goto out;
  }
  py_ret = PyUnicode_FromString (ret);
  free (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_private_data (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  uintptr_t ret;
  PyObject *py_ret = NULL;
  unsigned int private_data;

  if (!PyArg_ParseTuple (args, "OI:nbd_set_private_data", &py_h,
                         &private_data))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_private_data (h, private_data);
  Py_END_ALLOW_THREADS;
  py_ret = PyLong_FromLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_private_data (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  uintptr_t ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_private_data", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_private_data (h);
  Py_END_ALLOW_THREADS;
  py_ret = PyLong_FromLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_export_name (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  const char *export_name;

  if (!PyArg_ParseTuple (args, "Os:nbd_set_export_name", &py_h,
                         &export_name))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_export_name (h, export_name);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_export_name (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  char * ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_export_name", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_export_name (h);
  Py_END_ALLOW_THREADS;
  if (ret == NULL) {
    raise_exception ();
    goto out;
  }
  py_ret = PyUnicode_FromString (ret);
  free (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_request_block_size (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int request;

  if (!PyArg_ParseTuple (args, "Op:nbd_set_request_block_size", &py_h,
                         &request))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_request_block_size (h, request);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_request_block_size (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_request_block_size", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_request_block_size (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_full_info (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int request;

  if (!PyArg_ParseTuple (args, "Op:nbd_set_full_info", &py_h, &request))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_full_info (h, request);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_full_info (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_full_info", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_full_info (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_canonical_export_name (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  char * ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_canonical_export_name", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_canonical_export_name (h);
  Py_END_ALLOW_THREADS;
  if (ret == NULL) {
    raise_exception ();
    goto out;
  }
  py_ret = PyUnicode_FromString (ret);
  free (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_export_description (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  char * ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_export_description", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_export_description (h);
  Py_END_ALLOW_THREADS;
  if (ret == NULL) {
    raise_exception ();
    goto out;
  }
  py_ret = PyUnicode_FromString (ret);
  free (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_tls (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int tls;

  if (!PyArg_ParseTuple (args, "Oi:nbd_set_tls", &py_h, &tls))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_tls (h, tls);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_tls (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_tls", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_tls (h);
  Py_END_ALLOW_THREADS;
  py_ret = PyLong_FromLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_tls_negotiated (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_tls_negotiated", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_tls_negotiated (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_tls_certificates (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *py_dir = NULL;
  char *dir = NULL;

  if (!PyArg_ParseTuple (args, "OO&:nbd_set_tls_certificates", &py_h,
                         PyUnicode_FSConverter, &py_dir))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  dir = PyBytes_AS_STRING (py_dir);
  assert (dir != NULL);

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_tls_certificates (h, dir);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  Py_XDECREF (py_dir);
  return py_ret;
}

PyObject *
nbd_internal_py_set_tls_verify_peer (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int verify;

  if (!PyArg_ParseTuple (args, "Op:nbd_set_tls_verify_peer", &py_h, &verify))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_tls_verify_peer (h, verify);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_tls_verify_peer (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_tls_verify_peer", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_tls_verify_peer (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_tls_username (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  const char *username;

  if (!PyArg_ParseTuple (args, "Os:nbd_set_tls_username", &py_h, &username))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_tls_username (h, username);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_tls_username (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  char * ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_tls_username", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_tls_username (h);
  Py_END_ALLOW_THREADS;
  if (ret == NULL) {
    raise_exception ();
    goto out;
  }
  py_ret = PyUnicode_FromString (ret);
  free (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_tls_hostname (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  const char *hostname;

  if (!PyArg_ParseTuple (args, "Os:nbd_set_tls_hostname", &py_h, &hostname))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_tls_hostname (h, hostname);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_tls_hostname (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  char * ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_tls_hostname", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_tls_hostname (h);
  Py_END_ALLOW_THREADS;
  if (ret == NULL) {
    raise_exception ();
    goto out;
  }
  py_ret = PyUnicode_FromString (ret);
  free (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_tls_psk_file (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *py_filename = NULL;
  char *filename = NULL;

  if (!PyArg_ParseTuple (args, "OO&:nbd_set_tls_psk_file", &py_h,
                         PyUnicode_FSConverter, &py_filename))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  filename = PyBytes_AS_STRING (py_filename);
  assert (filename != NULL);

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_tls_psk_file (h, filename);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  Py_XDECREF (py_filename);
  return py_ret;
}

PyObject *
nbd_internal_py_set_request_extended_headers (PyObject *self,
                                              PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int request;

  if (!PyArg_ParseTuple (args, "Op:nbd_set_request_extended_headers", &py_h,
                         &request))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_request_extended_headers (h, request);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_request_extended_headers (PyObject *self,
                                              PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_request_extended_headers", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_request_extended_headers (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_extended_headers_negotiated (PyObject *self,
                                                 PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_extended_headers_negotiated",
                         &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_extended_headers_negotiated (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_request_structured_replies (PyObject *self,
                                                PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int request;

  if (!PyArg_ParseTuple (args, "Op:nbd_set_request_structured_replies",
                         &py_h, &request))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_request_structured_replies (h, request);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_request_structured_replies (PyObject *self,
                                                PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_request_structured_replies",
                         &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_request_structured_replies (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_structured_replies_negotiated (PyObject *self,
                                                   PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_structured_replies_negotiated",
                         &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_structured_replies_negotiated (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_request_meta_context (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int request;

  if (!PyArg_ParseTuple (args, "Op:nbd_set_request_meta_context", &py_h,
                         &request))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_request_meta_context (h, request);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_request_meta_context (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_request_meta_context", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_request_meta_context (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_handshake_flags (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OI:nbd_set_handshake_flags", &py_h, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  flags_u32 = flags;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_handshake_flags (h, flags_u32);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_handshake_flags (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  uint32_t ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_handshake_flags", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_handshake_flags (h);
  Py_END_ALLOW_THREADS;
  py_ret = PyLong_FromLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_pread_initialize (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int request;

  if (!PyArg_ParseTuple (args, "Op:nbd_set_pread_initialize", &py_h,
                         &request))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_pread_initialize (h, request);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_pread_initialize (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_pread_initialize", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_pread_initialize (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_strict_mode (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OI:nbd_set_strict_mode", &py_h, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  flags_u32 = flags;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_strict_mode (h, flags_u32);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_strict_mode (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  uint32_t ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_strict_mode", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_strict_mode (h);
  Py_END_ALLOW_THREADS;
  py_ret = PyLong_FromLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_opt_mode (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int enable;

  if (!PyArg_ParseTuple (args, "Op:nbd_set_opt_mode", &py_h, &enable))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_opt_mode (h, enable);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_opt_mode (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_opt_mode", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_opt_mode (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_opt_go (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_opt_go", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_opt_go (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_opt_abort (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_opt_abort", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_opt_abort (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_opt_starttls (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_opt_starttls", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_opt_starttls (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_opt_extended_headers (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_opt_extended_headers", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_opt_extended_headers (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_opt_structured_reply (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_opt_structured_reply", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_opt_structured_reply (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_opt_list (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  struct user_data *list_user_data = NULL;
  PyObject *py_list_fn;
  nbd_list_callback list = { .callback = list_wrapper,
                             .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OO:nbd_opt_list", &py_h, &py_list_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  list.user_data = list_user_data = alloc_user_data ();
  if (list_user_data == NULL) goto out;
  if (!PyCallable_Check (py_list_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter list is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_list_fn);
  list_user_data->fn = py_list_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_opt_list (h, list);
  Py_END_ALLOW_THREADS;
  list_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLong (ret);

 out:
  free_user_data (list_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_opt_info (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_opt_info", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_opt_info (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_opt_list_meta_context (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  struct user_data *context_user_data = NULL;
  PyObject *py_context_fn;
  nbd_context_callback context = { .callback = context_wrapper,
                                   .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OO:nbd_opt_list_meta_context", &py_h,
                         &py_context_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  context.user_data = context_user_data = alloc_user_data ();
  if (context_user_data == NULL) goto out;
  if (!PyCallable_Check (py_context_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter context is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_context_fn);
  context_user_data->fn = py_context_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_opt_list_meta_context (h, context);
  Py_END_ALLOW_THREADS;
  context_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLong (ret);

 out:
  free_user_data (context_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_opt_list_meta_context_queries (PyObject *self,
                                               PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *py_queries;
  char **queries = NULL;
  struct user_data *context_user_data = NULL;
  PyObject *py_context_fn;
  nbd_context_callback context = { .callback = context_wrapper,
                                   .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OOO:nbd_opt_list_meta_context_queries",
                         &py_h, &py_queries, &py_context_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  queries = nbd_internal_py_get_string_list (py_queries);
  if (!queries) goto out;
  context.user_data = context_user_data = alloc_user_data ();
  if (context_user_data == NULL) goto out;
  if (!PyCallable_Check (py_context_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter context is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_context_fn);
  context_user_data->fn = py_context_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_opt_list_meta_context_queries (h, queries, context);
  Py_END_ALLOW_THREADS;
  context_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLong (ret);

 out:
  nbd_internal_py_free_string_list (queries);
  free_user_data (context_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_opt_set_meta_context (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  struct user_data *context_user_data = NULL;
  PyObject *py_context_fn;
  nbd_context_callback context = { .callback = context_wrapper,
                                   .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OO:nbd_opt_set_meta_context", &py_h,
                         &py_context_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  context.user_data = context_user_data = alloc_user_data ();
  if (context_user_data == NULL) goto out;
  if (!PyCallable_Check (py_context_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter context is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_context_fn);
  context_user_data->fn = py_context_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_opt_set_meta_context (h, context);
  Py_END_ALLOW_THREADS;
  context_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLong (ret);

 out:
  free_user_data (context_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_opt_set_meta_context_queries (PyObject *self,
                                              PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *py_queries;
  char **queries = NULL;
  struct user_data *context_user_data = NULL;
  PyObject *py_context_fn;
  nbd_context_callback context = { .callback = context_wrapper,
                                   .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OOO:nbd_opt_set_meta_context_queries",
                         &py_h, &py_queries, &py_context_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  queries = nbd_internal_py_get_string_list (py_queries);
  if (!queries) goto out;
  context.user_data = context_user_data = alloc_user_data ();
  if (context_user_data == NULL) goto out;
  if (!PyCallable_Check (py_context_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter context is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_context_fn);
  context_user_data->fn = py_context_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_opt_set_meta_context_queries (h, queries, context);
  Py_END_ALLOW_THREADS;
  context_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLong (ret);

 out:
  nbd_internal_py_free_string_list (queries);
  free_user_data (context_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_add_meta_context (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  const char *name;

  if (!PyArg_ParseTuple (args, "Os:nbd_add_meta_context", &py_h, &name))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_add_meta_context (h, name);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_nr_meta_contexts (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  ssize_t ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_nr_meta_contexts", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_nr_meta_contexts (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_meta_context (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  char * ret;
  PyObject *py_ret = NULL;
  Py_ssize_t i;

  if (!PyArg_ParseTuple (args, "On:nbd_get_meta_context", &py_h, &i))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_meta_context (h, (size_t)i);
  Py_END_ALLOW_THREADS;
  if (ret == NULL) {
    raise_exception ();
    goto out;
  }
  py_ret = PyUnicode_FromString (ret);
  free (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_clear_meta_contexts (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_clear_meta_contexts", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_clear_meta_contexts (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_uri_allow_transports (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint32_t mask_u32;
  unsigned int mask; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OI:nbd_set_uri_allow_transports", &py_h,
                         &mask))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  mask_u32 = mask;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_uri_allow_transports (h, mask_u32);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_uri_allow_tls (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int tls;

  if (!PyArg_ParseTuple (args, "Oi:nbd_set_uri_allow_tls", &py_h, &tls))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_uri_allow_tls (h, tls);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_set_uri_allow_local_file (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int allow;

  if (!PyArg_ParseTuple (args, "Op:nbd_set_uri_allow_local_file", &py_h,
                         &allow))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_uri_allow_local_file (h, allow);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_connect_uri (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  const char *uri;

  if (!PyArg_ParseTuple (args, "Os:nbd_connect_uri", &py_h, &uri))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_connect_uri (h, uri);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_connect_unix (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *py_unixsocket = NULL;
  char *unixsocket = NULL;

  if (!PyArg_ParseTuple (args, "OO&:nbd_connect_unix", &py_h,
                         PyUnicode_FSConverter, &py_unixsocket))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  unixsocket = PyBytes_AS_STRING (py_unixsocket);
  assert (unixsocket != NULL);

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_connect_unix (h, unixsocket);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  Py_XDECREF (py_unixsocket);
  return py_ret;
}

PyObject *
nbd_internal_py_connect_vsock (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint32_t cid_u32;
  unsigned int cid; /* really uint32_t */
  uint32_t port_u32;
  unsigned int port; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OII:nbd_connect_vsock", &py_h, &cid, &port))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  cid_u32 = cid;
  port_u32 = port;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_connect_vsock (h, cid_u32, port_u32);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_connect_tcp (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  const char *hostname;
  const char *port;

  if (!PyArg_ParseTuple (args, "Oss:nbd_connect_tcp", &py_h, &hostname,
                         &port))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_connect_tcp (h, hostname, port);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_connect_socket (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int sock;

  if (!PyArg_ParseTuple (args, "Oi:nbd_connect_socket", &py_h, &sock))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_connect_socket (h, sock);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_connect_command (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *py_argv;
  char **argv = NULL;

  if (!PyArg_ParseTuple (args, "OO:nbd_connect_command", &py_h, &py_argv))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  argv = nbd_internal_py_get_string_list (py_argv);
  if (!argv) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_connect_command (h, argv);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  nbd_internal_py_free_string_list (argv);
  return py_ret;
}

PyObject *
nbd_internal_py_connect_systemd_socket_activation (PyObject *self,
                                                   PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *py_argv;
  char **argv = NULL;

  if (!PyArg_ParseTuple (args, "OO:nbd_connect_systemd_socket_activation",
                         &py_h, &py_argv))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  argv = nbd_internal_py_get_string_list (py_argv);
  if (!argv) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_connect_systemd_socket_activation (h, argv);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  nbd_internal_py_free_string_list (argv);
  return py_ret;
}

PyObject *
nbd_internal_py_set_socket_activation_name (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  const char *socket_name;

  if (!PyArg_ParseTuple (args, "Os:nbd_set_socket_activation_name", &py_h,
                         &socket_name))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_set_socket_activation_name (h, socket_name);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_socket_activation_name (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  char * ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_socket_activation_name", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_socket_activation_name (h);
  Py_END_ALLOW_THREADS;
  if (ret == NULL) {
    raise_exception ();
    goto out;
  }
  py_ret = PyUnicode_FromString (ret);
  free (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_is_read_only (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_is_read_only", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_is_read_only (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_can_flush (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_can_flush", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_can_flush (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_can_fua (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_can_fua", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_can_fua (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_is_rotational (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_is_rotational", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_is_rotational (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_can_trim (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_can_trim", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_can_trim (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_can_zero (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_can_zero", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_can_zero (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_can_fast_zero (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_can_fast_zero", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_can_fast_zero (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_can_block_status_payload (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_can_block_status_payload", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_can_block_status_payload (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_can_df (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_can_df", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_can_df (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_can_multi_conn (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_can_multi_conn", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_can_multi_conn (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_can_cache (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_can_cache", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_can_cache (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_can_meta_context (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  const char *metacontext;

  if (!PyArg_ParseTuple (args, "Os:nbd_can_meta_context", &py_h,
                         &metacontext))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_can_meta_context (h, metacontext);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_protocol (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  const char * ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_protocol", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_protocol (h);
  Py_END_ALLOW_THREADS;
  if (ret == NULL) {
    raise_exception ();
    goto out;
  }
  py_ret = PyUnicode_FromString (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_size (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int64_t ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_size", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_size (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLongLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_block_size (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int64_t ret;
  PyObject *py_ret = NULL;
  int size_type;

  if (!PyArg_ParseTuple (args, "Oi:nbd_get_block_size", &py_h, &size_type))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_block_size (h, size_type);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLongLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_pread (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *buf = NULL;
  Py_ssize_t count;
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OnKI:nbd_pread", &py_h, &count, &offset,
                         &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  flags_u32 = flags;
  buf = PyByteArray_FromStringAndSize (NULL, count);
  if (buf == NULL) goto out;
  offset_u64 = offset;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_pread (h, PyByteArray_AS_STRING (buf), count, offset_u64,
                   flags_u32);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = buf;
  buf = NULL;

 out:
  Py_XDECREF (buf);
  return py_ret;
}

PyObject *
nbd_internal_py_pread_structured (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *buf = NULL;
  Py_ssize_t count;
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  struct user_data *chunk_user_data = NULL;
  PyObject *py_chunk_fn;
  nbd_chunk_callback chunk = { .callback = chunk_wrapper,
                               .free = free_user_data };
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OnKOI:nbd_pread_structured", &py_h, &count,
                         &offset, &py_chunk_fn, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  flags_u32 = flags;
  buf = PyByteArray_FromStringAndSize (NULL, count);
  if (buf == NULL) goto out;
  offset_u64 = offset;
  chunk.user_data = chunk_user_data = alloc_user_data ();
  if (chunk_user_data == NULL) goto out;
  if (!PyCallable_Check (py_chunk_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter chunk is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_chunk_fn);
  chunk_user_data->fn = py_chunk_fn;
  chunk_user_data->view = nbd_internal_py_get_aio_view (buf, PyBUF_WRITE);
  if (!chunk_user_data->view) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_pread_structured (h, PyByteArray_AS_STRING (buf), count,
                              offset_u64, chunk, flags_u32);
  Py_END_ALLOW_THREADS;
  chunk_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = buf;
  buf = NULL;

 out:
  Py_XDECREF (buf);
  free_user_data (chunk_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_pwrite (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  Py_buffer buf = { .obj = NULL };
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "Oy*KI:nbd_pwrite", &py_h, &buf, &offset,
                         &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  flags_u32 = flags;
  offset_u64 = offset;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_pwrite (h, buf.buf, buf.len, offset_u64, flags_u32);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  if (buf.obj)
    PyBuffer_Release (&buf);
  return py_ret;
}

PyObject *
nbd_internal_py_shutdown (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OI:nbd_shutdown", &py_h, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  flags_u32 = flags;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_shutdown (h, flags_u32);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_flush (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OI:nbd_flush", &py_h, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  flags_u32 = flags;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_flush (h, flags_u32);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_trim (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint64_t count_u64;
  unsigned long long count; /* really uint64_t */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OKKI:nbd_trim", &py_h, &count, &offset,
                         &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  flags_u32 = flags;
  count_u64 = count;
  offset_u64 = offset;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_trim (h, count_u64, offset_u64, flags_u32);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_cache (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint64_t count_u64;
  unsigned long long count; /* really uint64_t */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OKKI:nbd_cache", &py_h, &count, &offset,
                         &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  flags_u32 = flags;
  count_u64 = count;
  offset_u64 = offset;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_cache (h, count_u64, offset_u64, flags_u32);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_zero (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint64_t count_u64;
  unsigned long long count; /* really uint64_t */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OKKI:nbd_zero", &py_h, &count, &offset,
                         &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  flags_u32 = flags;
  count_u64 = count;
  offset_u64 = offset;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_zero (h, count_u64, offset_u64, flags_u32);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_block_status (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint64_t count_u64;
  unsigned long long count; /* really uint64_t */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  struct user_data *extent_user_data = NULL;
  PyObject *py_extent_fn;
  nbd_extent_callback extent = { .callback = extent_wrapper,
                                 .free = free_user_data };
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OKKOI:nbd_block_status", &py_h, &count,
                         &offset, &py_extent_fn, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  flags_u32 = flags;
  count_u64 = count;
  offset_u64 = offset;
  extent.user_data = extent_user_data = alloc_user_data ();
  if (extent_user_data == NULL) goto out;
  if (!PyCallable_Check (py_extent_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter extent is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_extent_fn);
  extent_user_data->fn = py_extent_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_block_status (h, count_u64, offset_u64, extent, flags_u32);
  Py_END_ALLOW_THREADS;
  extent_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  free_user_data (extent_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_block_status_64 (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint64_t count_u64;
  unsigned long long count; /* really uint64_t */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  struct user_data *extent64_user_data = NULL;
  PyObject *py_extent64_fn;
  nbd_extent64_callback extent64 = { .callback = extent64_wrapper,
                                     .free = free_user_data };
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OKKOI:nbd_block_status_64", &py_h, &count,
                         &offset, &py_extent64_fn, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  flags_u32 = flags;
  count_u64 = count;
  offset_u64 = offset;
  extent64.user_data = extent64_user_data = alloc_user_data ();
  if (extent64_user_data == NULL) goto out;
  if (!PyCallable_Check (py_extent64_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter extent64 is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_extent64_fn);
  extent64_user_data->fn = py_extent64_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_block_status_64 (h, count_u64, offset_u64, extent64, flags_u32);
  Py_END_ALLOW_THREADS;
  extent64_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  free_user_data (extent64_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_block_status_filter (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint64_t count_u64;
  unsigned long long count; /* really uint64_t */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  PyObject *py_contexts;
  char **contexts = NULL;
  struct user_data *extent64_user_data = NULL;
  PyObject *py_extent64_fn;
  nbd_extent64_callback extent64 = { .callback = extent64_wrapper,
                                     .free = free_user_data };
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OKKOOI:nbd_block_status_filter", &py_h,
                         &count, &offset, &py_contexts, &py_extent64_fn,
                         &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  flags_u32 = flags;
  count_u64 = count;
  offset_u64 = offset;
  contexts = nbd_internal_py_get_string_list (py_contexts);
  if (!contexts) goto out;
  extent64.user_data = extent64_user_data = alloc_user_data ();
  if (extent64_user_data == NULL) goto out;
  if (!PyCallable_Check (py_extent64_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter extent64 is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_extent64_fn);
  extent64_user_data->fn = py_extent64_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_block_status_filter (h, count_u64, offset_u64, contexts,
                                 extent64, flags_u32);
  Py_END_ALLOW_THREADS;
  extent64_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  nbd_internal_py_free_string_list (contexts);
  free_user_data (extent64_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_poll (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int timeout;

  if (!PyArg_ParseTuple (args, "Oi:nbd_poll", &py_h, &timeout))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_poll (h, timeout);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_poll2 (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int fd;
  int timeout;

  if (!PyArg_ParseTuple (args, "Oii:nbd_poll2", &py_h, &fd, &timeout))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_poll2 (h, fd, timeout);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_connect (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *addr;
  struct sockaddr_storage addr_sa;
  socklen_t addr_len;

  if (!PyArg_ParseTuple (args, "OO:nbd_aio_connect", &py_h, &addr))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  if (nbd_internal_py_get_sockaddr (addr, &addr_sa, &addr_len) == -1)
    goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_connect (h, (struct sockaddr *)&addr_sa, addr_len);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_connect_uri (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  const char *uri;

  if (!PyArg_ParseTuple (args, "Os:nbd_aio_connect_uri", &py_h, &uri))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_connect_uri (h, uri);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_connect_unix (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *py_unixsocket = NULL;
  char *unixsocket = NULL;

  if (!PyArg_ParseTuple (args, "OO&:nbd_aio_connect_unix", &py_h,
                         PyUnicode_FSConverter, &py_unixsocket))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  unixsocket = PyBytes_AS_STRING (py_unixsocket);
  assert (unixsocket != NULL);

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_connect_unix (h, unixsocket);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  Py_XDECREF (py_unixsocket);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_connect_vsock (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint32_t cid_u32;
  unsigned int cid; /* really uint32_t */
  uint32_t port_u32;
  unsigned int port; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OII:nbd_aio_connect_vsock", &py_h, &cid,
                         &port))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  cid_u32 = cid;
  port_u32 = port;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_connect_vsock (h, cid_u32, port_u32);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_connect_tcp (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  const char *hostname;
  const char *port;

  if (!PyArg_ParseTuple (args, "Oss:nbd_aio_connect_tcp", &py_h, &hostname,
                         &port))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_connect_tcp (h, hostname, port);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_connect_socket (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int sock;

  if (!PyArg_ParseTuple (args, "Oi:nbd_aio_connect_socket", &py_h, &sock))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_connect_socket (h, sock);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_connect_command (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *py_argv;
  char **argv = NULL;

  if (!PyArg_ParseTuple (args, "OO:nbd_aio_connect_command", &py_h,
                         &py_argv))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  argv = nbd_internal_py_get_string_list (py_argv);
  if (!argv) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_connect_command (h, argv);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  nbd_internal_py_free_string_list (argv);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_connect_systemd_socket_activation (PyObject *self,
                                                       PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *py_argv;
  char **argv = NULL;

  if (!PyArg_ParseTuple (args,
                         "OO:nbd_aio_connect_systemd_socket_activation",
                         &py_h, &py_argv))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  argv = nbd_internal_py_get_string_list (py_argv);
  if (!argv) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_connect_systemd_socket_activation (h, argv);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  nbd_internal_py_free_string_list (argv);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_opt_go (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OO:nbd_aio_opt_go", &py_h,
                         &py_completion_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_opt_go (h, completion);
  Py_END_ALLOW_THREADS;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_opt_abort (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_aio_opt_abort", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_opt_abort (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_opt_starttls (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OO:nbd_aio_opt_starttls", &py_h,
                         &py_completion_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_opt_starttls (h, completion);
  Py_END_ALLOW_THREADS;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_opt_extended_headers (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OO:nbd_aio_opt_extended_headers", &py_h,
                         &py_completion_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_opt_extended_headers (h, completion);
  Py_END_ALLOW_THREADS;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_opt_structured_reply (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OO:nbd_aio_opt_structured_reply", &py_h,
                         &py_completion_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_opt_structured_reply (h, completion);
  Py_END_ALLOW_THREADS;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_opt_list (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  struct user_data *list_user_data = NULL;
  PyObject *py_list_fn;
  nbd_list_callback list = { .callback = list_wrapper,
                             .free = free_user_data };
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OOO:nbd_aio_opt_list", &py_h, &py_list_fn,
                         &py_completion_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  list.user_data = list_user_data = alloc_user_data ();
  if (list_user_data == NULL) goto out;
  if (!PyCallable_Check (py_list_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter list is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_list_fn);
  list_user_data->fn = py_list_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_opt_list (h, list, completion);
  Py_END_ALLOW_THREADS;
  list_user_data = NULL;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  free_user_data (list_user_data);
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_opt_info (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OO:nbd_aio_opt_info", &py_h,
                         &py_completion_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_opt_info (h, completion);
  Py_END_ALLOW_THREADS;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_opt_list_meta_context (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  struct user_data *context_user_data = NULL;
  PyObject *py_context_fn;
  nbd_context_callback context = { .callback = context_wrapper,
                                   .free = free_user_data };
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OOO:nbd_aio_opt_list_meta_context", &py_h,
                         &py_context_fn, &py_completion_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  context.user_data = context_user_data = alloc_user_data ();
  if (context_user_data == NULL) goto out;
  if (!PyCallable_Check (py_context_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter context is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_context_fn);
  context_user_data->fn = py_context_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_opt_list_meta_context (h, context, completion);
  Py_END_ALLOW_THREADS;
  context_user_data = NULL;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLong (ret);

 out:
  free_user_data (context_user_data);
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_opt_list_meta_context_queries (PyObject *self,
                                                   PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *py_queries;
  char **queries = NULL;
  struct user_data *context_user_data = NULL;
  PyObject *py_context_fn;
  nbd_context_callback context = { .callback = context_wrapper,
                                   .free = free_user_data };
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OOOO:nbd_aio_opt_list_meta_context_queries",
                         &py_h, &py_queries, &py_context_fn,
                         &py_completion_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  queries = nbd_internal_py_get_string_list (py_queries);
  if (!queries) goto out;
  context.user_data = context_user_data = alloc_user_data ();
  if (context_user_data == NULL) goto out;
  if (!PyCallable_Check (py_context_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter context is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_context_fn);
  context_user_data->fn = py_context_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_opt_list_meta_context_queries (h, queries, context,
                                               completion);
  Py_END_ALLOW_THREADS;
  context_user_data = NULL;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLong (ret);

 out:
  nbd_internal_py_free_string_list (queries);
  free_user_data (context_user_data);
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_opt_set_meta_context (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  struct user_data *context_user_data = NULL;
  PyObject *py_context_fn;
  nbd_context_callback context = { .callback = context_wrapper,
                                   .free = free_user_data };
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OOO:nbd_aio_opt_set_meta_context", &py_h,
                         &py_context_fn, &py_completion_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  context.user_data = context_user_data = alloc_user_data ();
  if (context_user_data == NULL) goto out;
  if (!PyCallable_Check (py_context_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter context is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_context_fn);
  context_user_data->fn = py_context_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_opt_set_meta_context (h, context, completion);
  Py_END_ALLOW_THREADS;
  context_user_data = NULL;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLong (ret);

 out:
  free_user_data (context_user_data);
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_opt_set_meta_context_queries (PyObject *self,
                                                  PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  PyObject *py_queries;
  char **queries = NULL;
  struct user_data *context_user_data = NULL;
  PyObject *py_context_fn;
  nbd_context_callback context = { .callback = context_wrapper,
                                   .free = free_user_data };
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };

  if (!PyArg_ParseTuple (args, "OOOO:nbd_aio_opt_set_meta_context_queries",
                         &py_h, &py_queries, &py_context_fn,
                         &py_completion_fn))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  queries = nbd_internal_py_get_string_list (py_queries);
  if (!queries) goto out;
  context.user_data = context_user_data = alloc_user_data ();
  if (context_user_data == NULL) goto out;
  if (!PyCallable_Check (py_context_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter context is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_context_fn);
  context_user_data->fn = py_context_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_opt_set_meta_context_queries (h, queries, context,
                                              completion);
  Py_END_ALLOW_THREADS;
  context_user_data = NULL;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLong (ret);

 out:
  nbd_internal_py_free_string_list (queries);
  free_user_data (context_user_data);
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_pread (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int64_t ret;
  PyObject *py_ret = NULL;
  PyObject *buf; /* Buffer-like object or nbd.Buffer */
  PyObject *buf_view = NULL; /* PyMemoryView of buf */
  Py_buffer *py_buf; /* buffer of buf_view */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OOKOI:nbd_aio_pread", &py_h, &buf, &offset,
                         &py_completion_fn, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  flags_u32 = flags;
  buf_view = nbd_internal_py_get_aio_view (buf, PyBUF_WRITE);
  if (!buf_view) goto out;
  py_buf = PyMemoryView_GET_BUFFER (buf_view);
  completion_user_data->view = buf_view;
  offset_u64 = offset;

  if (nbd_internal_py_init_aio_buffer (buf) < 0) goto out;
  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_pread (h, py_buf->buf, py_buf->len, offset_u64, completion,
                       flags_u32);
  Py_END_ALLOW_THREADS;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLongLong (ret);

 out:
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_pread_structured (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int64_t ret;
  PyObject *py_ret = NULL;
  PyObject *buf; /* Buffer-like object or nbd.Buffer */
  PyObject *buf_view = NULL; /* PyMemoryView of buf */
  Py_buffer *py_buf; /* buffer of buf_view */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  struct user_data *chunk_user_data = NULL;
  PyObject *py_chunk_fn;
  nbd_chunk_callback chunk = { .callback = chunk_wrapper,
                               .free = free_user_data };
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OOKOOI:nbd_aio_pread_structured", &py_h,
                         &buf, &offset, &py_chunk_fn, &py_completion_fn,
                         &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  flags_u32 = flags;
  buf_view = nbd_internal_py_get_aio_view (buf, PyBUF_WRITE);
  if (!buf_view) goto out;
  py_buf = PyMemoryView_GET_BUFFER (buf_view);
  completion_user_data->view = buf_view;
  offset_u64 = offset;
  chunk.user_data = chunk_user_data = alloc_user_data ();
  if (chunk_user_data == NULL) goto out;
  if (!PyCallable_Check (py_chunk_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter chunk is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_chunk_fn);
  chunk_user_data->fn = py_chunk_fn;
  chunk_user_data->view = nbd_internal_py_get_aio_view (buf, PyBUF_WRITE);
  if (!chunk_user_data->view) goto out;

  if (nbd_internal_py_init_aio_buffer (buf) < 0) goto out;
  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_pread_structured (h, py_buf->buf, py_buf->len, offset_u64,
                                  chunk, completion, flags_u32);
  Py_END_ALLOW_THREADS;
  chunk_user_data = NULL;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLongLong (ret);

 out:
  free_user_data (chunk_user_data);
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_pwrite (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int64_t ret;
  PyObject *py_ret = NULL;
  PyObject *buf; /* Buffer-like object or nbd.Buffer */
  PyObject *buf_view = NULL; /* PyMemoryView of buf */
  Py_buffer *py_buf; /* buffer of buf_view */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OOKOI:nbd_aio_pwrite", &py_h, &buf, &offset,
                         &py_completion_fn, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  flags_u32 = flags;
  buf_view = nbd_internal_py_get_aio_view (buf, PyBUF_READ);
  if (!buf_view) goto out;
  py_buf = PyMemoryView_GET_BUFFER (buf_view);
  completion_user_data->view = buf_view;
  offset_u64 = offset;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_pwrite (h, py_buf->buf, py_buf->len, offset_u64, completion,
                        flags_u32);
  Py_END_ALLOW_THREADS;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLongLong (ret);

 out:
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_disconnect (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OI:nbd_aio_disconnect", &py_h, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  flags_u32 = flags;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_disconnect (h, flags_u32);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_flush (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int64_t ret;
  PyObject *py_ret = NULL;
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OOI:nbd_aio_flush", &py_h,
                         &py_completion_fn, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  flags_u32 = flags;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_flush (h, completion, flags_u32);
  Py_END_ALLOW_THREADS;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLongLong (ret);

 out:
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_trim (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int64_t ret;
  PyObject *py_ret = NULL;
  uint64_t count_u64;
  unsigned long long count; /* really uint64_t */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OKKOI:nbd_aio_trim", &py_h, &count, &offset,
                         &py_completion_fn, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  flags_u32 = flags;
  count_u64 = count;
  offset_u64 = offset;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_trim (h, count_u64, offset_u64, completion, flags_u32);
  Py_END_ALLOW_THREADS;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLongLong (ret);

 out:
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_cache (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int64_t ret;
  PyObject *py_ret = NULL;
  uint64_t count_u64;
  unsigned long long count; /* really uint64_t */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OKKOI:nbd_aio_cache", &py_h, &count,
                         &offset, &py_completion_fn, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  flags_u32 = flags;
  count_u64 = count;
  offset_u64 = offset;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_cache (h, count_u64, offset_u64, completion, flags_u32);
  Py_END_ALLOW_THREADS;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLongLong (ret);

 out:
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_zero (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int64_t ret;
  PyObject *py_ret = NULL;
  uint64_t count_u64;
  unsigned long long count; /* really uint64_t */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OKKOI:nbd_aio_zero", &py_h, &count, &offset,
                         &py_completion_fn, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  flags_u32 = flags;
  count_u64 = count;
  offset_u64 = offset;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_zero (h, count_u64, offset_u64, completion, flags_u32);
  Py_END_ALLOW_THREADS;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLongLong (ret);

 out:
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_block_status (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int64_t ret;
  PyObject *py_ret = NULL;
  uint64_t count_u64;
  unsigned long long count; /* really uint64_t */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  struct user_data *extent_user_data = NULL;
  PyObject *py_extent_fn;
  nbd_extent_callback extent = { .callback = extent_wrapper,
                                 .free = free_user_data };
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OKKOOI:nbd_aio_block_status", &py_h, &count,
                         &offset, &py_extent_fn, &py_completion_fn, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  flags_u32 = flags;
  count_u64 = count;
  offset_u64 = offset;
  extent.user_data = extent_user_data = alloc_user_data ();
  if (extent_user_data == NULL) goto out;
  if (!PyCallable_Check (py_extent_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter extent is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_extent_fn);
  extent_user_data->fn = py_extent_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_block_status (h, count_u64, offset_u64, extent, completion,
                              flags_u32);
  Py_END_ALLOW_THREADS;
  extent_user_data = NULL;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLongLong (ret);

 out:
  free_user_data (extent_user_data);
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_block_status_64 (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int64_t ret;
  PyObject *py_ret = NULL;
  uint64_t count_u64;
  unsigned long long count; /* really uint64_t */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  struct user_data *extent64_user_data = NULL;
  PyObject *py_extent64_fn;
  nbd_extent64_callback extent64 = { .callback = extent64_wrapper,
                                     .free = free_user_data };
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OKKOOI:nbd_aio_block_status_64", &py_h,
                         &count, &offset, &py_extent64_fn,
                         &py_completion_fn, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  flags_u32 = flags;
  count_u64 = count;
  offset_u64 = offset;
  extent64.user_data = extent64_user_data = alloc_user_data ();
  if (extent64_user_data == NULL) goto out;
  if (!PyCallable_Check (py_extent64_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter extent64 is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_extent64_fn);
  extent64_user_data->fn = py_extent64_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_block_status_64 (h, count_u64, offset_u64, extent64,
                                 completion, flags_u32);
  Py_END_ALLOW_THREADS;
  extent64_user_data = NULL;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLongLong (ret);

 out:
  free_user_data (extent64_user_data);
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_block_status_filter (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int64_t ret;
  PyObject *py_ret = NULL;
  uint64_t count_u64;
  unsigned long long count; /* really uint64_t */
  uint64_t offset_u64;
  unsigned long long offset; /* really uint64_t */
  PyObject *py_contexts;
  char **contexts = NULL;
  struct user_data *extent64_user_data = NULL;
  PyObject *py_extent64_fn;
  nbd_extent64_callback extent64 = { .callback = extent64_wrapper,
                                     .free = free_user_data };
  struct user_data *completion_user_data = NULL;
  PyObject *py_completion_fn;
  nbd_completion_callback completion = { .callback = completion_wrapper,
                                         .free = free_user_data };
  uint32_t flags_u32;
  unsigned int flags; /* really uint32_t */

  if (!PyArg_ParseTuple (args, "OKKOOOI:nbd_aio_block_status_filter", &py_h,
                         &count, &offset, &py_contexts, &py_extent64_fn,
                         &py_completion_fn, &flags))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  completion.user_data = completion_user_data = alloc_user_data ();
  if (completion_user_data == NULL) goto out;
  if (py_completion_fn != Py_None) {
    if (!PyCallable_Check (py_completion_fn)) {
      PyErr_SetString (PyExc_TypeError,
                       "callback parameter completion is not callable");
      goto out;
    }
    /* Increment refcount since pointer may be saved by libnbd. */
    Py_INCREF (py_completion_fn);
    completion_user_data->fn = py_completion_fn;
  }
  else
    completion.callback = NULL; /* we're not going to call it */
  flags_u32 = flags;
  count_u64 = count;
  offset_u64 = offset;
  contexts = nbd_internal_py_get_string_list (py_contexts);
  if (!contexts) goto out;
  extent64.user_data = extent64_user_data = alloc_user_data ();
  if (extent64_user_data == NULL) goto out;
  if (!PyCallable_Check (py_extent64_fn)) {
    PyErr_SetString (PyExc_TypeError,
                     "callback parameter extent64 is not callable");
    goto out;
  }
  /* Increment refcount since pointer may be saved by libnbd. */
  Py_INCREF (py_extent64_fn);
  extent64_user_data->fn = py_extent64_fn;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_block_status_filter (h, count_u64, offset_u64, contexts,
                                     extent64, completion, flags_u32);
  Py_END_ALLOW_THREADS;
  extent64_user_data = NULL;
  completion_user_data = NULL;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLongLong (ret);

 out:
  nbd_internal_py_free_string_list (contexts);
  free_user_data (extent64_user_data);
  free_user_data (completion_user_data);
  return py_ret;
}

PyObject *
nbd_internal_py_aio_get_fd (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_aio_get_fd", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_get_fd (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_get_direction (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  unsigned ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_aio_get_direction", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_get_direction (h);
  Py_END_ALLOW_THREADS;
  py_ret = PyLong_FromLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_notify_read (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_aio_notify_read", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_notify_read (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_notify_write (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_aio_notify_write", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_notify_write (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_is_created (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_aio_is_created", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_is_created (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_is_connecting (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_aio_is_connecting", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_is_connecting (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_is_negotiating (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_aio_is_negotiating", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_is_negotiating (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_is_ready (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_aio_is_ready", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_is_ready (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_is_processing (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_aio_is_processing", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_is_processing (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_is_dead (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_aio_is_dead", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_is_dead (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_is_closed (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_aio_is_closed", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_is_closed (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_command_completed (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  uint64_t cookie_u64;
  unsigned long long cookie; /* really uint64_t */

  if (!PyArg_ParseTuple (args, "OK:nbd_aio_command_completed", &py_h,
                         &cookie))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;
  cookie_u64 = cookie;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_command_completed (h, cookie_u64);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_peek_command_completed (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int64_t ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_aio_peek_command_completed", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_peek_command_completed (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLongLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_aio_in_flight (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_aio_in_flight", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_aio_in_flight (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_connection_state (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  const char * ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_connection_state", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_connection_state (h);
  Py_END_ALLOW_THREADS;
  if (ret == NULL) {
    raise_exception ();
    goto out;
  }
  py_ret = PyUnicode_FromString (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_package_name (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  const char * ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_package_name", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_package_name (h);
  Py_END_ALLOW_THREADS;
  py_ret = PyUnicode_FromString (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_version (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  const char * ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_version", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_version (h);
  Py_END_ALLOW_THREADS;
  py_ret = PyUnicode_FromString (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_kill_subprocess (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  int signum;

  if (!PyArg_ParseTuple (args, "Oi:nbd_kill_subprocess", &py_h, &signum))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_kill_subprocess (h, signum);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = Py_None;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_subprocess_pid (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int64_t ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_subprocess_pid", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_subprocess_pid (h);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = PyLong_FromLongLong (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_supports_tls (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_supports_tls", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_supports_tls (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_supports_vsock (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_supports_vsock", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_supports_vsock (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_supports_uri (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_supports_uri", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_supports_uri (h);
  Py_END_ALLOW_THREADS;
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_get_uri (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  char * ret;
  PyObject *py_ret = NULL;

  if (!PyArg_ParseTuple (args, "O:nbd_get_uri", &py_h))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_get_uri (h);
  Py_END_ALLOW_THREADS;
  if (ret == NULL) {
    raise_exception ();
    goto out;
  }
  py_ret = PyUnicode_FromString (ret);
  free (ret);

 out:
  return py_ret;
}

PyObject *
nbd_internal_py_is_uri (PyObject *self, PyObject *args)
{
  PyObject *py_h;
  struct nbd_handle *h;
  int ret;
  PyObject *py_ret = NULL;
  const char *uri;

  if (!PyArg_ParseTuple (args, "Os:nbd_is_uri", &py_h, &uri))
    goto out;
  h = get_handle (py_h);
  if (!h) goto out;

  Py_BEGIN_ALLOW_THREADS;
  ret = nbd_is_uri (h, uri);
  Py_END_ALLOW_THREADS;
  if (ret == -1) {
    raise_exception ();
    goto out;
  }
  py_ret = ret ? Py_True : Py_False;
  Py_INCREF (py_ret);

 out:
  return py_ret;
}

