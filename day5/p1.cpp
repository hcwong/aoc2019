#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<int> split(std::string);
int processOpcode(std::vector<int>&, int index);
std::vector<int> getOpParam(int);

int main() {
    std::ifstream ifs ("input1.txt", std::ifstream::in);
    std::string line;
    getline(ifs, line);
    std::vector<int> nums;

    nums = split(line);
    int len = nums.size();
    int i = 0;

    while (i + 3 < len) {
        if (nums[i] == 99) {
            break;
        }

        int res = processOpcode(nums, i);
        if (res == -1) {
            std::cout << "Error occured";
            return 1;
        }
        i += res;
    }
    std::cout << nums[0];
}

std::vector<int> split(std::string s) {
    std::string tmp;
    std::vector<int> nums;
    std::stringstream ss(s);

    while (getline(ss, tmp, ',')) {
       nums.push_back(std::stoi(tmp)); 
    }
    return nums;
}

int processOpcode(std::vector<int> &nums, int index) {
    int val = nums[index];
    std::vector<int> paramCodes = getOpParam(val);
    std::vector<int> values;
    int opcode = paramCodes[0];

    for (int i = 1; i < paramCodes.size(); i++) {
        int parameter = paramCodes[i];
        if (parameter == 0) {
            values.push_back(nums[nums[index + i]]);
        } else {
            values.push_back(nums[index + i]);
        }
    }

    int sum, mult, input;
    switch(opcode) {
        case 1:
            sum = 0;
            for (int i = 0; i < values.size() - 1; i++) {
                sum += values[i];
            }
            nums[values[values.size() - 1]] = sum;
            break;
        case 2:
            mult = 1;
            for (int i = 0; i < values.size() - 1; i++) {
                mult *= values[i];
            }
            nums[values[values.size() - 1]] = mult;
            break;
        case 3:
            std::cout << "Give me the input: ";
            std::cin >> input;
            nums[values[0]] = input;
            return 2;
        case 4: 
            std::cout << values[0];
            return 2;
        case 99:
            break;
        default:
            return -1;
    }
    return 4;
}

// Gets the opcode and parameter codes
std::vector<int> getOpParam(int val) {
    std::vector<int> paramCodes;
    int opcode = val % 10;
    paramCodes.push_back(opcode);
    val /= 100;
    
    while (val > 0) {
        int last = val % 10;
        paramCodes.push_back(last);
        val /= 10;
    }

    return paramCodes;
}