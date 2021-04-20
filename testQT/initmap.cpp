// This will initiate all the static map data
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <QtSql>

#include "uepostgis.h"

void initMap(QSqlQueryModel& model, QString schema, bool diag){


// ~~~~~~~~~~~~~~~~~~~~ Import the OSM data from file ~~~~~~~~~~~~~~~~~~
    std::cout << "\n*********************"<< std::endl;
    std::cout << "      OSM IMPORT     "<< std::endl;
    std::cout << "*********************"<< std::endl;

    std::string str;
    str = "osm2pgsql -d cav -H localhost -U greg_chance -P 5432 -S "
          "/usr/local/share/osm2pgsql/default.style --hstore assertion_case_study/downend_road.osm";
    const char *command = str.c_str();
    system(command);
    if(diag) qDebug() << "OSM map data imported...";

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




// ~~~~~~~~~~~~~~~~~~~~ Import the lanelets data ~~~~~~~~~~~~~~~~~~
    std::cout << "\n*********************"<< std::endl;
    std::cout << "   LANELETS IMPORT   "<< std::endl;
    std::cout << "*********************"<< std::endl;

    // 1. follow the guide to get lanelets data from openDrive
    // 2. use Abanoubs code to convert map.xml to map.txt

    // read in the txt file into vector format
    std::ifstream file ("assertion_case_study/T_Junction_lanelets_bounds.txt");
    laneletVector(file, diag);

    // identify number of lanelets, capture ID and repeat for each

    // find the number of X's or Y's in the lanelet

    // loop through

    // generate a string of xy pairs comma separated, offset against reference lat/long

    // SELECT ST_GeomFromText('POLYGON((
    // X's Y's, (looped, last one has no comma)
    // ))',4326); (replace 4326 with SRID)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




// ~~~~~~~~~~~~~~~~~~~~ import map geoJSON ~~~~~~~~~~~~~~~~~~~~~

    //TODO

    // run this command
    // ogr2ogr -f "PGDump" test.sql "loop_geo.geojson" -oo NAME=agents
    // Open the file and paste into pgAdmin SQL query tool

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// ~~~~~~~~~~~~~~~~~~~~ select only buildings data from OSM ~~~~~~~~~~~~~~~~~~

//    //Select the agent(s) to test against
//        insert into agents.map_test (agent_id, asr_geom)
//        SELECT agent_id, geom
//        FROM agents.status

//    //Off-road or building collision test
//        insert into agents.assertions (agent_id, sim_time, asr_id, asr_result)
//        select g3.agent_id, 0 AS sim_time, 3 AS asr_id, ST_Overlaps(g1.asr_geom,g2.asr_geom) AS asr_result
//        FROM agents.map_test g3, agents.map_test g1, agents.map_test g2
//        WHERE g1.agent_id = 9 AND g2.agent_id != 9 --g1.sim_time = %1 AND g2.sim_time = %1
//        limit 10

// ~~~~~~~~~~~~~~~~~~~~ Import Buildings ~~~~~~~~~~~~~~~~~~

    // Create table for map polygons used for assertion testing
    QString sql_string;
    sql_string = "DROP TABLE IF EXISTS "+schema+".map_test";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "map_test table deleted...";
    sql_string = "create table "+schema+".map_test (agent_id int, sim_time float, "
                                        "PRIMARY KEY (agent_id, sim_time), agent_type int, geom geometry(polygon, 4326)) "; //
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# map_test table created...";

    // Transfer the map dat removing unecessay polygons
    sql_string = "insert into "+schema+".map_test (agent_id, sim_time, agent_type, geom)"
                 "SELECT osm_id, 0 AS sim_time, 0 AS agent_type, ST_Transform(way,4326) "
                 "FROM public.planet_osm_polygon as P "
                 "where p.landuse is NULL "
                 "and (p.amenity <> 'university' OR p.amenity is null) "
                 "and (p.amenity <> 'hospital' OR p.amenity is null) "
                 "and (p.amenity <> 'parking' OR p.amenity is null) "
                 "and (p.amenity <> 'school' OR p.amenity is null) "
                 "and (p.leisure <> 'recreation_ground' OR p.leisure is null)";
//    sql_string = "insert into "+schema+".map_test (agent_id, sim_time, agent_type, geom)"
//                 "SELECT osm_id, 0 AS sim_time, 0 AS agent_type, ST_Transform(way,4326) "
//                 "FROM public.planet_osm_polygon as P ";

    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#MAP_TRANSFER# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# map-transfer complete...";

    // Insert the into the map_test table
    sql_string = "insert into "+schema+".map_test (agent_id, agent_type, sim_time, geom) "
                                       "SELECT agent_id, agent_type, sim_time, geom FROM "+schema+".status";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#MAP_TRANSFER# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# map-transfer complete...";

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    // Add roadrunner curb location to map - this doesn't work without conversion to polygon, better to use ST_Boundary on line string
    //    insert into agents.map_test (agent_id, sim_time, agent_type, geom)
    //    select ogc_fid AS agent_id, 0 AS sim_time, 80 AS agent_type,
    //        st_makepolygon(st_geomfromtext(st_astext(wkb_geometry))) AS geom
    //    from agents.loop_geo as WG
    //    where WG.lanetype = 'Curb'
/*
    // Identify if AV has crossed Curb boundary as asserion
        DROP TABLE IF EXISTS agents.combo_map
        create table agents.combo_map (id SERIAL primary key, agent_id int, geom geometry)
        insert into agents.combo_map (agent_id, geom)
        select ogc_fid AS agent_id, wkb_geometry AS geom
        from agents.loop_geo
        where lanetype = 'Curb'

    //then do this
        insert into agents.combo_map (agent_id, geom)
        select agent_id, geom
        from agents.map_test
        where sim_time = 12.5

    //then check curb mounting with this
        select g2.agent_id, st_crosses(g1.geom, g2.geom)
        from agents.combo_map g1, agents.combo_map g2
        where g1.agent_id=189 and g2.agent_id != 189

    //or run animation by doing
        DROP TABLE IF EXISTS agents.combo_map
        create table agents.combo_map (id SERIAL primary key, sim_time float, agent_id int, geom geometry )
        insert into agents.combo_map (sim_time, agent_id, geom)
                select -1 AS sim_time, ogc_fid AS agent_id, wkb_geometry AS geom
                from agents.loop_geo
                where lanetype = 'Curb'
        insert into agents.combo_map (sim_time, agent_id, geom)
                select sim_time, agent_id, geom
                from agents.map_test

    //insert some cornering agents to centre the view on the DMR


*/

}
