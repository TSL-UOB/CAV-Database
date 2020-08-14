///*
//This class will read in the log file from the UE4 simulation expecting the current cvs format:
//1 agent ID (1=car, 2=ped ... ???)
//2 agentType
//3 time
//4 fps
//5 x (meters ???)
//6 y
//7 z
//8 yaw (rad?)
//*/


//#include "readsimlog.h"
//#include <iostream>
//#include <vector>
//#include <fstream> //reading config file
//#include <sstream> //splitting config file strings


//readSimLog::readSimLog()
//{

//public:
//        int a=0;
//        std::ifstream myfile;   //filename for the log

//        std::string header; //csv header
//        int fileLineCount = 0;
//        std::string line; //temporary holder for csv line

//        //Use a structure for csv columns
//        struct Record
//        {
//            int agentID;
//            std::string agentType;
//            double simTime, simFPS, simX, simY, simZ, simYaw;
//        };
//        std::vector<Record> my_records;

//void read_file(std::ifstream& myfile, std::string line);
//{
//    while (getline(myfile, line)){
//        Record record;
//            std::istringstream iss(line);
//            std::string token;

//            getline(iss, token, ',');
//            record.agentID = std::stoi(token);

//            getline(iss, record.agentType, ',');

//            getline(iss, token, ',');
//            record.simTime = std::stod(token);

//            getline(iss, token, ',');
//            record.simFPS = std::stod(token);

//            getline(iss, token, ',');
//            record.simX = std::stod(token);

//            getline(iss, token, ',');
//            record.simY = std::stod(token);

//            getline(iss, token, ',');
//            record.simZ = std::stod(token);

//            getline(iss, token, ',');
//            record.simYaw = std::stod(token);

//            my_records.push_back(record);
//    }

//    std::cout << "AV position is: " << my_records[0].simX << " " << my_records[0].simY << std::endl;
//}

//}
