#include <iostream>
#include <stdlib.h>
#include <QtDebug>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtCore/qglobal.h>
#include <QtCore/qarraydata.h> //for Qstringliteral
#include <sys/time.h> //for timer
#include <cstdlib> //for random number generation
#include <tuple> //used in rotate_coords
#include <cmath>
#include <fstream> //reading config file
#include <sstream> //splitting config file strings
#include <vector>


//// Add shapes to polygon table
//sql_string = "INSERT INTO ch03.my_polygons (agent_type, geom) VALUES ('road',ST_GeomFromText('POLYGON((0 0, 11 0, 11 6, 0 6, 0 0))'))";
//model2.setQuery(sql_string);
////sql_string = "INSERT INTO ch03.my_polygons (agent_type, geom) VALUES ('car2',ST_GeomFromText('POLYGON((4 2, 8 2,  8 4,  4 4, 4 2))'))";
////model2.setQuery(sql_string);
//sql_string = "INSERT INTO ch03.my_polygons (agent_type, geom) VALUES ('car1',ST_GeomFromText('POLYGON((2 1, 6 1,  6 3,  2 3, 2 1))'))";
//model2.setQuery(sql_string);
//sql_string = "INSERT INTO ch03.my_polygons (agent_type, geom) VALUES ('car3',ST_GeomFromText('POLYGON((10 1, 14 1,  14 3,  10 3, 10 1))'))";
//model2.setQuery(sql_string);
//sql_string = "INSERT INTO ch03.my_polygons (agent_type, geom) VALUES ('ped1',ST_GeomFromText('POLYGON((13 5, 14 5,  14 6,  13 6, 13 5))'))";
//model2.setQuery(sql_string);


//// Make a lookup table for the AgentTypes
//string agType[4] = {"car","ped","hgv","cyclist"};  // to access use agent_type[0] etc.
//double agWidth [4] = {2.0,0.5,4.5,0.5};
//double agLengt [4] = {4.0,0.5,8.5,1.5};
////    for (int i=0;i<3;i++){
////        cout << "agent type-ID is " << i;
////        cout << agType[i] << " " << agWidth[i] << " " << agLengt[i] << "\n";}

//// Drop table to clear then create it
//sql_string = "DROP TABLE IF EXISTS ch03.lookup_type";
//model2.setQuery(sql_string);
//sql_string = "create table ch03.lookup_type (agent_type varchar, width float, length float)";
//model2.setQuery(sql_string);

//// CAR
//add_agent(model2, agType[0],agWidth[0],agLengt[0],false);
//// HGV
//add_agent(model2, agType[1],agWidth[1],agLengt[1],false);
//// PED - Using function
//add_agent(model2, agType[2],agWidth[2],agLengt[2],false);
//// New type - Using function
//add_agent(model2, "cyclist", 0.5, 2.0,false);



//    //update a fake car doing an overtake manouvre
//    update_agent_shape(model2, 9, 0, 0.0, 4, 2, 0 , 0, 0);
//    update_agent_shape(model2, 9, 0, 0.1, 4, 2, 5 , .1, 5);
//    update_agent_shape(model2, 9, 0, 0.2, 4, 2, 10, .3, 10);
//    update_agent_shape(model2, 9, 0, 0.3, 4, 2, 15, .6, 5);
//    update_agent_shape(model2, 9, 0, 0.4, 4, 2, 20, .4, -5);
//    update_agent_shape(model2, 9, 0, 0.5, 4, 2, 25, .2, -10);
//    update_agent_shape(model2, 9, 0, 0.6, 4, 2, 30, 0.1, -5);

//    //add a cyclist
//    update_agent_shape(model2, 8, 3, 0.0, 0.5, 0.5, 0 , 1.8, 0);
//    update_agent_shape(model2, 8, 3, 0.1, 0.5, 0.5, 5 , 1.8, 0);
//    update_agent_shape(model2, 8, 3, 0.2, 0.5, 0.5, 10, 1.8, 0);
//    update_agent_shape(model2, 8, 3, 0.3, 0.5, 0.5, 15, 1.8, 0);
//    update_agent_shape(model2, 8, 3, 0.4, 0.5, 0.5, 20, 1.8, 0);
//    update_agent_shape(model2, 8, 3, 0.5, 0.5, 0.5, 25, 1.8, 0);
//    update_agent_shape(model2, 8, 3, 0.6, 0.5, 0.5, 30, 1.8, 0);

//    //assertion test for collision
//    check_agent_overlap(model2, "car1", "car1");
//    //double ass_check_agent_overlap(QSqlQueryModel& model, int A_id, int A_ty, int B_id, int B_ty, double sim_time, bool diag=true)
//    for(double st=0;st<0.7;st=st+0.1){
//        ass_check_agent_overlap(model2, 9, 0, 8, 3, st, true);}



//// To change an existing value in the databae, e.g.
//sql_string = "UPDATE ch03.my_polygons SET geom = ST_GeomFromText('POLYGON((10 1, 14 1,  12 3,  14 3, 10 1))') "\
//"WHERE agent_type = 'car3'";



// Test ST_Distance function
//    check_agent_dist(model2, "car1", "car1");
//    check_agent_dist(model2, "car1", "car2");
//    check_agent_dist(model2, "car1", "car3");
//    check_agent_dist(model2, "car1", "ped1");
//    check_agent_dist(model2, "car1", "road");

// Test ST_Within function
//    check_agent_within(model2, "car1", "car1");
//    check_agent_within(model2, "car1", "car2");
//    check_agent_within(model2, "car1", "car3");
//    check_agent_within(model2, "car1", "ped1");
//    check_agent_within(model2, "car1", "road");

// Test ST_Overlaps function
//    check_agent_overlap(model2, "car1", "car1");
//    check_agent_overlap(model2, "car1", "car2");
//    check_agent_overlap(model2, "car1", "car3");
//    check_agent_overlap(model2, "car1", "ped1");
//    check_agent_overlap(model2, "car1", "road");

// Test ST_Contains function
//    check_agent_contains(model2, "road", "car1");
//    check_agent_contains(model2, "road", "car2");
//    check_agent_contains(model2, "road", "car3");
//    check_agent_contains(model2, "road", "ped1");
//    check_agent_contains(model2, "road", "road");

int snippets() {
    bool diag = true;
    QSqlQueryModel model2;
    QString sql_string;

    // Create a table
    sql_string = "CREATE TABLE ch03.my_data (PersonID int)";
    model2.setQuery(sql_string);
    if(diag) qDebug() << "building DB table complete" << endl;

    // Create a table
    sql_string = "CREATE TABLE ch03.my_data (PersonID int)";
    model2.setQuery(sql_string);
    qDebug() << "building DB table complete" << endl;

    // ----------------------------------------------------------------
    // Add data to the table - with a string
    sql_string = "APPEND INTO ch03.my_data(PersonID) VALUES (20)";
    model2.setQuery(sql_string);

    //or use embedded variable in string
    int klm = 13;
    sql_string = QStringLiteral("APPEND INTO ch03.my_data(PersonID) VALUES (%1)").arg(klm);
    model2.setQuery(sql_string);
    // ----------------------------------------------------------------

    // List the new table
    sql_string = "SELECT * FROM ch03.my_data";
    model2.setQuery(sql_string);
    QSqlQuery sql_query; // Use QSqlQuery
    sql_query.exec(sql_string);
    //qDebug() << "table entries are:";
    while (sql_query.next()) {
        int personID = sql_query.value(0).toInt();
        //qDebug() << personID;
    }

    // prepare a linestring shape in a new table
    sql_string = "CREATE TABLE ch03.my_linestrings ("\
                 "id serial PRIMARY KEY,"\
                 "name varchar(20),"\
                 "my_linestrings geometry(LINESTRING)"\
                 ")";
    model2.setQuery(sql_string);
    sql_string = "INSERT INTO ch03.my_linestrings (name, my_linestrings)"
                 "VALUES"
                 "('car', ST_GeomFromText('LINESTRING(0 0, 2 0, 2 1, 0 0)')),"
                 "('truck', ST_GeomFromText('LINESTRING(0 0, 4 0, 4 2, 0 0)'))";
    model2.setQuery(sql_string);

    // Add a column to a table
    sql_string = "ALTER TABLE ch02.my_geometries ADD COLUMN name varchar";
    // List table entries
    sql_string = "SELECT * FROM ch03.my_linestrings";
    // Print entire table
    sql_string = "TABLE ch03.my_data";

    // ----------------------------------------------------------------
        // Add data to the table - with a string
        sql_string = "APPEND INTO ch03.my_data(PersonID) VALUES (20)";
        model2.setQuery(sql_string);

        //or use embedded variable in string
        klm = 13;
        sql_string = QStringLiteral("APPEND INTO ch03.my_data(PersonID) VALUES (%1)").arg(klm);
        model2.setQuery(sql_string);
    // ----------------------------------------------------------------

        // List the new table
        sql_string = "SELECT * FROM ch03.my_data";
        model2.setQuery(sql_string);
        //QSqlQuery sql_query; // Use QSqlQuery
        sql_query.exec(sql_string);
        //qDebug() << "table entries are:";
        while (sql_query.next()) {
            int personID = sql_query.value(0).toInt();
            //qDebug() << personID;
        }

        // prepare a linestring shape in a new table
        sql_string = "CREATE TABLE ch03.my_linestrings ("\
                "id serial PRIMARY KEY,"\
                "name varchar(20),"\
                "my_linestrings geometry(LINESTRING)"\
                ")";
        model2.setQuery(sql_string);
        sql_string = "INSERT INTO ch03.my_linestrings (name, my_linestrings)"
                "VALUES"
                "('car', ST_GeomFromText('LINESTRING(0 0, 2 0, 2 1, 0 0)')),"
                "('truck', ST_GeomFromText('LINESTRING(0 0, 4 0, 4 2, 0 0)'))";
        model2.setQuery(sql_string);

        // Add a column to a table
        sql_string = "ALTER TABLE ch02.my_geometries ADD COLUMN name varchar";
        // List table entries
        sql_string = "SELECT * FROM ch03.my_linestrings";
        // Print entire table
        sql_string = "TABLE ch03.my_data";

        // Update agent using function update_agent_position(model2, agentType, x1, y1, agLengt, agWidth, theta)
        unsigned int tyID = 0; // typeID=0 for cars
        double xPos = 21.0, yPos = 16.0;
        //update_agent_position(model2, agType[tyID], xPos, yPos, agLengt[tyID], agWidth[tyID], 0, false);
        //update_agent_position(model2, "car2", 4.0, 2.0, 4.2, 2.2, 0, false);

return 0;
};



//// *** Single Write Test ***************************************

//    //randomly generate n-agents and test database write speed, add clock counters
//    // start timer
//    bool update_test_single = false;
//    if(update_test_single){
//        for(int j=0;j<10;j++){
//            int array_size = 2;
//            //qDebug() << "Looping for " << array_size << " iterations" << endl;
//            // create array of random number 0-100 for x and y start positions
//            double* agent_xpos = new double[array_size];
//            double* agent_ypos = new double[array_size];
//            for(int i = 0; i < array_size; ++i) //crate random (0-100) positions
//            {
//                agent_xpos[i] = ( std::rand() % 100 );
//                agent_ypos[i] = ( std::rand() % 100 );
//            }
//            //qDebug() << *agent_xpos << endl;
//            // run agent updater for array size
//            int tyID = 0; // typeID=0 for cars
//            timestamp_t t0 = get_timestamp();
//            for (int i=0;i<array_size;i++) {
//                update_agent_position(model2, agType[tyID], agent_xpos[i], agent_ypos[i], agLengt[tyID], agWidth[tyID], 0, false);
//            }
//            // stop timer
//            timestamp_t t1 = get_timestamp();
//            // print time results and agent count
//            double secs = (t1 - t0) / 1000000.0;
//            //    qDebug() << "Time taken (s) = " << secs << endl;
//            //        qDebug() << secs ;
//        }
//    }



//check for overlap with ANY other agent type - this will return a table of data but not store it
//    select ST_Overlaps(g1.geom,g2.geom)
//    FROM ch03.my_polygons g1, ch03.my_polygons g2
//    WHERE g1.agent_type = 0 AND g2.agent_type != 0 AND g1.sim_time = 0.0
//better to do check and write directly to assertions table -> ch03.assertions
//    INSERT INTO
//        ch03.assertions (agent_id, sim_time, asr_id, asr_result)
//    SELECT
//        g2.agent_id, g2.sim_time,
//        1 AS asr_id,
//        ST_Overlaps(g1.geom,g2.geom) AS asr_result
//    FROM
//        ch03.my_polygons g1, ch03.my_polygons g2
//    WHERE
//        g1.agent_type = 0 AND g2.agent_type != 0 AND g1.sim_time = 0.0



//// Select tables (into variable)
//sql_string = "select geom from ch03.my_polygons where agent_type = 'road'";
//sql_string = "select geom from ch03.my_polygons AS g1 where agent_type = 'road'";

//// Distance between geometries
//sql_string = "select ST_Distance(g1.geom,g2.geom) FROM ch03.my_polygons g1, ch03.my_polygons g2 WHERE g1.agent_type = 'road' AND g2.agent_type = 'car1'";
//sql_string = "select ST_Distance(g1.geom,g2.geom) "\
//        "FROM ch03.my_polygons g1, ch03.my_polygons g2 "\
//        "WHERE g1.agent_type = 'car3' AND g2.agent_type = 'car1'";
////model2.setQuery(sql_string);
//double distanceTest = model2.record(0).value(0).toDouble();
////qDebug() << "Distance between geometries is " << distanceTest << endl;

//// Test the overlap functions
//// bool ST_Within(geometry g1, geometry g2, double precision distance_of_srid);
//bool within_bound;
//sql_string = "select ST_Within(g1.geom,g2.geom) "
//             "FROM ch03.my_polygons g1, ch03.my_polygons g2 WHERE g1.agent_type = 'car3' AND g2.agent_type = 'road'";
////model2.setQuery(sql_string);
//within_bound = model2.record(0).value(0).toBool();
////qDebug() << "Is car3 within road? True/False:" << within_bound << endl;

//// Check another within query
//sql_string = "select ST_Within(g1.geom,g2.geom) FROM ch03.my_polygons g1, ch03.my_polygons g2 WHERE g1.agent_type = 'ped1' AND g2.agent_type = 'road'";
////model2.setQuery(sql_string);
//within_bound = model2.record(0).value(0).toBool();
////qDebug() << "Is ped1 within road? True/False:" << within_bound << endl;


//// *** Batch Write Test ***************************************
//    bool update_test_batch = true;
//    if(update_test_batch){
//        //batch mode string updater

//        // agType[4] = {"car","ped","hgv","cyclist"};
//        string agType_lookup[5] = {"AV","car","ped","hgv","cyclist"};  // to access use agent_type[0] etc.
//        double agWidth_lookup [5] = {2.0,2.0,0.5,3.5,0.5};
//        double agLengt_lookup [5] = {4.0,4.0,0.5,6.5,1.5};

//        for(int k=0;k<1;k++){
//            int loops = 1;
//            double sum_time=0;

//            for(int j=0;j<loops;j++){
//                double sim_time = 0.0;
//                int nA = 10;            //*** SET nA=1 for single write test ***

//                //qDebug() << "Looping for " << array_size << " iterations" << endl;
//                int* ag_id      = new int[nA];
//                int* ag_typ     = new int[nA];
//                double* ag_len  = new double[nA];
//                double* ag_wid  = new double[nA];
//                double* posX    = new double[nA];
//                double* posY    = new double[nA];
//                double* yaw_deg = new double[nA];

//                // create array of random numbers
//                for(int i=0; i<nA; ++i) {
//                    ag_id[i]	 = ( std::rand()  ); //% 1000000
//                    ag_typ[i]	 = ( std::rand() % 4 ) + 1; //ag_typ=0 reserved for AV
//                    ag_len[i]	 = ( std::rand() % 4 );
//                    ag_wid[i]	 = ( std::rand() % 2 );
//                    posX[i]		 = ( std::rand() % 20 );
//                    posY[i]		 = ( std::rand() % 20 );
//                    yaw_deg[i]	 = ( std::rand() % 180 );
//                }
//                timestamp_t t0 = get_timestamp();
//                //update_agent_shape_batch(model2, ag_id, ag_typ, sim_time, nA, ag_len, ag_wid, posX, posY, yaw_deg, false);
//                update_agent_shape_batch_ID(model2, ag_id, ag_typ, sim_time, nA, ag_len, ag_wid,
//                                            posX, posY, yaw_deg, agWidth_lookup, agLengt_lookup, false);
//                // stop timer
//                timestamp_t t1 = get_timestamp();
//                // print time results and agent count
//                double secs = (t1 - t0) / 1000000.0;
//                //qDebug() << "Time taken (s) = " << secs << endl;
//                //qDebug() << "batch execution time for loop#" << j << " with "<< nA << " agents is " << secs ;
//                //qDebug() << secs ;
//                sum_time += secs;
//            }
//            //qDebug() << "average time is" << sum_time/(loops * nA);
//            //    qDebug() << sum_time/(loops * nA);
//        }
//    }




//std::string header, cfg_typ, cfg_wid, cfg_len;

////Open file and read complete lines
//ifstream myfile ("/home/is18902/Documents/testQT/agent_config.txt");

////class for reading config
//AgentConfig read_agent_config;
//read_agent_config.read_file(myfile);


//if (myfile.is_open()){
//    getline(myfile,header);
//    getline(myfile,cfg_typ);
//    getline(myfile,cfg_wid);
//    getline(myfile,cfg_len);
//}
//cout << header << endl;
//cout << cfg_typ << endl;
//cout << cfg_wid << endl;
//cout << cfg_len << endl;

////Split line into array of elements
//std::vector<string> vect_typ;
//std::vector<double> vect_wid, vect_len;
//std::stringstream ss_typ(cfg_typ), ss_wid(cfg_wid), ss_len(cfg_len);

//std::string token;
////Parse the width string
//while(std::getline(ss_typ, token, ',')) {
//    //std::cout << "while token: " << token << '\n';
//    vect_typ.push_back(token);
//    if (ss_typ.peek() == ',')
//        ss_typ.ignore();
//}
//if(diag){
//    for (unsigned i=0; i< vect_typ.size(); i++)
//        std::cout << vect_typ.at(i)<<std::endl;
//}

//double kk;
////Parse the width string
//while (ss_wid >> kk)
//{
//    vect_wid.push_back(kk);
//    if (ss_wid.peek() == ',')
//        ss_wid.ignore();
//}
//if(diag){
//    for (unsigned i=0; i< vect_wid.size(); i++)
//        std::cout << vect_wid.at(i)<<std::endl;
//}

//// Parse the length string
//while (ss_len >> kk)
//{
//    vect_len.push_back(kk);
//    if (ss_len.peek() == ',')
//        ss_len.ignore();
//}
//if(diag){
//    for (unsigned i=0; i< vect_len.size(); i++)
//        std::cout << vect_len.at(i)<<std::endl;
