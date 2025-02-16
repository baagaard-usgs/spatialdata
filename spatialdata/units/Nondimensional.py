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
from ._units import Nondimensional as CxxNondimensional


class NondimensionalMeta(type(Component), type(CxxNondimensional)):
    pass

class Nondimensional(Component, CxxNondimensional, metaclass=NondimensionalMeta):
    """
    Abstract base class for nondimensionalizing problems.
    """

    # PUBLIC METHODS /////////////////////////////////////////////////////

    def __init__(self, name="nondimensional"):
        """
        Constructor.
        """
        Component.__init__(self, name, facility="nondimensional")
        CxxNondimensional.__init__(self)

    def _configure(self):
        Component._configure(self)

    def setLengthScale(self, value):
        """
        Get length scale.
        """
        return CxxNondimensional.setLengthScale(self, value.value)

    def getLengthScale(self):
        """
        Get length scale.
        """
        from pythia.pyre.units.length import meter
        return CxxNondimensional.getLengthScale(self) * meter

    def setPressureScale(self, value):
        """
        Get length scale.
        """
        return CxxNondimensional.setPressureScale(self, value.value)

    def getPressureScale(self):
        """
        Get pressure scale.
        """
        from pythia.pyre.units.pressure import pascal
        return CxxNondimensional.getPressureScale(self) * pascal

    def setTimeScale(self, value):
        """
        Get time scale.
        """
        return CxxNondimensional.setTimeScale(self, value.value)

    def getTimeScale(self):
        """
        Get time scale.
        """
        from pythia.pyre.units.time import second
        return CxxNondimensional.getTimeScale(self) * second

    def setDensityScale(self, value):
        """
        Get density scale.
        """
        return CxxNondimensional.setDensityScale(self, value.value)

    def getDensityScale(self):
        """
        Get density scale.
        """
        from pythia.pyre.units.length import meter
        from pythia.pyre.units.mass import kilogram
        return CxxNondimensional.getDensityScale(self) * kilogram / meter**3

    def setTemperatureScale(self, value):
        """
        Get temperature scale.
        """
        return CxxNondimensional.setTemperatureScale(self, value.value)

    def getTemperatureScale(self):
        """
        Get temperature scale.
        """
        from pythia.pyre.units.temperature import kelvin
        return CxxNondimensional.getTemperatureScale(self) * kelvin

    def nondimensionalize(self, value, scale):
        """
        Make value dimensionless.
        """
        return value / scale

    def dimensionalize(self, value, scale):
        """
        Make value dimensional.
        """
        return value * scale


# FACTORIES ////////////////////////////////////////////////////////////

def nondimensional():
    """
    Factory associated with Nondimensional.
    """
    return Nondimensional()


# End of file
