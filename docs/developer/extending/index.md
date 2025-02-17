# Extending spatialdata

## Overview

This directory contains source files for building a user defined spatial database component consisting of C++ code, a Python module, and Python code.
The example implementation provides a uniform seismic velocity model than can be used to provide elastic physical properties in PyLith simulations using a georeferenced coordinate system.

The suggested path to customizing this component is to build and install the provided `UniformVelModel` component and then rename/copy the files and gradually adopt it to your specific needs.

## Requirements

In order to build the component and interface it with PyLith, you will need

    - C++ compiler
    - pybind11 (install via `pip install -m pybind11`)
    - Python with header files (version 3.8 or later)
    - autoconf tools

The C++ compiler must be compatible with the installed Python and both must be compatible with the C++ compiler and Python used to build PyLith.
The safest way to insure compatibility is to use the C++ compiler and Python provided with your operating system and build PyLith from source.
However, on many systems it should be possible to build the component and have it work with PyLith installed from a binary package.

## Files

`Makefile.am`
: automake parameters for constructing a Makefile

`UniformVelModel.cc`
: C++ source file implementing UniformVelModel object functions

`UniformVelModel.hh`
: C++ header file with class definition for UniformVelModel

`UniformVelModel_wrap.cc`
: pybind11 interface file for the C++ UniformVelModel object

`__init__.py`
: Python source file for module initialization

`UniformVelModel.py`
: Python source file implementing UniformVelModel object

`configure.ac`
: autoconf parameters for construction a configure script

`m4`: directory containing autoconf macros

`spatialdbcontrib.cc`
: SWIG interface file defining the spatialdbcontrib Python module

`tests`
: directory containing tests of the UniformVelModel object

## Building and installing the UniformVelModel component

1. Run `autoreconf -if` in this directory (`templates/spatialdb`).
2. Run `configure` either from this directory or a scratch build directory.
    Use the `--prefix=DIRECTORY` to indicate where the files should be installed.
    We strongly recommend that you install the component to the same location as where PyLith is installed.
    For example, if PyLith is installed in `$HOME/cig` then use thev`--prefix=$HOME/cig` command line argument to configure.
    To build in a separate directory simply invoke the configure script from the other directory.
    For example, from `$HOME/build/spatialdb-contrib` run `$HOME/src/spatialdata/templates/spatialdb/configure --prefix=$HOME/cig`.

Configure will check for a number of files including the location of spatialdata C++ header files, library, and SWIG interface files.
You may need to pass some additional command line arguments to configure or environment variables to help configure find the required files.

```bash
${HOME}/src/cig/spatialdata/templates/spatialdb/configure  \
  --prefix=${HOME}/tools/cig  \
  CPPFLAGS="-I${PROJ4_INCDIR} -I${CIG_INCDIR}"  \
  LDFLAGS="-L${PROJ4_LIBDIR} -L${CIG_LIBDIR}"
```

3. Run `make`, `make install"`, and `make check` from the top-level build directory.
   This will first build the C++ library and module, then install the files to the location specified by the `--prefix` command line argument to configure, and finally run some Python tests to verify that the UniformVelModel component was installed correctly.

## Customization

This is where the fun begins.
Read over the Python and C++ source code to become familiar with the features implemented with the UniformVelModel component.
The UniformVelModel Python object simply acquires user input using Pyre properties (and could use facilities as well) and in `_configure()` transfers that information to the UniformVelModel C++ object via the pybind11 module.
The UniformVelModel C++ object does the grunt work of managing which values will be returned in a query and performing the query itself.
In this case, the query is trivial (it simply returns the same value independent of the location where the physical properties are desired).
In a more practical UniformVelModel component, the query function could be quite complex and will usually involve interpolation of values to the location specified in the arguments of the query() function (the query location).

We recommend that you start by changing the default values assigned to the UniformVelModel, followed by changing the user controlled parameters (Pyre properties).
When you change the Pyre properties (vp, vs, density), you should change the corresponding names of the C++ accessor functions (`setVp()`, `setVs()`, `setDensity()`).
