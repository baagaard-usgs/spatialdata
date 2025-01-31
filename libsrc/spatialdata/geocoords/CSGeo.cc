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

#include "CSGeo.hh" // implementation of class methods

#include "Converter.hh" // USES Converter
#include "spatialdata/utils/LineParser.hh" // USES LineParser

extern "C" {
#include "proj.h" // USES PROJ
}

#include <cmath> // USES M_PI, cos(), sin()
#include <cstring> // USES memcpy()
#include <sstream> // USES std::ostringsgream
#include <iostream> // USES std::istream, std::ostream

#include <strings.h> // USES strcasecmp()
#include <stdexcept> // USES std::runtime_error, std::exception
#include <assert.h> // USES assert()

// ----------------------------------------------------------------------
// Default constructor
spatialdata::geocoords::CSGeo::CSGeo(void) :
    _string("EPSG:4326" /* WGS84 */),
    _converter(new spatialdata::geocoords::Converter) {
    setSpaceDim(3);
    setCSType(GEOGRAPHIC);
} // constructor


// ----------------------------------------------------------------------
// Default destructor
spatialdata::geocoords::CSGeo::~CSGeo(void) {
    _converter.reset();
}


// ----------------------------------------------------------------------
// Set string specifying coordinate system.
void
spatialdata::geocoords::CSGeo::setString(const char* value) {
    _string = value;
} // setString


// ----------------------------------------------------------------------
// Get string specifying coordinate system.
const char*
spatialdata::geocoords::CSGeo::getString(void) const {
    return _string.c_str();
} // getString


// ----------------------------------------------------------------------
// Set number of spatial dimensions in coordinate system.
void
spatialdata::geocoords::CSGeo::setSpaceDim(const size_t spaceDim) {
    if (( spaceDim < 2) || ( spaceDim > 3) ) {
        std::ostringstream msg;
        msg
            << "Number of spatial dimensions (" << spaceDim
            << ") must be >= 2 and <= 3.";
        throw std::runtime_error(msg.str());
    } // if
    CoordSys::setSpaceDim(spaceDim);
} // setSpaceDim


// ----------------------------------------------------------------------
// Get outward surface normal.
void
spatialdata::geocoords::CSGeo::computeSurfaceNormal(double* normalDir,
                                                    const double* coordinates,
                                                    const size_t numLocs,
                                                    const size_t spaceDim,
                                                    const double dx) const {
    assert( (0 < numLocs && normalDir) || (0 == numLocs && !normalDir) );
    assert( (0 < numLocs && coordinates) || (0 == numLocs && !coordinates) );

    if (spaceDim != getSpaceDim()) {
        std::ostringstream msg;
        msg
            << "Number of spatial dimensions of coordinates ("
            << spaceDim << ") does not match number of spatial dimensions ("
            << getSpaceDim() << ") of coordinate system.";
        throw std::runtime_error(msg.str());
    } // if

    if (spaceDim > 2) {
        PJ* const proj = proj_create(PJ_DEFAULT_CTX, _string.c_str());
        const PJ_TYPE projType = proj_get_type(proj);
        proj_destroy(proj);
        switch (projType) {
        case PJ_TYPE_GEOGRAPHIC_2D_CRS:
        case PJ_TYPE_GEOGRAPHIC_3D_CRS:
        case PJ_TYPE_GEODETIC_CRS:
        case PJ_TYPE_PROJECTED_CRS:
        case PJ_TYPE_OTHER_COORDINATE_OPERATION:
            for (size_t i = 0; i < numLocs; ++i) {
                normalDir[i*spaceDim+0] = +0.0;
                normalDir[i*spaceDim+1] = +0.0;
                normalDir[i*spaceDim+2] = +1.0;
            } // for
            break;
        case PJ_TYPE_GEOCENTRIC_CRS: {
            // Surface normal is associated with geodetic lon/lat
            const CSGeo* csSrc = this;
            CSGeo csDest;
            csDest.setString("EPSG:4326"); // WGS84
            std::vector<double> coordsGeo(numLocs*spaceDim);
            std::copy(coordinates, coordinates+numLocs*spaceDim, coordsGeo.data());
            assert(_converter);
            _converter->convert(coordsGeo.data(), numLocs, spaceDim, &csDest, csSrc);
            for (size_t i = 0; i < numLocs; ++i) {
                const double latRad = coordsGeo[i*spaceDim+0] * M_PI/180.0;
                const double lonRad = coordsGeo[i*spaceDim+1] * M_PI/180.0;
                normalDir[i*spaceDim+0] = cos(latRad) * cos(lonRad);
                normalDir[i*spaceDim+1] = cos(latRad) * sin(lonRad);
                normalDir[i*spaceDim+2] = sin(latRad);
            } // for
            break;
        } // PJ_TYPE_GEOCENTRIC_CRS
        default: {
            std::cout << "Internal error: Coordinate system type (" << projType
                      << ") not recognized for coordinate system '" << _string << "' "
                      << "when computing normal of ground surface. Using default value of (0, 0, +1).";
            for (size_t i = 0; i < numLocs; ++i) {
                normalDir[i*spaceDim+0] = +0.0;
                normalDir[i*spaceDim+1] = +0.0;
                normalDir[i*spaceDim+2] = +1.0;
            } // for
        } // default

        } // switch
    } else {
        throw std::runtime_error("Outward surface normal not defined for 2-D geographic coordinates.");
    } // if/else

} // computeSurfaceNormal


// ----------------------------------------------------------------------
// Convert coordinates from local coordinate system to geographic coordinate system.
void
spatialdata::geocoords::CSGeo::localToGeographic(double* coordinates,
                                                 const size_t numLocs,
                                                 const size_t spaceDim) const {
}


// ----------------------------------------------------------------------
// Convert coordinates from geographic coordinate system to local coordinate system.
void
spatialdata::geocoords::CSGeo::geographicToLocal(double* coordinates,
                                                 const size_t numLocs,
                                                 const size_t spaceDim) const {
}


// ----------------------------------------------------------------------
// Pickle coordinate system to ascii stream.
void
spatialdata::geocoords::CSGeo::pickle(std::ostream& s) const {
    s << "geographic {\n"
      << "  crs-string = " << _string << "\n"
      << "  space-dim = " << getSpaceDim() << "\n"
      << "}\n";
} // pickle


// ----------------------------------------------------------------------
// Unpickle coordinate system from ascii stream.
void
spatialdata::geocoords::CSGeo::unpickle(std::istream& s) {
    utils::LineParser parser(s, "//");
    parser.eatwhitespace(true);

    std::string token;
    std::istringstream buffer;
    const int maxIgnore = 512;
    const int maxBuffer = 1024;
    char cbuffer[maxBuffer];

    // Set parameters to empty values.
    _string = "EPSG:4326"; // WGS84
    setSpaceDim(3);

    parser.ignore('{');
    buffer.str(parser.next());
    buffer.clear();
    buffer >> token;
    while (buffer.good() && token != "}") {
        buffer.ignore(maxIgnore, '=');
        if (0 == strcasecmp(token.c_str(), "crs-string")) {
            buffer >> std::ws;
            buffer.get(cbuffer, maxBuffer, '\n');
            this->setString(cbuffer);
        } else if (0 == strcasecmp(token.c_str(), "space-dim")) {
            int spaceDim;
            buffer >> spaceDim;
            this->setSpaceDim(spaceDim);
        } else {
            std::ostringstream msg;
            msg << "Could not parse '" << token << "' into a CSGeo token.\n"
                << "Known CSGeo tokens:\n"
                << "  crs-string, space-dim";
            throw std::runtime_error(msg.str().c_str());
        } // else
        buffer.str(parser.next());
        buffer.clear();
        buffer >> token;
    } // while
    if (token != "}") {
        throw std::runtime_error("I/O error while parsing CSGeo settings.");
    }
} // unpickle


// End of file
