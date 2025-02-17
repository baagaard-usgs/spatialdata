# CompositeDB

% WARNING: Do not edit; this is a generated file!
:Full name: `spatialdata.spatialdb.CompositeDB`
:Journal name: `compositedb`

Virtual spatial database implemented as a combination of two spatial databases.
This spatial database is useful when you need to provide additional values beyond those present in an existing spatial database or some values have a different layout than others.

Implements `SpatialDB`.

## Pyre Facilities

* `dbs`: Names of values to query with database A.
  - **current value**: 'emptybin', from {default}
  - **configurable as**: emptybin, dbs

## Pyre Properties

* `description`=\<str\>: Description for database.
  - **default value**: ''
  - **current value**: '', from {default}
  - **validator**: <function validateDescription at 0x102bb07c0>

## Example

Example of setting `CompositeDB` Pyre properties and facilities in a parameter file.

:::{code-block} cfg
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
:::

