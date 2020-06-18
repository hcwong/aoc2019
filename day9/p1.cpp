#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "../utils.hpp"

struct state {
    long idx;
    long relativeBase;
};

int processOpcode(std::vector<long>&, state *s);
long accessNums(std::vector<long>& nums, long idx);
std::vector<long> getOpParam(long);

int main() {
    std::vector<long> nums;

    std::string line = getInput("input1.txt");
    nums = splitLong(line, ',');
    state s = {0, 0};

    while (true) {
        long res = processOpcode(nums, &s);
        if (res == -1) {
            std::cout << "Error occured";
            return 1;
        } else if (res == 1) {
            std::cout << "99 encountered";
            return 1;
        }
    }
}

long accessNums(std::vector<long> &nums, long idx) {
    if (idx >= nums.size()) {
        return 0;
    } else {
        return nums[idx];
    }
}

int processOpcode(std::vector<long> &nums, state *s) {
    long val = accessNums(nums, s->idx);
    std::vector<long> paramCodes = getOpParam(val);
    std::vector<long> values;
    long opcode = paramCodes[0];

    // Translates the param codes to actual values depending on the mode
    for (long i = 1; i < paramCodes.size(); i++) {
        long parameter = paramCodes[i];
        if (parameter == 0) {
            // Position mode
            values.push_back(accessNums(nums, accessNums(nums, s->idx + i)));
        } else if (parameter == 1){
            // Immediate mode
            values.push_back(accessNums(nums, s->idx + i));
        } else {
            // Relative mode
            // Functions like position mode, but we take longo account the relativeBase
            values.push_back(accessNums(nums, accessNums(nums, s->idx + i + s->relativeBase)));
        }
    }

    long sum, mult, input;
    switch(opcode) {
        // Ignore the last digit
        case 1:
            sum = 0;
            for (long i = 0; i < 2; i++) {
                sum += values[i];
            }
            nums[accessNums(nums, s->idx + 3)] = sum;
            s->idx += 4;
            return 0;
        case 2:
            mult = 1;
            for (long i = 0; i < 2; i++) {
                mult *= values[i];
            }
            nums[accessNums(nums, s->idx + 3)] = mult;
            s->idx += 4;
            return 0;
        case 3:
            std::cout << "Give me the input: ";
            std::cin >> input;
            nums[accessNums(nums, s->idx + 1)] = input;
            s->idx += 2;
            return 0;
        case 4: 
            //std::cout << nums[nums[s->idx + 1]];
            std::cout << values[0] << '\n';
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
                nums[accessNums(nums, s->idx + 3)] = 1;
            } else {
                nums[accessNums(nums, s->idx + 3)] = 0;
            }
            s->idx += 4;
            return 0;
        case 8:
            if (values[0] == values[1]) {
                nums[accessNums(nums, s->idx + 3)] = 1;
            } else {
                nums[accessNums(nums, s->idx + 3)] = 0;
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
std::vector<long> getOpParam(long val) {
    std::vector<long> paramCodes;
    long opcode = val % 100;
    paramCodes.push_back(opcode);
    val /= 100;
    
    while (val > 0) {
        long last = val % 10;
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
