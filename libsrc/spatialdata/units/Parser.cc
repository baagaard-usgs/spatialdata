// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include <portinfo>

#include "Parser.hh" // implementation of class methods

#include <sstream> // USES std::ostringstream
#include <stdexcept> // USES std::runtime_error
#include <assert.h> // USES assert()

PyObject* spatialdata::units::Parser::_parser = nullptr;

// ----------------------------------------------------------------------
// Default constructor
spatialdata::units::Parser::Parser(void) {
    // Initialize Python interpreter if it is not already initialized.
    _alreadyInitialized = Py_IsInitialized();
    if (!_alreadyInitialized) {
        Py_Initialize();
    }

    if (!_parser) {
        // Should check for NULL, decode the exception, and throw a C++ equivalent
        PyObject* mod = PyImport_ImportModule("pythia.pyre.units");
        if (!mod) {
            throw std::runtime_error("Could not import module 'pythia.pyre.units'.");
        } // if
        PyObject* cls = PyObject_GetAttrString(mod, "parser");
        Py_DECREF(mod);
        if (!cls) {
            throw std::runtime_error("Could not get 'parser' attribute in pythia.pyre.units module.");
        } // if
        _parser = PyObject_CallFunctionObjArgs(cls, NULL);
        Py_DECREF(cls);
        if (!_parser) {
            throw std::runtime_error("Could not create parser Python object.");
        } // if
    } // if
} // constructor


// ----------------------------------------------------------------------
// Default destructor
spatialdata::units::Parser::~Parser(void) {
    Py_CLEAR(_parser);

    if (!_alreadyInitialized) {
        Py_Finalize();
    } // if
} // destructor


// ----------------------------------------------------------------------
// Get SI scaling factor for units given by string. To get value in
// SI units, multiple value given by units by scaling factor.
double
spatialdata::units::Parser::parse(const char* units) {
    double scale = 1.0;

    /* Replicate Python functionality given by
     *
     * import pythia.pyre.units
     * p = pythia.pyre.units.parser()
     * x = p.parse(units) [units is a string]
     * scale = x.value
     */
    PyObject* pyUnit = PyObject_CallMethod(_parser, (char*)"parse", (char*)"s", units);
    if (!pyUnit) {
        if (PyErr_Occurred()) {
            PyErr_Clear();
        } // if
        std::ostringstream msg;
        msg << "Could not parse units string '" << units << "'.";
        throw std::runtime_error(msg.str());
    } // if
    PyObject* pyScale = PyObject_GetAttrString(pyUnit, "value");
    if (!pyScale) {
        Py_DECREF(pyUnit);
        if (PyErr_Occurred()) {
            PyErr_Clear();
        } // if
        std::ostringstream msg;
        msg << "Could not get floating point value when parsing units string '" << units << "'.";
        throw std::runtime_error(msg.str());
    } // if
    if (!PyFloat_Check(pyScale)) {
        Py_DECREF(pyScale);
        Py_DECREF(pyUnit);
        PyErr_Clear();
        std::ostringstream msg;
        msg << "Could not get floating point value when parsing units string '" << units << "'.";
        throw std::runtime_error(msg.str());
    } // if

    scale = PyFloat_AsDouble(pyScale);
    Py_DECREF(pyScale);
    Py_DECREF(pyUnit);

    return scale;
} // parse


// End of file
