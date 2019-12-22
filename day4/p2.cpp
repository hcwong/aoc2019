#include <iostream>

bool isMeetCriteria(int);

int main() {
    // Brute force solution :(
    int lowerBound = 359282;
    int upperBound = 820401;
    int count = 0;

    for (int i = lowerBound; i <= upperBound; i++) {
        if (isMeetCriteria(i)) {
            count++;
        }
    }

    std::cout << count; 
}

bool isMeetCriteria (int x) {
    int digits[6];
    digits[5] = x % 10;
    int curr = x / 10;
    
    // Check for decreasing order
    for (int i = 4; i >= 0; i--) {
        digits[i] = curr % 10;
        curr /= 10;
        if (digits[i] > digits[i+1]) {
            return false;
        }
    }

    // Check for adjacent and not larger order
    int repeatedLength = 1;
    int hasAdjacentOfTwo = 0;

    for (int i = 1; i < 6; i++) {
        if (digits[i] == digits[i-1]) {
            repeatedLength++;
            if (repeatedLength == 2) {
                hasAdjacentOfTwo++;
            } else if (repeatedLength == 3) {
                hasAdjacentOfTwo--;
            }           
        } else {
            repeatedLength = 1;
        }
    }

    return hasAdjacentOfTwo > 0;
}