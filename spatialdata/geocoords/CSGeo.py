# =================================================================================================
# This code is part of SpatialData, developed through the Computational Infrastructure
# for Geodynamics (https://github.com/geodynamics/spatialdata).
#
# Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
# All rights reserved.
#
# See https://mit-license.org/ and LICENSE.md and for license information. 
# =================================================================================================

from .CoordSys import CoordSys
from ._geocoords import CSGeo as CxxCSGeo


class CSGeoMeta(type(CoordSys), type(CxxCSGeo)):
    pass

class CSGeo(CoordSys, CxxCSGeo, metaclass=CSGeoMeta):
    """
    Python manager for geographic coordinate systems.

    Implements `CoordSys`.
    """
    DOC_CONFIG = {
        "cfg": """
            coordsys = spatialdata.geocoords.CSGeo

            [coordsys]
            # WGS84 (latitude, longitude) coordinate system
            crs_string = EPSG:4326
            space_dim = 2
            """,
    }

    import pythia.pyre.inventory

    crsString = pythia.pyre.inventory.str("crs_string", default="EPSG:4326")
    crsString.meta['tip'] = "String specifying coordinate system (PROJ parameters, EPSG, or WKT). Default is WGS84."

    spaceDim = pythia.pyre.inventory.int("space_dim", default=3)
    spaceDim.meta['tip'] = "Number of dimensions for coordinate system."

    # PUBLIC METHODS /////////////////////////////////////////////////////

    def __init__(self, name="csgeo"):
        """
        Constructor.
        """
        CoordSys.__init__(self, name)
        CxxCSGeo.__init__(self)

    # PRIVATE METHODS ////////////////////////////////////////////////////

    def _configure(self):
        """
        Setup members using inventory.
        """
        CoordSys._configure(self)
        CxxCSGeo.setString(self, self.crsString)
        CxxCSGeo.setSpaceDim(self, self.spaceDim)


# FACTORIES ////////////////////////////////////////////////////////////

def coordsys():
    """
    Factory associated with CoordSys.
    """
    return CSGeo()


# End of file
