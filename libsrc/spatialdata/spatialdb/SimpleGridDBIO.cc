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

#include "spatialdata/spatialdb/SimpleGridDBIO.hh" // implementation of class methods

#include "spatialdata/spatialdb/SimpleGridDBData.hh" // USES SimpleGridDBData

#include "spatialdata/geocoords/CSCart.hh" // USES CSCart
#include "spatialdata/geocoords/CSPicklerAscii.hh" // USES CSPicklerAscii

#include "spatialdata/utils/LineParser.hh" // USES LineParser

#include <fstream> // USES std::ofstream, std::ifstream
#include <iomanip> // USES setw(), setiosflags(), resetiosflags()
#include <cmath> // USES pow()
#include <algorithm> // USES std::sort()
#include <vector> // USES std::vector

#include <stdexcept> // USES std::runtime_error
#include <iostream> // USES std::cout
#include <sstream> // USES std::ostringsgream
#include <strings.h> // USES strcasecmp()
#include <cstring> // USES strlen()
#include <cassert> // USES assert()

// ----------------------------------------------------------------------
const char* spatialdata::spatialdb::SimpleGridDBIO::header = "#SPATIAL_GRID.ascii";

// ----------------------------------------------------------------------
// Read ascii database file.
void
spatialdata::spatialdb::SimpleGridDBIO::read(SimpleGridDBData* data,
                                             const char* filename) {
    assert(data);

    try {
        std::ifstream filein(filename);
        if (!filein.is_open() || !filein.good()) {
            std::ostringstream msg;
            msg << "Could not open spatial database file '" << filename
                << "' for reading.\n";
            throw std::runtime_error(msg.str());
        } // if

        _readHeader(data, filein);
        _readData(data, filein);

        if (!filein.good()) {
            throw std::runtime_error("Unknown error while reading.");
        } // if
        filein.close();
    } catch (const std::exception& err) {
        std::ostringstream msg;
        msg << "Error occurred while reading spatial database file '" << filename << "'.\n"
            << err.what();
        throw std::runtime_error(msg.str());
    } catch (...) {
        std::ostringstream msg;
        msg << "Unknown error occurred while reading spatial database file '" << filename << "'.\n";
        throw std::runtime_error(msg.str());
    } // try/catch
} // read


// ----------------------------------------------------------------------
// Write ascii database file.
void
spatialdata::spatialdb::SimpleGridDBIO::write(const SimpleGridDBData& data,
                                              const char* filename) {
    try {
        std::ofstream fileout(filename);
        if (!fileout.is_open() || !fileout.good()) {
            std::ostringstream msg;
            msg << "Could not open spatial database file '" << filename
                << "' for writing.\n";
            throw std::runtime_error(msg.str());
        } // if

        _writeHeader(data, fileout);
        _writeData(data, fileout);

        if (!fileout.good()) {
            throw std::runtime_error("Unknown error while writing.");
        }

        fileout.close();
    } catch (const std::exception& err) {
        std::ostringstream msg;
        msg << "Error occurred while writing spatial database file '" << filename << "'.\n"
            << err.what();
        throw std::runtime_error(msg.str());
    } catch (...) {
        std::ostringstream msg;
        msg << "Unknown error occurred while writing spatial database file '" << filename << "'.\n";
        throw std::runtime_error(msg.str());
    } // try/catch
} // write


// ----------------------------------------------------------------------
// Read data file header.
void
spatialdata::spatialdb::SimpleGridDBIO::_readHeader(SimpleGridDBData* data,
                                                    std::istream& filein) {
    assert(data);

    spatialdata::utils::LineParser parser(filein, "//");
    parser.eatwhitespace(true);

    std::istringstream buffer;

    buffer.str(parser.next());
    buffer.clear();

    const int headerLen = strlen(header);
    std::string hbuffer;
    hbuffer.resize(headerLen+1);
    buffer.read((char*) hbuffer.c_str(), sizeof(char)*headerLen);
    hbuffer[headerLen] = '\0';
    if (0 != strcasecmp(header, hbuffer.c_str())) {
        std::ostringstream msg;
        msg
            << "Magic header '" << buffer.str() << "' does not match expected header '"
            << header << " in SimpleGrid spatial database file.";
        throw std::runtime_error(msg.str());
    } // if

    std::string token;
    const int maxIgnore = 256;

    buffer.str(parser.next());
    buffer.clear();
    buffer >> token;
    if (0 != strcasecmp(token.c_str(), "SimpleGridDB")) {
        std::ostringstream msg;
        msg << "Could not parse '" << token << "' into 'SimpleGridDB'.\n";
        throw std::runtime_error(msg.str());
    } // else

    size_t numX = 0;
    size_t numY = 0;
    size_t numZ = 0;
    size_t spaceDim = 0; // deprecated
    size_t numValues = 0;
    std::vector<std::string> names;
    std::vector<std::string> units;

    buffer.str(parser.next());
    buffer.clear();
    buffer >> token;
    while (buffer.good() && token != "}") {
        if (0 == strcasecmp(token.c_str(), "num-x")) {
            buffer.ignore(maxIgnore, '=');
            buffer >> numX;
        } else if (0 == strcasecmp(token.c_str(), "num-y")) {
            buffer.ignore(maxIgnore, '=');
            buffer >> numY;
        } else if (0 == strcasecmp(token.c_str(), "num-z")) {
            buffer.ignore(maxIgnore, '=');
            buffer >> numZ;
        } else if (0 == strcasecmp(token.c_str(), "space-dim")) {
            buffer.ignore(maxIgnore, '=');
            buffer >> spaceDim;
        } else if (0 == strcasecmp(token.c_str(), "num-values")) {
            buffer.ignore(maxIgnore, '=');
            buffer >> numValues;
        } else if (0 == strcasecmp(token.c_str(), "value-names")) {
            if (numValues > 0) {
                names.resize(numValues);
            } else {
                throw std::runtime_error("Number of values must be specified BEFORE "
                                         "names of values in SimpleGridDB file.");
            }
            buffer.ignore(maxIgnore, '=');
            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                buffer >> names[iValue];
            } // for
        } else if (0 == strcasecmp(token.c_str(), "value-units")) {
            if (numValues > 0) {
                units.resize(numValues);
            } else {
                throw std::runtime_error("Number of values must be specified BEFORE "
                                         "units of values in SimpleGridDB file.");
            } // if/else
            buffer.ignore(maxIgnore, '=');
            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                buffer >> units[iValue];
            } // for
        } else if (0 == strcasecmp(token.c_str(), "cs-data")) {
            buffer.ignore(maxIgnore, '=');
            std::string rbuffer(buffer.str());
            filein.putback('\n');
            filein.clear();
            int i = rbuffer.length()-1;
            while (i >= 0) {
                filein.putback(rbuffer[i]);
                if ('=' == rbuffer[i--]) {
                    break;
                } // if
            } // while
            filein.clear();
            spatialdata::geocoords::CoordSys* cs = nullptr;
            spatialdata::geocoords::CSPicklerAscii::unpickle(filein, &cs);
            std::shared_ptr<spatialdata::geocoords::CoordSys> csShared(cs);
            data->setCoordSys(csShared);
        } else {
            std::ostringstream msg;
            msg << "Could not parse '" << token << "' into a SimpleGridDB setting.";
            throw std::domain_error(msg.str());
        } // else

        buffer.str(parser.next());
        buffer.clear();
        buffer >> token;
    } // while
    if (( token != "}") || !filein.good()) {
        throw std::runtime_error("I/O error while parsing SimpleGridDB settings.");
    }
    spaceDim = data->getSpaceDim();

    bool ok = true;
    std::ostringstream msg;
    if (numValues <= 0) {
        ok = false;
        msg << "SimpleGridDB settings must include 'num-values'.\n";
    } // if
    if (spaceDim <= 0) {
        ok = false;
        msg << "SimpleGridDB settings must include positive 'space-dim'.\n";
    } // if

    if (( spaceDim > 0) && ( numX <= 0) ) {
        ok = false;
        msg << "SimpleGridDB settings must include 'num-x'.\n";
    } // if
    if (( spaceDim > 1) && ( numY <= 0) ) {
        ok = false;
        msg << "SimpleGridDB settings must include 'num-y' with 2-D and 3-D data.\n";
    } // if
    if (( spaceDim > 2) && ( numZ <= 0) ) {
        ok = false;
        msg << "SimpleGridDB settings must include 'num-z' with 3-D data.\n";
    } // if
    if (0 == names.size()) {
        ok = false;
        msg << "SimpleGridDB settings must include 'value-names'.\n";
    } // if
    if (0 == units.size()) {
        ok = false;
        msg << "SimpleGridDB settings must include 'value-units'.\n";
    } // if

    if (!ok) {
        throw std::runtime_error(msg.str());
    } // if

    // Set data dimension based on dimensions of data.
    size_t dataDim = 0;
    if (numX > 1) {
        dataDim += 1;
    } // if
    if (numY > 1) {
        dataDim += 1;
    } // if
    if (numZ > 1) {
        dataDim += 1;
    } // if

    data->allocate(numX, numY, numZ, numValues, spaceDim, dataDim);
    data->setNames(names);
    data->setUnits(units);
    data->checkCompatibility();
} // _readHeader


// ----------------------------------------------------------------------
// Read data values.
void
spatialdata::spatialdb::SimpleGridDBIO::_readData(SimpleGridDBData* data,
                                                  std::istream& filein) {
    const int numX = data->getNumX();
    const int numY = data->getNumY();
    const int numZ = data->getNumZ();
    const int numValues = data->getNumValues();
    const int spaceDim = data->getSpaceDim();

    const int bufsize = 32768;
    utils::LineParser parser(filein, "//", bufsize);
    parser.eatwhitespace(true);

    std::istringstream buffer;

    const bool _verbose = false;

    size_t numLocs = 1;
    if (numX >= 1) {
        if (_verbose) {
            std::cout << "Using " << numX << " coordinates in x-direction.\n";
        } // if
        numLocs *= numX;

        std::vector<double>& coordinates = data->getX();
        assert(coordinates.size() == numX);
        buffer.str(parser.next());
        buffer.clear();
        for (int i = 0; i < numX; ++i) {
            if (!buffer.good()) {
                std::ostringstream msg;
                msg << "Error reading x-coordinates from buffer '" << buffer.str() << "'.";
                throw std::runtime_error(msg.str());
            } // if
            buffer >> coordinates[i];
        } // for
        std::sort(coordinates.begin(), coordinates.end());
    } // if

    if (numY >= 1) {
        if (_verbose) {
            std::cout << "Using " << numY << " coordinates in y-direction.\n";
        } // if
        numLocs *= numY;

        std::vector<double>& coordinates = data->getY();
        assert(coordinates.size() == numY);
        buffer.str(parser.next());
        buffer.clear();
        for (int i = 0; i < numY; ++i) {
            if (!buffer.good()) {
                std::ostringstream msg;
                msg << "Error reading y-coordinates from buffer '" << buffer.str() << "'.";
                throw std::runtime_error(msg.str());
            } // if
            buffer >> coordinates[i];
        } // for
        std::sort(coordinates.begin(), coordinates.end());
    } // if

    if (numZ >= 1) {
        if (_verbose) {
            std::cout << "Using " << numZ << " coordinates in z-direction.\n";
        } // if
        numLocs *= numZ;

        std::vector<double>& coordinates = data->getZ();
        assert(coordinates.size() == numZ);
        buffer.str(parser.next());
        buffer.clear();
        for (int i = 0; i < numZ; ++i) {
            if (!buffer.good()) {
                std::ostringstream msg;
                msg << "Error reading z-coordinates from buffer '" << buffer.str() << "'.";
                throw std::runtime_error(msg.str());
            } // if
            buffer >> coordinates[i];
        } // for
        std::sort(coordinates.begin(), coordinates.end());
    } // if

    assert(numLocs > 0);
    assert(numValues > 0);
    assert(spaceDim > 0);
    double coordinates[3];
    int count = 0;
    for (int iLoc = 0; iLoc < numLocs; ++iLoc, ++count) {
        buffer.str(parser.next());
        buffer.clear();
        for (int iDim = 0; iDim < spaceDim; ++iDim) {
            if (!buffer.good()) {
                std::ostringstream msg;
                msg << "Read data for " << count << " out of " << numLocs << " points.\n"
                    << "Error reading coordinates from buffer '" << buffer.str() << "'.";
                throw std::runtime_error(msg.str());
            } // if
            buffer >> coordinates[iDim];
        } // for

        const int i_data = data->getDataIndex(coordinates, spaceDim);
        double* const dataLoc = data->getData(i_data);
        for (size_t iValue = 0; iValue < numValues; ++iValue) {
            if (!buffer.good()) {
                std::ostringstream msg;
                msg << "Read data for " << count << " out of " << numLocs << " points.\n"
                    << "Error reading data from buffer '" << buffer.str() << "'.";
                throw std::runtime_error(msg.str());
            } // if
            buffer >> dataLoc[iValue];
        } // for
    } // for
    if (_verbose) {
        std::cout << "Read " << count << " lines of data.\n";
    } // if
    if (!filein.good()) {
        std::ostringstream msg;
        msg << "I/O error while reading SimpleGridDB data. ";
        if (count < numLocs) {
            msg << "Read " << count << " out of " << numLocs << " points before encountering the I/O error.";
        } else {
            msg << "Error occurred while reading data for final point.\n"
                << "Make sure that the last line with data ends with an end-of-line character.";
        } // if/else
        throw std::runtime_error(msg.str());
    } // if

    data->toSI();
} // _readData


// ----------------------------------------------------------------------
// Write the data file header.
void
spatialdata::spatialdb::SimpleGridDBIO::_writeHeader(const SimpleGridDBData& data,
                                                     std::ostream& fileout) {
    const int version = 1;
    const int numValues = data.getNumValues();

    fileout
        << header << " " << version << "\n"
        << "SimpleGridDB {\n"
        << "  num-x = " << data.getNumX() << "\n"
        << "  num-y = " << data.getNumY() << "\n"
        << "  num-z = " << data.getNumZ() << "\n"
        << "  num-values = " << numValues << "\n"
        << "  value-names =";

    const std::vector<std::string>& names = data.getNames();
    for (int iValue = 0; iValue < numValues; ++iValue) {
        fileout << "  " << names[iValue];
    }
    fileout << "\n";

    fileout << "  value-units =";
    for (int iValue = 0; iValue < numValues; ++iValue) {
        fileout << "  " << data.getUnits(iValue);
    }
    fileout << "\n";
    fileout << "  cs-data = ";
    spatialdata::geocoords::CSPicklerAscii::pickle(fileout, data.getCoordSys());
    fileout << "}\n";

    if (!fileout.good()) {
        throw std::runtime_error("I/O error while writing SimpleGridDB settings.");
    }
} // _writeHeader


// ----------------------------------------------------------------------
// Write data values.
void
spatialdata::spatialdb::SimpleGridDBIO::_writeData(const SimpleGridDBData& data,
                                                   std::ostream& fileout) {
    fileout
        << std::resetiosflags(std::ios::fixed)
        << std::setiosflags(std::ios::scientific)
        << std::setprecision(6);

    const int numX = data.getNumX();
    const int numY = data.getNumY();
    const int numZ = data.getNumZ();
    const int numValues = data.getNumValues();

    fileout << "// x-coordinates\n";
    const std::vector<double>& x = data.getX();
    for (int i = 0; i < numX; ++i) {
        fileout << std::setw(14) << x[i];
    } // for
    fileout << "\n";

    fileout << "// y-coordinates\n";
    const std::vector<double>& y = data.getY();
    for (int i = 0; i < numY; ++i) {
        fileout << std::setw(14) << y[i];
    } // for
    fileout << "\n";

    fileout << "// z-coordinates\n";
    const std::vector<double>& z = data.getZ();
    for (int i = 0; i < numZ; ++i) {
        fileout << std::setw(14) << z[i];
    } // for
    fileout << "\n";

    fileout << "// data\n";
    if (numZ > 0) {
        for (int iZ = 0; iZ < numZ; ++iZ) {
            for (int iY = 0; iY < numY; ++iY) {
                for (int iX = 0; iX < numX; ++iX) {
                    const size_t indexData = data.getDataIndex(iX, numX, iY, numY, iZ, numZ);
                    const double* const dataLoc = data.getData(indexData);
                    fileout
                        << std::setw(14) << x[iX]
                        << std::setw(14) << y[iY]
                        << std::setw(14) << z[iZ];
                    for (int iValue = 0; iValue < numValues; ++iValue) {
                        fileout << std::setw(14) << dataLoc[iValue];
                    } // for
                    fileout << "\n";
                } // for
            } // for
        } // for
    } else if (numY > 0) {
        const int iZ = 0;
        for (int iY = 0; iY < numY; ++iY) {
            for (int iX = 0; iX < numX; ++iX) {
                const size_t indexData = data.getDataIndex(iX, numX, iY, numY, iZ, numZ);
                const double* const dataLoc = data.getData(indexData);
                fileout
                    << std::setw(14) << x[iX]
                    << std::setw(14) << y[iY];
                for (int iValue = 0; iValue < numValues; ++iValue) {
                    fileout << std::setw(14) << dataLoc[iValue];
                } // for
                fileout << "\n";
            } // for
        } // for
    } else if (numX > 0) {
        const int iY = 0;
        const int iZ = 0;
        for (int iX = 0; iX < numX; ++iX) {
            const size_t indexData = data.getDataIndex(iX, numX, iY, numY, iZ, numZ);
            const double* const dataLoc = data.getData(indexData);
            fileout
                << std::setw(14) << x[iX];
            for (int iValue = 0; iValue < numValues; ++iValue) {
                fileout << std::setw(14) << dataLoc[iValue];
            } // for
            fileout << "\n";
        } // for
    } // if/else
} // _writeData


// End of file
