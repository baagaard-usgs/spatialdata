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
#include <string> // USES std::string

class spatialdata::spatialdb::Utilities {
public:

    // PUBLIC METHODS /////////////////////////////////////////////////////

    /** Find name in array of names.
     *
     * @param[in] names Array of names to search.
     * @param[in] name Target name.
     * @parma[in] description Description to include in error message.
     *
     * @returns Index of name in names.
     */
    static
    size_t search(const std::vector<std::string>& names,
                  const char* name,
                  const char* description);

    // PRIVATE METHODS ////////////////////////////////////////////////////
private:

    Utilities(void); ///< Not implemented

}; // Utilities

// End of file
