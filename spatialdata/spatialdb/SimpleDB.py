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
from ._spatialdb import SimpleDB as CxxSimpleDB


def validateFilename(value):
    """Validate filename.
    """
    if 0 == len(value):
        raise ValueError("Name of SimpleDB file must be specified.")
    if not pathlib.Path(value).is_file():
        raise IOError(f"SimpleDB file '{value}' not found.")
    return value


class SimpleDBMeta(type(SpatialDB), type(CxxSimpleDB)):
    pass


class SimpleDB(SpatialDB, CxxSimpleDB, metaclass=SimpleDBMeta):
    """
    Simple spatial database for points with arbitrary layout.
    Use SimpleGridDB for points on a logically rectangular grid aligned with the coordinate axes.

    Implements `SpatialDB`.
    """
    DOC_CONFIG = {
        "cfg": """
            db = spatialdata.spatialdb.SimpleDB

            [db]
            description = Material properties
            filename = mat_elastic.spatialdb
            query_type = linear
            """,
    }

    import pythia.pyre.inventory

    filename = pythia.pyre.inventory.str("filename", default="", validator=validateFilename)
    filename.meta['tip'] = "Name for SimpleGridDB file."

    queryType = pythia.pyre.inventory.str("query_type", default="nearest")
    queryType.validator = pythia.pyre.inventory.choice(["nearest", "linear"])
    queryType.meta['tip'] = "Type of query to perform."

    # PUBLIC METHODS /////////////////////////////////////////////////////

    def __init__(self, name="simpledb"):
        """
        Constructor.
        """
        SpatialDB.__init__(self, name)
        CxxSimpleDB.__init__(self, "SimpleDB :UNKNOWN:")

    # PRIVATE METHODS ////////////////////////////////////////////////////

    def _configure(self):
        """Set members based on inventory.
        """
        SpatialDB._configure(self)
        CxxSimpleDB.setFilename(self, self.filename)
        CxxSimpleDB.setQueryType(self, self._parseQueryString(self.queryType))

    def _parseQueryString(self, label):
        if label.lower() == "nearest":
            value = CxxSimpleDB.NEAREST
        elif label.lower() == "linear":
            value = CxxSimpleDB.LINEAR
        else:
            raise NotImplementedError(f"Unknown query type '{label}'.")
        return value


# FACTORIES ////////////////////////////////////////////////////////////

def spatial_database():
    """
    Factory associated with SimpleDB.
    """
    return SimpleDB()


# End of file
