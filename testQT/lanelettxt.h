#ifndef LANELETTXT_H
#define LANELETTXT_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

class laneletTXT
{
public:
    std::ifstream myfile;   //filename
    std::string header;
    unsigned long fileLineCount = 0;
    std::string line; //temporary holder for the line

    struct Record
    {
        int laneletID;
        std::string bound;
        double x, y;
    };
    std::vector<Record> my_records;
    void read(std::ifstream& myfile, bool diag);
};

#endif // LANELETTXT_H
