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

#include <string> // HASA std::string

/// C++ object for time dependence in spatial databases.
class spatialdata::spatialdb::TimeHistory {
    friend class TestTimeHistory;

public:

    // PUBLIC METHODS /////////////////////////////////////////////////////

    /** Constructor with description.
     *
     * @param description Description of database
     */
    TimeHistory(const char* description);

    /// Default destructor.
    ~TimeHistory(void);

    /** Set description of time history.
     *
     * @param[in] description Description of database.
     */
    void setDescription(const char* description);

    /** Get description of time history.
     *
     * @returns Description for time history.
     */
    const char* getDescription(void) const;

    /** Set filename for time history.
     *
     * @param filename Name of file.
     */
    void setFilename(const char* filename);

    /** Set filename for time history.
     *
     * @return Name of file.
     */
    const char* getFilename(void);

    /// Open the time history and prepare for querying.
    void open(void);

    /// Close the time history.
    void close(void);

    /** Query the database.
     *
     * @pre Must call open() before query()
     *
     * @param value Value in time history.
     * @param t Time for query.
     *
     * @returns 0 on success, 1 on failure (i.e., could not interpolate)
     */
    int query(double* value,
              const double t);

    /** Query the database.
     *
     * @pre Must call open() before query()
     *
     * @param value Value in time history.
     * @param t Time for query.
     *
     * @returns 0 on success, 1 on failure (i.e., could not interpolate)
     */
    int query(float* value,
              const float t);

private:

    // PRIVATE MEMBERS ////////////////////////////////////////////////////

    std::string _description; ///< Description of time history.
    std::string _filename; ///< Name of time history file
    std::vector<double> _time; ///< Time stamps for points in time history.
    std::vector<double> _amplitude; ///< Amplitude at points in time history.
    size_t _ilower; ///< Current index for point preceding current time.

private:

    // NOT IMPLEMENTED ////////////////////////////////////////////////////

    TimeHistory(void); ///< Not implemented
    TimeHistory(const TimeHistory&); ///< Not implemented
    const TimeHistory& operator=(const TimeHistory&); ///< Not implemented

}; // class TimeHistory

// End of file
