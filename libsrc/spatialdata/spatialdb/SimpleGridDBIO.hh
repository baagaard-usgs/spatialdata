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

#include "spatialdata/spatialdb/SimpleGridDB.hh" // USES SimpleGridDB

#include <iosfwd> // USES std::istream

// ----------------------------------------------------------------------
class spatialdata::spatialdb::SimpleGridDBIO {
public:

    // PUBLIC METHODS /////////////////////////////////////////////////////

    /** Read the database.
     *
     * @param data Database data
     * @param[in] filename Name of file.
     */
    static
    void read(SimpleGridDBData* data,
              const char* filename);

    /** Write the database.
     *
     * @param[in] data Database data.
     * @param[in] filename Name of file.
     */
    static
    void write(const SimpleGridDBData& data,
               const char* filename);

private:

    // PRIVATE METHODS ////////////////////////////////////////////////////

    /** Read data file header.
     *
     * @param[out] data Database data.
     * @param[inout] filein File input stream.
     */
    static
    void _readHeader(SimpleGridDBData* data,
                     std::istream& filein);

    /** Read data values.
     *
     * @param[out] data Database data.
     * @param[inout] filein File input stream.
     */
    static
    void _readData(SimpleGridDBData* data,
                   std::istream& filein);

    /** Write the data file header.
     *
     * @param[in] data Database data.
     * @param[inout] filein File input stream.
     */
    static
    void _writeHeader(const SimpleGridDBData& data,
                      std::ostream& fileout);

    /** Write data values.
     *
     * @param[in] data Database data.
     * @param[inout] filein File input stream.
     */
    static
    void _writeData(const SimpleGridDBData& data,
                    std::ostream& fileout);

private:

    // PRIVATE MEMBERS ////////////////////////////////////////////////////

    static const char* header; ///< Magic header in file.

}; // class SimpleGridDBIO

// End of file
