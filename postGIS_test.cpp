#include <iostream>
#include <stdlib.h>
#include <QtDebug>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtCore/qglobal.h>
//#include "/usr/include/postgresql/libpq-fe.h"
//#include "/home/is18902/Qt/5.12.0/Src/qtbase/src/plugins/sqldrivers/psql/qsql_psql.cpp"

using namespace std;

//forward declaration
//int SQL_connect();

int main()
{
    qDebug()<<"start";

    cout << "**************************" << endl;
    cout << "* postGIS Database Query *" << endl;
    cout << "**************************" << endl;

    /* Connect to SQL Server with credentials */
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL"); //these drivers for postgres support
    db.setHostName("localhost");
//    db.setDatabaseName("/var/lib/postgresql/9.5/main/dvdrental");
    db.setDatabaseName("dvdrental");
    db.setUserName("postgres");
    db.setPassword("testdB_pa55word");
//    db.setPort(5432);

    /* open the database */
    bool ok = db.open();
    if(ok){  cout << "Database opened" << endl; }

    cout << db.lastError().text().toStdString() << endl;

    /* Check connection options string */
    QString connect_string = db.connectOptions();
    cout << "Connection options:" << connect_string.toStdString() << endl;

    /* Check db connection */
    QSqlError err = db.lastError();
    cout << "Database error code is: " << err.type() << endl;
    switch(err.type()){
        case 0:
            cout << "Connection Successful" << endl; break;
        case 1:
            cout << "Connection Error" << endl; break;
        case 2:
            cout << "SQL statement syntax error" << endl; break;
        default :
            cout << "Invalid connection error type" << endl; }

//    /*Query db*/
//    QSqlQuery query("SELECT version()");

    QSqlQueryModel model;
    model.setQuery("SELECT * FROM actor");
    //model.setQuery("SELECT version()");


    for (int i = 0; i < model.rowCount(); ++i) {
        int id = model.record(i).value("actor_id").toInt();
        QString name = model.record(i).value("first_name").toString();
        qDebug() << id << name;
    }


    return 0;
}




