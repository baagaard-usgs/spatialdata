# AnalyticDB

:::{note}
New in v3.1.0.
:::


If the values for a field can be described by an analytic function, such as a polynomial `f(x) = a * x + b`, then you should use an `AnalyticDB`.
In addition to the analytical expressions for the fields, you also need to provide the name and units for each field, and the coordinate system (the default is `CSCart`).

:::{admonition} Pyre User Interface
See [AnalyticDB component](../components/spatialdb/AnalyticDB.md).
:::

## Examples

We create an `AnalyticDB` specifying a displacement field composed of polynomial functions.

::::{tab-set}

:::{tab-item} C++

```{code-block} c++
---
caption: Creating an `AnalyticDB` using C++. 
---
// Data to populate AnalyticDB.
const std::vector<std::string> names({ "displacement_x", "displacement_y" });
const std::vector<std::string> units({ "cm", "cm" });
const std::vector<std::string> expressions({
     "2.0 * x^2 - 1.5 * y + 5.0", 
     "7.5 * y^2 + 4.8 * x + 2.0 * y + 1.5",
      });
spatialdata::geocoords::CSCart cs;
cs.setSpaceDim(2);

// Create AnalyticDB 'db; and set the values.
spatialdata::spatialdb::AnalyticDB db("C++ example");
db.setCoordSys(cs)
db.setData(names, units, expressions);
```

:::

:::{tab-item} Python
Python applications should use the Pyre framework for creating the `AnalyticDB`.
We show how to do it programmatically in Python for completeness.

```{code-block} python
---
caption: Creating a `AnalyticDB` using Python.
---
from spatialdata.spatialdb.AnalyticDB import AnalyticDB
db = AnalyticDB()
db.description = "Python example"
db.label = "Analytic spatial database for displacement field"
db.values = ["displacement_x", "displacement_y"]
db.units = ["cm", "cm"]
db.expressions = ["2.0 * x^2 - 1.5 * y + 5.0", "7.5 * y^2 + 4.8 * x + 2.0 * y + 1.5"]
db._configure()
```

:::

::::
