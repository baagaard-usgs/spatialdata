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

#include "TimeHistoryIO.hh" // Implementation of class methods

// Include ios here to avoid some Python/gcc issues
#include <ios>

#include "spatialdata/utils/LineParser.hh" // USES LineParser
#include "spatialdata/units/Parser.hh" // USES Parser

#include <fstream> // USES std::ifstream, std::ofstream

#include <iomanip> // USES setw(), setiosflags(), resetiosflags()
#include <cassert> // USES assert()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream

// ----------------------------------------------------------------------
const char* spatialdata::spatialdb::TimeHistoryIO::header = "#TIME HISTORY ascii";

// ----------------------------------------------------------------------
// Read time history file.
void
spatialdata::spatialdb::TimeHistoryIO::read(std::vector<double>* time,
                                            std::vector<double>* amplitude,
                                            const char* filename) {
    assert(time);
    assert(amplitude);

    try {
        std::ifstream filein(filename);
        if (!filein.is_open() || !filein.good()) {
            std::ostringstream msg;
            msg << "Could not open time history file '" << filename << "' for reading.\n";
            throw std::runtime_error(msg.str());
        } // if

        utils::LineParser parser(filein, "//");
        parser.eatwhitespace(true);

        const int maxIgnore = 256;
        std::string token;
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
            msg << "Magic header '" << buffer.str() << "' does not match expected header '"
                << header << "' in time history file '" << filename << "'.\n";
            throw std::runtime_error(msg.str());
        } // if

        buffer.str(parser.next());
        buffer.clear();
        buffer >> token;
        if (0 != strcasecmp(token.c_str(), "TimeHistory")) {
            std::ostringstream msg;
            msg << "Could not parse '" << token << "' into 'TimeHistory'.\n";
            throw std::runtime_error(msg.str());
        } // else

        std::string timeUnits = "second";
        int numPoints = 0;
        buffer.str(parser.next());
        buffer.clear();
        buffer >> token;
        while (buffer.good() && token != "}") {
            if (0 == strcasecmp(token.c_str(), "num-points")) {
                buffer.ignore(maxIgnore, '=');
                buffer >> numPoints;
            } else if (0 == strcasecmp(token.c_str(), "time-units")) {
                buffer.ignore(maxIgnore, '=');
                buffer >> timeUnits;
            } else {
                std::ostringstream msg;
                msg << "Could not parse '" << token << "' into a TimeHistory setting.";
                throw std::domain_error(msg.str());
            } // else

            buffer.str(parser.next());
            buffer.clear();
            buffer >> token;
        } // while
        if (( token != "}") || !filein.good()) {
            throw std::runtime_error("I/O error while parsing TimeHistory settings.");
        }

        bool ok = true;
        std::ostringstream msg;
        if (0 == numPoints) {
            ok = false;
            msg << "TimeHistory settings must include 'num-points'.\n";
        } // if
        if (numPoints <= 0) {
            ok = false;
            msg << "TimeHistory must contain at least one point.\n";
        } // if
        if (!ok) {
            throw std::runtime_error(msg.str());
        }

        units::Parser uparser;
        const double scale = uparser.parse(timeUnits.c_str());

        time->resize(numPoints);
        amplitude->resize(numPoints);

        for (size_t i = 0; i < numPoints; ++i) {
            buffer.str(parser.next());
            buffer.clear();
            buffer >> (*time)[i];
            buffer >> (*amplitude)[i];
            (*time)[i] *= scale;
        } // for
        // Verify that the time stamps are ordered in time.
        for (size_t i = 1; i < numPoints; ++i) {
            if ((*time)[i-1] >= (*time)[i]) {
                throw std::runtime_error("Time history must be ordered in time.");
            } // if
        } // for

        if (!filein.good()) {
            throw std::runtime_error("Unknown error while reading.");
        } // if
    } catch (const std::exception& err) {
        std::ostringstream msg;
        msg << "Error occurred while reading time history file '"
            << filename << "'.\n"
            << err.what();
        throw std::runtime_error(msg.str());
    } catch (...) {
        std::ostringstream msg;
        msg << "Unknown error occurred while reading time history file '"
            << filename << "'.\n";
        throw std::runtime_error(msg.str());
    } // try/catch
} // read


// ----------------------------------------------------------------------
// Read time history file.
void
spatialdata::spatialdb::TimeHistoryIO::write(const std::vector<double>& time,
                                             const std::vector<double>& amplitude,
                                             const char* timeUnits,
                                             const char* filename) {
    try {
        if (time.size() != amplitude.size()) {
            std::ostringstream msg;
            msg << "Number of time stamps (" << time.size() << ") does not match the "
                << "number of amplitude points (" << amplitude.size() << ").";
            throw std::invalid_argument(msg.str());
        } // if

        std::ofstream fileout(filename);
        if (!fileout.is_open() || !fileout.good()) {
            std::ostringstream msg;
            msg << "Could not open time history file " << filename << "for writing.\n";
            throw std::runtime_error(msg.str());
        } // if

        const size_t numPoints = amplitude.size();

        fileout << header << "\n"
                << "TimeHistory {\n"
                << "  num-points = " << std::setw(6) << numPoints << "\n"
                << "  time-units = " << timeUnits << "\n"
                << "}\n";
        if (!fileout.good()) {
            throw std::runtime_error("I/O error while writing TimeHistory settings.");
        } // if

        fileout << std::resetiosflags(std::ios::fixed)
                << std::setiosflags(std::ios::scientific)
                << std::setprecision(6);
        for (size_t i = 0; i < numPoints; ++i) {
            fileout << std::setw(14) << time[i]
                    << std::setw(14) << amplitude[i]
                    << "\n";
        } // for
        if (!fileout.good()) {
            throw std::runtime_error("I/O error while writing TimeHistory data.");
        } // if
    } catch (const std::exception& err) {
        std::ostringstream msg;
        msg << "Error occurred while writing time history file '"
            << filename << "'.\n"
            << err.what();
        throw std::runtime_error(msg.str());
    } catch (...) {
        std::ostringstream msg;
        msg << "Unknown error occurred while writing time history file '"
            << filename << "'.";
        throw std::runtime_error(msg.str());
    } // try/catch
} // write


// End of file
