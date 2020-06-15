#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

struct amp {
  int idx;
  int phase;
  int number;
  int input;
  // Input Status
  // 2: Use phase settings
  // 1: Use input
  // 0: Block
  int inputStatus;
  std::vector<int> nums;
};

int runPermutation(std::vector<int> permutation);
int runAmplitude(amp *, int idx);
std::vector<int> getOpParam(int val);
std::vector<std::vector<int> > genPermutations(std::vector<int> array);
std::vector<int> split(std::string s);
int processOpcode(amp *amp, int idx);

int main() {
    std::vector<int> values = {5, 6, 7, 8, 9};
    std::vector<std::vector<int> > permutations = genPermutations(values);
    int maxOutput = 0;

    // For each pemutation, we run the 5 amplitudes on it and get the maximum thrust
    std::vector<std::vector<int> >::iterator it;
    for (it = permutations.begin(); it != permutations.end(); it++) {
        int output = runPermutation(*it);
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

int runPermutation(std::vector<int> permutation) {
    amp amplitudes[5]; 
    std::ifstream ifs ("input1.txt", std::ifstream::in);
    std::string line;
    getline(ifs, line);
    std::vector<int> nums;
    nums = split(line);

    for (int i = 0; i < permutation.size(); i++) {
        amplitudes[i] = {0, permutation[i], i, 0, 2, std::vector<int>(nums)};
    }

    int idx = 0;

    while (true) {
      int res = runAmplitude(amplitudes, idx);
      if (res == -3 && idx == permutation.size() - 1) {
        break;
      }
      idx = (idx + 1) % 5;
    }
    std::cout << "reee\n";
    return amplitudes[0].input;
}

int runAmplitude(amp *amplitudes, int idx) {
    int res = processOpcode(amplitudes, idx);
    while (res == 0) {
      res = processOpcode(amplitudes, idx);
    }
    return res;
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

int processOpcode(amp *amplitudes, int idx) {
    amp *amplitude = &amplitudes[idx];
    amp nextAmplitude = amplitudes[(idx + 1) % 5];

    int val = amplitude->nums[amplitude->idx];
    std::vector<int> paramCodes = getOpParam(val);
    std::vector<int> values;
    int opcode = paramCodes[0];

    for (int i = 1; i < paramCodes.size(); i++) {
        int parameter = paramCodes[i];
        if (parameter == 0) {
            values.push_back(amplitude->nums[amplitude->nums[amplitude->idx + i]]);
        } else {
            values.push_back(amplitude->nums[amplitude->idx + i]);
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
            amplitude->nums[amplitude->nums[amplitude->idx + 3]] = sum;
            break;
        case 2:
            mult = 1;
            for (int i = 0; i < 2; i++) {
                mult *= values[i];
            }
            amplitude->nums[amplitude->nums[amplitude->idx + 3]] = mult;
            break;
        case 3:
            if (amplitude->inputStatus == 0) {
              return -1;
            }
            amplitude->nums[amplitude->nums[amplitude->idx + 1]] = amplitude->input == 2 ? amplitude->phase : amplitude->input;
            amplitude->inputStatus--;
            amplitude->idx += 2;
            return 0;
        case 4:
            nextAmplitude.input = amplitude->nums[amplitude->nums[amplitude->idx + 1]];
            if (nextAmplitude.inputStatus < 2) {
              nextAmplitude.inputStatus = 1;
            }
            amplitude->idx += 2;
            return -1;
        case 5:
            a = values[0];
            b = values[1];
            if (values[0] != 0) {
                amplitude->idx = values[1];
            } else {
               amplitude->idx += 3;
            }
            return 0;
        case 6:
            if (values[0] == 0) {
                amplitude->idx = values[1];
            } else {
                amplitude->idx += 3;
            }
            return 0;
        case 7:
            if (values[0] < values[1]) {
                amplitude->nums[amplitude->nums[amplitude->idx + 3]] = 1;
            } else {
                amplitude->nums[amplitude->nums[amplitude->idx + 3]] = 0;
            }
            break;
        case 8:
            if (values[0] == values[1]) {
                amplitude->nums[amplitude->nums[amplitude->idx + 3]] = 1;
            } else {
                amplitude->nums[amplitude->nums[amplitude->idx + 3]] = 0;
            }
            break;
        case 99:
            std::cout << amplitude->idx << "\n";
            return -3;
        default:
            return -4;
    }
    amplitude->idx += 4;
    return 0;
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
