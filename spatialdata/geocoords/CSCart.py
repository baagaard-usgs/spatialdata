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
from ._geocoords import CSCart as CxxCSCart


class CSCartMeta(type(CoordSys), type(CxxCSCart)):
    pass

class CSCart(CoordSys, CxxCSCart, metaclass=CSCartMeta):
    """
    Python manager for Cartesian coordinate systems.

    Implements `CoordSys`.
    """
    DOC_CONFIG = {
        "cfg": """
            coordsys = spatialdata.geocoodes.CSCart

            [coordsys]
            units = meter
            space_dim = 3
            """,
    }

    import pythia.pyre.inventory

    units = pythia.pyre.inventory.str("units", default="m")
    units.meta['tip'] = "Units of coordinates."

    spaceDim = pythia.pyre.inventory.int("space_dim", default=3)
    spaceDim.meta['tip'] = "Number of dimensions for coordinate system."

    # PUBLIC METHODS /////////////////////////////////////////////////////

    def __init__(self, name="cscart"):
        """
        Constructor.
        """
        CoordSys.__init__(self, name)
        CxxCSCart.__init__(self)

    # PRIVATE METHODS ////////////////////////////////////////////////////

    def _configure(self):
        """
        Setup members using inventory.
        """
        CoordSys._configure(self)

        import pythia.pyre.units
        uparser = pythia.pyre.units.parser()
        coordUnits = uparser.parse(self.inventory.units)
        CxxCSCart.setToMeters(self, coordUnits.value)
        CxxCSCart.setSpaceDim(self, self.spaceDim)


# FACTORIES ////////////////////////////////////////////////////////////

def coordsys():
    """
    Factory associated with CoordSys.
    """
    return CSCart()


# End of file
