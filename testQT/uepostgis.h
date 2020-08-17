#ifndef UEPOSTGIS_H
#define UEPOSTGIS_H

#include <QtSql>

QSqlDatabase db_connect(char userID[]); //fwd declaration for db connection
void db_tables(QSqlQueryModel& model, QString schema="ch03", bool diag=true); //fwd declaration for creating tables

//assertions
void asr_01(QSqlQueryModel& model, QString schema, double sim_time, int asr_id=1, bool diag=true);
void asr_02(QSqlQueryModel& model, QString schema, double sim_time, double sd=1.5, int asr_id=2, bool diag=true);
void distance_based(QSqlQueryModel& model, QString schema, int target_actorID, double sim_time, double d_nearmiss=0.5, double d_collision=0.01, bool diag=true);

#endif // UEPOSTGIS_H


