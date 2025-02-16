# =================================================================================================
# This code is part of SpatialData, developed through the Computational Infrastructure
# for Geodynamics (https://github.com/geodynamics/spatialdata).
#
# Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
# All rights reserved.
#
# See https://mit-license.org/ and LICENSE.md and for license information. 
# =================================================================================================

import pathlib

from .SpatialDB import SpatialDB
from ._spatialdb import SimpleGridDB as CxxSimpleGridDB


def validateFilename(value):
    """
    Validate filename.
    """
    if 0 == len(value):
        raise ValueError("Name of SimpleGridDB file must be specified.")
    if not pathlib.Path(value).is_file():
        raise IOError(f"SimpleGridDB file '{value}' not found.")
    return value


class SimpleGridDBMeta(type(SpatialDB), type(CxxSimpleGridDB)):
    pass


class SimpleGridDB(SpatialDB, CxxSimpleGridDB, metaclass=SimpleGridDBMeta):
    """
    Simple spatial database on a logically rectangular grid aligned with the coordinate axes.
    Points along the coordinate axes do not have to be uniformly spaced.

    Implements `SpatialDB`.
    """
    DOC_CONFIG = {
        "cfg": """
            db = spatialdata.spatialdb.SimpleGridDB

            [db]
            description = Material properties
            filename = mat_elastic.spatialdb
            query_type = linear
            """,
    }

    import pythia.pyre.inventory

    filename = pythia.pyre.inventory.str("filename", default="", validator=validateFilename)
    filename.meta['tip'] = "Name for data file."

    queryType = pythia.pyre.inventory.str("query_type", default="nearest")
    queryType.validator = pythia.pyre.inventory.choice(["nearest", "linear"])
    queryType.meta['tip'] = "Type of query to perform."

    # PUBLIC METHODS /////////////////////////////////////////////////////

    def __init__(self, name="simplegriddb"):
        """
        Constructor.
        """
        SpatialDB.__init__(self, name)
        CxxSimpleGridDB.__init__(self, "SimpleGridDB :UNKNOWN:")

    # PRIVATE METHODS ////////////////////////////////////////////////////

    def _configure(self):
        """
        Set members based on inventory.
        """
        SpatialDB._configure(self)
        CxxSimpleGridDB.setFilename(self, self.filename)
        CxxSimpleGridDB.setQueryType(self, self._parseQueryString(self.queryType))

    def _parseQueryString(self, label):
        if label.lower() == "nearest":
            value = CxxSimpleGridDB.NEAREST
        elif label.lower() == "linear":
            value = CxxSimpleGridDB.LINEAR
        else:
            raise NotImplementedError(f"Unknown query type '{label}'.")
        return value


# FACTORIES ////////////////////////////////////////////////////////////

def spatial_database():
    """
    Factory associated with SimpleGridDB.
    """
    return SimpleGridDB()


# End of file
