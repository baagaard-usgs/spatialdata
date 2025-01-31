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

#include "spatialdbfwd.hh" // forward declarations

#include <stdexcept> // ISA std::runtime_error

/// C++ exceptions for spatial database.

class spatialdata::spatialdb::OutOfBounds : public std::runtime_error {
public:

    OutOfBounds(const std::string& msg);

}; // class OutOfBounds

class spatialdata::spatialdb::ValueNotFound : public std::runtime_error {
public:

    ValueNotFound(const std::string& msg);

}; // class ValueNotFound

#include "Exception.icc" // inline methods

// End of file
