// Assertion tests
// see https://docs.google.com/spreadsheets/d/1XinzHc-ErQPrVWkljRYQ75-38jqTZxG35ZEo9t7XVig/edit?usp=sharing
// for a full list of the assertions, to summarise:
//    A01	No collision with other actors, walls, trees etc.
//    A02	Keep a 'safe distance' to other cars
//    A03	No driving 'off-road'
//    A04	Do not pass a red light
//    A05	Keep at least 1.5m from a cyclist when overtaking
//    A06	Keep to the speed limit
//    A07	stop at a pedestrian crossing if there are people on it
//    A08	Only overtake when safe to do so

#include <iostream>
#include <QtSql>

// Assertion Test 01 -- check AV does not overlap with any other agent
// NB: ST_Overlaps and
// ST_Intersects will not work as the tolerance is 0.00001
void asr_01(QSqlQueryModel& model, QString schema, double sim_time, int asr_id=1, bool diag=true){
    // build string
    QString qs, q1, q2, q3, q4;
    q1 = "INSERT INTO "+schema+".assertions (agent_id, sim_time, asr_id, asr_result) SELECT ";
    q2 = QStringLiteral("g2.agent_id, g2.sim_time, %1 AS asr_id, ST_Distance(g1.geom::geography,g2.geom::geography) > 0.1 AS asr_result ").arg(asr_id);
    q3 = "FROM "+schema+".map_test g1, "+schema+".map_test g2 ";
    //q4 = QStringLiteral("WHERE g1.agent_type = 'AV' AND g2.agent_type != 'AV' AND g1.sim_time = %1 AND g2.sim_time = %1").arg(sim_time);
    q4 = QStringLiteral("WHERE g1.agent_id = 75 AND g2.agent_id != 75 AND g1.sim_time = %1 AND g2.sim_time = %1").arg(sim_time);
    qs = q1 + q2 + q3 + q4;
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#asr_01# " << model.lastError()<<"\033[0m";
    if(diag)
        qDebug() << "#asr_01# " << qs;
}


// Assertion Test 02 -- safe distance to other agents
// sd = safe_distance default safe distance to other agents is 1.5m
// FALSE if AV is 1.5m or more away from other agents as minimum 2D Cartesian distance
// TRUE if assertion violated, i,e, AV<1.5m from agents

//    Insert into agents.assertions (agent_id, sim_time, asr_id, asr_result)
//    SELECT g2.agent_id, 9 as sim_time, 2 AS asr_id, ST_Distance(g1.geom,g2.geom) < 1.5 AS asr_result
//    FROM agents.status g1, agents.status g2
//    WHERE g1.agent_type = 'AV' AND g2.agent_type != 'AV'
//    AND g1.sim_time = 0 AND g2.sim_time = 0

void asr_02(QSqlQueryModel& model, QString schema, double sim_time, double sd=0.00001, int asr_id=2, bool diag=true){
    // build string
    QString qs, q1, q2, q3, q4;
    q1 = "INSERT INTO "+schema+".assertions (agent_id, sim_time, asr_id, asr_result) SELECT ";
    q2 = QStringLiteral("g2.agent_id, g2.sim_time, %1 AS asr_id, "
        "ST_Distance(g1.geom::geography,g2.geom::geography) < %2 AS asr_result ").arg(asr_id).arg(sd);
//     "ST_Distance(g1.geom,g2.geom) > %2 AS asr_result ").arg(asr_id).arg(sd);
    q3 = "FROM "+schema+".status g1, "+schema+".status g2 ";
    //q4 = QStringLiteral("WHERE g1.agent_type = 'AV' AND g2.agent_type != 'AV' AND g1.sim_time = %1 AND g2.sim_time = %1").arg(sim_time);
    q4 = QStringLiteral("WHERE g1.agent_id = 75 AND g2.agent_id != 75 AND g1.sim_time = %1 AND g2.sim_time = %1").arg(sim_time);
    qs = q1 + q2 + q3 + q4;
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#asr_02# " << model.lastError()<<"\033[0m";
    if(diag)
        qDebug() << "#asr_02# " << qs;
}

//    A03	No driving 'off-road'
void asr_03(QSqlQueryModel& model, QString schema, double sim_time, double sd=1.5, int asr_id=2, bool diag=true){
    // build string
    QString qs, q1, q2, q3, q4;
    q1 = "INSERT INTO "+schema+".assertions (agent_id, sim_time, asr_id, asr_result) SELECT ";
    q2 = QStringLiteral("g2.agent_id, g2.sim_time, %1 AS asr_id, ST_Distance(g1.geom,g2.geom) > %2 AS asr_result ").arg(asr_id).arg(sd);
    q3 = "FROM "+schema+".status g1, "+schema+".status g2 ";
    q4 = QStringLiteral("WHERE g1.agent_type = 'AV' AND g2.agent_type != 'AV' AND g1.sim_time = %1 AND g2.sim_time = %1").arg(sim_time);
    qs = q1 + q2 + q3 + q4;
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#asr_02# " << model.lastError()<<"\033[0m";
    if(diag)
        qDebug() << "#asr_02# " << qs;
}

// universal distance query, will return TRUE if actor is outside of limit (sd), i.e. no violation and FALSE if inside it.
void distance_based(QSqlQueryModel& model, QString schema, int target_actorID, double sim_time, double d_nearmiss=0.5, double d_collision=0.01, bool diag=false){
    // build string
    QString qs, q1, q2, q3, q4;
    q1 = "INSERT INTO "+schema+".assertions (agent_id, sim_time, near_miss, collision, haz_distance) SELECT ";
    q2 = QStringLiteral("g2.agent_id, g2.sim_time,  "
        "not ST_Distance(g1.geom::geography,g2.geom::geography) < %1 AS near_miss, not ST_Distance(g1.geom::geography,g2.geom::geography) < %2 AS collision").arg(d_nearmiss).arg(d_collision);
    q3 = ", ST_Distance(g1.geom::geography,g2.geom::geography) as haz_distance FROM "+schema+".status g1, "+schema+".status g2 ";
    q4 = QStringLiteral("WHERE g1.agent_id = %1 AND g2.agent_id != %1 AND g1.sim_time = %2 AND g2.sim_time = %2").arg(target_actorID).arg(sim_time);
    qs = q1 + q2 + q3 + q4;
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#asr_02# " << model.lastError()<<"\033[0m";
    if(diag)
        qDebug() << "#asr_02# " << qs;
}
