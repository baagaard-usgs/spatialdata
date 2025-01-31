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

#include "SpatialDB.hh" // USES SimpleDB
#include "SimpleDB.hh" // USES SimpleDB
#include "SimpleDBQuery.hh" // implementation of class methods

#include "SimpleDBData.hh" // USEs SimpleDBData

#include "spatialdata/geocoords/CoordSys.hh" // USES CoordSys
#include "spatialdata/geocoords/Converter.hh" // USES Converter

#include "Exception.hh" // USES OutOfBounds

#include <math.h> // USES MAXFLOAT
#if !defined(MAXFLOAT)
#define MAXFLOAT 1e+30
#endif

#include <cstring> // USES memcpy()
#include <strings.h> // USES strcasecmp()
#include <cassert> // USES assert()
#include <sstream> // USES std::ostringsgream
#include <algorithm> // USES std::fill(), std::lower_bound(), std::distance()

// ----------------------------------------------------------------------
// Default constructor.
spatialdata::spatialdb::SimpleDBQuery::SimpleDBQuery(const SimpleDBData& data,
                                                     const char* description) :
    _data(data),
    _description(description),
    _queryType(SimpleDB::LINEAR),
    _converter(new spatialdata::geocoords::Converter) {}


// ----------------------------------------------------------------------
// Default destructor.
spatialdata::spatialdb::SimpleDBQuery::~SimpleDBQuery(void) {
    deallocate();
} // destructor


// ----------------------------------------------------------------------
// Deallocate data structures.
void
spatialdata::spatialdb::SimpleDBQuery::deallocate(void) {
    _nearest.clear();_nearest.shrink_to_fit();
    _queryIndices.clear();_queryIndices.shrink_to_fit();
} // deallocate


// ----------------------------------------------------------------------
// Set query type.
void
spatialdata::spatialdb::SimpleDBQuery::setQueryType(const SimpleDB::QueryEnum value) {
    _queryType = value;
} // setQueryType


// ----------------------------------------------------------------------
// Set values to be returned by queries.
void
spatialdata::spatialdb::SimpleDBQuery::setQueryValues(const std::vector<std::string>& names) {
    if (0 == names.size()) {
        std::ostringstream msg;
        msg << "Number of values for query in spatial database " << _description
            << " must be positive.\n";
        throw std::invalid_argument(msg.str());
    } // if

    const size_t querySize = names.size();
    _queryIndices.clear();
    _queryIndices.reserve(querySize);
    for (size_t iQuery = 0; iQuery < querySize; ++iQuery) {
        size_t iName = 0;
        const size_t numNames = _data.getNumValues();
        while (iName < numNames) {
            if (0 == strcasecmp(names[iQuery].c_str(), _data.getName(iName))) {
                break;
            } // if
            ++iName;
        } // while
        if (iName >= numNames) {
            std::ostringstream msg;
            msg << "Could not find value '" << names[iQuery] << "' in spatial database '"
                << _description << "'. Available values are:";
            for (size_t iName = 0; iName < numNames; ++iName) {
                msg << "\n  " << _data.getName(iName);
            }
            msg << "\n";
            throw std::out_of_range(msg.str());
        } // if
        _queryIndices.emplace_back(iName);
    } // for
} // setQueryVals


// ----------------------------------------------------------------------
// Query the database.
void
spatialdata::spatialdb::SimpleDBQuery::query(double* values,
                                             const size_t numValues,
                                             const double* coordinates,
                                             const spatialdata::geocoords::CoordSys* csCoordinates) {
    assert(!numValues || values);
    assert(coordinates);
    assert(csCoordinates);

    const size_t spaceDim = csCoordinates->getSpaceDim();
    const size_t querySize = _queryIndices.size();
    if (0 == querySize) {
        std::ostringstream msg;
        msg << "Values to be returned by spatial database " << _description << "\n"
            << "have not been set. Please call setQueryValues() before query().\n";
        throw std::logic_error(msg.str());
    } // if
    else if (numValues != querySize) {
        std::ostringstream msg;
        msg << "Number of values to be returned by spatial database "
            << _description << "\n"
            << "(" << querySize << ") does not match size of array provided ("
            << numValues << ").\n";
        throw std::invalid_argument(msg.str());
    } else if (spaceDim != _data.getSpaceDim()) {
        std::ostringstream msg;
        msg << "Spatial dimension (" << spaceDim
            << ") does not match spatial dimension of spatial database (" << _data.getSpaceDim() << ").";
        throw std::invalid_argument(msg.str());
    } // if

    const size_t numLocs = 1;
    _queryPoint[0] = 0.0;
    _queryPoint[1] = 0.0;
    _queryPoint[2] = 0.0;
    for (size_t i = 0; i < spaceDim; ++i) {
        _queryPoint[i] = coordinates[i];
    } // for
    assert(_converter);

    _converter->convert(_queryPoint, numLocs, spaceDim, _data.getCoordSys(), csCoordinates);

    switch (_queryType) {
    case SimpleDB::LINEAR:
        _queryLinear(values, numValues);
        break;
    case SimpleDB::NEAREST:
        _queryNearest(values, numValues);
        break;
    default:
        throw std::logic_error("Could not find requested query type.");
    } // switch
} // query


// ----------------------------------------------------------------------
// Query database using nearest neighbor algorithm.
void
spatialdata::spatialdb::SimpleDBQuery::_queryNearest(double* values,
                                                     const size_t numValues) {
    assert( (0 < numValues && values) ||
            (0 == numValues && !values) );
    assert(numValues == _queryIndices.size());

    size_t i_near = 0;
    const size_t spaceDim = _data.getSpaceDim();
    double point[3];
    _setPoint3(point, _data.getCoordinates(i_near), spaceDim);
    double nearDist = _distSquared(_queryPoint, point);

    const size_t numLocs = _data.getNumLocs();
    for (size_t iLoc = 1; iLoc < numLocs; ++iLoc) {
        _setPoint3(point, _data.getCoordinates(iLoc), spaceDim);
        const double dist = _distSquared(_queryPoint, point);
        if (dist < nearDist) {
            nearDist = dist;
            i_near = iLoc;
        } // if
    } // for

    const double* nearVals = _data.getData(i_near);
    const size_t querySize = _queryIndices.size();
    for (size_t iValue = 0; iValue < querySize; ++iValue) {
        values[iValue] = nearVals[_queryIndices[iValue]];
    } // for
} // _queryNearest


// ----------------------------------------------------------------------
// Query database using linear interpolation algorithm.
void
spatialdata::spatialdb::SimpleDBQuery::_queryLinear(double* values,
                                                    const size_t numValues) {
    assert( (0 < numValues && values) ||
            (0 == numValues && !values) );
    const size_t querySize = _queryIndices.size();
    assert(numValues == querySize);

    if (0 == _data.getDataDim()) {
        const int index = 0;
        const double* nearVals = _data.getData(index);
        for (size_t iValue = 0; iValue < numValues; ++iValue) {
            values[iValue] = nearVals[_queryIndices[iValue]];
        }
    } else { // else
        // Find nearest locations in database
        _findNearest();

        // Get interpolation weights
        std::vector<Weighting> weights;
        _getWeights(&weights);

        // Interpolate values
        const size_t numWts = weights.size();
        for (size_t iValue = 0; iValue < numValues; ++iValue) {
            double val = 0;
            for (size_t iWt = 0; iWt < numWts; ++iWt) {
                const size_t iLoc = _nearest[weights[iWt].i_near];
                const double* locVals = _data.getData(iLoc);
                val += weights[iWt].wt * locVals[_queryIndices[iValue]];
            } // for
            values[iValue] = val;
        } // for
    } // else
} // _queryLinear


// ----------------------------------------------------------------------
void
spatialdata::spatialdb::SimpleDBQuery::_findNearest(void) {
    const size_t maxnear = 100;
    const size_t numLocs = _data.getNumLocs();
    const size_t nearSize = (numLocs < maxnear) ? numLocs : maxnear;
    _nearest.resize(nearSize);
    std::fill(_nearest.begin(), _nearest.end(), -1);

    std::vector<double> nearestDist(nearSize);
    std::fill(nearestDist.begin(), nearestDist.end(), MAXFLOAT);

    // find closest nearSize points
    double point[3];
    const size_t spaceDim = _data.getSpaceDim();
    for (size_t iLoc = 0; iLoc < numLocs; ++iLoc) {
        // use square of distance to find closest
        _setPoint3(point, _data.getCoordinates(iLoc), spaceDim);
        const double dist2 = _distSquared(_queryPoint, point);

        // find place in nearest list if it exists
        const std::vector<double>::iterator pNearDist =
            std::lower_bound(nearestDist.begin(), nearestDist.end(), dist2);
        if (dist2 <= *pNearDist) { // if found place in nearest list
#if !defined(_RWSTD_NO_CLASS_PARTIAL_SPEC)
            size_t index = std::distance(nearestDist.begin(), pNearDist);
#else
            size_t index = 0;
            std::distance(nearestDist.begin(), pNearDist, index);
#endif

            // move values to allow insertion of current
            for (size_t moveIndex = nearSize-1; moveIndex > index; --moveIndex) {
                nearestDist[moveIndex] = nearestDist[moveIndex-1];
                _nearest[moveIndex] = _nearest[moveIndex-1];
            } // for
            if (index < nearSize) {
                // insert current
                nearestDist[index] = dist2;
                _nearest[index] = iLoc;
            } // if
        } // if
    } // for
} // _findNearest


// ----------------------------------------------------------------------
void
spatialdata::spatialdb::SimpleDBQuery::_getWeights(std::vector<Weighting>* pWeights) {
    assert(pWeights);

    /* Start with nearest point. Add next nearest points as necessary
     * to obtain appropriate interpolation. For example, adding 1 point
     * results in linear interpolation, adding 2 results in areal
     * interpolation, etc.
     */
    const size_t dataDim = _data.getDataDim();
    if (0 == dataDim) {
        const int numWts = 1;
        pWeights->resize(numWts);
        _findPointPoint(pWeights);
    } else if (1 == dataDim) {
        const int numWts = 2;
        pWeights->resize(numWts);
        _findPointPoint(pWeights);
        _findLinePoint(pWeights);
    } else if (2 == dataDim) {
        const int numWts = 3;
        pWeights->resize(numWts);
        _findPointPoint(pWeights);
        _findLinePoint(pWeights);
        _findAreaPoint(pWeights);
    } else if (3 == dataDim) {
        const int numWts = 4;
        pWeights->resize(numWts);
        _findPointPoint(pWeights);
        _findLinePoint(pWeights);
        _findAreaPoint(pWeights);
        _findVolumePoint(pWeights);
    } else {
        throw std::logic_error("Could not set weights for unknown data dimension.");
    } // if/else
} // _getWeights


// ----------------------------------------------------------------------
void
spatialdata::spatialdb::SimpleDBQuery::_findPointPoint(std::vector<Weighting>* pWeights) {
    assert(pWeights);

    (*pWeights)[0].wt = 1.0;
    (*pWeights)[0].i_near = 0;
} // _findPointPoint


// ----------------------------------------------------------------------
void
spatialdata::spatialdb::SimpleDBQuery::_findLinePoint(std::vector<Weighting>* pWeights) {
    assert(pWeights);

    const size_t spaceDim = _data.getSpaceDim();

    // best case is to use next nearest pt
    const size_t nearIndexA = (*pWeights)[0].i_near;
    size_t nearIndexB = nearIndexA + 1;

    const size_t locIndexA = _nearest[nearIndexA];
    double ptA[3];
    assert(locIndexA >= 0);
    _setPoint3(ptA, _data.getCoordinates(locIndexA), spaceDim);

    double wtA = 0;
    double wtB = 0;
    double ptB[3];

    // find nearest pt where we can interpolate
    const size_t nearSize = _nearest.size();
    while (nearIndexB < nearSize) {
        const size_t locIndexB = _nearest[nearIndexB];
        _setPoint3(ptB, _data.getCoordinates(locIndexB), spaceDim);

        // wtA = DotProduct(pb, ab) / DotProduct(ab, ab)
        // wtB = DotProduct(ap, ab) / DotProduct(ab, ab)
        const double abX = ptB[0] - ptA[0];
        const double abY = ptB[1] - ptA[1];
        const double abZ = ptB[2] - ptA[2];
        const double abdotab = abX*abX + abY*abY + abZ*abZ;
        const double pbdotab = (ptB[0]-_queryPoint[0])*abX + (ptB[1]-_queryPoint[1])*abY + (ptB[2]-_queryPoint[2])*abZ;
        const double apdotab = (_queryPoint[0]-ptA[0])*abX + (_queryPoint[1]-ptA[1])*abY + (_queryPoint[2]-ptA[2])*abZ;
        wtA = pbdotab / abdotab;
        wtB = apdotab / abdotab;

        const double tolerance = 1e-06;
        if (( wtA >= -tolerance) &&
            ( wtB >= -tolerance) ) {
            // found 2nd point if weights are positive within tolerance level
            // (i.e. interpolation, not extrapolation)
            break;
        }
        ++nearIndexB;
    } // while
    if (nearIndexB >= nearSize) {
        throw OutOfBounds("Could not find points for linear interpolation.");
    }
    (*pWeights)[0].wt = wtA;
    (*pWeights)[1].wt = wtB;
    (*pWeights)[1].i_near = nearIndexB;
} // _findLinePoint


// ----------------------------------------------------------------------
void
spatialdata::spatialdb::SimpleDBQuery::_findAreaPoint(std::vector<Weighting>* pWeights) { // _findAreaPoint
    assert(pWeights);

    const size_t spaceDim = _data.getSpaceDim();

    // best case is to use next nearest pt
    const size_t nearIndexA = (*pWeights)[0].i_near;
    const size_t locIndexA = _nearest[nearIndexA];
    double ptA[3];
    _setPoint3(ptA, _data.getCoordinates(locIndexA), spaceDim);

    const size_t nearIndexB = (*pWeights)[1].i_near;
    const size_t locIndexB = _nearest[nearIndexB];
    double ptB[3];
    _setPoint3(ptB, _data.getCoordinates(locIndexB), spaceDim);

    double wtA = 0;
    double wtB = 0;
    double wtC = 0;
    double ptC[3];

    // find nearest pt where we can interpolate
    const size_t nearSize = _nearest.size();
    size_t nearIndexC = nearIndexB + 1;
    while (nearIndexC < nearSize) {
        const size_t locIndexC = _nearest[nearIndexC];
        _setPoint3(ptC, _data.getCoordinates(locIndexC), spaceDim);

        double areaABC = 0;
        double dirABC[3];
        _area(&areaABC, dirABC, ptA, ptB, ptC);

#if 0
        // Alternate method of determining co-linearity.
        // Compute unit vectors AB and AC, then compute the dot product.
        // If the absolute value of the dot product is somewhat less than 1,
        // the points are not collinear.
        double vecAB[3];
        double vecAC[3];
        double magAB = 0.0;
        double magAC = 0.0;
        for (int iDir = 0; iDir < numCoords; ++iDir) {
            vecAB[iDir] = ptA[iDir] - ptB[iDir];
            vecAC[iDir] = ptA[iDir] - ptC[iDir];
            magAB += vecAB[iDir] * vecAB[iDir];
            magAC += vecAC[iDir] * vecAC[iDir];
        } // for
        magAB = sqrt(magAB);
        magAC = sqrt(magAC);
        double abdotac = 0.0;
        for (int iDir = 0; iDir < numCoords; ++iDir) {
            abdotac += (vecAB[iDir]/magAB) * (vecAC[iDir]/magAC);
        } // for

        const double tolerance = 0.98;
        if (fabs(abdotac) < tolerance) {
#else
        // make sure A,B,C are not collinear by checking if area of
        // triangle ABC is not a tiny fraction of the distance AB

        // length(ab)^2
        const double ab2 =
            pow(ptA[0]-ptB[0], 2) +
            pow(ptA[1]-ptB[1], 2) +
            pow(ptA[2]-ptB[2], 2);

        const double tolerance = 1.0e-06;
        if (areaABC > tolerance*ab2) {
#endif
            // project P onto abc plane
            double qProj[3];
            const double qmod = dirABC[0]*_queryPoint[0] + dirABC[1]*_queryPoint[1] + dirABC[2]*_queryPoint[2];
            qProj[0] = _queryPoint[0] - dirABC[0]*qmod;
            qProj[1] = _queryPoint[1] - dirABC[1]*qmod;
            qProj[2] = _queryPoint[2] - dirABC[2]*qmod;

            // wtA = areaBCQ / areaABC * DotProduct(dirBCQ, dirABC);
            double areaBCQ = 0;
            double dirBCQ[3];
            _area(&areaBCQ, dirBCQ, ptB, ptC, qProj);
            wtA = areaBCQ/areaABC * (dirBCQ[0]*dirABC[0] +
                                     dirBCQ[1]*dirABC[1] +
                                     dirBCQ[2]*dirABC[2]);

            // wtB = areaCAQ / areaABC * DotProduct(dirCAQ, dirABC);
            double areaCAQ = 0;
            double dirCAQ[3];
            _area(&areaCAQ, dirCAQ, ptC, ptA, qProj);
            wtB = areaCAQ/areaABC * (dirCAQ[0]*dirABC[0] +
                                     dirCAQ[1]*dirABC[1] +
                                     dirCAQ[2]*dirABC[2]);

            // wtC = areaABQ / areaABC * DotProduct(dirABQ, dirABC);
            double areaABQ = 0;
            double dirABQ[3];
            _area(&areaABQ, dirABQ, ptA, ptB, qProj);
            wtC = areaABQ/areaABC * (dirABQ[0]*dirABC[0] +
                                     dirABQ[1]*dirABC[1] +
                                     dirABQ[2]*dirABC[2]);

            const double tolerance = 1e-06;
            if (( wtA >= -tolerance) &&
                ( wtB >= -tolerance) &&
                ( wtC >= -tolerance) ) {
                // found 3rd point if weights are positive within tolerance level
                // (i.e. interpolation, not extrapolation)
                break;
            }
        } // if
        ++nearIndexC;
    } // while
    if (nearIndexC >= nearSize) {
        throw OutOfBounds("Could not find points for areal interpolation.");
    }
    (*pWeights)[0].wt = wtA;
    (*pWeights)[1].wt = wtB;
    (*pWeights)[2].wt = wtC;
    (*pWeights)[2].i_near = nearIndexC;
} // _findAreaPoint


// ----------------------------------------------------------------------
void
spatialdata::spatialdb::SimpleDBQuery::_findVolumePoint(std::vector<Weighting>* pWeights) {
    assert(pWeights);

    // best case is to use next nearest pt

    const size_t spaceDim = _data.getSpaceDim();

    const size_t nearIndexA = (*pWeights)[0].i_near;
    const size_t locIndexA = _nearest[nearIndexA];
    double ptA[3];
    _setPoint3(ptA, _data.getCoordinates(locIndexA), spaceDim);

    const size_t nearIndexB = (*pWeights)[1].i_near;
    const size_t locIndexB = _nearest[nearIndexB];
    double ptB[3];
    _setPoint3(ptB, _data.getCoordinates(locIndexB), spaceDim);

    const size_t nearIndexC = (*pWeights)[2].i_near;
    const size_t locIndexC = _nearest[nearIndexC];
    double ptC[3];
    _setPoint3(ptC, _data.getCoordinates(locIndexC), spaceDim);

    double wtA = 0;
    double wtB = 0;
    double wtC = 0;
    double wtD = 0;
    double ptD[3];

    // find nearest pt where we can interpolate
    const size_t nearSize = _nearest.size();
    size_t nearIndexD = nearIndexC + 1;
    while (nearIndexD < nearSize) {
        const size_t locIndexD = _nearest[nearIndexD];
        _setPoint3(ptD, _data.getCoordinates(locIndexD), spaceDim);

        // make sure A,B,C,D are not coplanar by checking if volume of
        // tetrahedron ABCD is not a tiny fraction of the distance AB

        // volume abcd
        const double abcd = _volume(ptA, ptB, ptC, ptD);

        // ab = length(ab)
        const double ab3 =
            pow(pow(ptA[0] - ptB[0], 2) +
                pow(ptA[1] - ptB[1], 2) +
                pow(ptA[2] - ptB[2], 2), 3);

        const double tolerance = 1.0e-06;
        if (fabs(abcd) > tolerance*ab3) {
            // volume pbcd
            const double pbcd = _volume(_queryPoint, ptB, ptC, ptD);
            // wtA = vol(pbcd)/vol(abcd)
            wtA = pbcd / abcd;

            // volume apcd
            const double apcd = _volume(ptA, _queryPoint, ptC, ptD);
            // wtB = vol(apcd)/vol(abcd)
            wtB = apcd / abcd;

            // volume abpd
            const double abpd = _volume(ptA, ptB, _queryPoint, ptD);
            // wtC = vol(abpd)/vol(abcd)
            wtC = abpd / abcd;

            // volume abcp
            const double abcp = _volume(ptA, ptB, ptC, _queryPoint);
            // wtD = vol(abcp)/vol(abcd)
            wtD = abcp / abcd;

            const double tolerance = 1e-06;
            if (( wtA >= -tolerance) &&
                ( wtB >= -tolerance) &&
                ( wtC >= -tolerance) &&
                ( wtD >= -tolerance) ) {
                // found 4th point if weights are positive within tolerance level
                // (i.e. interpolation, not extrapolation)
                break;
            }
        } // if
        ++nearIndexD;
    } // while
    if (nearIndexD >= nearSize) {
        throw OutOfBounds("Could not find points for volumetric interpolation.");
    }
    (*pWeights)[0].wt = wtA;
    (*pWeights)[1].wt = wtB;
    (*pWeights)[2].wt = wtC;
    (*pWeights)[3].wt = wtD;
    (*pWeights)[3].i_near = nearIndexD;
} // _findVolumePoint


// ----------------------------------------------------------------------
// Set coordiantes of point in 3-D space using coordinates in
// current coordinate system.
void
spatialdata::spatialdb::SimpleDBQuery::_setPoint3(double pt3[3],
                                                  const double* pt,
                                                  const size_t spaceDim) {
    pt3[0] = 0.0;
    pt3[1] = 0.0;
    pt3[2] = 0.0;
    memcpy(pt3, pt, spaceDim*sizeof(double));
} // _setPoint3


// ----------------------------------------------------------------------
// Compute square of distance between points A and B.
double
spatialdata::spatialdb::SimpleDBQuery::_distSquared(const double a[3],
                                                    const double b[3]) {
    const double abX = b[0]-a[0];
    const double abY = b[1]-a[1];
    const double abZ = b[2]-a[2];
    const double dist2 = abX*abX + abY*abY + abZ*abZ;
    return dist2;
} // _distSquared


// ----------------------------------------------------------------------
/* Compute area and "direction" of triangle abc. Direction is vector
 * normal to triangular surface.
 */
void
spatialdata::spatialdb::SimpleDBQuery::_area(double* pArea,
                                             double dir[3],
                                             const double* a,
                                             const double* b,
                                             const double* c) {
    assert(pArea);
    assert(dir);

    // AB
    const double abX = b[0] - a[0];
    const double abY = b[1] - a[1];
    const double abZ = b[2] - a[2];

    // AC
    const double acX = c[0] - a[0];
    const double acY = c[1] - a[1];
    const double acZ = c[2] - a[2];

    dir[0] = abY*acZ - abZ*acY;
    dir[1] = abZ*acX - abX*acZ;
    dir[2] = abX*acY - abY*acX;

    *pArea = sqrt(dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2]);
    if (0 != *pArea) {
        dir[0] /= *pArea;
        dir[1] /= *pArea;
        dir[2] /= *pArea;
    } // if
    *pArea *= 0.5;
} // _area


// ----------------------------------------------------------------------
double
spatialdata::spatialdb::SimpleDBQuery::_volume(const double a[3],
                                               const double b[3],
                                               const double c[3],
                                               const double d[3]) {
    /* Compute volume of tetrahedral defined by four points. */
    // _volume
    const double det1 =
        b[0] * (c[1]*d[2] - c[2]*d[1]) +
        b[1] * (c[2]*d[0] - c[0]*d[2]) +
        b[2] * (c[0]*d[1] - c[1]*d[0]);
    const double det2 =
        -c[0] * (d[1]*a[2] - d[2]*a[1]) -
        c[1] * (d[2]*a[0] - d[0]*a[2]) -
        c[2] * (d[0]*a[1] - d[1]*a[0]);
    const double det3 =
        d[0] * (a[1]*b[2] - a[2]*b[1]) +
        d[1] * (a[2]*b[0] - a[0]*b[2]) +
        d[2] * (a[0]*b[1] - a[1]*b[0]);
    const double det4 =
        -a[0] * (b[1]*c[2] - b[2]*c[1]) -
        a[1] * (b[2]*c[0] - b[0]*c[2]) -
        a[2] * (b[0]*c[1] - b[1]*c[0]);
    return (det1 + det2 + det3 + det4) / 6.0;
} // _volume


// End of file
