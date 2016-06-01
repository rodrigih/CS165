#include <iostream>
#include <string>
#include <cmath>
#include <map>
#include <vector>
#include "ParamChecker.h"

void encode(int, int, int, const std::string);
int main(int argc, char* argv[])
{
    int N = 11; //number of bits to encode window offset (9, 14, 11)
    int L = 4; //number of bits to encode match length (3, 4, 4)
    int S = 3; //number of bits to encode length of literal string (1, 5, 3)
    std::string fileName;

    // Check parameters for errors
    if(checkArgs(&N,&L,&S,&fileName,argc,argv) > 0)
       std::cerr << "Exiting Program" << std::endl;

    // For debugging only
    std::cout << "N = " << N << std::endl;
    std::cout << "L = " << L << std::endl;
    std::cout << "S = " << S << std::endl;
    std::cout << "FILE = " << fileName << std::endl;

    std::string c = "abbaabbbabab";
    encode(N, L, S, c);
}

void encode(int N, int L, int S, std::string content){
	int W = pow(2, N);
	int F = pow(2, L) - 1;


	std::map<std::string, std::string> dict; //index, value
	std::map<std::string, std::string>::iterator it;
	std::string output;

	std::string w = content.substr(0, 1);
	int i = 1;
	while(i < content.length()){
		int length = w.length(), offset = 0;
		bool found = false;

		std::string c = content.substr(i, 1);
		std::string wc = w + c;

		for(it = dict.begin(); it != dict.end(); it++){
			if(it->first == wc){
				w = wc;
				found = true;
				output += std::to_string(length);
				output += std::to_string(offset);
			}
			offset++;
		}
		if(!found){
			output += std::to_string(length);
			output += std::to_string(offset);
			output += w;
			dict[wc] = std::to_string(length);
			dict[wc] += std::to_string(offset);
			w = c;
		}
		i++;
	}
	std::cout << content << std::endl;
	std::cout << output;
}
