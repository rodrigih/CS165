#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "DoublyLinkedList.h"

/* Checks the number of arguments*/
int checkArgs(int argc)
{    
    if(argc > 2)
    {
        std::cerr << "Too many arguments. Exiting Program" << std::endl;
        return 1;
    }
 
    return 0;
}

/* Increments the index of the buffer. 
 * If the index goes out of range of the buffer, the file is read from
 * again and the index is reset to 0; 
 */
void getNextChar(std::istream* input, char* buffer, int F, int* index)
{
    ++(*index);

    if(*(index) > (F -1))
    {
        input->read(buffer,F);
        *index = 0;
    }
}

/* Reads bits of the current byte starting from the bit index.
 * If the bit index goes out of range, the next character is read
 */
void readBits(std::istream* input, int n, char* buffer, int F,
        int* charIndex,int* bits,int* bitIndex)
{
    unsigned char c = buffer[*charIndex];
    int i = 0;
    while(n != 0)
    {
        if(*(bitIndex) > 7)
        {
            getNextChar(input,buffer,F,charIndex);
            c = buffer[*charIndex];
            *bitIndex = 0;
        }   
        bits[i++] = (c >> (*bitIndex)++) & 1;
        --n;
    }
}

int bitToInt(int* bits, int length)
{
    int byte = 0;
    for(int i = 0; i < length; i++)
    {
        byte |= bits[i] << i;
    }

    return byte;
}

void expand(std::istream* input,int N, int L, int S)
{
    //Calculate variables using N, L, and S
    int W = pow(2,N);
    int F = pow(2,L)-1;

    DoublyLinkedList window(W);
    int bits[N]; // Check if this causes segFault
    int bitIndex = 0; // position of next bit to read in "byte"
    int charIndex = 0; // position of next char to read in buffer

    char buffer[F]; // Check if this causes segFault

    int length;
    int offset;

    int flag = 1;
    //Read initial chars
    input->read(buffer,F);
    while(flag) //File Not empty
    {
        readBits(input,L,buffer,F,&charIndex,bits,&bitIndex);

        length = bitToInt(bits,L);


        if(length > 0)
        {
            length++;
            // Read N bits to get offset
            readBits(input,N,buffer,F,&charIndex,bits,&bitIndex);
            offset = bitToInt(bits,N);
            // output len character 
            window.copyChars(length,offset); // Automatically shifts forward
        }
        else
        {
            // read S bits to get strlen 
            readBits(input,S,buffer,F,&charIndex,bits,&bitIndex);
            length = bitToInt(bits,S);
            // if strlen is zero, exit program
            if(length == 0)
            {
                flag = !(flag); 
                continue;
            }
            // Otherwise, read and output strlen 8-bit characters
            for(int i = 0; i < length;i++)
            {
                char c = 0; //reset char after each read
                readBits(input,8,buffer,F,&charIndex,bits,&bitIndex);
                c = bitToInt(bits,8);
                window.addNode(c); // Automatically shifts window
                std::cout << c;
            }
        }
    }

    window.printList();
}

int main(int argc, char* argv[])
{
    // Decoder variables
    int N;
    int L;
    int S;

    char params[3];

    // I/O streams
    std::ifstream inFile;
    std::istream* input = &std::cin;
    
    // Check number of arguments
    if(checkArgs(argc) > 0)
        return 1;
        
    // If file exists, open it and use as input
    if(argc == 2)
    {
        inFile.open(argv[1], std::ios::in | std::ios:: binary);
        if(inFile.good())
            input = &inFile;
        else
        {
            std::cerr << "Error opening: " << argv[1] << std::endl 
                << "Closing program" << std::endl;
            return 1;
        }
    }

    // First read in 3 Bytes to get the N,L, and S parameters
    input->read(params,3);
    N = params[0];
    L = params[1];
    S = params[2];

    //For debugging only
    std::cout << "N: " << N << std::endl;
    std::cout << "L: " << L << std::endl;
    std::cout << "S: " << S << std::endl;

    expand(input,N,L,S);
    inFile.close();
}

