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

/** @file pkgs/geocoords/lib/CSGeo.h
 *
 * @brief C++ CSGeo object
 *
 * C++ object for managing parameters defining geographic coordinate systems.
 */

#if !defined(spatialdata_geocoords_csgeo_h)
#define spatialdata_geocoords_csgeo_h

namespace spatialdata {
  namespace geocoords {
    class CSGeo;
    class Geoid;
    class TestCSGeo; // forward declaration
  } // geocoords
} // spatialdata

#include "proj4fwd.h" // Proj4 forward declaration
#include <string> // USES std::string

/// C++ object for managing parameters defining geographic coordinate systems
class spatialdata::geocoords::CSGeo : public CoordSys
{ // class CSGeo
  friend class TestCSGeo;

 public :
  // PUBLIC METHODS /////////////////////////////////////////////////////

  /// Default constructor
  CSGeo(void);

  /// Default destructor
  virtual ~CSGeo(void);

  /** Clone coordinate system.
   *
   * @returns Pointer to copy
   */
  virtual CoordSys* clone(void) const;

  /// Initialize the coordinate system.
  virtual void initialize(void);

  /** Set reference ellipsoid.
   *
   * @param name Name of reference ellipsoid
   */
  void ellipsoid(const std::string& name);

  /** Get reference ellipsoid.
   *
   * @returns Name of reference ellipsoid
   */
  const char* ellipsoid(void) const;

  /** Set horizontal datum.
   *
   * @param name Name of horizontal datum
   */
  void datumHoriz(const std::string& name);

  /** Get horizontal datum.
   *
   * @returns Name of datum
   */
  const char* datumHoriz(void) const;

  /** Set vertical datum.
   *
   * @param name Name of vertical datum
   */
  void datumVert(const std::string& name);

  /** Get vertical datum.
   *
   * @returns Name of datum
   */
  const char* datumVert(void) const;

  /** Set geocentric flag.
   *
   * @param geocentric True if geocentric, false if lon/lat
   */
  virtual void isGeocentric(bool geocentric);

  /** Get geocentric flag.
   *
   * @returns True if geocentric, false if lon/lat
   */
  bool isGeocentric(void) const;

  /** Set factor to convert Cartesian coordinates to meters.
   *
   * @param factor Factor to convert Cartesian coordinates to meters.
   */
  void toMeters(const double factor);

  /** Get factor to convert Cartesian coordinates to meters.
   *
   * @returns Factor to convert Cartesian coordinates to meters.
   */
  double toMeters(void) const;

  /** Get proj form vertical datum.
   *
   * @returns Name of datum
   */
  virtual const char* projDatumVert(void) const;

  /** Get PROJ coordinate system.
   *
   * @returns Coordinate system
   */
  virtual const projPJ projCoordSys(void) const;

  /** Convert coordinates to PROJ4 useable form.
   *
   * @param ppCoords Pointer to array of coordinates [#locs*3]
   * @param numLocs Number of locations
   * @param is2D True if coordinates are 2D, false if 3D
   */
  virtual void toProjForm(double** ppCoords,
			  const int numLocs,
			  bool is2D) const;
  
  /** Convert coordinates from PROJ4 form to form associated w/coordsys.
   *
   * @param ppCoords Pointer to array of coordinates [#locs*3]
   * @param numLocs Number of locations
   * @param is2D True if coordinates are 2D, false if 3D
   */
  virtual void fromProjForm(double** ppCoords,
			    const int numLocs,
			    bool is2D) const;
  /** Get geoid.
   *
   * @returns Geoid
   */
  static Geoid& geoid(void);
  
  /** Pickle coordinate system to ascii stream.
   *
   * @param s Output stream
   */
  virtual void pickle(std::ostream& s) const;

  /** Unpickle coordinate system from ascii stream.
   *
   * @param s Input stream
   */
  virtual void unpickle(std::istream& s);

protected :
  // PROTECTED METHODS //////////////////////////////////////////////////

  /** Copy constructor
   *
   * @param cs Coordinate system to copy
   */
  CSGeo(const CSGeo& cs);

protected :
  // PROTECTED MEMBERS /////////////////////////////////////////////////

  /** Get the PROJ4 string associated with the coordinate system.
   *
   * @returns string
   */
  virtual std::string _projCSString(void) const;
  
private :
 // PRIVATE MEMBERS ////////////////////////////////////////////////////

  double _toMeters; ///< Factor to convert Cartesian coordinates to meters
  std::string _ellipsoid; ///< Name of reference ellipsoid
  std::string _datumHoriz; ///< Name of horizontal geographic datum
  std::string _datumVert; ///< Name of vertical datum
  
  projPJ _pCS; ///< Pointer to coordinate system
  
  bool _isGeocentric; ///< True if geocentric, false if lat/lon
  
  static Geoid _geoid; ///< Geoid for mean sea level <-> ellipsoid ht

}; // class CSGeo

#include "CSGeo.icc" // inline methods

#endif // spatialdata_geocoodrs_csgeo_h

// version
// $Id$

// End of file 
