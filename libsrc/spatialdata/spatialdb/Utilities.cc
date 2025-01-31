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

#include "Utilities.hh" // Implementation of class methods

#include "Exception.hh"

#include <sstream> // USES std::ostringstream
#include <strings.h> // USES strcasecmp()

// ----------------------------------------------------------------------
size_t
spatialdata::spatialdb::Utilities::search(const std::vector<std::string>& names,
                                          const char* name,
                                          const char* description) {
    const size_t numNames = names.size();
    for (size_t iName = 0; iName < numNames; ++iName) {
        if (0 == strcasecmp(name, names[iName].c_str())) {
            return iName;
        } // if
    } // for

    std::ostringstream msg;
    msg << "Could not find value '" << name << "' in spatial database '"
        << description << "'. Available values are:";
    for (const std::string& vname : names) {
        msg << "\n  " << vname;
    }
    msg << "\n";
    throw ValueNotFound(msg.str());

    return 0;
} // search


// End of file
