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

#include "geocoordsfwd.hh"

#include <cstddef> // USES size_t
#include <memory> // USES std::shared_ptr

namespace spatialdata {
    namespace geocoords {
        namespace _converter {
            class Cache;
        } // _Converter
    } // geocoords
} // spatialdata

class spatialdata::geocoords::Converter {
    friend class TestConverter; // unit testing

public:

    // PUBLIC METHODS /////////////////////////////////////////////////////

    /// Default constructor
    Converter(void);

    /// Default destructor
    ~Converter(void);

    /** Convert coordinates from source coordinate system to destination
     * coordinate system.
     *
     * @param[inout] coordinates Array of coordinates
     * @param[in] numLocs Number of location
     * @param[in] spaceDim Number of spatial dimensions in coordinates
     * @param[in] csDest Pointer to destination coordinate system
     * @param[in] csSrc Pointer to source coordinate system
     */
    void convert(double* coordinates,
                 const size_t numLocs,
                 const size_t spaceDim,
                 const CoordSys* csDest,
                 const CoordSys* csSrc);

private:

    // PRIVATE MEMBERS ////////////////////////////////////////////////////

    std::unique_ptr<_converter::Cache> _cache;

}; // class Converter

// End of file
