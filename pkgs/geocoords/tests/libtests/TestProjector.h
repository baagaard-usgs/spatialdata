// -*- C++ -*-
//
// ----------------------------------------------------------------------
//
//                           Brad T. Aagaard
//                        U.S. Geological Survey
//
// {LicenseText}
//
// ----------------------------------------------------------------------
//

/** @file tests/libtests/TestProjector.h
 *
 * @brief C++ TestProjector object
 *
 * C++ unit testing for TestProjector.
 */

#if !defined(spatialdata_testprojector_h)
#define spatialdata_testprojector_h

#include <cppunit/extensions/HelperMacros.h>

/// Namespace for spatialdata package
namespace spatialdata {
  namespace geocoords {
    class TestProjector;
  } // geocoords
} // spatialdata

/// C++ unit testing for Projector
class spatialdata::geocoords::TestProjector : public CppUnit::TestFixture
{ // class TestProjector

  // CPPUNIT TEST SUITE /////////////////////////////////////////////////
  CPPUNIT_TEST_SUITE( TestProjector );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testProjection );
  CPPUNIT_TEST( testUnits );
  CPPUNIT_TEST( testInitialize );
  CPPUNIT_TEST( testProject );
  CPPUNIT_TEST( testInvproject );
  CPPUNIT_TEST_SUITE_END();

  // PUBLIC METHODS /////////////////////////////////////////////////////
public :

  /// Test constructor
  void testConstructor(void);

  /// Test projection()
  void testProjection(void);

  /// Test units()
  void testUnits(void);

  /// Test initialize()
  void testInitialize(void);

  /// Test project()
  void testProject(void);

  /// Test invproject()
  void testInvproject(void);

  // PRIVATE METHODS ////////////////////////////////////////////////////
private :

  static const double _XY[]; ///< Array of projected locations
  static const double _LONLAT[]; ///< Array of geographic locations
  static const int _NUMLOCS; ///< Number of locations
  static const char* _PROJECTION; ///< Name of projection
  static const char* _UNITS; ///< Units in projection

}; // class TestProjector

#endif // spatialdata_geocoords_testprojector

// version
// $Id: TestProjector.h,v 1.1 2005/05/25 17:29:42 baagaard Exp $

// End of file 
