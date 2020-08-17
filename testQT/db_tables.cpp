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

    // ~~~~~~~~~~~~~~~~~~~~ STATUS~~~~~~~~~~~~~~~~~~
    sql_string = "DROP TABLE IF EXISTS "+schema+".status";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# status deleted...";

    // Make polygons table
    // sql_string = "create table "+schema+".status (agent_id int, agent_type vehicles, sim_time float, "
    // "PRIMARY KEY (agent_id,sim_time), geom geometry(POLYGON))";
    sql_string = "create table "+schema+".status (agent_id int, agent_type int, sim_time float, "
                 "PRIMARY KEY (agent_id,sim_time), geom geometry(POLYGON))";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# status created...";

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
    sql_string = "DROP TABLE IF EXISTS "+schema+".assertions";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "assertions table deleted...";
    sql_string = "create table "+schema+".assertions (id serial primary key, agent_id int, sim_time float, "
                 "near_miss bool, collision bool)"; //
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# assertions table created...";

    // ~~~~~~~~~~~~~~~~~~~~ AGENT~~~~~~~~~~~~~~~~~~
    // Create table for Agent Lookup
    sql_string = "DROP TABLE IF EXISTS "+schema+".lookup";
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "lookup table deleted...";
    sql_string = "create table "+schema+".lookup (agent_type vehicles PRIMARY KEY, "
                 "agent_width float, agent_length float)"; //
    model.setQuery(sql_string);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#db_tables# lookup table created...";

}
