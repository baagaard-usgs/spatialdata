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

/** @file pkgs/geocoords/lib/CoordSys.h
 *
 * @brief C++ CoordSys object
 *
 * C++ object for managing parameters defining a coordinate system.
 */

#if !defined(spatialdata_geocoords_coordsys_h)
#define spatialdata_geocoords_coordsys_h

#include <iosfwd> // USES std::istream, std::ostream

namespace spatialdata {
  namespace geocoords {
    class CoordSys;
  } // geocoords
} // spatialdata

/// C++ object for managing parameters defining a coordinate system
class spatialdata::geocoords::CoordSys
{ // class CoordSys
public :
  // PUBLIC METHODS /////////////////////////////////////////////////////

  /// Type of coordinate system
  enum CSTypeEnum {
    CARTESIAN, ///< Cartesian coordinate system
    GEOGRAPHIC ///< Geographic coordinate system
  };

public :
  // PUBLIC METHODS /////////////////////////////////////////////////////

  /// Default constructor
  CoordSys(void);

  /// Default destructor
  virtual ~CoordSys(void);

  /** Clone coordinate system.
   *
   * @returns Copy of *this
   */
  virtual CoordSys* clone(void) const = 0;

  /// Initialize the coordinate system.
  virtual void initialize(void) = 0;

  /// Get type of coordinate system
  CSTypeEnum csType(void) const;

  /** Pickle coordinate system to ascii stream.
   *
   * @param s Output stream
   */
  virtual void pickle(std::ostream& s) const = 0;

  /** Unpickle coordinate system from ascii stream.
   *
   * @param s Input stream
   */
  virtual void unpickle(std::istream& s) = 0;

protected :
  // PROTECTED METHODS //////////////////////////////////////////////////

  /** Copy destructor.
   *
   * @param cs Coordinate system to copy
   */
  CoordSys(const CoordSys& cs);

  /** Set type of coordinate system.
   *
   * @param cs Type of coordinate system
   */
  void csType(const CSTypeEnum cs);

private :
  // PRIVATE MEMBERS ////////////////////////////////////////////////////

  CSTypeEnum _csType; ///< Type of coordinate system

}; // class CoordSys

#include "CoordSys.icc" // inline methods

#endif // spatialdata_geocoords_coordsys_h

// version
// $Id$

// End of file 
