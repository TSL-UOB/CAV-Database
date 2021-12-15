# CAV-Database

This program will read a Carla simulation log and convert the actors to postgis geometry. Shapes are based on an actor lookup table.

OSM maps can be imported. GeoJSON data can also be included but is currently a manual step. Additional dynamic data is being added, including braking and thinking distances.

QGIS rendering of a test map in south glos. showing vehicles and pedestrians.
![](database_braking_zone_1.png)

The red hashed area is the braking distance and the green area could be considered thinking distance.
![](database_braking_zone_2.png)

This code runs in QTcreator.

Below is a simulation snapshot of a junction manoeuvre where a vehilce close passes other road users and pedestrians.
![](Test003.gif)

By reading the simulation logs into the database code we can generate an abstracted version of the scenario. This abstracted view allows assertion testing to be undertaken using environmental information from map data for example.
![](pg_demo.gif)

Another example of a pedestrian crossing scenario and the corresponding database view

<img src="https://github.com/TSL-UOB/CAV-Database/blob/master/testB_crop.gif" alt="test B" width="450"/>

<img src="https://github.com/TSL-UOB/CAV-Database/blob/master/testB_data.gif" alt="test B data" width="450"/>


