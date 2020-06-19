#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> split(std::string s, char delim) {
    std::string tmp;
    std::vector<int> nums;
    std::stringstream ss(s);

    while (getline(ss, tmp, delim)) {
       nums.push_back(std::stoi(tmp)); 
    }
    return nums;
}

std::vector<long> splitLong(std::string s, char delim) {
    std::string tmp;
    std::vector<long> nums;
    std::stringstream ss(s);

    while (getline(ss, tmp, delim)) {
       nums.push_back(std::stol(tmp)); 
    }
    return nums;
}

std::string getInput(std::string filepath) {
    std::ifstream ifs (filepath, std::ifstream::in);
    std::string line;
    getline(ifs, line);
    return line;
}

