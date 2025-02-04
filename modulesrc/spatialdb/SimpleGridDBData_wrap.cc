// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/spatialdb/SimpleGridDBData.hh"

#include "spatialdata/geocoords/CoordSys.hh"

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"

#include <sstream> // USES std::ostringstream
#include <stdexcept> // USES std::invalid_argument

namespace py = pybind11;

void
init_SimpleGridDBData(py::module_ &m) {
    py::class_<spatialdata::spatialdb::SimpleGridDBData>(m, "SimpleGridDBData")
    .def(py::init<>())
    .def("allocate", &spatialdata::spatialdb::SimpleGridDBData::allocate)
    .def("deallocate", &spatialdata::spatialdb::SimpleGridDBData::deallocate)

    .def_property_readonly("numX", &spatialdata::spatialdb::SimpleGridDBData::getNumX)
    .def_property_readonly("numY", &spatialdata::spatialdb::SimpleGridDBData::getNumY)
    .def_property_readonly("numZ", &spatialdata::spatialdb::SimpleGridDBData::getNumZ)
    .def_property_readonly("numValues", &spatialdata::spatialdb::SimpleGridDBData::getNumValues)
    .def_property_readonly("dataDim", &spatialdata::spatialdb::SimpleGridDBData::getDataDim)
    .def_property_readonly("spaceDim", &spatialdata::spatialdb::SimpleGridDBData::getSpaceDim)

    .def_property("names",
                  &spatialdata::spatialdb::SimpleGridDBData::getNames,
                  &spatialdata::spatialdb::SimpleGridDBData::setNames)
    .def_property("units",
                  nullptr,
                  &spatialdata::spatialdb::SimpleGridDBData::setUnits)
    .def_property("coordsys",
                  &spatialdata::spatialdb::SimpleGridDBData::getCoordSys,
                  &spatialdata::spatialdb::SimpleGridDBData::setCoordSys)

    .def_property("x",
                  nullptr,
                  [](spatialdata::spatialdb::SimpleGridDBData& dbData,
                     py::array_t<double, py::array::c_style | py::array::forcecast> coordinatesArray){
        py::buffer_info coordinatesInfo = coordinatesArray.request();
        double* coordinates = static_cast<double*>(coordinatesInfo.ptr);
        if (1 != coordinatesInfo.ndim) {
            throw std::range_error(
                      "Error in 'coordinates' argument to SimpleGridDBData.setX(). Dimensions must be 1 [numLocs]."
                      );
        } // if
        const size_t numLocs = coordinatesInfo.shape[0];
        std::vector<double>& coordsData = dbData.getX();
        for (size_t i = 0; i < numLocs; ++i) {
            coordsData[i] = coordinates[i];
        } // for
    })
    .def_property("y",
                  nullptr,
                  [](spatialdata::spatialdb::SimpleGridDBData& dbData,
                     py::array_t<double, py::array::c_style | py::array::forcecast> coordinatesArray){
        py::buffer_info coordinatesInfo = coordinatesArray.request();
        double* coordinates = static_cast<double*>(coordinatesInfo.ptr);
        if (1 != coordinatesInfo.ndim) {
            throw std::range_error(
                      "Error in 'coordinates' argument to SimpleGridDBData.setY(). Dimensions must be 1 [numLocs]."
                      );
        } // if
        const size_t numLocs = coordinatesInfo.shape[0];
        std::vector<double>& coordsData = dbData.getY();
        for (size_t i = 0; i < numLocs; ++i) {
            coordsData[i] = coordinates[i];
        } // for
    })
    .def_property("z",
                  nullptr,
                  [](spatialdata::spatialdb::SimpleGridDBData& dbData,
                     py::array_t<double, py::array::c_style | py::array::forcecast> coordinatesArray){
        py::buffer_info coordinatesInfo = coordinatesArray.request();
        double* coordinates = static_cast<double*>(coordinatesInfo.ptr);
        if (1 != coordinatesInfo.ndim) {
            throw std::range_error(
                      "Error in 'coordinates' argument to SimpleGridDBData.setZ(). Dimensions must be 1 [numLocs]."
                      );
        } // if
        const size_t numLocs = coordinatesInfo.shape[0];
        std::vector<double>& coordsData = dbData.getZ();
        for (size_t i = 0; i < numLocs; ++i) {
            coordsData[i] = coordinates[i];
        } // for
    })

    .def("setData", [](spatialdata::spatialdb::SimpleGridDBData& dbData,
                       py::array_t<double, py::array::c_style | py::array::forcecast> coordinatesArray,
                       py::array_t<double, py::array::c_style | py::array::forcecast> valuesArray) {
        py::buffer_info coordinatesInfo = coordinatesArray.request();
        double* coordinates = static_cast<double*>(coordinatesInfo.ptr);
        if (2 != coordinatesInfo.ndim) {
            throw std::range_error(
                      "Error in 'coordinates' argument to SimpleGridDBData.setData(). Dimensions must be 2 [numLocs, spaceDim]."
                      );
        } // if
        const size_t numLocs = coordinatesInfo.shape[0];
        const size_t spaceDim = coordinatesInfo.shape[1];

        py::buffer_info valuesInfo = valuesArray.request();
        double* values = static_cast<double*>(valuesInfo.ptr);
        if (2 != valuesInfo.ndim) {
            throw std::range_error(
                      "Error in 'values' argument to SimpleGridDBData.setData(). Dimensions must be 2 [numLocs, numValues]."
                      );
        } // if
        const size_t numLocsV = valuesInfo.shape[0];
        const size_t numValues = valuesInfo.shape[1];

        if (numLocs != numLocsV) {
            std::ostringstream msg;
            msg << "Dimensions of coordinates (" << numLocs << ", " << spaceDim << ") "
                << "is incompatible with dimensions of values (" << numLocsV << ", " << numValues << ") "
                << "for SimpleGridDB data. Number of locations (dim=0) must match.";
            throw std::invalid_argument(msg.str());
        } // if
        for (size_t iLoc = 0, index = 0; iLoc < numLocs; ++iLoc) {
            const size_t indexLoc = dbData.getDataIndex(&coordinates[iLoc*spaceDim], spaceDim);
            double* const dataLoc = dbData.getData(indexLoc);
            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                dataLoc[iValue] = values[index++];
            } // for
        } // for
    })
    ;
}
