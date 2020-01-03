#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Hacky fix
int output = 0;
int isFirstDone = false;

void runPermutation(std::vector<int> permutation);
void runAmplitude(int first, int second);
std::vector<int> getOpParam(int val);
std::vector<std::vector<int> > genPermutations(std::vector<int> array);
std::vector<int> split(std::string s);
int processOpcode(std::vector<int> &nums, int index, int input);

int main() {
    std::vector<int> values = {0, 1, 2, 3, 4};
    std::vector<std::vector<int> > permutations = genPermutations(values);
    int size = permutations.size();
    int maxOutput = 0;

    // For each pemutation, we run the 5 amplitudes on it and get the maximum thrust
    std::vector<std::vector<int> >::iterator it;
    for (it = permutations.begin(); it != permutations.end(); it++) {
        runPermutation(*it);
        if (output > maxOutput) {
            maxOutput = output;
        }
    }

    std::cout << maxOutput;
}

std::vector<std::vector<int> > genPermutations(std::vector<int> array) {
    std::vector<std::vector<int> > permutations;

    if (array.size() == 1) {
        permutations.push_back(array);
        return permutations;
    }

    for (int i = 0; i < array.size(); i++) {
        int curr = array[i];
        std::vector<int> front = std::vector<int> (array.begin(), array.begin() + i);
        std::vector<int> end = std::vector<int> (array.begin() + i + 1, array.end());
        front.insert(front.end(), end.begin(), end.end());
        std::vector<std::vector<int> > subPerms = genPermutations(front);
        for (int i = 0; i < subPerms.size(); i++) {
            subPerms[i].push_back(curr);
            permutations.push_back(subPerms[i]);
        }
    }

    return permutations;
}

void runPermutation(std::vector<int> permutation) {
    output = 0;
    for (int i = 0; i < permutation.size(); i++) {
        runAmplitude(permutation[i], output);
    }
}

void runAmplitude(int first, int second) {
    std::ifstream ifs ("input1.txt", std::ifstream::in);
    std::string line;
    getline(ifs, line);
    std::vector<int> nums;

    nums = split(line);
    int len = nums.size();
    int i = 0;

    isFirstDone = false;
    while (i < len) {
        int res = isFirstDone ? processOpcode(nums, i, second) : processOpcode(nums, i, first);
        if (res == -1) {
            std::cout << "Error occured";
            return;
        } else if (res == -2) {
            std::cout << "99 encountered";
            return;
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

int processOpcode(std::vector<int> &nums, int index, int input) {
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

    int sum, mult,a,b;
    
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
            isFirstDone = true;
            nums[nums[index + 1]] = input;
            return index + 2;
        case 4: 
            output = nums[nums[index + 1]];
            return index + 2;
        case 5:
            a = values[0];
            b = values[1];
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