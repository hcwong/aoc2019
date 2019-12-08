#include <fstream>
#include <iostream>
#include <string>

int calcFuel(int value);

int main() {
    int sum = 0;
    std::ifstream ifs ("input1.txt", std::ifstream::in);
    std::string line;

    while (ifs.good()) {
        getline(ifs, line);
        int value = std::stoi(line);
        sum += calcFuel(value); 
    }
    std::cout << sum;
}

int calcFuel(int value) {
    return value / 3 - 2;
}