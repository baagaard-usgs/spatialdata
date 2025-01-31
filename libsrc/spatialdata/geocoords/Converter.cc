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

#include "Converter.hh" // implementation of class methods

#include "CoordSys.hh" // USES CoordSys
#include "CSGeo.hh" // USES CSGeo
#include "CSCart.hh" // USES CSCart

extern "C" {
#include "proj.h" // USES PROJ
}

#include <cmath> // USES HUGE_VAL
#include <strings.h> // USES strcasecmp()
#include <stdexcept> // USES std::runtime_error, std::exception
#include <sstream> // USES std::ostringsgream
#include <cassert> // USES assert()

namespace spatialdata {
    namespace geocoords {
        namespace _converter {
            class Cache {
public:

                std::string csDest;
                std::string csSrc;
                PJ* proj;

                Cache(void) :
                    csDest(""),
                    csSrc(""),
                    proj(NULL) {}


                ~Cache(void) {
                    csDest = "";
                    csSrc = "";
                    proj_destroy(proj);proj = NULL;
                }

            }; // Cache

            /** Convert coordinates from source geographic coordinate system to
             * destination geographic coordinate system.
             *
             * @param[inout] coordinates Array of coordinates
             * @param[in] numLocs Number of location
             * @param[in] spaceDim Number of spatial dimensions in coordinates
             * @param[in] csDest Destination coordinate system
             * @param[in] csSrc Source coordinate system
             * @param[in] cache Cached projection.
             */
            static
            void convert(double* coordinates,
                         const size_t numLocs,
                         const size_t spaceDim,
                         const CSGeo* csDest,
                         const CSGeo* csSrc,
                         const std::unique_ptr<Cache>& cache);

            /** Convert coordinates from source Cartesian coordinate system to
             * destination Cartesian coordinate system.
             *
             * @param[inout] coordinates Array of coordinates
             * @param[in] numLocs Number of location
             * @param[in] spaceDim Number of spatial dimensions in coordinates
             * @param[in] csDest Destination coordinate system
             * @param[in] csSrc Source coordinate system
             * @param[in] cache Cached projection.
             */
            static
            void convert(double* coordinates,
                         const size_t numLocs,
                         const size_t spaceDim,
                         const CSCart* csDest,
                         const CSCart* csSrc,
                         const std::unique_ptr<Cache>& cache);

        } // _converter
    } // geocoords
} // spatialdata

// ----------------------------------------------------------------------
// Default constructor
spatialdata::geocoords::Converter::Converter(void) :
    _cache(new _converter::Cache) {}


// ----------------------------------------------------------------------
// Default destructor
spatialdata::geocoords::Converter::~Converter(void) {
    _cache.reset();
} // destructor


// ----------------------------------------------------------------------
// Convert coordinates from source coordinate system to destination
// coordinate system.
void
spatialdata::geocoords::Converter::convert(double* coordinates,
                                           const size_t numLocs,
                                           const size_t spaceDim,
                                           const CoordSys* csDest,
                                           const CoordSys* csSrc) {
    assert( (0 < numLocs && 0 != coordinates) ||
            (0 == numLocs && 0 == coordinates));
    assert(csDest);
    assert(csSrc);

    if (csSrc->getCSType() != csDest->getCSType()) {
        throw std::invalid_argument("Cannot convert between coordinate systems of different types.");
    } // if
    if (csSrc->getSpaceDim() != csDest->getSpaceDim()) {
        std::ostringstream msg;
        msg << "Cannot convert between coordinate systems with different spatial dimensions.\n"
            << "Source and destination coordinate systems have "
            << csSrc->getSpaceDim() << " and " << csDest->getSpaceDim()
            << " dimensions, respectively.";
        throw std::invalid_argument(msg.str());
    } // if

    switch (csSrc->getCSType()) {
    case spatialdata::geocoords::CoordSys::GEOGRAPHIC:
    { // GEOGRAPHIC
        const CSGeo* csGeoDest = dynamic_cast<const CSGeo*>(csDest);
        const CSGeo* csGeoSrc = dynamic_cast<const CSGeo*>(csSrc);
        _converter::convert(coordinates, numLocs, spaceDim, csGeoDest, csGeoSrc, _cache);
        break;
    } // GEOGRAPHIC
    case spatialdata::geocoords::CoordSys::CARTESIAN:
    { // CARTESIAN
        const CSCart* csCartDest = dynamic_cast<const CSCart*>(csDest);
        const CSCart* csCartSrc = dynamic_cast<const CSCart*>(csSrc);
        _converter::convert(coordinates, numLocs, spaceDim, csCartDest, csCartSrc, _cache);
        break;
    } // CARTESIAN
    default:
        throw std::logic_error("Could not parse coordinate system type.");
    } // switch
} // convert


// ----------------------------------------------------------------------
// Convert coordinates from source geographic coordinate system to
// destination geographic coordinate system.
void
spatialdata::geocoords::_converter::convert(double* coordinates,
                                            const size_t numLocs,
                                            const size_t spaceDim,
                                            const CSGeo* csDest,
                                            const CSGeo* csSrc,
                                            const std::unique_ptr<Cache>& cache) {
    assert(csDest);
    assert(csSrc);
    assert( (0 < numLocs && 0 != coordinates) ||
            (0 == numLocs && 0 == coordinates));

    double* const x = (spaceDim >= 2) ? coordinates + 0 : NULL;
    double* const y = (spaceDim >= 2) ? coordinates + 1 : NULL;
    double* const z = (spaceDim >= 3) ? coordinates + 2 : NULL;
    const size_t stride = spaceDim * sizeof(double);

    bool needsNewProj = false;
    assert(cache);
    if ((0 == cache->csSrc.length()) || (0 != strcasecmp(cache->csSrc.c_str(), csSrc->getString()))) { needsNewProj = true; }
    if ((0 == cache->csDest.length()) || (0 != strcasecmp(cache->csDest.c_str(), csDest->getString()))) { needsNewProj = true; }
    if (needsNewProj) {
        proj_destroy(cache->proj);
        cache->proj = proj_create_crs_to_crs(PJ_DEFAULT_CTX, csSrc->getString(), csDest->getString(), NULL);
        if (!cache->proj) {
            std::stringstream msg;
            msg << "Error creating projection from '" << csSrc->getString() << "' to '" << csDest->getString() << "'.\n"
                << proj_errno_string(proj_errno(cache->proj));
            throw std::runtime_error(msg.str());
        } // if
        cache->csSrc = csSrc->getString();
        cache->csDest = csDest->getString();
    } // if

    csSrc->localToGeographic(coordinates, numLocs, spaceDim);

    double t = HUGE_VAL;
    const size_t numSuccessful =
        proj_trans_generic(cache->proj, PJ_FWD,
                           x, stride, numLocs,
                           y, stride, numLocs,
                           z, stride, numLocs,
                           &t, 0, numLocs);
    if (numSuccessful < numLocs) {
        std::ostringstream msg;
        msg << "Error while converting coordinates:\n"
            << "  " << proj_errno_string(proj_errno(cache->proj));
        throw std::runtime_error(msg.str());
    } // if

    csDest->geographicToLocal(coordinates, numLocs, spaceDim);

} // convert


// ----------------------------------------------------------------------
// Convert coordinates from source Cartesian coordinate system to
// destination Cartesian coordinate system.
void
spatialdata::geocoords::_converter::convert(double* coordinates,
                                            const size_t numLocs,
                                            const size_t spaceDim,
                                            const CSCart* csDest,
                                            const CSCart* csSrc,
                                            const std::unique_ptr<Cache>& cache) {
    assert(csDest);
    assert(csSrc);
    assert( (0 < numLocs && 0 != coordinates) ||
            (0 == numLocs && 0 == coordinates));

    const int size = numLocs*spaceDim;
    const double scale = csSrc->getToMeters() / csDest->getToMeters();
    for (int i = 0; i < size; ++i) {
        coordinates[i] *= scale;
    } // for
} // convert


// End of file
