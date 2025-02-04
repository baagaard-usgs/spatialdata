// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/spatialdb/TimeHistory.hh"

#include "SpatialDB_pyapi.hh" // Python interface for query()

#include "pybind11/pybind11.h"
#include "pybind11/numpy.h"

namespace py = pybind11;

void
init_TimeHistory(py::module_ &m) {
    py::class_<spatialdata::spatialdb::TimeHistory, std::shared_ptr<spatialdata::spatialdb::TimeHistory> >(m, "TimeHistory")
    .def(py::init<const char*>())
    .def("setDescription", &spatialdata::spatialdb::TimeHistory::setDescription)
    .def("getDescription", &spatialdata::spatialdb::TimeHistory::getDescription)
    .def("setFilename", &spatialdata::spatialdb::TimeHistory::setFilename)
    .def("getFilename", &spatialdata::spatialdb::TimeHistory::getFilename)
    .def("open", &spatialdata::spatialdb::TimeHistory::open)
    .def("close", &spatialdata::spatialdb::TimeHistory::close)

    .def("query", [](spatialdata::spatialdb::TimeHistory& th,
                     py::array_t<double, py::array::c_style | py::array::forcecast > timesArray) {
        py::buffer_info timesInfo = timesArray.request();
        const double* const times = static_cast<const double*>(timesInfo.ptr);
        if (1 != timesInfo.ndim) {
            throw std::range_error(
                      "Error in 'times' argument to TimeHistory.query(). Dimensions must be 1 [numTimes]."
                      );
        } // if
        const size_t numTimes = timesInfo.shape[0];

        py::array_t<double> resultArray(numTimes);
        py::buffer_info resultInfo = resultArray.request();
        double* result = static_cast<double*>(resultInfo.ptr);

        py::array_t<int> errorArray(numTimes);
        py::buffer_info errorInfo = errorArray.request();
        int* error = static_cast<int*>(errorInfo.ptr);

        for (size_t iTime = 0; iTime < numTimes; ++iTime) {
            error[iTime] = th.query(&result[iTime], times[iTime]);
        } // for

        return std::make_tuple(resultArray, errorArray);
    })
    ;
}
