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

#include "spatialdata/spatialdb/SimpleDB.hh" // USES SimpleDB

#include <iosfwd> // USES std::istream

// ----------------------------------------------------------------------
class spatialdata::spatialdb::SimpleDBIO {
public:

    // PUBLIC METHODS /////////////////////////////////////////////////////

    /** Read the database.
     *
     * @param[out] data Database data.
     * @param[in] filename Name of file.
     */
    static
    void read(SimpleDBData* data,
              const char* filename);

    /** Write the database.
     *
     * @param[in] data Database data.
     * @param[in] filename Name of file.
     */
    static
    void write(const SimpleDBData& data,
               const char* filename);

private:

    // PRIVATE METHODS ////////////////////////////////////////////////////

    /** Read database version 1 file.
     *
     * @param[out] data Database data.
     * @param[inout] filein File input stream.
     */
    static
    void _readV1(SimpleDBData* data,
                 std::istream& filein);

private:

    // PRIVATE MEMBERS ////////////////////////////////////////////////////

    static const char* header; ///< Magic header in file.

}; // class SimpleDBIO

// End of file
