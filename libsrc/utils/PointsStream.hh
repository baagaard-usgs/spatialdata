// -*- C++ -*-
//
// ======================================================================
//
//                           Brad T. Aagaard
//                        U.S. Geological Survey
//
// {LicenseText}
//
// ======================================================================
//

/** @file libsrc/utils/PointsStream.hh
 *
 * @brief C++ PointsStream object.
 *
 * C++ object for reading/writing points to/from stdin/stdout.
 */

#if !defined(spatialdata_utils_pointsstream_hh)
#define spatialdata_utils_pointsstream_hh

namespace spatialdata {
  namespace utils {
    class PointsStream;
    class TestPointsStream; // forward declaration
  } // utils
} // spatialdata

#include <string> // HASA std::string
#include <iosfwd> // USES std::istream, std::ostream

/// C++ object for reading/writing points to/from stdin/stdout.
class spatialdata::utils::PointsStream
{ // class PointsStream
  friend class TestPointsStream;

 public :
  // PUBLIC METHODS /////////////////////////////////////////////////////

  /// Default constructor
  PointsStream(void);

  /// Default destructor
  ~PointsStream(void);

  /** Set comment flag.
   *
   * @param flag String identifying comments.
   */
  void commentFlag(const char* flag);

  /** Get comment flag.
   *
   * @returns String identifying comments.
   */
  const char* commentFlag(void) const;

  /** Set field width.
   *
   * @returns Width of field.
   */
  void fieldWidth(const int width);

  /** Get field width.
   *
   * @returns Width of field.
   */
  int fieldWidth(void) const;

  /** Set precision.
   *
   * @returns Precision.
   */
  void precision(const int value);

  /** Get precision.
   *
   * @returns Precision.
   */
  int precision(void) const;

  /** Set input stream.
   *
   * @param pIn Pointer to input stream
   */
  void input(std::istream* pIn);

  /** Set output stream.
   *
   * @param pOut Pointer to output stream
   */
  void output(std::ostream* pOut);

  /** Read points from stdin.
   *
   * @param ppPoints Pointer to array of points
   * @param pNumPts Pointer to number of points
   * @param pNumDims Pointer to number of dimensions
   */
  void read(double** ppPoints,
	    int* pNumPts,
	    int* pNumDims) const;

  /** Write points to stdout.
   *
   * @param pPoints Array of points
   * @param numPts Number of points
   * @param numDims Number of dimensions
   */
  void write(const double* pPoints,
	     const int numPts,
	     const int numDims) const;

private :
 // PRIVATE METHODS ////////////////////////////////////////////////////

  PointsStream(const PointsStream& p); ///< Not implemented
  const PointsStream& operator=(const PointsStream& p); ///< Not implemented

private :
 // PRIVATE MEMBERS ////////////////////////////////////////////////////

  std::string _commentFlag; ///< String identifying comments in input
  int _fieldWidth; ///< Width of field in output
  int _precision; ///< Precision in floating point output

  std::istream* _pIn; ///< Pointer to input stream
  std::ostream* _pOut; ///< Pointer to output stream

}; // class PointsStream

#include "PointsStream.icc" // inline methods

#endif // spatialdata_geocoodrs_csgeo_hh

// version
// $Id$

// End of file 
