# =================================================================================================
# This code is part of SpatialData, developed through the Computational Infrastructure
# for Geodynamics (https://github.com/geodynamics/spatialdata).
#
# Copyright (c) 2010-2024, University of California, Davis and the SpatialData Development Team.
# All rights reserved.
#
# See https://mit-license.org/ and LICENSE.md and for license information. 
# =================================================================================================


def write(time, amplitude, units, filename):
    """
    Write time history file.
    """
    from ._spatialdb import TimeHistoryIO as CxxTimeHistoryIO
    CxxTimeHistoryIO.write(time, amplitude, units, filename)


# End of file