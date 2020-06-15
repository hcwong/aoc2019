#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "../utils.hpp"

int LENGTH = 25;
int HEIGHT = 6;
int LAYER_LENGTH = LENGTH * HEIGHT;

struct res {
    int zeroes, ones, twos;
};

int getMaxCounts(std::vector<int> &nums);
res getLayerCounts(std::vector<int>::iterator *it);

int main() {
    std::string line = getInput("input1.txt");
    std::vector<int> nums;
    for (std::vector<char>::iterator it = line.begin(); it != line.end(); it++) {
        nums.push_back(*it - '0');
    }
    
    std::cout << getMaxCounts(nums);
}

// Gets the counts of the max ones and zeros in each layer
int getMaxCounts(std::vector<int> &nums) {
    // Initialize dummy variable
    res correctLayer = {LAYER_LENGTH + 1, -1, -1};
    std::vector<int>::iterator it = nums.begin();
    while (it != nums.end()) {
        res currLayer = getLayerCounts(&it);
        if (currLayer.zeroes < correctLayer.zeroes) {
            correctLayer = currLayer;
        }
    }
    return correctLayer.ones * correctLayer.twos;
}

res getLayerCounts(std::vector<int>::iterator *it) {
    int idx = 0;
    res results = {0, 0, 0};
    while (idx < LAYER_LENGTH) {
        if (**it == 0) {
            results.zeroes++;
        }  else if (**it == 1) {
            results.ones++;
        } else if (**it == 2) {
            results.twos++;
        }
        idx++;
        (*it)++;
    }
    return results;
}
