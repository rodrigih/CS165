#ifndef PARAMCHECKER_H
#define PARAMCHECKER_H

#include <iostream>
#include <stdlib.h>
#include <string>

void setValue(int* param, char arg[], int* flag)
{
    std::string value = "";

    for(int i = 3; arg[i] != '\0'; i++)
    {
        if(!isdigit(arg[i]))
        {
            std::cerr << i;
            std::cerr << arg[i] << " is not a digit" << std::endl;
            *flag = 1;    
            return;
        }
        value+= arg[i];
    }
    
    if(value == "")
    {
        *flag = 1;  
        return;
    }
    
    *param = atoi(value.c_str());
}

void checkRange(int* param,char arg[], int min, int max, int* flag)
{
    if(!(min <= *(param) && *(param) <= max))
    {
       std::cerr << "Parameter '" << arg << "'  out of range." << std::endl;
       std::cerr << "Please enter value between " << min << " and " << max << std::endl;
       *flag = 1;
    }

}

int checkArgs(int* N, int* L, int* S, std::string* fileName,int argc, char* argv[])
{
    int errFlag = 0;

    char* current;

    if(argc > 5)
    {
        std::cerr << "Too many arguments." << std::endl;
        errFlag = 1;
    }

    for(int i = 1; i < argc; i++)
    {
        current = argv[i];
        if(current[0] == '-')
        {
        
            switch(toupper(current[1]))
            {
                case 'N':
                    setValue(N,current,&errFlag);
                    checkRange(N,current,9,14,&errFlag);
                    break;
                case 'L':
                    setValue(L,current,&errFlag);
                    checkRange(L,current,3,4,&errFlag);
                    break;
                case 'S':
                    setValue(S,current,&errFlag);
                    checkRange(S,current,1,5,&errFlag);
                    break;
                default:
                    std::cerr << "Invalid argument: " << current << std::endl;
                    errFlag = 1;
                    break;
            }
        }
        else
        {
            *fileName = std::string(current); 
        }
    }

    return errFlag;
}

#endif /* PARAMCHECKER_H */
