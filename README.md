# CAV-Database

This program will read a Carla simulation log and convert the actors to postgis geometry. Shapes are based on an actor lookup table.

OSM maps can be imported. GeoJSON data can also be included but is currently a manual step. Additional dynamic data is being added, including braking and thinking distances.

QGIS rendering of a test map in south glos. showing vehicles and pedestrians.
![](database_braking_zone_1.png)

The red hashed area is the braking distance and the green area could be considered thinking distance.
![](database_braking_zone_2.png)

This code runs in QTcreator.
