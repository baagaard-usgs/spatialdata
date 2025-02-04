// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/geocoords/Converter.hh"

#include "spatialdata/geocoords/CoordSys.hh" // USES CoordSys

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"

namespace py = pybind11;

namespace spatialdata {
    namespace geocoords {
        class WrapConverter;
    }
}

class spatialdata::geocoords::WrapConverter : public spatialdata::geocoords::Converter {
public:

    using spatialdata::geocoords::Converter::Converter;

    inline
    void convert(py::array_t<double, py::array::c_style | py::array::forcecast> coordinatesArray,
                 const CoordSys* csDest,
                 const CoordSys* csSrc) {
        py::buffer_info coordinatesInfo = coordinatesArray.request();
        double* coordinates = static_cast<double*>(coordinatesInfo.ptr);
        if (2 != coordinatesInfo.ndim) {
            throw std::range_error(
                      "Error in 'coordinates' argument to Converter::convert(). Dimensions must be 2 [numLocs, spaceDim]."
                      );
        } // if
        const size_t numLocs = coordinatesInfo.shape[0];
        const size_t spaceDim = coordinatesInfo.shape[1];
        if ((spaceDim < 2) || (spaceDim > 3)) {
            throw std::range_error(
                      "Error in 'coordinates' argument to Converter::convert(). Second dimension (spatial dimension) must be 2 or 3."
                      );
        } // if
        spatialdata::geocoords::Converter::convert(coordinates, numLocs, spaceDim, csDest, csSrc);
    }

};

void
init_Converter(py::module_ &m) {
    py::class_<spatialdata::geocoords::WrapConverter, std::shared_ptr<spatialdata::geocoords::WrapConverter> >(m, "Converter")
    .def(py::init<>())
    .def("convert", &spatialdata::geocoords::WrapConverter::convert)
    ;
}
