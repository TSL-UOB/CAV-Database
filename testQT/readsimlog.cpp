//read sim data file

#include "readsimlog.h"
#include <iostream>

void readSimLog::read_file(std::ifstream& myfile, bool diag)
{
    std::cout << "\n***********************"<< std::endl;
    std::cout << " Reading Sim Data File "<< std::endl;
    std::cout << "***********************"<< std::endl;

    if (myfile.is_open())
    {
        if (diag) std::cout << "Sim Data File opened successfully." << std::endl;
        getline(myfile,header);

        while (getline(myfile, line))
        {
            //9 columns: agentID, agentType, agentTypeNo, time, fps, x, y, z, yaw
            Record record;
            fileLineCount++;
            std::istringstream iss(line);
            std::string token;

            getline(iss, token, ',');
            record.agentID = std::stoi(token);

            getline(iss, record.agentType, ',');

            getline(iss, token, ',');
            record.agentTypeNo = std::stoi(token);

            getline(iss, token, ',');
            record.simTime = std::stod(token);

            getline(iss, token, ',');
            record.simFPS = std::stod(token);

            getline(iss, token, ',');
            record.simX = std::stod(token);

            getline(iss, token, ',');
            record.simY = std::stod(token);

            getline(iss, token, ',');
            record.simZ = std::stod(token);

            getline(iss, token, ',');
            record.simYaw = std::stod(token);

            my_records.push_back(record);

        }//while getline

    } else {
        std::cout << "\033[0;31m#readSimLog.read_file# ERROR: Sim File can't be opened! Ensure the file is in the BUILD directory\n" <<"\033[0m";
    }
}

void readSimLog::read_file_carla(std::ifstream& myfile, bool diag=true)
{
    std::cout << "\n***********************"<< std::endl;
    std::cout << " Reading Carla Data File "<< std::endl;
    std::cout << "***********************"<< std::endl;

    if (myfile.is_open())
    {
        if (diag) std::cout << "Sim Data File opened successfully." << std::endl;
        getline(myfile,header);

        while (getline(myfile, line))
        {
            if (line.empty())
            {
                if(diag) std::cout << "##read_file_carla## empty line detected, skipping" << std::endl;
            }
            else
            {
            //11 columns: repeatNo, agentNo, agentID, agentType, agentTypeNo, time, fps, x, y, z, yaw
            Record record;
            fileLineCount++;
            std::istringstream iss(line);
            std::string token;

            //repeat no. (int)
            getline(iss, token, ',');
            record.repeatNo = std::stoi(token);

            //agent no.
            getline(iss, token, ',');
            record.agentNo = std::stoi(token);

            //agent ID
            getline(iss, token, ',');
            record.agentID = std::stoi(token);

            //agent type
            getline(iss, record.agentType, ',');

            //agent type no
            getline(iss, token, ',');
            record.agentTypeNo = std::stoi(token);

            //simulation time
            getline(iss, token, ',');
            record.simTime = std::stod(token);

            //simulation FPS
            getline(iss, token, ',');
            record.simFPS = std::stod(token);

            //agent X, Y, Z & Yaw
            getline(iss, token, ',');
            record.simX = std::stod(token);
            getline(iss, token, ',');
            record.simY = std::stod(token);
            getline(iss, token, ',');
            record.simZ = std::stod(token);
            getline(iss, token, ',');
            record.simYaw = std::stod(token);

            my_records.push_back(record);
            }

        }//while getline

    } else {
        std::cout << "\033[0;31m#readSimLog.read_file# ERROR: Sim File can't be opened! Ensure the file is in the BUILD directory\n" <<"\033[0m";
    }
}

void readSimLog::read_file_testbench(std::ifstream& myfile, bool diag=true)
{
    std::cout << "\n***********************"<< std::endl;
    std::cout << " Reading Carla Data File "<< std::endl;
    std::cout << "***********************"<< std::endl;

    if (myfile.is_open())
    {
        if (diag) std::cout << "Testbench Data File opened successfully." << std::endl;
        getline(myfile,header);

        while (getline(myfile, line))
        {
            if (line.empty())
            {
                if(diag) std::cout << "##read_file_testbench## empty line detected, skipping" << std::endl;
            }
            else
            {
                //17 columns: 1 testNo ,2 repeatNo, 3 agentNo, 4 agentID, 5 agentType,
                //            6 agentTypeNo, 7/8/9/10 x, y, z, yaw,
                //            11/12/13 vel_x, vel_y, vel_z,
                //            14 speed, 15 time, 16 sim_time, 17 fps
                Record record;
                fileLineCount++;
                std::istringstream iss(line);
                std::string token;

                //1 testNo (int)
                getline(iss, token, ',');
                record.testNo = std::stoi(token);

                //2 repeatNo (int)
                getline(iss, token, ',');
                record.repeatNo = std::stoi(token);

                //3 agentNo. (int)
                getline(iss, token, ',');
                record.agentNo = std::stoi(token);

                //4 agentID
                getline(iss, token, ',');
                record.agentID = std::stoi(token);

                //5 agentType
                getline(iss, record.agentType, ',');

                //6 agent type no
                getline(iss, token, ',');
                record.agentTypeNo = std::stoi(token);

                //7/8/9/10 agent X, Y, Z & Yaw
                getline(iss, token, ',');
                record.simX = std::stod(token);
                getline(iss, token, ',');
                record.simY = std::stod(token);
                getline(iss, token, ',');
                record.simZ = std::stod(token);
                getline(iss, token, ',');
                record.simYaw = std::stod(token);

                //11/12/13  vel_x, vel_y, vel_z
                getline(iss, token, ',');
                record.velX = std::stod(token);
                getline(iss, token, ',');
                record.velY = std::stod(token);
                getline(iss, token, ',');
                record.velZ = std::stod(token);

                //14 speed
                getline(iss, token, ',');
                record.speed = std::stod(token);

                //15 time (cyclic simulation time)
                getline(iss, token, ',');
                record.simTime = std::stod(token);

                //16 sim_time (wallClock)
                getline(iss, token, ',');
                record.wallClock = std::stod(token);

                //17 simulation FPS
                getline(iss, token, ',');
                record.simFPS = std::stod(token);


                my_records.push_back(record);
            }

        }//while getline

    } else {
        std::cout << "\033[0;31m#readSimLog.read_file# ERROR: Sim File can't be opened! Ensure the file is in the BUILD directory\n" <<"\033[0m";
    }
};
