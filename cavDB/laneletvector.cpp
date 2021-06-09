#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "lanelettxt.h"

void laneletVector(std::ifstream& file, bool diag){

    //Read the lanelet file and generate vector array
    laneletTXT testLog;
    std::string line;

    //std::ifstream file ("loop_geo_RHD.txt");
    testLog.read(file, true);


    unsigned long nS = testLog.fileLineCount;
    std::cout << "#laneletVector# number of file lines in sim log ..." << nS <<std::endl;

    int* laneletID = new int[nS];
    std::vector<std::string> bound;
    double* x = new double[nS];
    double* y = new double[nS];

    for(unsigned long i=0;i<nS;i++){

        laneletID[i] = testLog.my_records[i].laneletID;
        bound.push_back(testLog.my_records[i].bound);
        x[i] = testLog.my_records[i].x;
        y[i] = testLog.my_records[i].y;

        //debug diag
        if(diag and i<5){
            std::cout << "laneletID " << laneletID[i] << std::endl;
            std::cout << "bound " << bound[i]<< std::endl;
            std::cout << "x " << x[i];
            std::cout << "y " << y[i];
         }
     }
}
