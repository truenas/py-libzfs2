from setuptools import setup, Extension

pylibzfs2 = Extension(
    name='libzfs2',
    sources=[
        'src/error.c',
        'src/py_zfs.c',
        'src/py_zfs_dataset.c',
        'src/py_zfs_iter.c',
        'src/py_zfs_object.c',
        'src/py_zfs_pool.c',
        'src/py_zfs_prop.c',
        'src/py_zfs_resource.c',
        'src/pylibzfs2.c',
        'src/utils.c',
    ],
    libraries = [
        'zfs',
        'zfs_core',
        'nvpair',
        'uutil'
    ],
    include_dirs = ['/usr/include/libzfs', '/usr/include/libspl'],
    library_dirs = ['/usr/lib/x86_64-linux-gnu/'])

setup(name='pylibzfs2',
      version='0.1',
      description='pylibzfs2 provides python bindings for libzfs',
      ext_modules=[pylibzfs2])
