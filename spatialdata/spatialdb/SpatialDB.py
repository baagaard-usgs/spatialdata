# =================================================================================================
# This code is part of SpatialData, developed through the Computational Infrastructure
# for Geodynamics (https://github.com/geodynamics/spatialdata).
#
# Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
# All rights reserved.
#
# See https://mit-license.org/ and LICENSE.md and for license information. 
# =================================================================================================


from pythia.pyre.components.Component import Component
from ._spatialdb import SpatialDB as CxxSpatialDB


def validateDescription(value):
    """
    Validate description for spatial database.
    """
    if 0 == len(value):
        raise ValueError("Description for spatial database not specified.")
    return value


class SpatialDBMeta(type(Component), type(CxxSpatialDB)):
    pass

class SpatialDB(Component, CxxSpatialDB, metaclass=SpatialDBMeta):
    """
    Python abstract base class for spatial database.
    """

    import pythia.pyre.inventory

    description = pythia.pyre.inventory.str("description", default="", validator=validateDescription)
    description.meta['tip'] = "Description for database."

    # PUBLIC METHODS /////////////////////////////////////////////////////

    def __init__(self, name="spatialdb"):
        """
        Constructor.
        """
        Component.__init__(self, name, facility="spatial_database")
        CxxSpatialDB.__init__(self, "SpatialDB :UNKNOWN:")

    # PRIVATE METHODS ////////////////////////////////////////////////////

    def _configure(self):
        """
        Set attributes based on inventory.
        """
        Component._configure(self)
        CxxSpatialDB.setDescription(self, self.description)


# End of file
