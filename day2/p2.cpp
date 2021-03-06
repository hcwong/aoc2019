#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<int> split(std::string);
bool processOpcode(std::vector<int>&, int);
bool checkInputs(std::vector<int>, int, int);

int main() {
    std::ifstream ifs ("input2.txt", std::ifstream::in);
    std::string line;
    getline(ifs, line);
    std::vector<int> nums;

    nums = split(line);
    for (int noun = 0; noun < 100; noun++) {
        for (int verb = 0; verb < 100; verb++) {
            if (checkInputs(nums, noun, verb)) {
                return 0;
            }
        }
    }
    return 1;
}

bool checkInputs(std::vector<int> nums, int noun, int verb) {
    int len = nums.size();
    int i = 0;
    nums[1] = noun;
    nums[2] = verb;

    while (i + 3 < len) {
        if (nums[i] == 99) {
            break;
        }

        bool res = processOpcode(nums, i);
        if (!res) {
            std::cout << "Error occured";
            return 1;
        }
        i += 4;
    }
    if (nums[0] != 19690720) {
        return false;
    }

    std::cout << 100 * noun + verb;
    return true;
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

bool processOpcode(std::vector<int> &nums, int index) {
    int opcode = nums[index];
    int pos1 = nums[index + 1];
    int pos2 = nums[index + 2];
    int pos3 = nums[index + 3];

    switch(opcode) {
        case 1:
            nums[pos3] = nums[pos1] + nums[pos2];
            break;
        case 2:
            nums[pos3] = nums[pos1] * nums[pos2];
            break;
        case 99:
            break;
        default:
            return false;
    }
    return true;
}