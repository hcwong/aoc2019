#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <math.h>

struct coord {
    long x;
    long y;
};

std::vector<coord> getInput(std::string filepath);
void parseLine(std::vector<coord> &vec, std::string s, int x_val);
int getAsteroids(std::vector<coord> &coords, std::vector<coord>::iterator station);

int main() {
    std::vector<coord> asteroids = getInput("input.txt");
    
    int max_asteroids = 0;
    std::vector<coord>::iterator it;

    for (it = asteroids.begin(); it != asteroids.end(); it++) {
        max_asteroids = std::max(max_asteroids, getAsteroids(asteroids, it));
    }

    std::cout << max_asteroids << "\n";
}

std::vector<coord> getInput(std::string filepath) {
    std::ifstream ifs(filepath, std::ifstream::in);
    std::vector<coord> coords;
    std::string line;
    int x_val = 0;

    while (getline(ifs, line)) {
        parseLine(coords, line, x_val);
        x_val++;
    }

    return coords;
}

void parseLine(std::vector<coord> &vec, std::string s, int x_val) {
    std::vector<char>::iterator it;

    for (it = s.begin(); it != s.end(); it++) {
        if (*it == '#') {
            vec.push_back(coord{x_val, it - s.begin()});
        }
    }
}

int getAsteroids(std::vector<coord> &coords, std::vector<coord>::iterator station) {
    int asteroids_seen = 0;
    std::vector<coord>::iterator it;
    std::map<double, double> seen;

    for (it = coords.begin(); it != coords.end(); it++) {
        if (it == station) {
            continue;
        }

        double vec_x = it->x - station->x;
        double vec_y = it->y - station->y;
        double arctan_val = atan2(vec_y, vec_x);
        if (seen.count(arctan_val) == 0) {
            seen[arctan_val] = 1;
            asteroids_seen++;
        } 
    }

    return asteroids_seen;
}
