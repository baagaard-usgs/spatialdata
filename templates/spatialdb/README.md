# Template for extending spatialdb

This directory contains source files for building a user defined spatial database component consisting of C++ code, a Python module, and Python code.
The example implementation provides a uniform seismic velocity model than can be used to provide elastic physical properties in PyLith simulations using a georeferenced coordinate system.

The suggested path to customizing this component is to build and install the provided `UniformVelModel` component and then rename/copy the files and gradually adopt it to your specific needs.

Refer to the `Extending` section of the Developer Guide for more information.
