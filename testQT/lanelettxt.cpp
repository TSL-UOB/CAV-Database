#include "lanelettxt.h"

void laneletTXT::read(std::ifstream& myfile, bool diag)
{
    std::cout << "\n***********************"<< std::endl;
    std::cout << " Reading Lanelet File "<< std::endl;
    std::cout << "***********************"<< std::endl;

    if (myfile.is_open())
    {
        if (diag) std::cout << "Lanelet Data File opened successfully." << std::endl;
        getline(myfile,header);

        while (getline(myfile, line))
        {
            if (line.empty())
            {
                if(diag) std::cout << "##read_file_testbench## empty line detected, skipping" << std::endl;
            }
            else
            {
                //4 columns: 1 lanelt_id (int), 2 bound (string), 3 x (double), 4 y (double)
                Record record;
                fileLineCount++;
                std::istringstream iss(line);
                std::string token;

                //1 lanelt_id (int)
                getline(iss, token, ',');
                record.laneletID = std::stoi(token);

                //2 bound (string)
                getline(iss, record.bound, ',');

                //3 x (double)
                getline(iss, token, ',');
                record.x = std::stod(token);

                //4 y (double)
                getline(iss, token, ',');
                record.y = std::stod(token);

                my_records.push_back(record);
            }

        }//while getline

    } else {
        std::cout << "\033[0;31m#LaneletTXT.read# ERROR: File can't be opened! Ensure the file is in the BUILD directory\n" <<"\033[0m";
    } //if myfile is_open


        //if (diag) std::cout << "AV position is: " << my_records[1].simX << " " << my_records[1].simY << std::endl;
};
