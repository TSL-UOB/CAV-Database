//#include <iostream>
//#include <QtSql>

//// Rotation matrix fuinction, returns x' and y' given x, y, yaw(rads)
//std::tuple<double, double> rotate_coords(double x, double y, double yaw, bool diag=true)
//{
//    double xd = x * cos(yaw) - y * sin(yaw);
//    double yd = x * sin(yaw) + y * cos(yaw);
//    if(diag)
//        qDebug() << "#rotate_coords# coordinates rotated";
//    return std::make_tuple(xd,yd);
//}

////Get rotated coords from agent pos and shape
//double * return_coords(int ag_typ, double ag_len, double ag_wid, double posX, double posY, double yaw_deg, bool diag=true){

//    static double return_coordiantes[10];

//    //yaw to radians
//    double rads = yaw_deg * 3.141592653 / 180;

//    // calculate shape coordiantes from posX/Y and yaw
//    double x1= -1. * ag_len/2., x4 = -1. * ag_len/2.;
//    double x2= +1. * ag_len/2., x3 = +1. * ag_len/2.;
//    double y1= -1. * ag_wid/2., y2 = -1. * ag_wid/2.;
//    double y3= +1. * ag_wid/2., y4 = +1. * ag_wid/2.;

//    //use rotationmatrix to get rotated coordinates
//    double x1r, y1r, x2r, y2r, x3r, y3r, x4r, y4r;
//    std::tie(x1r, y1r) = rotate_coords(x1, y1, rads, false); //unpack tuple elements
//    std::tie(x2r, y2r) = rotate_coords(x2, y2, rads, false); //unpack tuple elements
//    std::tie(x3r, y3r) = rotate_coords(x3, y3, rads, false); //unpack tuple elements
//    std::tie(x4r, y4r) = rotate_coords(x4, y4, rads, false); //unpack tuple elements
//    if(diag){
//        qDebug() << "#update_agent_shape# x1r" << x1r << "y1r" << y1r;
//        qDebug() << "#update_agent_shape# x2r" << x2r << "y2r" << y2r;
//        qDebug() << "#update_agent_shape# x3r" << x3r << "y3r" << y3r;
//        qDebug() << "#update_agent_shape# x4r" << x4r << "y4r" << y4r;
//    }

//    // Add the original position to the coordinates to transform back to mapp coordinates
//    x1=x1r+posX; x2=x2r+posX; x3=x3r+posX; x4=x4r+posX;
//    y1=y1r+posY; y2=y2r+posY; y3=y3r+posY; y4=y4r+posY;

//    return_coordiantes[0] = x1;
//    return_coordiantes[1] = x2;
//    return_coordiantes[2] = x3;
//    return_coordiantes[3] = x4;
//    return_coordiantes[4] = y1;
//    return_coordiantes[5] = y2;
//    return_coordiantes[6] = y3;
//    return_coordiantes[7] = y4;

//    return return_coordiantes;
//}




//// updated to include width/length selection from enum list
//void update_agent_shape_batch_ID(QSqlQueryModel& model, int ag_id[], int ag_typ[], double sim_time, int nA,
//                 double ag_len[], double ag_wid[], double posX[], double posY[], double yaw_deg[],
//                double agWidth_lookup[], double agLengt_lookup[], bool diag=true)
//{
//    //Update the width/length based on the agent ID
//    if(nA>1){
//        for(int i=0;i<nA;i++){
//            ag_wid[i] = agWidth_lookup[ag_typ[i]];
//            ag_len[i] = agLengt_lookup[ag_typ[i]];
//            if(false)
//                std::cout <<"ag_typ[i]="<<ag_typ[i]<<" ag_wid[i]="<<ag_wid[i]<<" ag_len[i]"<<ag_len[i];
//        }
//    }

//    //call the coordinates
//    double *rc;
//    rc = return_coords(ag_typ[0], ag_len[0], ag_wid[0], posX[0], posY[0], yaw_deg[0],false); //edit diag
//    double x1=rc[0],x2=rc[1],x3=rc[2],x4=rc[3],y1=rc[4],y2=rc[5],y3=rc[6],y4=rc[7];

//    // build string
//    QString qs, q1, q2, q3, q4, qEnd, dummy;
//    q1 = "INSERT INTO ch03.my_polygons (";
//    q2 = "agent_id, agent_type, sim_time, geom) VALUES ";
//    q3 = QStringLiteral("(%1, %2, %3, ST_GeomFromText('POLYGON((").arg(ag_id[0]).arg(ag_typ[0]).arg(sim_time);
//    q4 = QStringLiteral(" %1 %2, %3 %4, %5 %6, %7 %8, %1 %2))'))").
//         arg(x1).arg(y1).arg(x2).arg(y2).arg(x3).arg(y3).arg(x4).arg(y4);
//    qs = q1 + q2 + q3 + q4;
//    //start loop here for compunt string
//    if(nA>1){
//        for(int i=1;i<nA;i++){
//            rc = return_coords(ag_typ[i], ag_len[i], ag_wid[i], posX[i], posY[i], yaw_deg[i],false); //edit diag
//            double x1=rc[0],x2=rc[1],x3=rc[2],x4=rc[3],y1=rc[4],y2=rc[5],y3=rc[6],y4=rc[7];
//            dummy = QStringLiteral(", (%1, %2, %3, ST_GeomFromText('POLYGON(( %4 %5, %6 %7, %8 %9, %10 %11, %4 %5))'))")
//            .arg(ag_id[i]).arg(ag_typ[i]).arg(sim_time).arg(x1).arg(y1).arg(x2).arg(y2).arg(x3).arg(y3).arg(x4).arg(y4);
//           qs = qs + dummy;
//        }
//    }
////    qEnd = ")";
////    qs = qs + qEnd;
//    model.setQuery(qs);
//    if (model.lastError().isValid())
//        qDebug() << "\033[0;31m#update_agent_shape_batch_ID# " << model.lastError()<<"\033[0m";
//    if(diag)
//        qDebug() << "#update_agent_shape_batch_ID# " << qs;
//}
