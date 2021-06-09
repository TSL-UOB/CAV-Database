// This function will handle the connections to the postgreSQL database
// Important things are the databse name, username password and port number
//#include <stdio.h>
#include <iostream>
#include <QtSql>
#include <cstring>

QSqlDatabase db_connect(char userID[])
{
    std::cout << "*********************"<< std::endl;
    std::cout << " Database Connection "<< std::endl;
    std::cout << "*********************"<< std::endl;

    // Connect to SQL Server with credentials
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL"); //these drivers for postgres support
    char user1[] = "greg";
    char user2[] = "severin";
    char user3[] = "dockerTest";

    if(strcmp(userID, user1) == 0)
    {
        db.setHostName("localhost");
        db.setDatabaseName("cav");
        db.setUserName("greg_chance");
        db.setPassword("greg_chance");
    }
    if(strcmp(userID, user2) == 0)
    {
        db.setHostName("localhost");
        db.setDatabaseName("robopilotdb");
        db.setUserName("postgres");
        db.setPassword("postgres");
    }
    if(strcmp(userID, user3) == 0)
    {
        db.setHostName("localhost");
        db.setDatabaseName("hippo");
        db.setUserName("hippo");
        db.setPassword("datalake");
    }

    // open the database
    bool ok = db.open();
    if(ok)  std::cout << "Database opened!" << std::endl;
    if(!ok) std::cout << "Database failed to open" << std::endl;

    //print any errors, may be empty
    std::string conn_string = db.lastError().text().toStdString();
    if (conn_string.length()>1){std::cout << "Connection errors are:" << conn_string << std::endl;}

    /* Check connection options string */
    QString connect_string = db.connectOptions();
    if (!connect_string.isEmpty()){std::cout << "Connection options:" << connect_string.toStdString() << std::endl;}

    /* Check db connection */
    QSqlError err = db.lastError();
    std::cout << "Database error code is: " << err.type() << std::endl;
    switch(err.type()){
        case 0:
            std::cout << "Connection Successful" << std::endl;
            break;
        case 1:
            std::cout << "Connection Error" << std::endl; break;
        case 2:
            std::cout << "SQL statement syntax error" << std::endl; break;
        default :
            std::cout << "Invalid connection error type" << std::endl; }

    //run postGIS specific query
    QSqlQueryModel query_model;
    query_model.setQuery("SELECT PostGIS_full_version()");
    QString pg_version = query_model.record(0).value(0).toString();
    qDebug() << pg_version;

    return db;
}
