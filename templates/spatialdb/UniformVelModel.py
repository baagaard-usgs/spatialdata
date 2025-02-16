#!/usr/bin/env python
#
# =================================================================================================
# This code is part of SpatialData, developed through the Computational Infrastructure
# for Geodynamics (https://github.com/geodynamics/spatialdata).
#
# Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
# All rights reserved.
#
# See https://mit-license.org/ and LICENSE.md and for license information. 
# =================================================================================================

from spatialdata.spatialdb.SpatialDB import SpatialDB
from ._spatialdbcontrib import UniformVelModel as CxxUniformVelModel


class UniformVelModelMeta(type(SpatialDB), type(CxxUniformVelModel)):
    """Metaclass for proper resolution of UniformVelModel parent classes."""
    pass

class UniformVelModel(SpatialDB, CxxUniformVelModel, metaclass=UniformVelModelMeta):
    """
    A simple seismic velocity (wave speed) model with uniform properties.

    Implements `SpatialDB`.
    """
    DOC_CONFIG = {
        "cfg": """
            db = contrib.spatialdb.UniformVelModel

            [db]
            description = Material properties
            """,
    }
    import pythia.pyre.inventory

    # Units used in properties
    from pythia.pyre.units.time import s
    from pythia.pyre.units.length import km, m
    from pythia.pyre.units.mass import kg

    # Create a pythia.pyre property named vs with a default value of 2.6 km/s.
    vs = pythia.pyre.inventory.dimensional("vs", default=2.6*km/s)
    vs.meta['tip'] = "S wave speed."

    # Create a pythia.pyre property named vp with a default value of 4.5 km/s.
    vp = pythia.pyre.inventory.dimensional("vp", default=4.5*km/s)
    vp.meta['tip'] = "P wave speed."

    # Create a pythia.pyre property named density with a default value of 2500 kg/m**3.
    density = pythia.pyre.inventory.dimensional("density", default=2.5e+3*kg/m**3)
    density.meta['tip'] = "Density."

    # PUBLIC METHODS /////////////////////////////////////////////////////

    def __init__(self, name="uniformvelmodel"):
        """
        Constructor. This function is called automatically when the Python
        UniformVelModel object is created.
        """
        SpatialDB.__init__(self, name)
        CxxUniformVelModel.__init__(self, "UniformVelModel :UNKNOWN:")

    # PRIVATE METHODS ////////////////////////////////////////////////////

    def _configure(self):
        """
        Set members based on inventory. This function is called
        automatically when the component is setup.
        """
        SpatialDB._configure(self)  # Call parent function.

        # Transfer inventory to C++ object
        CxxUniformVelModel.setVs(self, self.inventory.vs.value)
        CxxUniformVelModel.setVp(self, self.inventory.vp.value)
        CxxUniformVelModel.setDensity(self, self.inventory.density.value)
        return


# FACTORIES ////////////////////////////////////////////////////////////

# Factory used when setting UniformVelModel to a pythia.pyre 'spatial_database' facility.
def spatial_database():
    """
    Factory associated with UniformVelModel.
    """
    return UniformVelModel()


# End of file
