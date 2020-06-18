#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "../utils.hpp"

struct state {
    int idx;
    int relativeBase;
};

int processOpcode(std::vector<int>&, state *s);
std::vector<int> getOpParam(int);

int main() {
    std::vector<int> nums;

    std::string line = getInput("input1.txt");
    nums = split(line, ',');
    int len = nums.size();
    state s = {0, 0};

    while (s.idx < len) {
        int res = processOpcode(nums, &s);
        if (res == -1) {
            std::cout << "Error occured";
            return 1;
        } else if (res == 1) {
            std::cout << "99 encountered";
            return 1;
        }
    }
}

int processOpcode(std::vector<int> &nums, state *s) {
    int val = nums[s->idx];
    std::vector<int> paramCodes = getOpParam(val);
    std::vector<int> values;
    int opcode = paramCodes[0];

    // Translates the param codes to actual values depending on the mode
    for (int i = 1; i < paramCodes.size(); i++) {
        int parameter = paramCodes[i];
        if (parameter == 0) {
            // Position mode
            values.push_back(nums[nums[s->idx + i]]);
        } else if (parameter == 1){
            // Immediate mode
            values.push_back(nums[s->idx + i]);
        } else {
            // Relative mode
            values.push_back() 
        }
    }

    int sum, mult, input;
    switch(opcode) {
        // Ignore the last digit
        case 1:
            sum = 0;
            for (int i = 0; i < 2; i++) {
                sum += values[i];
            }
            nums[nums[s->idx + 3]] = sum;
            s->idx += 4;
            return 0;
        case 2:
            mult = 1;
            for (int i = 0; i < 2; i++) {
                mult *= values[i];
            }
            nums[nums[s->idx + 3]] = mult;
            s->idx += 4;
            return 0;
        case 3:
            std::cout << "Give me the input: ";
            std::cin >> input;
            nums[nums[s->idx + 1]] = input;
            s->idx += 2;
            return 0;
        case 4: 
            std::cout << nums[nums[s->idx + 1]];
            std::cout << '\n';
            s->idx += 2;
            return 0;
        case 5:
            if (values[0] != 0) {
                s->idx = values[1];
            } else {
                s->idx += 3;
            }
            return 0;
        case 6:
            if (values[0] == 0) {
                s->idx = values[1];
            } else {
                s->idx += 3;
            }
            return 0;
        case 7:
            if (values[0] < values[1]) {
                nums[nums[s->idx + 3]] = 1;
            } else {
                nums[nums[s->idx + 3]] = 0;
            }
            s->idx += 4;
            return 0;
        case 8:
            if (values[0] == values[1]) {
                nums[nums[s->idx + 3]] = 1;
            } else {
                nums[nums[s->idx + 3]] = 0;
            }
            s->idx += 4;
            return 0;
        case 9:
            s->relativeBase += values[0];
            s->idx += 2;
            return 0;
        case 99:
            return 1;
        default:
            return -1;
    }
    return -1;
}

// Gets the opcode and parameter codes
std::vector<int> getOpParam(int val) {
    std::vector<int> paramCodes;
    int opcode = val % 100;
    paramCodes.push_back(opcode);
    val /= 100;
    
    while (val > 0) {
        int last = val % 10;
        paramCodes.push_back(last);
        val /= 10;
    }

    if (opcode <= 2 || opcode == 7 || opcode == 8) {
        while (paramCodes.size() < 4) {
            paramCodes.push_back(0);
        }
    } else if (opcode == 5 || opcode == 6) {
        while (paramCodes.size() < 3) {
            paramCodes.push_back(0);
        }
    } else {
        while (paramCodes.size() < 2) {
            paramCodes.push_back(0);
        }
    }

    return paramCodes;
}
