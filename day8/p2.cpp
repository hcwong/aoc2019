#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "../utils.hpp"

const int LENGTH = 25;
const int HEIGHT = 6;
const int LAYER_LENGTH = LENGTH * HEIGHT;

void run(std::vector<int> &nums);
void parseLayer(std::vector<int>::iterator *it, int matrix[HEIGHT][LENGTH]);
void printMatrix(int matrix[HEIGHT][LENGTH]);
std::string printAnsi(int cell);

int main() {
    std::string line = getInput("input1.txt");
    std::vector<int> nums;
    for (std::vector<char>::iterator it = line.begin(); it != line.end(); it++) {
        nums.push_back(*it - '0');
    }

    run(nums);
}

// Gets the counts of the max ones and zeros in each layer
void run(std::vector<int> &nums) {
    int matrix[HEIGHT][LENGTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < LENGTH; j++) {
            matrix[i][j] = 2;
        }
    }

    std::vector<int>::iterator it = nums.begin();
    while (it != nums.end()) {
        parseLayer(&it, matrix);
    }

    printMatrix(matrix);
}

void parseLayer(std::vector<int>::iterator *it, int matrix[HEIGHT][LENGTH]) {
    int idx = 0;
    while (idx < LAYER_LENGTH) {
        int i = idx / LENGTH;
        int j = idx % LENGTH;
        // If transparent, go ahead, else skip
        if (matrix[i][j] == 2) {
            matrix[i][j] = **it;
        }
        idx++;
        (*it)++;
    }
}

void printMatrix(int matrix[HEIGHT][LENGTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < LENGTH; j++) {
            std::cout << printAnsi(matrix[i][j]);
        }
        std::cout << "\n";
    }
}

std::string printAnsi(int digit) {
    if (digit == 0) {
        return "\u001b[40;1m  \u001b[0m";
    } else if (digit == 1) {
        return "\u001b[107;1m  \u001b[0m";
    } else {
        return "\u001b[41;1m  \u001b[0m";
    }
}
