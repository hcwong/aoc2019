#include <fstream>
#include <iostream>
#include <string>

int calcFuel(int value, int acc);

int main() {
    int sum = 0;
    std::ifstream ifs ("input1.txt", std::ifstream::in);
    std::string line;

    while (ifs.good()) {
        getline(ifs, line);
        int value = std::stoi(line);
        sum += calcFuel(value, 0); 
    }
    std::cout << sum;
}

int calcFuel(int value, int acc) {
    int next = value / 3 - 2;
    if (next <= 0) {
        return acc;        
    } else {
        return calcFuel(next, acc + next);
    }
}