// This function will create tables for the db
#include <iostream>
#include <QtSql>

void db_tables(QSqlQueryModel& model, QString schema, bool diag=true)
{

    QString sql_string;

    std::cout << "\n*********************"<< std::endl;
    std::cout << " Table Configuration "<< std::endl;
    std::cout << "*********************"<< std::endl;

    // Create a new Schema for agents

    // ~~~~~~~~~~~~~~~~~~~~ SCHEMA ~~~~~~~~~~~~~~~~~~
    sql_string = "CREATE SCHEMA IF NOT EXISTS " + schema;
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# schema "+schema+" created...";

    // Remove "+schema+".status to prevent multiple entries


    sql_string = "CREATE TYPE vehicles AS ENUM ('AV', 'car', 'pedestrian', 'hgv', 'cyclist', 'smallcar')";
    model.setQuery(sql_string);

//    // ~~~~~~~~~~~~~~~~~~~~ STATUS~~~~~~~~~~~~~~~~~~
//    sql_string = "DROP TABLE IF EXISTS "+schema+".status";
//    model.setQuery(sql_string);
//    if (model.lastError().isValid())
//        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
//    if(diag) qDebug() << "#db_tables# status deleted...";

//    // Make polygons table
//    // sql_string = "create table "+schema+".status (agent_id int, agent_type vehicles, sim_time float, "
//    // "PRIMARY KEY (agent_id,sim_time), geom geometry(POLYGON))";
//    sql_string = "create table "+schema+".status (agent_id int, agent_type int, sim_time float, "
//                 "PRIMARY KEY (agent_id,sim_time), geom geometry(POLYGON))";
//    model.setQuery(sql_string);
//    if (model.lastError().isValid())
//        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
//    if(diag) qDebug() << "#db_tables# status created...";

//    // ~~~~~~~~~~~~~~~~~~~~ ASSERTION ~~~~~~~~~~~~~~~~~~
//    // Create table for Assertion tests
//    sql_string = "DROP TABLE IF EXISTS "+schema+".assertions";
//    model.setQuery(sql_string);
//    if (model.lastError().isValid())
//        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
//    if(diag) qDebug() << "assertions table deleted...";
//    sql_string = "create table "+schema+".assertions (id serial primary key, agent_id int, sim_time float, asr_id int, "
//                 "asr_result bool)"; //
//    model.setQuery(sql_string);
//    if (model.lastError().isValid())
//        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
//    if(diag) qDebug() << "#db_tables# assertions table created...";

    // ~~~~~~~~~~~~~~~~~~~~ USER READABLE ASSERTION ~~~~~~~~~~~~~~~~~~
    // Create table for Assertion tests

    sql_string = "CREATE TABLE IF NOT EXISTS "+schema+".assertions (id serial primary key, agent_id int, sim_time float, "
                 "near_miss bool, collision bool, haz_distance float)"; //
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# assertions table created...";

    sql_string = "DELETE FROM "+schema+".assertions";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "assertions table deleted...";

    // ~~~~~~~~~~~~~~~~~~~~ AGENT~~~~~~~~~~~~~~~~~~
    // Create table for Agent Lookup
    sql_string = "CREATE TABLE IF NOT EXISTS "+schema+".lookup (agent_type vehicles PRIMARY KEY, "
                 "agent_width float, agent_length float)"; //
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# lookup table created...";

    sql_string = "DELETE FROM "+schema+".lookup";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "lookup table deleted...";

    // ~~~~~~~~~~~~~~~~~~~~ Actors ~~~~~~~~~~~~~~~~~~


    sql_string = "CREATE TABLE IF NOT EXISTS "+schema+".actors (agent_id int, agent_type int, sim_time float, "
                 "PRIMARY KEY (agent_id,sim_time), geom geometry(POLYGON))";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# braking created...";

    // Create dynamic data table for Actors
    sql_string = "DELETE FROM "+schema+".actors";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "actors table deleted...";

    // ~~~~~~~~~~~~~~~~~~~~ Zones~~~~~~~~~~~~~~~~~~
    // Create dynamic data table for Braking Zone
    sql_string = "CREATE TABLE IF NOT EXISTS "+schema+".braking (agent_id int, agent_type int, sim_time float, "
                 "PRIMARY KEY (agent_id,sim_time), geom geometry(POLYGON))";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# braking created...";

    sql_string = "DELETE FROM "+schema+".braking";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "braking zone table deleted...";


    //-------------------------------------------
    // Create dynamic data table for Thinking Zone
    sql_string = "CREATE TABLE IF NOT EXISTS "+schema+".thinking (agent_id int, agent_type int, sim_time float, "
                 "PRIMARY KEY (agent_id,sim_time), geom geometry(POLYGON))";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# thinking created...";

    sql_string = "DELETE FROM "+schema+".thinking";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "thinking zone table deleted...";



    //-------------------------------------------
    // Create LIVE tables for QGIS videos

    sql_string = "CREATE TABLE IF NOT EXISTS "+schema+".frame_braking (agent_id int, agent_type int, sim_time float, "
                 "PRIMARY KEY (agent_id,sim_time), geom geometry(POLYGON))";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# frame_braking created...";

    sql_string = "DELETE FROM "+schema+".frame_braking";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "frame_braking zone table deleted...";

//    // and for THINKING...

    sql_string = "CREATE TABLE IF NOT EXISTS "+schema+".frame_thinking (agent_id int, agent_type int, sim_time float, "
                 "PRIMARY KEY (agent_id,sim_time), geom geometry(POLYGON))";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# frame_thinking created...";

    sql_string = "DELETE FROM "+schema+".frame_thinking";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "frame_thinking  zone table deleted...";

}
