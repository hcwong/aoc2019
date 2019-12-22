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
    int prev = x % 10;
    int remainder = x / 10;
    int curr = remainder % 10;
    bool hasAdjacent = prev == curr;

    if (prev < curr) {
        return false;
    }

    while (remainder > 0) {
        remainder /= 10;
        prev = curr;
        curr = remainder % 10;
        hasAdjacent = hasAdjacent ? hasAdjacent : prev == curr;
        if (prev < curr) {
            return false;
        }
    }
    
    return hasAdjacent;
}