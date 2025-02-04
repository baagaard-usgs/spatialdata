// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/units/Nondimensional.hh"

#include "pybind11/pybind11.h"
#include "pybind11/numpy.h"

namespace py = pybind11;

void
init_Nondimensional(py::module_ &m) {
    py::class_<spatialdata::units::Nondimensional, std::shared_ptr<spatialdata::units::Nondimensional> >(m, "Nondimensional")
    .def(py::init<>())
    .def("setLengthScale", &spatialdata::units::Nondimensional::setLengthScale)
    .def("getLengthScale", &spatialdata::units::Nondimensional::getLengthScale)
    .def("setPressureScale", &spatialdata::units::Nondimensional::setPressureScale)
    .def("getPressureScale", &spatialdata::units::Nondimensional::getPressureScale)
    .def("setTimeScale", &spatialdata::units::Nondimensional::setTimeScale)
    .def("getTimeScale", &spatialdata::units::Nondimensional::getTimeScale)
    .def("setDensityScale", &spatialdata::units::Nondimensional::setDensityScale)
    .def("getDensityScale", &spatialdata::units::Nondimensional::getDensityScale)
    .def("setTemperatureScale", &spatialdata::units::Nondimensional::setTemperatureScale)
    .def("getTemperatureScale", &spatialdata::units::Nondimensional::getTemperatureScale)
    .def("computeDensityScale", &spatialdata::units::Nondimensional::computeDensityScale)
    .def("computePressureScale", &spatialdata::units::Nondimensional::computePressureScale)

    // Static methods.
    .def_static("nondimensionalize", [](const double value,
                                        const double scale) {
        return spatialdata::units::Nondimensional::nondimensionalize(value, scale);
    })

    .def_static("dimensionalize", [](const double value,
                                     const double scale)  {
        return spatialdata::units::Nondimensional::dimensionalize(value, scale);
    })

    .def_static("nondimensionalize", [](py::array_t<double>  values,
                                        const double scale) {
        py::buffer_info valuesInfo = values.request();
        if (valuesInfo.ndim != 1) {
            throw std::runtime_error("Nondimensional::nondimensionalize() input array must have 1 dimension.");
        } // if
        double* valuesPtr = static_cast<double*>(valuesInfo.ptr);
        spatialdata::units::Nondimensional::nondimensionalize(valuesPtr, valuesInfo.size, scale);
    })

    .def_static("dimensionalize", [] (py::array_t<double>  values,
                                      const double scale) {
        py::buffer_info valuesInfo = values.request();
        if (valuesInfo.ndim != 1) {
            throw std::runtime_error("Nondimensional::dimensionalize() input array must have 1 dimension.");
        } // if
        double* valuesPtr = static_cast<double*>(valuesInfo.ptr);
        spatialdata::units::Nondimensional::dimensionalize(valuesPtr, valuesInfo.size, scale);
    })

    .def_static("nondimensionalize", [](py::array_t<float>  values,
                                        const float scale) {
        py::buffer_info valuesInfo = values.request();
        if (valuesInfo.ndim != 1) {
            throw std::runtime_error("Nondimensional::nondimensionalize() input array must have 1 dimension.");
        } // if
        float* valuesPtr = static_cast<float*>(valuesInfo.ptr);
        spatialdata::units::Nondimensional::nondimensionalize(valuesPtr, valuesInfo.size, scale);
    })

    .def_static("dimensionalize", [](py::array_t<float>  values,
                                     const float scale)  {
        py::buffer_info valuesInfo = values.request();
        if (valuesInfo.ndim != 1) {
            throw std::runtime_error("Nondimensional::dimensionalize() input array must have 1 dimension.");
        } // if
        float* valuesPtr = static_cast<float*>(valuesInfo.ptr);
        spatialdata::units::Nondimensional::dimensionalize(valuesPtr, valuesInfo.size, scale);
    })
    ;
}
