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
    """Generate SimpleGridDB file from data.

    @param data Dictionary with data for database.
    @param filename Name of output file.

    data = {
        'points': 2-D array (numLocs, spaceDim),
        'x': Array of x coordinates,
        'y': Array of y coordinates,
        'z': Array of z coordinates,
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
    from ._spatialdb import SimpleGridDBIO as CxxSimpleGridDBIO
    from ._spatialdb import SimpleGridDBData as CxxSimpleGridDBData

    (numLocs, spaceDim) = data['points'].shape
    numValues = len(data['values'])
    names = []
    units = []
    values = numpy.zeros((numLocs, numValues), dtype=numpy.float64)
    i = 0
    for value in data['values']:
        names.append(value['name'])
        units.append(value['units'])
        values[:, i] = value['data'][:]
        i += 1

    numX = data['x'].shape[0]
    numY = data['y'].shape[0]
    if data['coordsys'].getSpaceDim() == 2:
        numZ = 0
        if (numLocs != numX * numY):
            raise ValueError("Number of locations (%d) does not match coordinate dimensions (%d, %d)." %
                                (numLocs, numX, numY))
    else:
        numZ = data['z'].shape[0]
        if (numLocs != numX * numY * numZ):
            raise ValueError("Number of locations (%d) does not match coordinate dimensions (%d, %d, %d)." %
                                (numLocs, numX, numY, numZ))

    dbData = CxxSimpleGridDBData()
    dbData.coordsys = data['coordsys']
    dbData.allocate(numX, numY, numZ, numValues, spaceDim, data['data_dim'])
    dbData.x = data['x']
    dbData.y = data['y']
    if data['coordsys'].getSpaceDim() == 3:
        dbData.z = data['z']
    dbData.names = names
    dbData.units = units
    dbData.setData(data['points'], values)

    CxxSimpleGridDBIO.write(dbData, filename)


# End of file