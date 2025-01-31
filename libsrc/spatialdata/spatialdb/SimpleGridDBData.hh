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

#include "spatialdbfwd.hh" // forward declarations

#include "spatialdata/geocoords/geocoordsfwd.hh" // USES CoordSys

#include <memory> // HASA std::memory
#include <string> // HASA std::string

class spatialdata::spatialdb::SimpleGridDBData {
    friend class TestSimpleGridDBData; // unit testing

public:

    // PUBLIC MEMBERS ///////////////////////////////////////////////////////
public:

    /// Constructor
    SimpleGridDBData(void);

    /// Destructor
    ~SimpleGridDBData(void);

    /** Allocate room for data.
     *
     * @param numX Number of locations along x axis.
     * @param numY Number of locations along y axis.
     * @param numZ Number of locations along z axis.
     * @param numValues Number of values at each location.
     * @param spaceDim Spatial dimension.
     * @param dataDim Spatial dimension of topology.
     */
    void allocate(const size_t numX,
                  const size_t numY,
                  const size_t numZ,
                  const size_t numValues,
                  const size_t spaceDim,
                  const size_t dataDim);

    /// Deallocate data.
    void deallocate(void);

    /** Set names of data values.
     *
     * @param names Names of values.
     */
    void setNames(const std::vector<std::string>& names);

    /** Get names of data values.
     *
     * @returns Names of values.
     */
    const std::vector<std::string>& getNames(void) const;

    /** Set units of data values.
     *
     * @param units Units of values.
     */
    void setUnits(const std::vector<std::string>& units);

    /** Set coordinate system for spatial database.
     *
     * @param[in] cs Coordinate system for spatial database.
     */
    void setCoordSys(std::shared_ptr<spatialdata::geocoords::CoordSys>& cs);

    /** Get coordinate system.
     *
     * @returns Units of value.
     */
    spatialdata::geocoords::CoordSys* const getCoordSys(void) const;

    /** Get number of locations along x axis;
     *
     * @returns Number of locations along x axis.
     */
    size_t getNumX(void) const;

    /** Get number of locations along y axis;
     *
     * @returns Number of locations along y axis.
     */
    size_t getNumY(void) const;

    /** Get number of locations along z axis;
     *
     * @returns Number of locations along z axis.
     */
    size_t getNumZ(void) const;

    /** Get dimension of data topology.
     *
     * @returns Dimension of data topology.
     */
    size_t getDataDim(void) const;

    /** Get spatial dimension.
     *
     * @returns Spatial dimension.
     */
    size_t getSpaceDim(void) const;

    /** Get number of values at each location.
     *
     * @returns Number of values at each location.
     */
    size_t getNumValues(void) const;

    /** Get coordinates along x axis;
     *
     * @returns Coordinates along x axis.
     */
    std::vector<double>& getX(void);

    /** Get coordinates along x axis;
     *
     * @returns Coordinates along x axis.
     */
    const std::vector<double>& getX(void) const;

    /** Get coordinates along y axis;
     *
     * @returns Coordinates along y axis.
     */
    std::vector<double>& getY(void);

    /** Get coordinates along y axis;
     *
     * @returns Coordinates along y axis.
     */
    const std::vector<double>& getY(void) const;

    /** Get coordinates along z axis;
     *
     * @returns Coordinates along z axis.
     */
    std::vector<double>& getZ(void);

    /** Get coordinates along z axis;
     *
     * @returns Coordinates along z axis.
     */
    const std::vector<double>& getZ(void) const;

    /** Get index into data array.
     *
     * @param index0 Adjusted index for coordinate 0.
     * @param size0 Adjusted size for coordinate 0.
     * @param index1 Adjusted index for dimension 1.
     * @param size1 Adjusted size for dimension 1.
     * @param index2 Adjusted index for dimension 2.
     * @param size2 Adjusted size for dimension 2.
     *
     * @returns Index into data array.
     */
    size_t getDataIndex(const size_t index0,
                        const size_t size0,
                        const size_t index1,
                        const size_t size1,
                        const size_t index2,
                        const size_t size2) const;

    /** Get index into data array.
     *
     * @param Coordinates of point.
     * @param spaceDim Number of coordinate dimensions.
     *
     * @returns Index into data array.
     */
    size_t getDataIndex(const double* const coords,
                        const size_t spaceDim) const;

    /** Get index into data array.
     *
     * @returns Data values at location index.
     */
    const double* const getData(const size_t index) const;

    /** Get index into data array.
     *
     * @returns Data values at location index.
     */
    double* const getData(const size_t index);

    /** Get units of data values.
     *
     * @param index Index of value.
     * @returns Units of value.
     */
    const char* getUnits(const size_t index) const;

    /** Get name of value.
     *
     * @param index Index of value.
     * @returns Name of value.
     */
    const char* getName(const size_t index) const;

    /** Check compatibility of spatial database parameters.
     *
     * @param[in] cs Coordinate system for spatial database.
     */
    void checkCompatibility(void) const;

    /// Convert values to SI units.
    void toSI(void);

    /** Bilinear search for coordinate.
     *
     * Returns index of target as a double.
     *
     * @param target Coordinate of target.
     * @param coordinate Array of ordered coordinate to search.
     * @param nearest If true, get nearest location if target it out of bounds.
     */
    static
    double search(const double target,
                  const std::vector<double>& coordinate,
                  const bool nearest);

    /** Adjust indices to account for optimizations for lower dimension
     *  distribution.
     *
     * @param index0 Adjusted index for coordinate 0.
     * @param size0 Adjusted size for coordinate 0.
     * @param index1 Adjusted index for dimension 1.
     * @param size1 Adjusted size for dimension 1.
     */
    void reindex2d(double* const index0,
                   size_t* const size0,
                   double* const index1,
                   size_t* const size1) const;

    /** Adjust indices to account for optimizations for lower dimension
     *  distribution.
     *
     * @param index0 Adjusted index for coordinate 0.
     * @param size0 Adjusted size for coordinate 0.
     * @param index1 Adjusted index for dimension 1.
     * @param size1 Adjusted size for dimension 1.
     * @param index2 Adjusted index for dimension 2.
     * @param size2 Adjusted size for dimension 2.
     */
    void reindex3d(double* const index0,
                   size_t* const size0,
                   double* const index1,
                   size_t* const size1,
                   double* const index2,
                   size_t* const size2) const;

    // PRIVATE MEMBERS //////////////////////////////////////////////////////
private:

    std::vector<double> _data; ///< Array of data values.
    std::vector<double> _x; ///< Array of x coordinates.
    std::vector<double> _y; ///< Array of y coordinates.
    std::vector<double> _z; ///< Array of z coordinates.

    size_t _numX; ///< Number of points along x dimension.
    size_t _numY; ///< Number of points along y dimension.
    size_t _numZ; ///< Number of points along z dimension.
    size_t _dataDim; ///< Dimension of data topology.
    size_t _numValues; ///< Number of values in database.
    std::vector<std::string> _names; ///< Names of data values.
    std::vector<std::string> _units; ///< Units of values.
    std::shared_ptr<spatialdata::geocoords::CoordSys> _cs; ///< Coordinate system

    // NOT IMPLEMENTED //////////////////////////////////////////////////////
private:

    SimpleGridDBData(const SimpleGridDBData&); ///< Not implemented
    const SimpleGridDBData& operator=(const SimpleGridDBData&); ///< Not implemented

}; // SimpleGridDBData

#include "SimpleGridDBData.icc" // inline methods

// End of file
