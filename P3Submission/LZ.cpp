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

void writeBits(char* writeBuffer, int* charIndex,int* bitIndex, int n, int value)
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
        writeBits(writeBuffer,charIndex,bitIndex,8,c);
    } 
}

/*
void addtoLiteral(std::string literal,char c, char* window, char* buffer, char* bIndex)
{
     *literal = *literal + sub; //Add one character 
    //Advance window and buffer by 1
    *window = window->substr(1) + buffer[*index];
    (*index)++; //Advance buffer

   
}
*/
void advanceBuffers(std::string* window, char* buffer, int* bIndex, int* buffSize, int n)
{
    *window = window->substr(n);

    int i = 0;

    while(*bIndex <= *buffSize && i < n)
    {
        *window = *window + buffer[(*bIndex)++];
        ++i;
    }
}

void continueMatch(std::string* window,int currentPos,int* pos, char* buffer, 
        int* buffIndex, std::string* match, int *maxSize, int maxBuff)
{

    std::string wStr = *match; // std in window
    int wIndex = *pos + 2;
    int wSize = window->size();
    std::string bStr = *match; // str in buffer
    int bIndex = *buffIndex + 2;

    int inBuffer = 0; // Flag whether to read from buffer once it reaches end of window

    while(1)
    {
        if(inBuffer)
            wStr += buffer[wIndex++]; 
        else
            wStr += (*window)[wIndex++];

        bStr += buffer[bIndex++];

        if(wStr.compare(bStr) != 0) //No longer a match
        {
            wStr = wStr.substr(0,wStr.size()-1);
            if(match->size() < wStr.size())
            {
                *match = wStr;
                *pos = currentPos;
            }
            return; 
        }

        // If reached end of buffer or match is max length
        if(bIndex >= maxBuff || wStr.length() >= *maxSize)
        {
            if(match->size() < wStr.size())
            {
                *match = wStr;
                *pos = currentPos;
            }
            return;
        }

        // If reached bounds of index
        if(wIndex >= wSize) 
        {
            inBuffer = 1;
            wIndex = *buffIndex; //start looking from beginning of buffer
        }

    }
}

void findLongestMatch(std::string* window, char* readBuffer,int* buffSize, int* index,
        int* maxLength,int* F, std::string* literal,int* length, int* offset)
{
    std::string longestMatch = "";
    std::string sub; // Substring in lookahead buffer
    int pos = 0; // pos of match

    int maxBuff = *index + *F;

    if(maxBuff >= *buffSize)
    {
        maxBuff = *buffSize - 1; 
    }
    

    // If buffer has one char left
    if(*index >= *buffSize-1)
    {
        sub = readBuffer[*index]; 
        // Add to literal and leave
        //addToLiteral(literal,sub);
        *literal = *literal + sub;
        advanceBuffers(window,readBuffer,index,&maxBuff,1);
        return;
    }
    // Otherwise, get 2 chars and find longest match
    sub = readBuffer[*index];
    sub += readBuffer[*index + 1];
    pos = window->find(sub);

    // No match, add to literal
    if(pos < 0)
    {
        //addToLiteral(literal,sub);
        *literal = *literal + sub;
        advanceBuffers(window,readBuffer,index,&maxBuff,1);
        *length = 0;
        *offset = 0;
        return;
    }

    // Match continually
    int tempPos = pos;
    longestMatch = sub;
    do
    {
        continueMatch(window,tempPos,&pos,readBuffer,index,&longestMatch,maxLength,maxBuff);
        tempPos = window->find(longestMatch,pos+1);

    }while(pos >= 0);
        
    *length = longestMatch.size();
    *offset = pos;

}

void encode(char* readBuffer,int readSize, char* writeBuffer,int writeSize,int N, int L,int S)
{
    int W = pow(2,N);
    int F = pow(2,L)-1;
    int maxLength = pow(2,S)-1;

    std::string window = "";
    window.resize(W);
    int lookIndex = 0; // This is the beginning of the lookahead buffer

    std::string literal = ""; //this is the cache of writing string literals
    int length = 0; // Length in token double
    int offset = 0; // offset in token double

    while(1)
    {
        //Check if string literal is of max size
        // If so, output triple
        // writebits(triple)
        
        if(literal.size() == maxLength)
        {
            std::cout << "reached max size for literal" << std::endl;
            std::cout << "String literal: " << literal << std::endl;
            literal = "";
        }

        findLongestMatch(&window,readBuffer,&readSize,&lookIndex,&maxLength,
                &F,&literal,&length,&offset);     

        if(length < 2)
        {
            /*
            //Output string literal 
            std::cout << "String literal: " << literal << std::endl;
            literal = "";
            */
        }
        else
        {
            //Check if string literal is empty.
            // If not output triple, then double
            if(literal.compare(""))
            {
                std::cout << "String literal: " << literal << std::endl;
                literal = "";
            }
            std::cout << "Token Double: <" << length << "," << offset << ">" << std::endl;
            //advanceBuffers(&window,readBuffer,&lookIndex,&readSize,length);
        }
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

    

    long fileSize = getSize(fileName);
    char readBuffer[fileSize];

    int writeSize = 0;
    char writeBuffer[fileSize];

    if(fileSize < 0) // Negative num means error occurred opening file
    {
        return 1;
    }

    getBuffer(fileName,readBuffer,fileSize);

    /*
    //Output stuff to file
    std::cout.write(reinterpret_cast<char*>(&N),1);
    std::cout.write(reinterpret_cast<char*>(&L),1);
    std::cout.write(reinterpret_cast<char*>(&S),1);
    */

    //For debugging only:
    char* testBuffer = "abbaabbbabab";
    encode(testBuffer,12,writeBuffer,writeSize,N,L,S);
    
    //encode(readBuffer,fileSize,writeBuffer,writeSize,N,L,S);

    return 0;
}
