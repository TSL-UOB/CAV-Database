#ifndef READSIMLOG_H
#define READSIMLOG_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

class readSimLog
{
public:
    std::ifstream myfile;   //filename for the log
    std::string header; //csv header
    unsigned long fileLineCount = 0;
    std::string line; //temporary holder for csv line
    //Use a structure for csv columns
    struct Record
    {
        int agentID;
        std::string agentType;
        int testNo, repeatNo, agentNo, agentTypeNo;
        double simTime, simFPS, simX, simY, simZ, simYaw, velX, velY, velZ, speed, wallClock;
    };
    std::vector<Record> my_records;

    void read_file(std::ifstream& myfile, bool diag);
    void read_file_carla(std::ifstream& myfile, bool diag);
    void read_file_testbench(std::ifstream& myfile, bool diag);
};

#endif // READSIMLOG_H
