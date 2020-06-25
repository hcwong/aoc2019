#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include "../utils.hpp"

struct state {
    long idx;
    long relativeBase;
    std::map<long, long> numsMap;
    std::map<long, std::map<long, int>> visited;
    long x;
    long y;
    long direction;
};

int processOpcode(std::vector<long>&, state *s);
long accessNums(std::vector<long>& nums, long idx, state *s);
void insertNums(std::vector<long> &nums, long idx, long val, state *s);
std::vector<long> getOpParam(long);

int main() {
    std::vector<long> nums;
    std::map<long, long> numsMap;
    std::map<long, std::map<long, int>> visited;

    std::string line = getInput("input1.txt");
    nums = splitLong(line, ',');

    state s = {0, 0, numsMap, visited, 0, 0};

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

long accessNums(std::vector<long> &nums, long idx, state *s) {
    if (idx >= nums.size()) {
        // If found, get the value, else insert the value into the specified spot
        if (s->numsMap.count(idx) == 1) {
            auto it = s->numsMap.find(idx);
            return it->second;
        } else {
            s->numsMap[idx] = 0;
            return s->numsMap[idx];
        }
    } else {
        return nums[idx];
    }
}

void insertNums(std::vector<long> &nums, long idx, long val, state *s) {
    if (idx >= nums.size()) {
        s->numsMap[idx] = val;
    } else {
        nums[idx] = val;
    }
}

int processOpcode(std::vector<long> &nums, state *s) {
    long val = accessNums(nums, s->idx, s);
    std::vector<long> paramCodes = getOpParam(val);
    std::vector<long> values;
    long opcode = paramCodes[0];
    long paramsWithInsert[5] = {1,2,3,7,8};

    // Translates the param codes to actual values depending on the mode
    for (long i = 1; i < paramCodes.size(); i++) {
        long parameter = paramCodes[i];
        if (parameter == 0) {
            // Write action
            if (std::find(paramsWithInsert, std::end(paramsWithInsert), opcode) != std::end(paramsWithInsert) && i == paramCodes.size() - 1) {
                values.push_back(accessNums(nums, s->idx + i, s));
            } else {
                values.push_back(accessNums(nums, accessNums(nums, s->idx + i, s), s));
            }
        } else if (parameter == 1){
            // Immediate mode
            values.push_back(accessNums(nums, s->idx + i, s));
        } else {
            // Write action
            if (std::find(paramsWithInsert, std::end(paramsWithInsert), opcode) != std::end(paramsWithInsert) && i == paramCodes.size() - 1) {
                values.push_back(accessNums(nums, s->idx + i, s) + s->relativeBase);
            } else {
                values.push_back(accessNums(nums, accessNums(nums, s->idx + i, s) + s->relativeBase, s));
            }
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
            insertNums(nums, values[2], sum, s);
            s->idx += 4;
            return 0;
        case 2:
            mult = 1;
            for (long i = 0; i < 2; i++) {
                mult *= values[i];
            }
            insertNums(nums, values[2], mult, s);
            s->idx += 4;
            return 0;
        case 3:
            // TODO change
            std::cout << "Give me the input: ";
            std::cin >> input;
            insertNums(nums, values[0], input, s);
            s->idx += 2;
            return 3;
        case 4: 
            // TODO change
            std::cout << values[0] << '\n';
            s->idx += 2;
            return 4;
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
                insertNums(nums, values[2], 1, s);
            } else {
                insertNums(nums, values[2], 0, s);
            }
            s->idx += 4;
            return 0;
        case 8:
            if (values[0] == values[1]) {
                insertNums(nums, values[2], 1, s);
            } else {
                insertNums(nums, values[2], 0, s);
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
