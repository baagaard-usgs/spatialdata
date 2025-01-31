// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================
#pragma once

#include "unitsfwd.hh"

#include <Python.h>

#include <vector> // USES std::vector
#include <string> // USES std::string

/// C++ interface to Pyre units parser.
class spatialdata::units::Parser { // class Parser
    friend class TestParser; // Unit testing

public:

    // PUBLIC METHODS /////////////////////////////////////////////////////

    /* MATT- I am not sure if we want just a single static method or we
     *   want an object so that any setup/teardown is done once per
     *   instantiation rather than each call to parse.
     */

    /// Default constructor
    Parser(void);

    /// Default destructor
    ~Parser(void);

    /** Get SI scaling factor for units given by string. To get value in
     * SI units, multiple value given by units by scaling factor.
     *
     * @returns Scaling factor to convert to SI units.
     */
    double parse(const char* units);

    /** Convert values to SI units.
     *
     * @param data Array of data [numLocs*numValues].
     * @param units Units for values [numValues].
     * @param numLocs Number of locations.
     * @param numValues Number of values per location.
     */
    static
    void toSI(double* values,
              const std::vector<std::string>& units,
              const size_t numLocs,
              const size_t numValues);

private:

    // PRIVATE MEMBERS ////////////////////////////////////////////////////

    static PyObject* _parser;
    bool _alreadyInitialized;

}; // class Parser

// End of file
