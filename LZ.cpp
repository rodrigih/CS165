#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include <sys/stat.h>

#include "ParamChecker.h"

long getSize(std::string fileName)
{
    const char* cFileName;
    struct stat results;
    long size;

    cFileName = fileName.c_str();
    
    if(stat(cFileName,&results) == 0)
    {
        size = results.st_size;
    }
    else
    {
        std::cerr << "An error occurred when opening a file" << std::endl <<
            "Exiting program..." << std::endl;
        return -1;
    }

    return size;
}

void getBuffer(std::string fileName,char* buffer,long size)
{
    //Open file
    const char* cFileName = fileName.c_str();
    std::ifstream inFile;

    inFile.open(cFileName, std::ios::in | std::ios::binary);

    inFile.read(buffer,size);

    inFile.close();
}

void writeBits(char* writeBuffer, int n, int* charIndex,
        int* bitIndex,int value)
{
    int i = 0;
    while(n != 0)
    {
        if(*(bitIndex) > 7)
        {
            (*charIndex)++;
            *bitIndex = 0;
        }
        writeBuffer[*charIndex] |= ((value >> i++) & 1) << (*bitIndex)++;
        --n;
    }
}

void writeString(char* writeBuffer, int n, int* charIndex,
        int* bitIndex, std::string value)
{
    for(char c: value)
    {
        writeBits(writeBuffer,8,charIndex,bitIndex,c);
    } 
}

int main(int argc, char* argv[])
{
    int N = 11;
    int L = 4;
    int S = 3;
    std::string fileName;

    // Check parameters for errors and set their values
    if(checkArgs(&N,&L,&S,&fileName,argc,argv) > 0)
    {
       std::cerr << "Exiting Program" << std::endl; 
       return 1;
    }

    int W = pow(2,N);
    int F = pow(2,L)-1;
    int encodedLength = pow(2,S)-1;
    
    int lookIndex = 0;

    long fileSize = getSize(fileName);
    char readBuffer[fileSize];

    int writeSize = 0;
    char writeBuffer[fileSize];

    if(fileSize < 0) // Negative num means error occurred opening file
    {
        return 1;
    }

    getBuffer(fileName,buffer,fileSize);


    // For debugging only
    // (Checks parameters passed)
    std::cout << "N = " << N << std::endl;
    std::cout << "L = " << L << std::endl;
    std::cout << "S = " << S << std::endl;
    std::cout << "FILE = " << fileName << std::endl;

    std::cout << "Window Size = " << W << std::endl
        << "Look ahead buffer size = " << F << std::endl 
        << "Longest encoded literal string = " << encodedLength << std::endl;
        

    // For Debugging only
    std::cout << "Size of file is: " << fileSize << std::endl;
    std::cout << "The following are the contents of the file:" << std::endl
        << buffer << std::endl;

    //Output stuff to file
    std::cout.write(reinterpret_cast<char*>(&N),1);
    std::cout.write(reinterpret_cast<char*>(&L),1);
    std::cout.write(reinterpret_cast<char*>(&S),1);
    std::cout.write(buffer,fileSize);
    return 0;
}
