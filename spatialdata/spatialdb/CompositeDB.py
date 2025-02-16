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
from .SpatialDB import SpatialDB
from ._spatialdb import CompositeDB as CxxCompositeDB

class DBEntry(Component):
    """
    Information for a virtual spatial database.
    """
    DOC_CONFIG = {
        "cfg": """
            [compositedb.dbA]
            db = spatialdata.spatialdb.SimpleDB
            db.description = Wave speed spatial database.
            db.filename = vpvs.spatialdb

            values = [vp, vs]
            """,
    }

    import pythia.pyre.inventory

    values = pythia.pyre.inventory.list("values", default=[])
    values.meta['tip'] = "Names of values to query in database."

    from .UniformDB import UniformDB
    db = pythia.pyre.inventory.facility("db", factory=UniformDB, family="spatial_database")
    db.meta['tip'] = "Spatial database."


    # PUBLIC METHODS /////////////////////////////////////////////////////

    def __init__(self, name="dbentry"):
        """
        Constructor.
        """
        Component.__init__(self, name, facility="db_entry")

    # PRIVATE METHODS ////////////////////////////////////////////////////

    def _configure(self):
        """
        Set members based on inventory.
        """
        Component._configure(self)
        self._validateParameters(self.inventory)

    def _validateParameters(self, data):
        """
        Validate parameters.
        """
        if (0 == len(data.values)):
            raise ValueError(f"Error in spatial database entry '{self.name}'\n"
                             "Names of values to query in database not set.")

class EmptyBin(Component):
  """
  Empty container for a collection of objects.
  """

  def __init__(self, name="emptybin"):
    """Constructor.
    """
    Component.__init__(self, name, facility="empty_bin")


def dbFactory(name):
    """Factory for spatial database items.
    """
    from pythia.pyre.inventory import facility
    from spatialdata.spatialdb.UniformDB import UniformDB
    return facility(name, family="spatial_database", factory=UniformDB)


class CompositDBMeta(type(SpatialDB), type(CxxCompositeDB)):
    pass


class CompositeDB(SpatialDB, CxxCompositeDB, metaclass=CompositDBMeta):
    """
    Virtual spatial database implemented as a combination of two spatial databases.
    This spatial database is useful when you need to provide additional values beyond those present in an existing spatial database or some values have a different layout than others.

    Implements `SpatialDB`.
    """
    DOC_CONFIG = {
        "cfg": """
            db = spatialdata.spatialdb.CompositeDB

            [db]
            dbs = [density, wavespeeds]
            
            [db.dbs.density]
            values = [density]
            db = spatialdata.spatialdb.UniformDB
            db.description = Density spatial database.
            db.values = [density]
            db.data = [3000*kg/m**3]

            [db.dbs.wavespeeds]
            values = [vp, vs]
            db = spatialdata.spatialdb.SimpleDB
            db.description = Wave speed spatial database.
            db.filename = vpvs.spatialdb
            """,
    }

    import pythia.pyre.inventory

    dbs = pythia.pyre.inventory.facilityArray("dbs", itemFactory=dbFactory, factory=EmptyBin)
    dbs.meta['tip'] = "Names of values to query with database A."

    # PUBLIC METHODS /////////////////////////////////////////////////////

    def __init__(self, name="compositedb"):
        """
        Constructor.
        """
        SpatialDB.__init__(self, name)
        CxxCompositeDB.__init__(self, "CompositeDB :UNKNOWN:")

    # PRIVATE METHODS ////////////////////////////////////////////////////

    def _configure(self):
        """
        Set members based on inventory.
        """
        SpatialDB._configure(self)
        for db in self.dbs.components():
            CxxCompositeDB.addDB(self, db.db, db.values)


# FACTORIES ////////////////////////////////////////////////////////////

def spatial_database():
    """
    Factory associated with CompositeDB.
    """
    return CompositeDB()


# End of file
