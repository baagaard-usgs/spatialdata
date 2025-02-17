# CompositeDB

For some use cases, it is advantageous to combine multiple spatial databases into one.
For example, you may want to combine elastic properties from a seismic velocity model with viscoelatic properties.
Rather than creating an entirely new spatial database with elastic and viscoelatic properties, a `CompositeDB` allows you to combine the spatial databases with elastic properties with a separate spatial database with viscoelastic properties.

Another case when a `CompositeDB` is advantageous is when the fields within a spatial database have different geometric distributions of values.
`SimpleDB` and `SimpleGridDB` both require single a single set of points for all values, so you cannot specify different geometric distributions for different values.
However, you can use a `CompositeDB` to combine spatial databases with different geometric distributions.
For example, in specifying elastic properties the density might vary with depth, whereas the shear modulus might vary in both the horizontal and vertical directions.
In such a case, you could use one spatial database with points in the vertical direction for the density and another spatial database with a set of points distributed in a volume for the shear modulus.

:::{admonition} Pyre User Interface
See [CompositeDB component](../components/spatialdb/CompositeDB.md).
:::

## Examples

We create a `CompositeDB` with density, P wave speed (Vp), and S wave speed (Vs) from a `SimpleGridDB`  spatial database and viscosity from a `SimpleDB` spatial database.

::::{tab-set}

:::{tab-item} C++

```{code-block} c++
---
caption: Creating a `CompositeDB` in C++ that combines a `SimpleGridDB` with a `SimpleDB`.
---
spatialdata::spatialdb::SimpleGridDB dbElastic("elastic");
dbElastic.setFilename("elastic.spatialdb");
const std::vector<std::string> namesElastic({ "density", "Vp", "Vs" });

spatialdata::spatialdb::SimpleDB dbViscoelastic("viscoelastic");
dbViscoelastic.setFilename("viscoelastic.spatialdb");
const size_t numViscoelastic = 1;
const std::vector<std::string> namesViscoelastic({ "viscosity" });

spatialdata::spatialdb::CompositeDB dbComposite("composite");
dbComposite.addDB(&dbElastic, namesElastic);
dbComposite.addDB(&dbViscoelastic, namesViscoelastic);
```

:::

:::{tab-item} Python

Python applications should use the Pyre framework for creating the `CompositeDB`.
We show how to do it programmatically in Python for completeness.

```{code-block} python
---
caption: Creating a `CompositeDB` using Python.
---
from pythia.pyre.components.Component import Component

from spatialdata.spatialdb.SimpleGridDB import SimpleGridDB
from spatialdata.spatialdb.SimpleDB import SimpleDB
from spatialdata.spatialdb.CompositeDB import (CompositeDB, DBEntry)

class Two(Component):
    import pythia.pyre.inventory
    from spatialdata.spatialdb.UniformDB import UniformDB
    db1 = pythia.pyre.inventory.facility("db1", family="spatial_database", factory=UniformDB)
    db1.meta['tip'] = "Database A"
    db2 = pythia.pyre.inventory.facility("db2", family="spatial_database", factory=UniformDB)
    db2.meta['tip'] = "Database A"

    def __init__(self, name="two"):
        """Constructor.
        """
        Component.__init__(self, name, facility="two")


dbElastic = SimpleGridDB()
dbElastic.filename = "elastic.spatialdb"
dbElastic._configure()

compositeElastic = DBEntry()
compositeElastic.inventory.db = dbElastic
compositeElastic.inventory.values = ["density", "Vp", "Vs"]
compositeElastic._configure()

dbViscoelastic = SimpleDB()
dbViscoelastic.filename = "viscoelastic.spatialdb"
dbViscoelastic._configure()

compositeViscoelastic = DBEntry()
compositeViscoelastic.inventory.db = dbViscoelastic
compositeViscoelastic.inventory.values = ["viscosity"]
compositeViscoelastic._configure()

twoDBs = Two()
twoDBs.inventory.db1 = compositeElastic
twoDBs.inventory.db2 = compositeViscoelastic
twoDBs._configure()

db = CompositeDB()
db.inventory.dbs = twoDBs
db._configure()
```

:::

::::