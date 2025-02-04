// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/spatialdb/SimpleDBData.hh"

#include "spatialdata/geocoords/CoordSys.hh"

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"

#include <sstream> // USES std::ostringstream
#include <stdexcept> // USES std::invalid_argument

namespace py = pybind11;

void
init_SimpleDBData(py::module_ &m) {
    py::class_<spatialdata::spatialdb::SimpleDBData>(m, "SimpleDBData")
    .def(py::init<>())
    .def("allocate", &spatialdata::spatialdb::SimpleDBData::allocate)
    .def("deallocate", &spatialdata::spatialdb::SimpleDBData::deallocate)

    .def_property_readonly("numLocs", &spatialdata::spatialdb::SimpleDBData::getNumLocs)
    .def_property_readonly("numValues", &spatialdata::spatialdb::SimpleDBData::getNumValues)
    .def_property_readonly("dataDim", &spatialdata::spatialdb::SimpleDBData::getDataDim)
    .def_property_readonly("spaceDim", &spatialdata::spatialdb::SimpleDBData::getSpaceDim)

    .def_property("names",
                  &spatialdata::spatialdb::SimpleDBData::getNames,
                  &spatialdata::spatialdb::SimpleDBData::setNames)
    .def_property("units",
                  nullptr,
                  &spatialdata::spatialdb::SimpleDBData::setUnits)
    .def_property("coordsys",
                  &spatialdata::spatialdb::SimpleDBData::getCoordSys,
                  &spatialdata::spatialdb::SimpleDBData::setCoordSys)

    .def("setData", [](spatialdata::spatialdb::SimpleDBData& dbData,
                       py::array_t<double, py::array::c_style | py::array::forcecast> coordinatesArray,
                       py::array_t<double, py::array::c_style | py::array::forcecast> valuesArray) {
        py::buffer_info coordinatesInfo = coordinatesArray.request();
        double* coordinates = static_cast<double*>(coordinatesInfo.ptr);
        if (2 != coordinatesInfo.ndim) {
            throw std::range_error(
                      "Error in 'coordinates' argument to SimpleDBData.setData(). Dimensions must be 2 [numLocs, spaceDim]."
                      );
        } // if
        const size_t numLocs = coordinatesInfo.shape[0];
        const size_t spaceDim = coordinatesInfo.shape[1];

        py::buffer_info valuesInfo = valuesArray.request();
        double* values = static_cast<double*>(valuesInfo.ptr);
        if (2 != valuesInfo.ndim) {
            throw std::range_error(
                      "Error in 'values' argument to SimpleDBData.setData(). Dimensions must be 2 [numLocs, numValues]."
                      );
        } // if
        const size_t numLocsV = valuesInfo.shape[0];
        const size_t numValues = valuesInfo.shape[1];

        if (numLocs != numLocsV) {
            std::ostringstream msg;
            msg << "Dimensions of coordinates (" << numLocs << ", " << spaceDim << ") "
                << "is incompatible with dimensions of values (" << numLocsV << ", " << numValues << ") "
                << "for SimpleDB data. Number of locations (dim=0) must match.";
            throw std::invalid_argument(msg.str());
        } // if
        for (size_t iLoc = 0, indexC = 0, indexV = 0; iLoc < numLocs; ++iLoc) {
            double* coordsLoc = dbData.getCoordinates(iLoc);
            for (size_t iDim = 0; iDim < spaceDim; ++iDim) {
                coordsLoc[iDim] = coordinates[indexC++];
            } // for

            double* valuesLoc = dbData.getData(iLoc);
            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                valuesLoc[iValue] = values[indexV++];
            } // for
        } // for
    })
    ;
}
