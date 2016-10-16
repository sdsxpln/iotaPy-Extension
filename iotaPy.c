/*
 * iotaPy.c
 *
 *  Created on: Oct 15, 2016
 *      Author: Alfred Keller
 */
//Python.h has all the required function definitions to manipulate the Python objects
#include <Python.h>

static int8_t curlF[] = { 1, 0, -1, 1, -1, 0, -1, 1, 0 };

//This is the function that is called from your python code
static PyObject* iotaPy_transform(PyObject* self, PyObject* args) {
    PyObject *integer_list;
    PyObject *item;
    PyObject *py_list;
    char state[729];
    char state_copy[729];
    int8_t value;
    int32_t list_size, i, round, index;

	if (!PyArg_ParseTuple(args, "O", &integer_list))
		return NULL;

	if (!PySequence_Check(integer_list)) {
		PyErr_SetString(PyExc_TypeError, "expected list of integers");
		return NULL;
	}

	list_size = PyObject_Length(integer_list);

	if (list_size > 729) {
		PyErr_SetString(PyExc_ValueError, "list must not be longer than 729");
		return NULL;
	}

	for (i=0; i<list_size; i++) {
		item = PySequence_GetItem(integer_list, i);
		if (item == NULL || !PyInt_Check(item)) {
			PyErr_SetString(PyExc_ValueError, "list contains invalid elements");
			return NULL;
		}
		value = (int8_t)PyInt_AsLong(item);
		if (value < -1 || value > 1) {
			PyErr_SetString(PyExc_ValueError, "list may only conatin the values -1, 0, 1");
			return NULL;
		}
		state[i] = (int8_t)value;
	}

	for (round = 0, index = 0; round < 27; round++) {
		memcpy(state_copy, state, 729);
		for (i = 0; i < 729; i++) {
			int32_t index2 = index + (index < 365 ? 364 : -365);
			int32_t index3 = state_copy[index] + state_copy[index2] * 3 + 4;
			state[i] = curlF[index3];
			index = index2;
		}
	}
    py_list = PyTuple_New(729);
	for (i=0; i<list_size; i++) {
		item = PyInt_FromLong((long)state[i]);
		PyTuple_SetItem(py_list, i, item);
	}
	return py_list;
}

static char iotaPy_docs_transform[] = "transform( ): to be documented";

static PyMethodDef iotaPy_funcs[] = { { "transform",
		(PyCFunction) iotaPy_transform, METH_VARARGS, iotaPy_docs_transform }, {
		NULL, NULL, 0, NULL } };

PyMODINIT_FUNC initiotaPy(void) {
	Py_InitModule3("iotaPy", iotaPy_funcs, "Transform utility function");
}
