#include "pylibzfs2.h"

static
PyObject *zfs_type_table_to_dict(void)
{
	PyObject *dict_out = NULL;
	int err;
	uint i;

	dict_out = PyDict_New();
	if (dict_out == NULL)
		return NULL;

	for (i=0; i < ARRAY_SIZE(zfs_type_table); i++) {
		PyObject *val = NULL;

		val = PyLong_FromLong(zfs_type_table[i].type);
		if (val == NULL)
			goto fail;

		err = PyDict_SetItemString(dict_out,
					   zfs_type_table[i].name,
					   val);
		Py_DECREF(val);
		if (err)
			goto fail;
	}

	return dict_out;
fail:
	Py_XDECREF(dict_out);
	return NULL;
}

static
PyObject *zfs_prop_table_to_dict(void)
{
	PyObject *dict_out = NULL;
	int err;
	uint i;

	dict_out = PyDict_New();
	if (dict_out == NULL)
		return NULL;

	for (i=0; i < ARRAY_SIZE(zpool_prop_table); i++) {
		PyObject *val = NULL;

		val = PyLong_FromLong(zfs_prop_table[i].prop);
		if (val == NULL)
			goto fail;

		err = PyDict_SetItemString(dict_out,
					   zfs_prop_table[i].name,
					   val);
		Py_DECREF(val);
		if (err)
			goto fail;
	}

	return dict_out;
fail:
	Py_XDECREF(dict_out);
	return NULL;
}

static
PyObject *zpool_prop_table_to_dict(void)
{
	PyObject *dict_out = NULL;
	int err;
	uint i;

	dict_out = PyDict_New();
	if (dict_out == NULL)
		return NULL;

	for (i=0; i < ARRAY_SIZE(zpool_prop_table); i++) {
		PyObject *val = NULL;

		val = PyLong_FromLong(zpool_prop_table[i].prop);
		if (val == NULL)
			goto fail;

		err = PyDict_SetItemString(dict_out,
					   zpool_prop_table[i].name,
					   val);
		Py_DECREF(val);
		if (err)
			goto fail;
	}

	return dict_out;
fail:
	Py_XDECREF(dict_out);
	return NULL;
}

static
PyObject *zfs_dosflag_table_to_dict(void)
{
	PyObject *dict_out = NULL;
	int err;
	uint i;

	dict_out = PyDict_New();
	if (dict_out == NULL)
		return NULL;

	for (i=0; i < ARRAY_SIZE(zfs_dosflag_table); i++) {
		PyObject *val = NULL;

		val = PyLong_FromLong(zfs_dosflag_table[i].flag);
		if (val == NULL)
			goto fail;

		err = PyDict_SetItemString(dict_out,
					   zfs_dosflag_table[i].name,
					   val);
		Py_DECREF(val);
		if (err)
			goto fail;
	}

	return dict_out;
fail:
	Py_XDECREF(dict_out);
	return NULL;
}

static
PyObject *zfs_err_table_to_dict(void)
{
	PyObject *zfserr_dict = NULL;
	int err;
	uint i;

	zfserr_dict = PyDict_New();
	if (zfserr_dict == NULL)
		return NULL;

	for (i=0; i < ARRAY_SIZE(zfserr_table); i++) {
		PyObject *val = NULL;

		val = PyLong_FromLong(zfserr_table[i].error);
		if (val == NULL)
			goto fail;

		err = PyDict_SetItemString(zfserr_dict,
					   zfserr_table[i].name,
					   val);
		Py_DECREF(val);
		if (err)
			goto fail;
	}

	return zfserr_dict;
fail:
	Py_XDECREF(zfserr_dict);
	return NULL;
}

static
PyObject *zpool_status_table_to_dict(void)
{
	PyObject *status_dict = NULL;
	int err;
	uint i;

	status_dict = PyDict_New();
	if (status_dict == NULL)
		return NULL;

	for (i=0; i < ARRAY_SIZE(zpool_status_table); i++) {
		PyObject *val = NULL;

		val = PyLong_FromLong(zpool_status_table[i].status);
		if (val == NULL)
			goto fail;

		err = PyDict_SetItemString(status_dict,
					   zpool_status_table[i].name,
					   val);
		Py_DECREF(val);
		if (err)
			goto fail;
	}

	return status_dict;
fail:
	Py_XDECREF(status_dict);
	return NULL;
}

static
PyObject *build_args_tuple_enum(const char *class_name,
				PyObject *(*get_dict)(void))
{
	PyObject *name = NULL;
	PyObject *attrs = NULL;
	PyObject *args_out = NULL;

	name = PyUnicode_FromString(class_name);
	if (name == NULL)
		goto out;

	attrs = get_dict();
	if (attrs == NULL)
		goto out;

	args_out = PyTuple_Pack(2, name, attrs);

out:
	Py_XDECREF(name);
	Py_XDECREF(attrs);
	return args_out;
}

static
int add_enum(PyObject *module,
	     PyObject *enum_type,
	     const char *class_name,
	     PyObject *(*get_dict)(void),
	     PyObject *kwargs)
{
	PyObject *args = NULL;
	PyObject *enum_obj = NULL;

	args = build_args_tuple_enum(class_name, get_dict);
	if (args == NULL)
		return -1;

	// Create the enum via the functional API for python enums
	// https://docs.python.org/3/howto/enum.html#functional-api
	enum_obj = PyObject_Call(enum_type, args, kwargs);
	Py_DECREF(args);

	// steals reference to enum_obj
	return PyModule_AddObject(module, class_name, enum_obj);
}

/*
 * This function is used to add lookup tables form pylibzfs_enum.h
 * to the module as enum.IntEnum.
 */
int
py_add_zfs_enums(PyObject *module)
{
	int err = -1;
	PyObject *enum_mod = NULL;
	PyObject *int_enum = NULL;
	PyObject *intflag_enum = NULL;
	PyObject *kwargs = NULL;

	kwargs = Py_BuildValue("{s:s}", "module", PYLIBZFS_MODULE_NAME);
	if (kwargs == NULL)
		goto out;

	enum_mod = PyImport_ImportModule("enum");
	if (enum_mod == NULL)
		goto out;

	int_enum = PyObject_GetAttrString(enum_mod, "IntEnum");
	if (int_enum == NULL)
		goto out;

	intflag_enum = PyObject_GetAttrString(enum_mod, "IntFlag");
	if (intflag_enum == NULL)
		goto out;

	err = add_enum(module, int_enum, "ZFSError",
		       zfs_err_table_to_dict, kwargs);
	if (err)
		goto out;

	err = add_enum(module, int_enum, "ZPOOLStatus",
		       zpool_status_table_to_dict, kwargs);
	if (err)
		goto out;

	err = add_enum(module, int_enum, "ZFSType",
		       zfs_type_table_to_dict, kwargs);
	if (err)
		goto out;

	err = add_enum(module, intflag_enum, "ZFSDOSFlag",
		       zfs_dosflag_table_to_dict, kwargs);
	if (err)
		goto out;

	err = add_enum(module, int_enum, "ZFSProperty",
		       zfs_prop_table_to_dict, kwargs);
	if (err)
		goto out;


	err = add_enum(module, int_enum, "ZPOOLProperty",
		       zpool_prop_table_to_dict, kwargs);
	if (err)
		goto out;

out:
	Py_XDECREF(kwargs);
	Py_XDECREF(int_enum);
	Py_XDECREF(intflag_enum);
	Py_XDECREF(enum_mod);
	return err;
}
