# =================================================================================================
# This code is part of SpatialData, developed through the Computational Infrastructure
# for Geodynamics (https://github.com/geodynamics/spatialdata).
#
# Copyright (c) 2010-2024, University of California, Davis and the SpatialData Development Team.
# All rights reserved.
#
# See https://mit-license.org/ and LICENSE.md and for license information. 
# =================================================================================================

def write(data: dict, filename: str):
    """
    Write SimpleDB data to file.

    @param data Dictionary of the following form.
    @param filename Name of spatial database file.

    data = {
        'points': 2-D array (numLocs, spaceDim),
        'coordsys': Coordinate system associated with locations,
        'data_dim': Dimension of spatial distribution,
        'values': [{
            'name': Name of value,
            'units': Units of value,
            'data': Data for value (numLocs)
            }]
        }
    """
    import numpy
    from ._spatialdb import SimpleDBIO as CxxSimpleDBIO
    from ._spatialdb import SimpleDBData as CxxSimpleDBData

    (numLocs, spaceDim) = data['points'].shape
    dataDim = data['data_dim']
    numValues = len(data['values'])
    names = [value["name"] for value in data["values"]]
    units = [value["units"] for value in data["values"]]

    values = numpy.zeros((numLocs, numValues), dtype=numpy.float64)
    for i, value in enumerate(data['values']):
        values[:, i] = value['data'][:]

    dbData = CxxSimpleDBData()
    dbData.coordsys = data['coordsys']
    dbData.allocate(numLocs, numValues, spaceDim, dataDim)
    dbData.names = names
    dbData.units = units
    dbData.setData(data['points'], values)

    CxxSimpleDBIO.write(dbData, filename)


# End of file