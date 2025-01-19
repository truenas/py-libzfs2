#ifndef	_PYZFS_STATE_H
#define _PYZFS_STATE_H

typedef struct { zfs_type_t type; PyObject *obj; } pystate_zfstype_t;
#if 0
typedef struct { enum zfs_error_t error; PyObject *obj; } pystate_zfserror_t;
typedef struct { enum zfs_prop_t prop; PyObject *obj; } pystate_zfsprop_t;
typedef struct { enum zpool_prop_t prop; PyObject *obj; } pystate_zpoolprop_t;
typedef struct { enum zpool_status_t prop; PyObject *obj; } pystate_zstatus_t;
#endif

typedef struct {
	/* Per-module instance lookup tables for enum entries */
	pystate_zfstype_t zfs_type_enum[ARRAY_SIZE(zfs_type_table)];
} pylibzfs_state_t;

extern int init_py_zfs_state(PyObject *module);
#endif /* _PYZFS_STATE_H */
