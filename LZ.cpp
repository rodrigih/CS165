#include <iostream>
#include <string>

#include "ParamChecker.h"

int main(int argc, char* argv[])
{
    int N = 11;
    int L = 4;
    int S = 3;
    std::string fileName;

    // Check parameters for errors
    if(checkArgs(&N,&L,&S,&fileName,argc,argv) > 0)
       std::cerr << "Exiting Program" << std::endl; 

    // For debugging only
    std::cout << "N = " << N << std::endl;
    std::cout << "L = " << L << std::endl;
    std::cout << "S = " << S << std::endl;
    std::cout << "FILE = " << fileName << std::endl;
}
