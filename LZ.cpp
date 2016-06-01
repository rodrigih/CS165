#include <iostream>
#include <string>
#include <cmath>
#include <map>
#include <vector>
#include "ParamChecker.h"

void encode(int, int, int, const std::string);
int search(std::string, std::map<std::string, int>);

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
	int S2 = pow(2, S) - 1;

	//initialize dictionary
	std::map<std::string, int> dict;
	for(int i = 0; i < W-F; i++)
		dict[""] = 0;

	std::string result = "";
	std::string prefix = "";
	int i = 0;
	while(i < content.length()){

		std::string b = content.substr(i, 1); //next byte of input
		std::string prefixb = prefix + b;
//std::cout << prefix << "-" << prefixb << std::endl;
		int offset = search(prefixb, dict);
		//std::cout << offset << std::endl;

		if(offset != -1){ //if found
			prefix = prefixb;
			result += "(" + std::to_string(prefix.length()) + ", " + std::to_string(offset) + ")";//result += prefix;
		}
		else{
			dict[prefix] = prefix.length();
			result += "(0, " + std::to_string(prefix.length()) + ", " + prefix + ")";
			prefix = b;
		}
		//check prefix length, write if above max
		if(prefix.length() >= S){
			result += "(0, " + std::to_string(prefix.length()) + ", " + prefix + ")";
			prefix = b;
		}
		i++;
	}
	std::cout << result;
}

int search(const std::string s, const std::map<std::string, int> map){
	int len = 0;
	std::map<std::string, int> m = map;
	std::string str = s;

	for(std::map<std::string, int>::iterator it = m.begin(); it != m.end(); it++){
		std::cout << it->first << std::endl;
		if(str.compare(it->first) == 0){
			std::cout << "true";
			return len; //if found, grab current length (offset) and continue to see if match found later
		}
		len += it->second;
	}

	return -1;
}
