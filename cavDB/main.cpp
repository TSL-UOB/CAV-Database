#include <iostream>
#include <stdlib.h>
#include <QtSql>
#include <sys/time.h> //for timer
#include <cstdlib> //for random number generation
#include <tuple> //used in rotate_coords
#include <cmath>
#include <fstream> //reading config file
#include <sstream> //splitting config file strings
#include <vector>
#include <iomanip> //setprecision
#include "uepostgis.h"
#include <tuple>
#include <unistd.h> //for pause timer
#include <iterator> //for vector indexing

#include "readsimlog.h"

typedef  long timestamp_t;
static timestamp_t
get_timestamp ()
{
  struct timeval now;
  gettimeofday (&now, nullptr);
  return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}
double check_agent_dist(QSqlQueryModel& model, QString schema, QString A, QString B)
{
// Quick function for ST_Distance
// Returns the minimum 2D Cartesian distance between two geometries in projected units
    QString sql_string = "select ST_Distance(g1.geom,g2.geom) "\
            "FROM "+schema+".status g1, "+schema+".status g2 "\
            "WHERE g1.agent_type = '"+A+"' AND g2.agent_type = '"+B+"'";
    model.setQuery(sql_string);
    double distanceTest = model.record(0).value(0).toDouble();
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#check_agent_dist# " << model.lastError()<<"\033[0m";
    qDebug() << "#check_agent_dist# Distance between "<< A<<
                " and "<<B<<" is " << distanceTest << endl;
    return distanceTest;
}
double check_agent_within(QSqlQueryModel& model, QString schema,QString A, QString B)
{
// Function for ST_within
// Returns TRUE if geometry A is completely inside geometry B.
    QString sql_string = "select ST_Within(g1.geom,g2.geom) "\
            "FROM "+schema+".status g1, "+schema+".status g2 "\
            "WHERE g1.agent_type = '"+A+"' AND g2.agent_type = '"+B+"'";
    model.setQuery(sql_string);
    bool within_bound = model.record(0).value(0).toBool();
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#check_agent_within# " << model.lastError()<<"\033[0m";
    qDebug() << "#check_agent_within# Is "<< A<<
                " within bounds of "<<B<<"? " << within_bound << endl;
    return within_bound;
}
double check_agent_overlap(QSqlQueryModel& model, QString schema,QString A, QString B)
{
// Function for ST_Overlaps
// Returns TRUE if the Geometries "spatially overlap". By that we mean they intersect,
// but one does not completely contain another.
    QString sql_string = "select ST_Overlaps(g1.geom,g2.geom) "\
            "FROM "+schema+".status g1, "+schema+".status g2 "\
            "WHERE g1.agent_type = '"+A+"' AND g2.agent_type = '"+B+"'";
    model.setQuery(sql_string);
    bool agent_overlap = model.record(0).value(0).toBool();
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#check_agent_overlap# " << model.lastError()<<"\033[0m";
    qDebug() << "#check_agent_overlap# Do "<< A<<
                " and "<<B<<" overlap? " << agent_overlap << endl;
    return agent_overlap;
}
double ass_check_agent_overlap(QSqlQueryModel& model, QString schema,int A_id, int A_ty,
                               int B_id, int B_ty, double sim_time, bool diag=true)
{
    QString qs, q1, q2, q3;
    q1 = "select ST_Overlaps(g1.geom,g2.geom) "
            "FROM "+schema+".status g1, "+schema+".status g2 ";
    q2 = QStringLiteral("WHERE g1.agent_id = %1 AND g1.agent_type = %2 AND g1.sim_time = %3 ").arg(A_id).arg(A_ty).arg(sim_time);
    q3 = QStringLiteral("AND g2.agent_id = %1 AND g2.agent_type = %2 AND g2.sim_time = %3 ").arg(B_id).arg(B_ty).arg(sim_time);
    qs = q1+q2+q3;

    model.setQuery(qs);
    bool agent_overlap = model.record(0).value(0).toBool();
    if (model.lastError().isValid())
        qDebug() << "#\033[0;31mass_check_agent_overlap# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#ass_check_agent_overlap# Do "<< A_id<<
                " and "<<B_id<<" overlap? " << agent_overlap << endl;
    return agent_overlap;
}
double check_agent_contains(QSqlQueryModel& model, QString schema,QString A, QString B)
{
// Function for ST_Contains
// Geometry A contains Geometry B if and only if no points of B lie in the exterior of
// A, and at least one point of the interior of B lies in the interior of A. An
// important subtlety of this definition is that A does not contain its boundary,
// but A does contain itself. Contrast that to ST_ContainsProperly where geometry
// A does not Contain Properly itself.
// Returns TRUE if geometry B is completely inside geometry A.
    QString sql_string = "select ST_Contains(g1.geom,g2.geom) "\
            "FROM "+schema+".status g1, "+schema+".status g2 "\
            "WHERE g1.agent_type = '"+A+"' AND g2.agent_type = '"+B+"'";
    model.setQuery(sql_string);
    bool agent_contains = model.record(0).value(0).toBool();
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#check_agent_contains# " << model.lastError()<<"\033[0m";
    qDebug() << "#check_agent_contains# Does "<< A<<
                " completely contain "<<B<<"? " << agent_contains << endl;
    return agent_contains;
}
void add_agent(QSqlQueryModel& model, QString schema,std::string agentType, double agentWidth,
               double agentLength, bool diag=true)
{
// Add agents to database lookup table
    QString qs, q1, q2, q3;
    q1 = "INSERT INTO "+schema+".lookup_type (agent_type,width,length) VALUES ( '";
    q2 = QString::fromStdString(agentType);
    q3 = QStringLiteral("', %1, %2)").arg(agentWidth).arg(agentLength);
    qs = q1 + q2 + q3;
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#add_agent# " << model.lastError()<<"\033[0m";
    if(diag) qDebug() << "#add_agent# Database updated:"  << qs;
}
void update_agent_position(QSqlQueryModel& model, QString schema,std::string agentType,
                           double x1, double y1,double agLengt, double agWidth, bool diag=true)
{
// Update agent position
    double x2, y2;
    x2 = x1 + agLengt;
    y2 = y1 + agWidth;
    QString qs, q1, q2, q3, q4, qAT;
    q1 = "INSERT INTO "+schema+".status (";
    q2 = "agent_type, geom) VALUES ('";
    qAT =QString::fromStdString(agentType);
    q3 = "',ST_GeomFromText('POLYGON((";
    q4 = QStringLiteral(" %1 %2, %3 %4, %5 %6, %7 %8, %1 %2))'))").
         arg(x1).arg(y1).arg(x2).arg(y1).arg(x2).arg(y2).arg(x1).arg(y2);
    qs = q1 + q2 + qAT + q3 + q4;
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#update_agent_position# " << model.lastError()<<"\033[0m";
    if (diag)
        qDebug() << "#update_agent_position# Agents updated:"  << qs;
}
std::tuple<double, double> rotate_coords(double x, double y, double yaw, bool diag=true)
{
// Rotation matrix fuinction, returns x' and y' given x, y, yaw(rads)
    double xd = x * cos(yaw) - y * sin(yaw);
    double yd = x * sin(yaw) + y * cos(yaw);
    if(diag)
        qDebug() << "#rotate_coords# coordinates rotated";
    return std::make_tuple(xd,yd);
}
void update_agent_position_batch(QSqlQueryModel& model, std::string batch_string,
                                 bool diag=true)
{
// Update agent position - BATCH VERSION
// INSERT INTO db (ID,NAME) VALUES (4, 'Mark'), (5, 'David'), (6, 'Ken') etc...
    // Receive batch string from AG
    QString qs =QString::fromStdString(batch_string);
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#update_agent_position_batch# " << model.lastError()<<"\033[0m";
    if(diag)
        qDebug() << "#update_agent_position_batch# batch update";
}
void update_agent_shape(QSqlQueryModel& model, QString schema,int ag_id, int ag_typ,
                        double sim_time,double ag_len, double ag_wid, double posX,
                        double posY, double yaw_deg, bool diag=true)
{
// Create shapes from position, type and orientation
    //yaw to radians
    double rads = yaw_deg * 3.141592653 / 180;

    // calculate shape coordiantes from posX/Y and yaw
    double x1= -1. * ag_len/2., x4 = -1. * ag_len/2.;
    double x2= +1. * ag_len/2., x3 = +1. * ag_len/2.;
    double y1= -1. * ag_wid/2., y2 = -1. * ag_wid/2.;
    double y3= +1. * ag_wid/2., y4 = +1. * ag_wid/2.;

    //use rotationmatrix to get rotated coordinates
    double x1r, y1r, x2r, y2r, x3r, y3r, x4r, y4r;
    std::tie(x1r, y1r) = rotate_coords(x1, y1, rads, false); //unpack tuple elements
    std::tie(x2r, y2r) = rotate_coords(x2, y2, rads, false); //unpack tuple elements
    std::tie(x3r, y3r) = rotate_coords(x3, y3, rads, false); //unpack tuple elements
    std::tie(x4r, y4r) = rotate_coords(x4, y4, rads, false); //unpack tuple elements
    if(diag){
        qDebug() << "#update_agent_shape# x1r" << x1r << "y1r" << y1r;
        qDebug() << "#update_agent_shape# x2r" << x2r << "y2r" << y2r;
        qDebug() << "#update_agent_shape# x3r" << x3r << "y3r" << y3r;
        qDebug() << "#update_agent_shape# x4r" << x4r << "y4r" << y4r;
    }

    // Add the original position to the coordinates to transform back to mapp coordinates
    x1=x1r+posX; x2=x2r+posX; x3=x3r+posX; x4=x4r+posX;
    y1=y1r+posY; y2=y2r+posY; y3=y3r+posY; y4=y4r+posY;
    if(diag){
        qDebug() << "#update_agent_shape# transforming offset" ;
        qDebug() << "#update_agent_shape# x1" << x1 << "y1" << y1;
        qDebug() << "#update_agent_shape# x2" << x2 << "y2" << y2;
        qDebug() << "#update_agent_shape# x3" << x3 << "y3" << y3;
        qDebug() << "#update_agent_shape# x4" << x4 << "y4" << y4;
    }

    // build string
    QString qs, q1, q2, q3, q4, qAT;
    q1 = "INSERT INTO "+schema+".status (";
    q2 = "agent_id, agent_type, sim_time, geom) VALUES (";
    qAT =QStringLiteral("%1, %2, %3").arg(ag_id).arg(ag_typ).arg(sim_time);
    q3 = ",ST_GeomFromText('POLYGON((";
    q4 = QStringLiteral(" %1 %2, %3 %4, %5 %6, %7 %8, %1 %2))'))").
         arg(x1).arg(y1).arg(x2).arg(y2).arg(x3).arg(y3).arg(x4).arg(y4);
    qs = q1 + q2 + qAT + q3 + q4;
    if(diag)
        qDebug() << "#update_agent_shape# QString: " << qs;

    // send string
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#update_agent_shape# " << model.lastError()<<"\033[0m";
    if(diag)
        qDebug() << "#update_agent_shape# agent shape update";
}
double * return_coords(double ag_len, double ag_wid, double posX, double posY, double yaw,
                       bool rad_format=true, bool diag=true, double scale = 1)
{
//Get rotated coords from agent pos and shape
    static double return_coordiantes[10];

    //yaw to radians conversion if necessary
    double rads;
    if(rad_format){
        rads = yaw; //no conversion required from UE4
    }else{
        rads = yaw * 3.141592653 / 180;
    }

    // calculate shape coordiantes from posX/Y and yaw
    double x1= -1. * scale * ag_len/2., x4 = -1. * scale * ag_len/2.;
    double x2= +1. * scale * ag_len/2., x3 = +1. * scale * ag_len/2.;
    double y1= -1. * scale * ag_wid/2., y2 = -1. * scale * ag_wid/2.;
    double y3= +1. * scale * ag_wid/2., y4 = +1. * scale * ag_wid/2.;

    //use rotationmatrix to get rotated coordinates
    double x1r, y1r, x2r, y2r, x3r, y3r, x4r, y4r;
    std::tie(x1r, y1r) = rotate_coords(x1, y1, rads, false); //unpack tuple elements
    std::tie(x2r, y2r) = rotate_coords(x2, y2, rads, false); //unpack tuple elements
    std::tie(x3r, y3r) = rotate_coords(x3, y3, rads, false); //unpack tuple elements
    std::tie(x4r, y4r) = rotate_coords(x4, y4, rads, false); //unpack tuple elements
    if(diag){
        qDebug() << "#update_agent_shape# x1r" << x1r << "y1r" << y1r;
        qDebug() << "#update_agent_shape# x2r" << x2r << "y2r" << y2r;
        qDebug() << "#update_agent_shape# x3r" << x3r << "y3r" << y3r;
        qDebug() << "#update_agent_shape# x4r" << x4r << "y4r" << y4r;
    }

    // Add the original position to the coordinates to transform back to map coordinates
    x1=x1r+posX; x2=x2r+posX; x3=x3r+posX; x4=x4r+posX;
    y1=y1r+posY; y2=y2r+posY; y3=y3r+posY; y4=y4r+posY;

    return_coordiantes[0] = x1;
    return_coordiantes[1] = x2;
    return_coordiantes[2] = x3;
    return_coordiantes[3] = x4;
    return_coordiantes[4] = y1;
    return_coordiantes[5] = y2;
    return_coordiantes[6] = y3;
    return_coordiantes[7] = y4;

    return return_coordiantes;
}
void draw_geom(QSqlQueryModel& model, QString schema, QString zone, int agentID,
               int agentsTypeNo, double SimTime, double * geo, int SRID = 4326,
               bool verbose=false)
{
    // dump new zone coords in a table
    double gx1=geo[0],gx2=geo[1],gx3=geo[2],gx4=geo[3],gy1=geo[4],gy2=geo[5],gy3=geo[6],gy4=geo[7];

    //int SRID = 4326;
    QString qs, q1, q2, q3, q4, qAT;
    q1 = "INSERT INTO "+schema+"."+zone+" (";
    q2 = "agent_id, agent_type, sim_time, geom) VALUES (";
    qAT =QStringLiteral("%1, %2, %3").arg(agentID).arg(agentsTypeNo).arg(SimTime);
    q3 = ", ST_GeomFromText('POLYGON((";
    q4 = QStringLiteral(" %1 %2, %3 %4, %5 %6, %7 %8, %1 %2))', %9))").
                    arg(gx1,10,'f',7).arg(gy1,10,'f',7).arg(gx2,10,'f',7).arg(gy2,10,'f',7).
                    arg(gx3,10,'f',7).arg(gy3,10,'f',7).arg(gx4,10,'f',7).arg(gy4,10,'f',7).arg(SRID);
    qs = q1 + q2 + qAT + q3 + q4;
    // send string
    if(verbose) qDebug() << "#update_agent_shape# QString: " << qs;
    model.setQuery(qs);
    if (model.lastError().isValid())
            qDebug() << "\033[0;31m#update_agent_shape# " << model.lastError()<<"\033[0m";
    if(verbose) qDebug() << "#update_agent_shape# agent shape update";
}
double * carla_to_geo(double x1,double x2,double x3,double x4,double y1,double y2,double y3,
                      double y4,double m_per_deg_lon,double m_per_deg_lat,
                      double minlon, double minlat)
{
//std::tuple<float,float,float,float,float,float,float,float> carla_to_geo(float x1,float x2,float x3,float x4,float y1,float y2,float y3,float y4,float m_per_deg_lon,float m_per_deg_lat, float minlon, float minlat)
    static double return_coordiantes[10];
    x1 = x1/m_per_deg_lon + minlon; y1 = y1/m_per_deg_lat + minlat;
    x2 = x2/m_per_deg_lon + minlon; y2 = y2/m_per_deg_lat + minlat;
    x3 = x3/m_per_deg_lon + minlon; y3 = y3/m_per_deg_lat + minlat;
    x4 = x4/m_per_deg_lon + minlon; y4 = y4/m_per_deg_lat + minlat;

    return_coordiantes[0] = x1;
    return_coordiantes[1] = x2;
    return_coordiantes[2] = x3;
    return_coordiantes[3] = x4;
    return_coordiantes[4] = y1;
    return_coordiantes[5] = y2;
    return_coordiantes[6] = y3;
    return_coordiantes[7] = y4;

    return return_coordiantes;
}
double * zone_coords(double braking_distance, double velocity, double ag_len,
            double ag_wid, double posX, double posY, double yaw, QString zone_type,
            bool rad_format=false, bool diag=true, double scale=1)
{
//Get rotated coords from agent pos and shape
    static double return_coordiantes[10];

    //yaw to radians conversion if necessary
    double rads;
    if(rad_format){
        rads = yaw; //no conversion required
    }else{
        rads = yaw * 3.141592653 / 180;
    }

    // front of the vehicle are x/y 2/3

    // calculate shape coordiantes from posX/Y realtive to (0,0) at the centre
    double x1= -1. * ag_len*scale/2., x4 = -1. * ag_len*scale/2.;
    double x2= +1. * ag_len*scale/2., x3 = +1. * ag_len*scale/2.;
    double y1= -1. * ag_wid*scale/2., y2 = -1. * ag_wid*scale/2.;
    double y3= +1. * ag_wid*scale/2., y4 = +1. * ag_wid*scale/2.;

    //use rotationmatrix to get rotated coordinates
    double x1r, y1r, x2r, y2r, x3r, y3r, x4r, y4r;
    std::tie(x1r, y1r) = rotate_coords(x1, y1, rads, false); //unpack tuple elements
    std::tie(x2r, y2r) = rotate_coords(x2, y2, rads, false); //unpack tuple elements
    std::tie(x3r, y3r) = rotate_coords(x3, y3, rads, false); //unpack tuple elements
    std::tie(x4r, y4r) = rotate_coords(x4, y4, rads, false); //unpack tuple elements
    if(diag){
        qDebug() << "#update_agent_shape# x1r" << x1r << "y1r" << y1r;
        qDebug() << "#update_agent_shape# x2r" << x2r << "y2r" << y2r;
        qDebug() << "#update_agent_shape# x3r" << x3r << "y3r" << y3r;
        qDebug() << "#update_agent_shape# x4r" << x4r << "y4r" << y4r;
    }

    // add the zone
    double Zx1=0, Zx4=0; //rear of vehicle in x
    double Zx2=0, Zx3=0; //front of vehicle in x
    double Zy1=0, Zy4=0; //rear of vehicle in y
    double Zy3=0, Zy2=0; //front of vehicle in y

    // Adjust the zone start locations to the end of previous
    if (zone_type=="braking"){
        //zone_start = end of vehcile shape
        Zx1=x2r;
        Zy1=y2r;
        Zx4=x3r;
        Zy4=y3r;
        //zone_end = zone_start + braking_distance
        Zx2 = x2r + (braking_distance*scale * cos(rads));
        Zy2 = y2r + (braking_distance*scale * sin(rads));
        Zx3 = x3r + (braking_distance*scale * cos(rads));
        Zy3 = y3r + (braking_distance*scale * sin(rads));
    }
    if(zone_type=="thinking"){
        //zone_start = end of the braking zone
        Zx1=x2r + (braking_distance*scale * cos(rads));
        Zy1=y2r + (braking_distance*scale * sin(rads));
        Zx4=x3r + (braking_distance*scale * cos(rads));
        Zy4=y3r + (braking_distance*scale * sin(rads));
        //zone_end = zone_start + velocity * precondition_time
        Zx2 = Zx1 + (braking_distance*scale * cos(rads));
        Zy2 = Zy1 + (braking_distance*scale * sin(rads));
        Zx3 = Zx4 + (braking_distance*scale * cos(rads));
        Zy3 = Zy4 + (braking_distance*scale * sin(rads));
    }

    // Add the original position to the coordinates to transform back to map coordinates
    x1=Zx1+posX; x2=Zx2+posX; x3=Zx3+posX; x4=Zx4+posX;
    y1=Zy1+posY; y2=Zy2+posY; y3=Zy3+posY; y4=Zy4+posY;

    return_coordiantes[0] = x1;
    return_coordiantes[1] = x2;
    return_coordiantes[2] = x3;
    return_coordiantes[3] = x4;
    return_coordiantes[4] = y1;
    return_coordiantes[5] = y2;
    return_coordiantes[6] = y3;
    return_coordiantes[7] = y4;

    return return_coordiantes;
}
void update_agent_shape_batch(QSqlQueryModel& model, QString schema, int ag_id[],
                              int ag_typ[],
                              double sim_time, int nA,double ag_len[],
                              double ag_wid[], double posX[], double posY[],
                              double yaw_deg[], bool diag=true)
{
    //call the coordinates
// Update agent position - BATCH VERSION
// INSERT INTO db (ID,NAME) VALUES (4, 'Mark'), (5, 'David'), (6, 'Ken') etc...
    double *rc;
    rc = return_coords(ag_len[0], ag_wid[0], posX[0], posY[0], yaw_deg[0],diag);
    double x1=rc[0],x2=rc[1],x3=rc[2],x4=rc[3],y1=rc[4],y2=rc[5],y3=rc[6],y4=rc[7];

    // build string
    QString qs, q1, q2, q3, q4, qEnd, dummy;
    q1 = "INSERT INTO "+schema+".status (";
    q2 = "agent_id, agent_type, sim_time, geom) VALUES ";
    q3 = QStringLiteral("(%1, %2, %3, ST_GeomFromText('POLYGON((").arg(ag_id[0]).arg(ag_typ[0]).arg(sim_time);
    q4 = QStringLiteral(" %1 %2, %3 %4, %5 %6, %7 %8, %1 %2))'))").
         arg(x1).arg(y1).arg(x2).arg(y2).arg(x3).arg(y3).arg(x4).arg(y4);
    qs = q1 + q2 + q3 + q4;
    //start loop here for compunt string
    if(nA>1){
        for(int i=1;i<nA;i++){
            rc = return_coords(ag_len[i], ag_wid[i], posX[i], posY[i], yaw_deg[i],diag);
            double x1=rc[0],x2=rc[1],x3=rc[2],x4=rc[3],y1=rc[4],y2=rc[5],y3=rc[6],y4=rc[7];
            dummy = QStringLiteral(", (%1, %2, %3, ST_GeomFromText('POLYGON(( %4 %5, %6 %7, %8 %9, %10 %11, %4 %5))'))")
            .arg(ag_id[i]).arg(ag_typ[i]).arg(sim_time).arg(x1).arg(y1).arg(x2).arg(y2).arg(x3).arg(y3).arg(x4).arg(y4);
           qs = qs + dummy;
        }
    }
//    qEnd = ")";
//    qs = qs + qEnd;
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#update_agent_shape_batch# " << model.lastError()<<"\033[0m";
    if(diag)
        qDebug() << "#update_agent_shape_batch# " << qs;
}
void update_agent_shape_batch_ID(QSqlQueryModel& model, QString schema, int ag_id[],
    unsigned long ag_typ[], double sim_time, unsigned long nA,double ag_len[],
                                 double ag_wid[],
                                 double posX[], double posY[], double yaw_deg[],
                                 std::vector<double>& agWidth_lookup,
                                 std::vector<double>& agLengt_lookup, bool diag=true)
{
// updated to include width/length selection from enum list
    //Update the width/length based on the agent ID
    if(nA>1){
        for(unsigned long i=0;i<nA;i++){
            ag_wid[i] = agWidth_lookup[ag_typ[i]];
            ag_len[i] = agLengt_lookup[ag_typ[i]];
            //std::cout <<"ag_typ[i]="<<ag_typ[i]<<" ag_wid[i]="<<ag_wid[i]<<" ag_len[i]"<<ag_len[i];
        }
    }

    //call the coordinates
    double *rc;
    rc = return_coords(ag_len[0], ag_wid[0], posX[0], posY[0], yaw_deg[0],false); //edit diag
    double x1=rc[0],x2=rc[1],x3=rc[2],x4=rc[3],y1=rc[4],y2=rc[5],y3=rc[6],y4=rc[7];

    // build string
    QString qs, q1, q2, q3, q4, qEnd, dummy;
    q1 = "INSERT INTO "+schema+".status (";
    q2 = "agent_id, agent_type, sim_time, geom) VALUES ";
    q3 = QStringLiteral("(%1, %2, %3, ST_GeomFromText('POLYGON((").arg(ag_id[0]).arg(ag_typ[0]).arg(sim_time);
    q4 = QStringLiteral(" %1 %2, %3 %4, %5 %6, %7 %8, %1 %2))'))").
         arg(x1).arg(y1).arg(x2).arg(y2).arg(x3).arg(y3).arg(x4).arg(y4);
    qs = q1 + q2 + q3 + q4;
    //start loop here for compunt string
    if(nA>1){
        for(unsigned long i=1;i<nA;i++){
            rc = return_coords(ag_len[i], ag_wid[i], posX[i], posY[i], yaw_deg[i],false); //edit diag
            double x1=rc[0],x2=rc[1],x3=rc[2],x4=rc[3],y1=rc[4],y2=rc[5],y3=rc[6],y4=rc[7];
            dummy = QStringLiteral(", (%1, %2, %3, ST_GeomFromText('POLYGON(( %4 %5, %6 %7, %8 %9, %10 %11, %4 %5))'))")
            .arg(ag_id[i]).arg(ag_typ[i]).arg(sim_time).arg(x1).arg(y1).arg(x2).arg(y2).arg(x3).arg(y3).arg(x4).arg(y4);
           qs = qs + dummy;
        }
    }
//    qEnd = ")";
//    qs = qs + qEnd;
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#update_agent_shape_batch_ID# " << model.lastError()<<"\033[0m";
    if(diag)
        qDebug() << "#update_agent_shape_batch_ID# " << qs;
}
void update_test_batch(QSqlQueryModel& model, QString schema, double sim_time,
                       std::vector<double>& agWidth_lookup,std::vector<double>& agLengt_lookup,
                       unsigned long nA = 10, bool diag=true)
{
//batch mode string updater - set number of random agents nA
    std::string agType_lookup[5] = {"AV","car","ped","hgv","cyclist"};  // to access use agent_type[0] etc.
    //double agWidth_lookup [5] = {2.0,2.0,0.5,3.5,0.5};
    //double agLengt_lookup [5] = {4.0,4.0,0.5,6.5,1.5};

    for(int k=0;k<1;k++){
        unsigned long loops = 1;
        double sum_time=0;

        for(unsigned long j=0;j<loops;j++){
            //double sim_time = 0.0;
            //int nA = 10;            //*** SET nA=1 for single write test ***

            //qDebug() << "Looping for " << array_size << " iterations" << endl;
            int* ag_id      = new int[nA];
            unsigned long* ag_typ     = new unsigned long[nA];
            double* ag_len  = new double[nA];
            double* ag_wid  = new double[nA];
            double* posX    = new double[nA];
            double* posY    = new double[nA];
            double* yaw_deg = new double[nA];

            // create array of random numbers
            for(unsigned long i=0; i<nA; ++i) {
                ag_id[i]	 = ( std::rand()  ); //% 1000000
                ag_typ[i]	 = ( std::rand() % 4 ) + 1; //ag_typ=0 reserved for AV
                ag_len[i]	 = ( std::rand() % 4 );
                ag_wid[i]	 = ( std::rand() % 2 );
                posX[i]		 = ( std::rand() % 20 );
                posY[i]		 = ( std::rand() % 20 );
                yaw_deg[i]	 = ( std::rand() % 180 );
            }
            timestamp_t t0 = get_timestamp();
            //update_agent_shape_batch

            update_agent_shape_batch_ID(model, schema, ag_id, ag_typ, sim_time, nA, ag_len, ag_wid,
                                        posX, posY, yaw_deg, agWidth_lookup, agLengt_lookup, diag);
            // stop timer
            timestamp_t t1 = get_timestamp();
            // print time results and agent count
            double secs = (t1 - t0) / 1000000.0;
            //qDebug() << "Time taken (s) = " << secs << endl;
            //qDebug() << "batch execution time for loop#" << j << " with "<< nA << " agents is " << secs ;
            //qDebug() << secs ;
            sum_time += secs;
        }
        if(diag) qDebug() << "average time is" << sum_time/(loops * nA);
        //    qDebug() << sum_time/(loops * nA);
    }
}
class AgentConfig
{
    // Use the index to access agent types, modify the agent_config.txt to change
    // or modify values
    // INDEX 0=AV, 1=Car, 2=Ped, 3=hgv, 4=cyclist

public: std::vector<std::string> vect_typ;
        std::vector<double> vect_wid, vect_len, vect_amax;
        std::string cfg_typ;
        std::string cfg_wid;
        std::string cfg_len;
        std::string cfg_amax;
        std::string header;

        void read_file(std::ifstream& myfile, bool diag=true)
        {
            std::cout << "\n*********************"<< std::endl;
            std::cout << " Reading Config File "<< std::endl;
            std::cout << "*********************"<< std::endl;

            if (myfile.is_open())
            {
                getline(myfile,header);
                getline(myfile,cfg_typ);
                getline(myfile,cfg_wid);
                getline(myfile,cfg_len);
                getline(myfile,cfg_amax);
            } else {
                qDebug() << "\033[0;31m#AgentConfig.read_file# ERROR: Config File can't be opened! Ensure the file is in the BUILD directory\n" <<"\033[0m";
            }

            if(diag) std::cout << "#AgentConfig.read_file#" << std::endl;
            if(diag) std::cout << header  << std::endl;
            if(diag) std::cout << cfg_typ << std::endl;
            if(diag) std::cout << cfg_wid << std::endl;
            if(diag) std::cout << cfg_len << std::endl;
            if(diag) std::cout << cfg_amax << std::endl;

            std::stringstream ss_typ(cfg_typ), ss_wid(cfg_wid), ss_len(cfg_len), ss_amax(cfg_amax);
            std::string token;

            //Parse the type string
            while(std::getline(ss_typ, token, ',')) {
                //std::cout << "while token: " << token << '\n';
                vect_typ.push_back(token);
                if (ss_typ.peek() == ',')
                    ss_typ.ignore();
            }
            {
                for (unsigned i=0; i< vect_typ.size(); i++)
                    std::cout <<"config for "<< vect_typ.at(i)<<" updated..."<<std::endl;
            }

            double kk;
            //Parse the width string
            while (ss_wid >> kk)
            {
                vect_wid.push_back(kk);
                if (ss_wid.peek() == ',')
                    ss_wid.ignore();
            }
            if(diag){
                for (unsigned i=0; i< vect_wid.size(); i++)
                    if(diag) std::cout << vect_wid.at(i)<<std::endl;
            }

            // Parse the length string
            while (ss_len >> kk)
            {
                vect_len.push_back(kk);
                if (ss_len.peek() == ',')
                    ss_len.ignore();
            }
            if(diag){
                for (unsigned i=0; i< vect_len.size(); i++)
                    if(diag) std::cout << vect_len.at(i)<<std::endl;
            }


            // Parse the a_max string
            while (ss_amax >> kk)
            {
                vect_amax.push_back(kk);
                if (ss_amax.peek() == ',')
                    ss_amax.ignore();
            }
            if(diag){
                for (unsigned i=0; i< vect_amax.size(); i++)
                    if(diag) std::cout << vect_amax.at(i)<<std::endl;
            }
        }
};


//*********************************************************************
//
//                         M    A    I    N
//
//*********************************************************************

int main()
{

    // Set the diagnotics level for the terminal

    bool verbose_check  = true;
    bool verbose = false;
    bool diag = false;
    bool genDynamicShapes = true;

    double pi = 3.14159265359;


    std::cout << "**************************" << std::endl;
    std::cout << "*   databse connection   *" << std::endl;
    std::cout << "**************************\n" << std::endl;


    //Connect to the database options: <greg, severin>
    char userID[] = "greg";
    //char userID[] = "severin";
    //char userID[] = "dockerTest";

    QSqlDatabase db = db_connect(userID);
    QSqlQueryModel model; //create model for queries

    //create database tables
    QString schema="sim_log";
    db_tables(model, schema, true);

    //Open agent config file
    //Ensure the file is in the local BUILD directory
    std::ifstream myfile ("agent_config.txt");

    //class for reading config
    std::cout << "Starting agent config...\n";
    AgentConfig cfg;
    cfg.read_file(myfile, false);
    std::vector<std::string> vect_typ= cfg.vect_typ;
    std::vector<double> vect_wid= cfg.vect_wid;
    std::vector<double> vect_len= cfg.vect_len;
    std::vector<double> vect_amax= cfg.vect_amax;


    // check config is being read correctly
    if(verbose)for (unsigned i=0; i< vect_len.size(); i++){
        std::cout << vect_len.at(i)<<std::endl;
        std::cout << vect_amax.at(i)<<std::endl;
    }


    // ~~~ Initiate map and static data
    initMap(model, schema, diag);


// ~~~~~~~~~~~~~~~~~~~~~~~ hard-coded values ~~~~~~~~~~~~~~~~~~~~~
//    If you want to use hard-coded values for testing...
//    //generated agents: AV, car, ped, cyclist
//    int nS = 4;
//    std::vector<std::string> agentsTypeList = {"AV","car","pedestrian","cyclist"};
//    double agentsListLen[4] = {vect_len[0],vect_len[0],vect_len[2],vect_len[4]};
//    double agentsListWid[4] = {vect_wid[0],vect_wid[0],vect_wid[2],vect_wid[4]};
//    double agentPosLon[4] = {-2.6045037,
//                             -2.6047037,
//                             -2.6046007,  //ped
//                             -2.6048037}; //cycl
//    double agentPosLat[4] = {51.4553431,
//                             51.4553931,
//                             51.4553431,  //ped
//                             51.4554031}; //cycl
//    int agentYawList[4] = {-45,-17,0,-17};
//    loop = agentsTypeList.size();
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



// ~~~~~~~~~~~~~~~~~~~~~~~~~ sim-log values ~~~~~~~~~~~~~~~~~~~~~~

    //If reading from the sim log
    unsigned long loop;
    readSimLog testLog;
    std::string line;
    //testLog.read_file(simLogFile, true); //for UE4 log
//    std::ifstream simLogFile ("TEST004_short.txt");testLog.read_file_carla
//    (simLogFile, true); bool useCyclicTime = false;//for carla logs
//    std::ifstream simLogFile ("logging_example_v2.txt");testLog.read_file_testbench
//    (simLogFile, true); bool useCyclicTime = true;//for carla testbench
//    std::ifstream simLogFile ("lboro_static_tests_short.txt");testLog.read_file_testbench
//    (simLogFile, true); bool useCyclicTime = true; bool rad_format=false;//for carla logs
    std::ifstream simLogFile ("assertion_case_study/sim_log/all_data.txt");
    testLog.read_file_testbench (simLogFile, true); bool useCyclicTime = true;
    bool rad_format=true;//for AItest21 paper

    //if(verbose) std::cout<< "Agent "<<testLog.my_records[2].agentID<<" at time "
//    <<testLog.my_records[2].simTime<<" is at XY " <<
    //            testLog.my_records[2].simX<<" "<<testLog.my_records[2].simY<<std::endl;

    // Add this to the log file to generate marks tht ensure the pgAdmin view stays centrered
    //1, 1, 0,  marker1,		2, 0, 20, 1200, 1627, 0, 0
    //1, 1, 1,  marker2,		2, 0, 20, 1200, 1733, 0, 0
    //1, 1, 2,  marker3,		2, 0, 20, 1250, 1627, 0, 0
    //1, 1, 3,  marker4,		2, 0, 20, 1250, 1723, 0, 0

    //convert sim coord to lat/long
    unsigned long nS = testLog.fileLineCount;
    std::cout << "number of file lines in sim log ..." << nS <<std::endl;

    int* agentsListID      = new int[nS];
    std::vector<std::string> agentsListType;
    int* agentListNo       = new int[nS];
    int* agentsListTypeNo  = new int[nS];
    double* agentsSimTime  = new double[nS];
    double* agentsListLen  = new double[nS];
    double* agentsListWid  = new double[nS];
    double* agentPosY      = new double[nS];
    double* agentPosX      = new double[nS];
    double* agentYawList   = new double[nS];
    double* agentSpeed     = new double[nS];

    for(unsigned long i=0;i<nS;i++){

        unsigned long curr_ID;
        curr_ID = testLog.my_records[i].agentTypeNo;
        agentListNo[i] = testLog.my_records[i].agentNo;
        agentsListID[i] = testLog.my_records[i].agentID;
        agentsListType.push_back(testLog.my_records[i].agentType);
        agentsListTypeNo[i] = testLog.my_records[i].agentTypeNo;
        if (not useCyclicTime){
            agentsSimTime[i] = testLog.my_records[i].simTime;
            //**** if cyclic time gives primary key errors, use wall-clock
            if(i==0)qDebug("Using standard time for sim data");
        }
        if(useCyclicTime){
            agentsSimTime[i] = testLog.my_records[i].wallClock * 100; //x100 to include decimal in PK
            if(i==0)qDebug("Using cyclical time for sim data");
        }
        agentPosX[i] = testLog.my_records[i].simX;
        //agentPosY[i] = testLog.my_records[i].simY;
        agentPosY[i] = -1 * testLog.my_records[i].simY;     //NB y-coordiante is revered here
        agentYawList[i] = -1 * testLog.my_records[i].simYaw;   //flipping left to RH euler axis
//        agentYawList[i] = testLog.my_records[i].simYaw;//********************************
        agentSpeed[i]   = testLog.my_records[i].speed;

        agentsListLen[i] = vect_len[curr_ID];
        agentsListWid[i] = vect_wid[curr_ID];

        //if(verbose and i<5){
        if(verbose_check and i<5){
            qDebug() << "curr_ID " << curr_ID;
            std::cout << "agentsListID " << agentsListID[i] << std::endl;
            std::cout << "agentsListType " << agentsListType[i]<< std::endl;
            qDebug() << "agentsListTypeNo " << agentsListTypeNo[i];
            qDebug() << "agentsSimTime " << agentsSimTime[i];

            qDebug() << "agentPosX " << agentPosX[i];
            qDebug() << "agentPosY " << agentPosY[i];
            qDebug() << "vect_len " << vect_len;
            qDebug() << "vect_len[0] " << vect_len[0];
            qDebug() << "vect_len[curr_ID] " << vect_len[curr_ID];
            qDebug() << "testLog.my_records[i].simYaw " << testLog.my_records[i].simYaw;
            qDebug() << "agentYawList[i] " << agentYawList[i];
         }
    }
    loop = nS;
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



// ~~~~~~~~~~~~~~~~~~~~ add sim data to database ~~~~~~~~~~~~~~~~~~

    std::cout << "\n*********************"<< std::endl;
    std::cout << " Sim Data Upload"<< std::endl;
    std::cout << "*********************"<< std::endl;

    for (unsigned long sc=0;sc<loop;sc++)
    {
        if(verbose) std::cout << "looping sc=" <<  sc << std::endl;

        //Map Integration and offset management
        //-2.6061375,51.4561091 position of clifton triangle junction
        // need to set SRID
        // need to give agents offset wrt map locale
        // must be a reference in the map file that we can use to give the correct offset?

        // from the triangle.OSM map we find:
        //double minlon = -2.6079546,minlat = 51.4566831;
        //double minlon = -2.6065037, minlat = 51.4560631;

        //double minlon = -2.500172, minlat = 51.489233; //For clifton triangle
//        double minlon = -2.499825, minlat = 51.489065; //for Double Mini_Roundabout
//        double minlon = -2.499825, minlat = 51.489030; //for testB

      //double minlon = -2.512869, minlat = 51.485902; //for overtaking assertion downend road
        double minlon = -2.512495, minlat = 51.486073; //for overtaking assertion downend road

        //get rotated coordinates
        double *rc;
        if(verbose){
            qDebug() << " agentsListLen = "     << agentsListLen[sc] <<
                        "\n agentsListWid = "   << agentsListWid[sc] <<
                        "\n agentPosX = "       << agentPosX[sc] <<
                        "\n agentPosX = "       << agentPosX[sc] <<
                        "\n agentPosX = "       << agentPosX[sc] <<
                        "\n agentPosY = "       << agentPosY[sc] <<
                        "\n agentYawList = "    << agentYawList[sc];
        }

        double scale = 1; //drawing scale for geom
        rc = return_coords(agentsListLen[sc], agentsListWid[sc], agentPosX[sc],
                           agentPosY[sc], agentYawList[sc], rad_format, diag, scale);
        double x1=rc[0],x2=rc[1],x3=rc[2],x4=rc[3],y1=rc[4],y2=rc[5],y3=rc[6],y4=rc[7];

        if(verbose_check and sc<1){
            qDebug() << "#Sim Data Upload# x1" << x1 << "y1" << y1;
            qDebug() << "#Sim Data Upload# x2" << x2 << "y2" << y2;
            qDebug() << "#Sim Data Upload# x3" << x3 << "y3" << y3;
            qDebug() << "#Sim Data Upload# x4" << x4 << "y4" << y4;
        }

        //Note x is longitude and y is latitude for the postGIS functions that use lat/long directly

        double latMid = 51.582838; //use this as rough approximation but could do
//        better for big maps >60 miles
        double m_per_deg_lat = 111132.92 - 559.82*cos(2*latMid*pi/180) +
                1.175*cos(4*latMid*pi/180) - 0.0023*cos(6*latMid*pi/180);
        double m_per_deg_lon = 111412.84*cos(latMid*pi/180) - 93.5*cos(3*latMid*pi/180) +
                0.118*cos(5*latMid*pi/180);
        if(verbose) std::cout << "m_per_deg_lat = " << m_per_deg_lat <<"\n";
        if(verbose) std::cout << "m_per_deg_lon = " << m_per_deg_lon <<"\n";

        //convert and offset the lat & lon
        x1 = x1/m_per_deg_lon + minlon; y1 = y1/m_per_deg_lat + minlat;
        x2 = x2/m_per_deg_lon + minlon; y2 = y2/m_per_deg_lat + minlat;
        x3 = x3/m_per_deg_lon + minlon; y3 = y3/m_per_deg_lat + minlat;
        x4 = x4/m_per_deg_lon + minlon; y4 = y4/m_per_deg_lat + minlat;
        if(verbose_check and sc<1){
                qDebug() << "#Sim Data Transform# x1" << x1 << "y1" << y1;
                qDebug() << "#Sim Data Transform# x2" << x2 << "y2" << y2;
                qDebug() << "#Sim Data Transform# x3" << x3 << "y3" << y3;
                qDebug() << "#Sim Data Transform# x4" << x4 << "y4" << y4;
        }

        // build string
        int SRID = 4326;
        //double sim_time=0;
        QString qs, q1, q2, q3, q4, qAT;
        q1 = "INSERT INTO "+schema+".actors (";
        q2 = "agent_id, agent_type, sim_time, geom) VALUES (";
        qAT =QStringLiteral("%1, %2, %3").arg(agentListNo[sc]).
                arg(agentsListTypeNo[sc]).arg(agentsSimTime[sc]); //changed from agentID to agentNo
        q3 = ", ST_GeomFromText('POLYGON((";
        q4 = QStringLiteral(" %1 %2, %3 %4, %5 %6, %7 %8, %1 %2))', %9))").
                        arg(x1,10,'f',7).arg(y1,10,'f',7).arg(x2,10,'f',7).arg(y2,10,'f',7).
                        arg(x3,10,'f',7).arg(y3,10,'f',7).arg(x4,10,'f',7).arg(y4,10,'f',7).
                        arg(SRID);
        qs = q1 + q2 + qAT + q3 + q4;

        // send string
        if(verbose) qDebug() << "#update_agent_shape# QString: " << qs;
        model.setQuery(qs);
        if (model.lastError().isValid())
                qDebug() << "\033[0;31m#update_agent_shape# " << model.lastError()<<"\033[0m";
        if(verbose) qDebug() << "#update_agent_shape# agent shape update";



        // ~~~~~~~~~~~~~~~~~~~~ Dynamic Shape Generation ~~~~~~~~~~~~~~~
        // generate dynamic shapes to help with  assertion checking
        if(genDynamicShapes){
        if(sc<1){
            std::cout << "\n**************************"<< std::endl;
            std::cout << " Dynamic Shape Generation "<< std::endl;
            std::cout << "**************************"<< std::endl;
        }

        // check if vehicle, type? use index of agent_config.txt
        // 0=pod, 1=adult, 2=child, 3=AV, 4=cav, 5=hgv, 6=cyclist, 7=aygo

        // find speed and maximum deceleration, should be ~3 m/s^2
        double maximum_deceleration = vect_amax.at(agentsListTypeNo[sc]);
        double velocity = agentSpeed[sc];
        //double bearing = agentYawList[sc];

        // determine the braking & stopping distance
        double thinking_time = 0.5;
        double braking_distance = pow(velocity,2) / (2 * maximum_deceleration);


        // for each vehicle type, re-draw the polygon with delta-offset
        double *zb;
        QString zone = "braking";
        zb = zone_coords(braking_distance, velocity, agentsListLen[sc], agentsListWid[sc],
                         agentPosX[sc], agentPosY[sc], agentYawList[sc], zone, rad_format,
                         diag, scale);
        double x1=zb[0],x2=zb[1],x3=zb[2],x4=zb[3],y1=zb[4],y2=zb[5],y3=zb[6],y4=zb[7];

        //convert to lat long
        double * geo;
        geo = carla_to_geo(x1,x2,x3,x4,y1,y2,y3,y4,m_per_deg_lon,m_per_deg_lat, minlon, minlat);
        //double gx1=geo[0],gx2=geo[1],gx3=geo[2],gx4=geo[3],gy1=geo[4],gy2=geo[5],gy3=geo[6],gy4=geo[7];

        // draw geometry to table
        draw_geom(model, schema, zone, agentsListID[sc], agentsListTypeNo[sc],
                  agentsSimTime[sc], geo, SRID, verbose);

        if(verbose){
            qDebug() << "*** BRAKING ZONE ***";
            qDebug() << "braking_distance       = " << braking_distance;
            qDebug() << "velocity               = " << velocity;
            qDebug() << "maximum_deceleration   = " << maximum_deceleration;
            qDebug() << "agentsListLen[sc]      = " << agentsListLen[sc];
            qDebug() << "agentsListWid[sc]      = " << agentsListWid[sc];
            qDebug() << "agentPosX[sc]          = " << agentPosX[sc];
            qDebug() << "agentPosY[sc]          = " << agentPosY[sc];
            qDebug() << "agentYawList[sc]       = " << agentYawList[sc];
            qDebug() << "zone                   = " << zone;
            qDebug() << "rad_format             = " << rad_format;
            qDebug() << "diag                   = " << diag;
            qDebug() << "scale                  = " << scale;
            qDebug() << "agentsListID[sc]       = " << agentsListID[sc];
        }

        //----------------------------------------------
        // repeat for precondition zone
        double *zp;
        zone = "thinking";
        zp = zone_coords(braking_distance, velocity, agentsListLen[sc], agentsListWid[sc],
                         agentPosX[sc], agentPosY[sc], agentYawList[sc], zone,
                         rad_format, diag, scale);
        x1=zp[0];x2=zp[1];x3=zp[2];x4=zp[3];y1=zp[4];y2=zp[5];y3=zp[6];y4=zp[7];

        //convert to lat long
        geo = carla_to_geo(x1,x2,x3,x4,y1,y2,y3,y4,m_per_deg_lon,m_per_deg_lat, minlon, minlat);
        //double gx1=geo[0],gx2=geo[1],gx3=geo[2],gx4=geo[3],gy1=geo[4],gy2=geo[5],gy3=geo[6],gy4=geo[7];

        // draw geometry to table
        draw_geom(model, schema, zone, agentsListID[sc], agentsListTypeNo[sc],
                  agentsSimTime[sc], geo, SRID, verbose);

        if(verbose){
            qDebug() << "*** THINKING ZONE ***";
            qDebug() << "braking_distance       = " << braking_distance;
            qDebug() << "velocity               = " << velocity;
            qDebug() << "maximum_deceleration   = " << maximum_deceleration;
            qDebug() << "agentsListLen[sc]      = " << agentsListLen[sc];
            qDebug() << "agentsListWid[sc]      = " << agentsListWid[sc];
            qDebug() << "agentPosX[sc]          = " << agentPosX[sc];
            qDebug() << "agentPosY[sc]          = " << agentPosY[sc];
            qDebug() << "agentYawList[sc]       = " << agentYawList[sc];
            qDebug() << "zone                   = " << zone;
            qDebug() << "rad_format             = " << rad_format;
            qDebug() << "diag                   = " << diag;
            qDebug() << "scale                  = " << scale;
            qDebug() << "agentsListID[sc]       = " << agentsListID[sc];
            }


        //----------------------------------------------
        // Maybe have this as a separate file that is
        // used only if overtaking is detected
        //----------------------------------------------

        // has an asserion zone been detected?
        // assertion triggered based on actor types and situation

        // step through on each time
        double time = 0.00; //may need to pull this from first line of the logs
        // agentsSimTime[sc]

        // find the stopping distance for ego, lead and oncoming
        //double thinking_distance = velocity(at_time_index) * thinking_time;
        //double stopping_distance = thinking_distance + braking_distance;

        double L_EV = 0; //length of the vehicle that is doing the overtaking
        double S_EV = 0; //stopping distance of the vehicle that is doing the overtaking

        double L_LV = 0; //length of the vehicle that is being overtaken
        double S_LV = 0; //stopping distance of the vehicle that is being overtaken

        // find the overtaking distance
        double overtaking_distance = L_EV + S_EV + L_LV + S_LV;


        }


        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~









    }
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// For WP4 Demo - using test003.txt
//select * FROM agents.map_test as g1
//where
//((g1.sim_time = 24.0) and ((g1.agent_id = 100) OR (g1.agent_id = 102)
//    OR (g1.agent_id = 99) OR (g1.agent_id = 146) OR (g1.agent_id = 123)
//OR (g1.agent_id = 111) OR (g1.agent_id = 113)))
//OR ((g1.sim_time = 0) and ((g1.agent_id = 550508837) OR
//    (g1.agent_id = 550508846) OR (g1.agent_id = 551154786)))


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// For MO Demo - testA TEST_A_OUTPUT_Markers.txt
//    Use this and iterate the time to show the vehicles moving
//    set AV to agent_id 74 in assertions and set sd to 0.000007 in the asr_02 call
//    select * FROM agents.map_test as g1 where ((g1.sim_time = 6.0) and
//    ((g1.agent_id = 74) OR (g1.agent_id = 75) OR (g1.agent_id = 76)))
//    OR ((g1.sim_time = 0) and ((g1.agent_id = 0) OR (g1.agent_id = 1) OR
//            (g1.agent_id = 2) OR  (g1.agent_id > 76)))

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// For MO Demo - testC2 TEST_C2_crash_OUTPUT.txt
//    Use this and iterate the time to show the vehicles moving
//    Set AV to agent_id 32,
//    select * FROM agents.map_test as g1 where ((g1.sim_time = 6.0) and
//    ((g1.agent_id = 32) OR (g1.agent_id = 33) OR (g1.agent_id = 34) OR
//            (g1.agent_id = 35) OR (g1.agent_id = 36) OR (g1.agent_id = 37)))
//    OR ((g1.sim_time = 0) and ((g1.agent_id = 0) OR (g1.agent_id = 1)
//    OR (g1.agent_id = 2) OR (g1.agent_id = 3) OR  (g1.agent_id > 37)))


    // ASSERION TESTING ----------------------------------------
    std::cout << "\n*********************"<< std::endl;
    std::cout << "      WP4 DEMO        "<< std::endl;
    std::cout << "*********************"<< std::endl;

//    SELECT g2.agent_id, g2.sim_time, 2 AS asr_id,
//    -- ST_Distance(g1.geom,g2.geom) < 0.00001,
//    ST_Distance(g1.geom::geography,g2.geom::geography),
//    ST_Distance(g1.geom::geography,g2.geom::geography) < 1.5
//    AS asr_result
//    FROM agents.status g1, agents.status g2--, public.dmr_uk_fit p1
//    WHERE g1.agent_id = 100 AND g2.agent_id != 100 AND
//    g1.sim_time = 24.0 AND g2.sim_time = 24.0
//    -- AND g3.sim_time = 24.0




// ~~~~~~~~~~~~~~~~~~~~~~ ASSERION TESTING ~~~~~~~~~~~~~~~~~~~~~
    // ASSERION TESTING ----------------------------------------
    std::cout << "\n*********************"<< std::endl;
    std::cout << "  Assertion Testing  "<< std::endl;
    std::cout << "*********************"<< std::endl;

    // Assertion implementation
    double sim_timer = 0.1; //TODO need to read this from sim log
    double sim_time_max = nS;
    timestamp_t t0 = get_timestamp();

    for(unsigned long time=0;time<sim_time_max;time++) //TODO read the sim log time limits
    {
        if(verbose) std::cout << "sim time is " << sim_timer << std::endl;

        //check assertions
        // need to define/find the target AV ID
        int target_actorID = 116;

        //asr_01(model, schema, sim_timer);
        //use asr_02 for bespoke hazard distance
        double near_miss_dist = 0.5; //distance for geography needs to be in meters
        double collision_dist = 0.05; //distance for geography needs to be in meters
        //asr_02(model, schema, sim_timer, haz_dist, 2, true);
        //distance_based(model,schema, target_actorID, sim_timer, near_miss_dist, 1, diag);
        //distance_based(model,schema, target_actorID, sim_timer, collision_dist, 2, diag);
        distance_based(model,  schema, target_actorID,  sim_timer,
                       near_miss_dist, collision_dist, diag);

        sim_timer = sim_timer + 0.1;
    }

    // stop timer
    timestamp_t t1 = get_timestamp();
    double secs = (t1 - t0) / 1000000.0;
    if(diag) std::cout << "assertion checking time is :" << secs<<"s \n";

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Summary Stats ----------------------------------------
    std::cout << "\n*********************"<< std::endl;
    std::cout << "    Summary Stats    "<< std::endl;
    std::cout << "*********************"<< std::endl;

//      now create summary stats for easy result interpretation
//      select
//      sum(case when near_miss is False then 1 else 0 end) as no_near_miss,
//      sum(case when collision is False then 1 else 0 end) as no_collisions
//      from simdata.assertions

//      TODO maybe add Experiment number to the log file to easily summarise
//      between different types and actors?

//    // Create table for Stats
//    QString sql_string;
//    sql_string = "DROP TABLE IF EXISTS "+schema+".stats";
//    model.setQuery(sql_string);
//    if (model.lastError().isValid())
//        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
//    if(diag) qDebug() << "stats table deleted...";
//    sql_string = "create table "+schema+".stats (exp_no
//    int primary key, hazard_actor varchar(255), orientation float, "
//                 "pod_route varchar(255), grid_posn_w float,
//    no_near_miss int, no_collision int)"; //
//    model.setQuery(sql_string);
//    if (model.lastError().isValid())
//        qDebug() << "\033[0;31m#db_tables# " << model.lastError()<<"\033[0m";
//    if(diag) qDebug() << "#db_tables# assertions table created...";

    // summarise experiment data
    //      sum(case when near_miss is False then 1 else 0 end) as no_near_miss,
    //      sum(case when collision is False then 1 else 0 end) as no_collisions
    //      from simdata.assertions

    // ExpNo - from log
    // HazardActor - from log
    // Orientation - from log
    // PodRoute - make entry
    // GridPosn  - make entry
    // NoNearMiss/Collison - from SQL

//    // build string
//    QString qs, q1, q2, q3;
//    q1 = "INSERT INTO "+schema+".stats (exp_no, hazard_actor,
//    orientation, pod_route, grid_posn_w, no_near_miss, no_collision) SELECT ";
//    q2 = QStringLiteral("1 AS exp_no, 'adult' as hazard_actor, 45 AS orientation, "
//        "'left 45' AS pod_route, -3 AS grid_posn_w, "
//        " sum(case when near_miss is False then 1 else 0 end) as no_near_miss, "
//     "sum(case when collision is False then 1 else 0 end) as no_collisions ");
//    q3 = "FROM "+schema+".assertions ";
//    qs = q1 + q2 + q3 ;
//    model.setQuery(qs);
//    if (model.lastError().isValid())
//        qDebug() << "\033[0;31m#asr_02# " << model.lastError()<<"\033[0m";
//    if(diag)
//        qDebug() << "#asr_02# " << qs;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // TODO - automate file reading and take exp number from the log file entry
    // TODO AG get pod route and grid position from log or make new entry
    // TODO automate the reading batch files
    // TODO change all table deletes to insert updates?

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    std::cout<<"\n*********************"<< std::endl;
    std::cout << "    Live QGis Demo    "<< std::endl;
    std::cout << "*********************"<< std::endl;
    // Run through the sim log with a time step to allow qgis refresh rate to
    // update approx 100ms.
    bool qgisDemo = true;

if (qgisDemo){
    std::cout << "starting QGIS demo..." << std::endl;
    double sim_timer = 0.1;
    double sim_time_max = nS;
    double time_interval = 0.1;
    unsigned int time_interval_ms = static_cast<unsigned int>(time_interval * 1000);
    timestamp_t t0 = get_timestamp();

    //first delete the table contents without dropping
    QString qs = "delete from "+schema+".frame";
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#qgisDemo# "<< model.lastError()<<"\033[0m";

    qs = "delete from "+schema+".frame_braking";
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#qgisDemo# "<< model.lastError()<<"\033[0m";

    qs = "delete from "+schema+".frame_thinking";
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#qgisDemo# "<< model.lastError()<<"\033[0m";

    //convert array to vector
    std::vector<double> v(agentsSimTime, agentsSimTime + nS);
    std::vector<double>::iterator ip;

    //find the unique time values then resize
    ip = std::unique(v.begin(), v.begin() + nS);
    v.resize(std::distance(v.begin(), ip));

    //use the vector of unique times to draw actors
    int count=0;
    for(int i=0; i < v.size(); i++){

    if(verbose){
        std::cout << "element "<< i << " of array = " << agentsSimTime[i*3] << std::endl;
        std::cout << "element "<< i << " of vector = " << v[i] << std::endl;}

    //update the Frame table used for live updating
    QString q1 = "insert into "+schema+".frame (agent_id, agent_type, sim_time, geom) ";
    QString q2 = "SELECT g1.agent_id, g1.agent_type, g1.sim_time, g1.geom ";
    QString q3 = QStringLiteral("FROM sim_log.actors g1 WHERE g1.sim_time = %1 ").arg(v[i]);
    qs = q1 + q2 + q3;
    qDebug() << "#QGIS demo# " << qs;

    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#qgisDemo# "
                 << model.lastError()<<"\033[0m";

    // update the braking frame
    q1 = "insert into "+schema+".frame_braking (agent_id, agent_type, sim_time, geom) ";
    q2 = "SELECT g1.agent_id, g1.agent_type, g1.sim_time, g1.geom ";
    q3 = QStringLiteral("FROM sim_log.braking g1 WHERE g1.sim_time = %1 ").arg(v[i]);
    qs = q1 + q2 + q3; qDebug() << "#QGIS demo frame_braking# " << qs;
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#qgisDemo# "
                 << model.lastError()<<"\033[0m";

    // update the thinking frame
    q1 = "insert into "+schema+".frame_thinking (agent_id, agent_type, sim_time, geom) ";
    q2 = "SELECT g1.agent_id, g1.agent_type, g1.sim_time, g1.geom ";
    q3 = QStringLiteral("FROM sim_log.thinking g1 WHERE g1.sim_time = %1 ").arg(v[i]);
    qs = q1 + q2 + q3; qDebug() << "#QGIS demo frame_thinking# " << qs;
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#qgisDemo# "
                 << model.lastError()<<"\033[0m";

    //update the timer
    usleep(time_interval_ms * 1000);
    sim_timer = sim_timer + time_interval;
    count++;
    if(count%10==0)std::cout << "frame time " << count << std::endl;

    //delete the table contents again?
    QString qs = "delete from "+schema+".frame";
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#qgisDemo# "<< model.lastError()<<"\033[0m";

    qs = "delete from "+schema+".frame_braking";
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#qgisDemo# "<< model.lastError()<<"\033[0m";

    qs = "delete from "+schema+".frame_thinking";
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#qgisDemo# "<< model.lastError()<<"\033[0m";

    }

// stop timer
timestamp_t t1 = get_timestamp();
double secs = (t1 - t0) / 1000000.0;
if(diag) std::cout << "assertion checking time is :" << secs<<"s \n";

}//if (qgisDemo)






bool gen_distance_graph = false;
if(gen_distance_graph){
    std::cout<<"\n*********************"<< std::endl;
    std::cout << "    Distance Table    "<< std::endl;
    std::cout << "*********************"<< std::endl;

    // create a table, e.g.
    // create table sim_log.distance_graph (sim_time float, distance float, primary key (sim_time, distance));

    //convert array to vector
    std::vector<double> v(agentsSimTime, agentsSimTime + nS);
    std::vector<double>::iterator ip;

    //find the unique time values then resize
    ip = std::unique(v.begin(), v.begin() + nS);
    v.resize(std::distance(v.begin(), ip));

    //delete the table contents without dropping
    QString qs = "delete from "+schema+".distance_graph";
    model.setQuery(qs);
    if (model.lastError().isValid())
        qDebug() << "\033[0;31m#gen_distance_graph# "
                 << model.lastError()<<"\033[0m";

    int i = 0;
    for (i=0; i<v.size(); i++)
    {
        //update the Frame table used for live updating
        QString q1 = "insert into "+schema+".distance_graph SELECT g2.sim_time AS time, ";
        QString q2 = "ST_Distance(g1.geom::geography,g2.geom::geography) ";
        QString q3 = "FROM sim_log.actors g1, sim_log.actors g2 WHERE g1.agent_id = 1 AND g2.agent_id = 3 ";
        QString q4 = QStringLiteral("AND g1.sim_time = %1 AND g2.sim_time = %1 ").arg(v[i]);
        qs = q1 + q2 + q3 + q4;

        qDebug() << "#QGIS demo# " << qs;

        model.setQuery(qs);
        if (model.lastError().isValid())
            qDebug() << "\033[0;31m#qgisDemo# "
                     << model.lastError()<<"\033[0m";
    }
}




//======================================
qDebug() << "closing connection" << endl;
db.close();
return 0;


}
