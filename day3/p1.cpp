#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Coord {
    int x;
    int y;
};

std::vector<std::string> split(std::string);
// Coord updateCoord(Coord, std::string);
// int detectIntersection(Coord, Coord, Coord, Coord);
// bool isSameSign(int, int);

int main() {
    std::ifstream ifs ("input1.txt", std::ifstream::in);
    std::string line1;
    std::string line2;
    getline(ifs, line1);
    getline(ifs, line2);
    std::vector<std::string> coords1 = split(line1);
    std::vector<std::string> coords2 = split(line2);

    // Do a 20000 x 20000 matrix and hope for no seg fault
    int matrix[20000][20000];
    int minD = INT_MAX;
    int i = 1;

    // Coord c1 = {0, -789};
    // Coord c2 = {0, -606};
    // int length1 = coords1.size();
    // int length2 = coords2.size();
    // int minD = INT_MAX;
    // int i = 1;

    // while (i < length1 && i < length2) {
    //     Coord c1New = updateCoord(c1, coords1[i]);
    //     Coord c2New = updateCoord(c2, coords2[i]);
    //     int result = detectIntersection(c1, c2, c1New, c2New);
    //     c1 = c1New;
    //     c2 = c2New;

    //     if (result < minD) {
    //         minD = result;
    //     }
    //     i++;
    // }

    // std::cout << minD;
}

std::vector<std::string> split(std::string s) {
    std::string tmp;
    std::vector<std::string> values;
    std::stringstream ss(s);

    while (getline(ss, tmp, ',')) {
       values.push_back(tmp); 
    }
    return values;
}

void updateMatrix() {}

// int detectIntersection(Coord c1, Coord c2, Coord c1New, Coord c2New) {
//     if (isSameSign(c1.x - c2.x, c1New.x - c2.x) 
//         && isSameSign(c1.y - c2.y, c1New.y - c2New.y)) {
//         return INT_MAX;
//     }
    
//     int unchanged1, unchanged2;
//     if (c1.x == c1New.x) {
//         unchanged1 = c1.x;
//     } else {
//         unchanged1 = c1.y;
//     }
    
//     if (c2.x == c2New.x) {
//         unchanged2 = c2.x;
//     } else {
//         unchanged2 = c2.y;
//     }
//     return std::abs(unchanged1) + std::abs(unchanged2);
// }


// bool isSameSign(int x, int y) {
//     return (x > 0 && y > 0) || (x < 0 && y < 0);
// }

void updateCoord(int matrix[20000][20000], std::string input, Coord& c) {
    int value = std::stoi(input.substr(1, input.size()));
    switch(input[0]) {
        case 'R':
            
            c.x += value;
            break;
        case 'L':
            c.x -= value;
            break;
        case 'U':
            c.y += value;
            break;
        case 'D':
            c.y -= value;
            break;
    }
}