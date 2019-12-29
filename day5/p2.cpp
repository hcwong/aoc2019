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

    while (i < len) {
        int res = processOpcode(nums, i);
        if (res == -1) {
            std::cout << "Error occured";
            return 1;
        } else if (res == -2) {
            std::cout << "99 encountered";
            return 1;
        }
        i = res;
    }
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
        // Ignore the last digit
        case 1:
            sum = 0;
            for (int i = 0; i < 2; i++) {
                sum += values[i];
            }
            nums[nums[index + 3]] = sum;
            break;
        case 2:
            mult = 1;
            for (int i = 0; i < 2; i++) {
                mult *= values[i];
            }
            nums[nums[index + 3]] = mult;
            break;
        case 3:
            std::cout << "Give me the input: ";
            std::cin >> input;
            nums[nums[index + 1]] = input;
            return index + 2;
        case 4: 
            std::cout << nums[nums[index + 1]];
            std::cout << '\n';
            return index + 2;
        case 5:
            if (values[0] != 0) {
                return values[1];
            } else {
                return index + 3;
            }
        case 6:
            if (values[0] == 0) {
                return values[1];
            } else {
                return index + 3;
            }
        case 7:
            if (values[0] < values[1]) {
                nums[nums[index + 3]] = 1;
            } else {
                nums[nums[index + 3]] = 0;
            }
            break;
        case 8:
            if (values[0] == values[1]) {
                nums[nums[index + 3]] = 1;
            } else {
                nums[nums[index + 3]] = 0;
            }
            break;
        case 99:
            return -2;
        default:
            return -1;
    }
    return index + 4;
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