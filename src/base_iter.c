/**
 * Base Python Wrapper for MCMC Samplers
 */

#include <Python.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#define NO_IMPORT_ARRAY
#define PY_ARRAY_UNIQUE_SYMBOL BAYESIAN_CLUSTERING_C_ARRAY_API
#include <numpy/arrayobject.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/mixture.h"
#include "../include/type_check.h"


/**
 * Wrapper for MCMC iteration; handles error checking, GIL, type unpacking
 */
PyObject *base_iter(
		PyObject *self, PyObject *args,
		bool (*error_check)(struct mixture_model_t *),
		bool (*iter)(double *, uint16_t *, struct mixture_model_t *))
{
    // Get args
    PyArrayObject *data_py;
    PyArrayObject *assignments_py;
    PyObject *model_py;
    bool success = PyArg_ParseTuple(
        args, "O!O!O",
        &PyArray_Type, &data_py,
        &PyArray_Type, &assignments_py,
        &model_py);
    if(!success) { return NULL; }
    if(!type_check(data_py, assignments_py)) { return NULL; }

    // Unpack capsule
    struct mixture_model_t *model = (
        (struct mixture_model_t *) PyCapsule_GetPointer(
            model_py, MIXTURE_MODEL_API));
    // Check that models supports gibbs
    if(!error_check(model)) { return NULL; }

    bool iter_success; 
    // GIL free zone ----------------------------------------------------------
    Py_INCREF(data_py);
    Py_INCREF(assignments_py);
    Py_INCREF(model_py);
    Py_BEGIN_ALLOW_THREADS;

    iter_success = iter(
        (double *) PyArray_DATA(data_py),
        (uint16_t *) PyArray_DATA(assignments_py),
        model);

    Py_END_ALLOW_THREADS;
    Py_DECREF(data_py);
    Py_DECREF(assignments_py);
    Py_DECREF(model_py);
    // ------------------------------------------------------------------------

    // Check for exception
    if(!iter_success) { return NULL; }
    else { Py_RETURN_NONE; }
}