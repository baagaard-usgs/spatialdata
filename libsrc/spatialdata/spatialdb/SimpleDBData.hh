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

#include "spatialdata/geocoords/geocoordsfwd.hh" // HOLDSA CoordSys

#include <memory> // USES std::memory
#include <string> // USES std::string

class spatialdata::spatialdb::SimpleDBData {
    friend class TestSimpleDBData; // unit testing

public:

    // PUBLIC METHODS /////////////////////////////////////////////////////

    /// Default constructor
    SimpleDBData(void);

    /// Default destructor
    ~SimpleDBData(void);

    /** Allocate data structues.
     *
     * @param numLocs Number of locations.
     * @param numValues Number of values.
     * @param spaceDim Spatial dimension of domain.
     * @param dataDim Spatial dimension of data distribution.
     */
    void allocate(const size_t numLocs,
                  const size_t numValues,
                  const size_t spaceDim,
                  const size_t dataDim);

    /// Deallocate data.
    void deallocate(void);

    /** Get number of locations for data.
     *
     * @returns Number of locations.
     */
    size_t getNumLocs(void) const;

    /** Get number of values for data.
     *
     * @returns Number of values.
     */
    size_t getNumValues(void) const;

    /** Get spatial dimension of data distribution.
     *
     * @returns Spatial dimension.
     */
    size_t getDataDim(void) const;

    /** Get spatial dimension of locations.
     *
     * @returns Spatial dimension.
     */
    size_t getSpaceDim(void) const;

    /** Set names of values.
     *
     * @pre Must call allocate() before setNames().
     *
     * @param names Array of names of values [numValues].
     */
    void setNames(const std::vector<std::string>& names);

    /** Get names of values.
     *
     * @returns Names of values.
     */
    const std::vector<std::string>& getNames(void) const;

    /** Set units of values.
     *
     * @pre Must call allocate() before setUnits().
     *
     * @param units Array of units of values [numValues].
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

    /** Get coordinates of location in database.
     *
     * @param index Index of location in database
     * @returns Coordinates of location.
     */
    const double* getCoordinates(const size_t index) const;

    /** Get coordinates of location in database.
     *
     * @param index Index of location in database
     * @returns Coordinates of location.
     */
    double* getCoordinates(const size_t index);

    /** Get data values at location in database.
     *
     * @param index Index of location in database.
     * @returns Data values at location.
     */
    const double* getData(const size_t index) const;

    /** Get data values at location in database.
     *
     * @param index Index of location in database.
     * @returns Data values at location.
     */
    double* getData(const size_t index);

    /** Get name of value.
     *
     * @param index Index of value.
     * @returns Name of value.
     */
    const char* getName(const size_t index) const;

    /** Get units of value.
     *
     * @param index Index of value.
     * @returns Units of value.
     */
    const char* getUnits(const size_t index) const;

    /** Check compatibility of topology and spatial distribution.
     *
     * Currently, this compatiblity check only involves making sure
     * there are enough number of points to perform the interpolation
     * corresponding to the topology of the spatial distribution. It
     * does not check the actual topology of the distribution.
     */
    void checkCompatibility(void) const;

    /// Convert values to SI units.
    void toSI(void);

private:

    // PRIVATE METHODS ////////////////////////////////////////////////////

    std::vector<double> _data; ///< Array of data values.
    std::vector<double> _coordinates; ///< Array of coordinates of locations.
    std::vector<std::string> _names; ///< Names of data values.
    std::vector<std::string> _units; ///< Units of values.
    size_t _numLocs; ///< Number of locations.
    size_t _numValues; ///< Number of values.
    size_t _dataDim; ///< Spatial dimension of data distribution.
    std::shared_ptr<spatialdata::geocoords::CoordSys> _cs; ///< Coordinate system

}; // class SpatialDBData

#include "SimpleDBData.icc" // inline methods

// End of file
