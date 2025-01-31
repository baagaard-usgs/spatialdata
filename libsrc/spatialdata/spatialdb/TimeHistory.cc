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

#include "TimeHistory.hh" // Implementation of class methods

#include "TimeHistoryIO.hh" // USES TimeHistory

#include <cassert> // USES assert()

// ----------------------------------------------------------------------
/// Constructor with description
spatialdata::spatialdb::TimeHistory::TimeHistory(const char* description) :
    _description(description ? description : "TimeHistory :UNKNOWN:"),
    _filename(":UNKNOWN:"),
    _ilower(0) {}


// ----------------------------------------------------------------------
/// Default destructor
spatialdata::spatialdb::TimeHistory::~TimeHistory(void) {} // destructor


// ----------------------------------------------------------------------
// Get description of time history.
void
spatialdata::spatialdb::TimeHistory::setDescription(const char* description) {
    _description = description ? description : "TimeHistory :UNKNOWN:";
}


// ----------------------------------------------------------------------
// Get description of time history.
const char*
spatialdata::spatialdb::TimeHistory::getDescription(void) const {
    return _description.c_str();
}


// ----------------------------------------------------------------------
// Set filename for time history.
void
spatialdata::spatialdb::TimeHistory::setFilename(const char* filename) {
    if (!filename) {
        throw std::logic_error("Null argument to SimpleDB::setFilename().");
    } // if
    _filename = filename;
}


// ----------------------------------------------------------------------
// Set filename for time history.
const char*
spatialdata::spatialdb::TimeHistory::getFilename(void) {
    return _filename.c_str();
}


// ----------------------------------------------------------------------
// Open the time history and prepare for querying.
void
spatialdata::spatialdb::TimeHistory::open(void) {
    TimeHistoryIO::read(&_time, &_amplitude, _filename.c_str());
    _ilower = 0;
} // open


// ----------------------------------------------------------------------
// Close the time history.
void
spatialdata::spatialdb::TimeHistory::close(void) {
    _time.clear();_time.shrink_to_fit();
    _amplitude.clear();_amplitude.shrink_to_fit();
} // close


// ----------------------------------------------------------------------
// Query the database.
int
spatialdata::spatialdb::TimeHistory::query(double* value,
                                           const double t) {
    assert(value);
    const size_t numPoints = _time.size();
    assert(_time.size() == _amplitude.size());

    *value = 0.0;
    if (numPoints > 1) {
        if (t < _time[_ilower]) {
            while (_ilower > 0) {
                if (t >= _time[_ilower]) {
                    break;
                }
                --_ilower;
            } // while
        } else if (t > _time[_ilower+1]) {
            const size_t imax = numPoints-2;
            while (_ilower < imax) {
                if (t <= _time[_ilower+1]) {
                    break;
                }
                ++_ilower;
            } // while
        } // if/else

        assert(_ilower < numPoints-1);
        if (( t >= _time[_ilower]) && ( t <= _time[_ilower+1]) ) {
            const double tL = _time[_ilower];
            const double tU = _time[_ilower+1];
            const double wtL = (tU - t) / (tU - tL);
            const double wtU = (t - tL) / (tU - tL);
            *value = wtL * _amplitude[_ilower] + wtU * _amplitude[_ilower+1];
        } else {
            return 1;
        } // else
    } else {
        *value = _amplitude[0];
    } // else

    return 0;
} // query


// ----------------------------------------------------------------------
// Query the database.
int
spatialdata::spatialdb::TimeHistory::query(float* value,
                                           const float t) {
    double valueD = 0.0;
    const double tD = t;

    const int err = query(&valueD, tD);
    *value = valueD;
    return err;
} // query


// End of file
