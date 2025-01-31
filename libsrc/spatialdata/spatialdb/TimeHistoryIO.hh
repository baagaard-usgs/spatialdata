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

#include "spatialdbfwd.hh"

#include <vector> // USES std::vector
#include <cstdlib> // USES size_t

/// C++ object for reading/writing time history files.
class spatialdata::spatialdb::TimeHistoryIO {
public:

    // PUBLIC METHODS /////////////////////////////////////////////////////

    /** Read time history file.
     *
     * @param time Time stamps.
     * @param amplitude Amplitude values in time history.
     * @param filename Filename for time history.
     */
    static
    void read(std::vector<double>* time,
              std::vector<double>* amplitude,
              const char* filename);

    /** Read time history file. Number of time history points given by
     * nptsT must equal nptsA.
     *
     * @param time Time stamps.
     * @param amplitude Amplitude values in time history.
     * @param timeUnits Units associated with time stamps.
     * @param filename Filename for time history.
     */
    static
    void write(const std::vector<double>& time,
               const std::vector<double>& amplitude,
               const char* timeUnits,
               const char* filename);

private:

    // PRIVATE MEMBERS ////////////////////////////////////////////////////

    static const char* header; ///< Header for time history files.

}; // class TimeHistoryIO

// End of file
