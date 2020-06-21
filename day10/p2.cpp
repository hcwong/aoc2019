#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <math.h>

struct coord {
    long x;
    long y;

    bool operator <(const coord &b) const {
        long magnitude_a = x * x + y * y;
        long magnitude_b = b.x * b.x + b.y * b.y;

        if (magnitude_a > magnitude_b) {
            return false;
        } else {
            return true;
        }
    }
};

double convertToDeg(double arctan_val);
struct cmpByDeg {
    bool operator()(const double& a, const double& b) const {
        if (convertToDeg(a) > convertToDeg(b)) {
            return false;
        } else {
            return true;
        }
    }
};

std::vector<coord> getInput(std::string filepath);
void parseLine(std::vector<coord> &vec, std::string s, int x_val);
std::map<double, std::vector<coord>, cmpByDeg> getAsteroids(std::vector<coord> &coords, std::vector<coord>::iterator station);
bool magnitudeComparator(coord *a, coord *b);

int main() {
    std::vector<coord> asteroids = getInput("input.txt");
    
    int max_asteroids = 0;
    int idx = 0;
    std::vector<coord>::iterator it;
    std::map<double, std::vector<coord>, cmpByDeg> asteroid_map;
    coord station;

    for (it = asteroids.begin(); it != asteroids.end(); it++) {
        std::map<double, std::vector<coord>, cmpByDeg> curr_map = getAsteroids(asteroids, it);
        if (curr_map.size() > max_asteroids) {
            max_asteroids = curr_map.size();
            asteroid_map = curr_map;
            station = *it;
            idx = it - asteroids.begin();
        }
    }
    
    std::cout << asteroid_map.size() << "\n";

    // Remove the station from vector
    asteroids.erase(asteroids.begin() + idx);
    int asteroids_destroyed = 0;
    std::map<double, std::vector<coord>, cmpByDeg>::iterator mapit;

    // We just need to sort by the keys of the map
    std::vector<double> keys;
    // Sort all the asteroids in each arctan by magnitude
    for (mapit = asteroid_map.begin(); mapit != asteroid_map.end(); mapit++) {
        std::sort(mapit->second.begin(), mapit->second.end());
        //keys.push_back(mapit->first);
        //std::cout << mapit->first << "\n";
    }
    //std::sort(keys.begin(), keys.end(), arctanComparator);

    while (true) {
        for (mapit = asteroid_map.begin(); mapit != asteroid_map.end(); mapit++) {
            // Grab the vector, and delete the asteroid there, along with the vector if necessary
            if (mapit->second.size() == 0) {
                continue;
            }

            if (asteroids_destroyed == 199) {
                // stop and print next asteroid to destroy
                coord target = *mapit->second.begin();
                std::cout << target.x << " " << target.y << "\n";
                return 0;
            }

            mapit->second.erase(mapit->second.begin());
            asteroids_destroyed++;
        }
    }
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
    std::map<double, std::vector<coord>> m;

    for (it = s.begin(); it != s.end(); it++) {
        if (*it == '#') {
            vec.push_back(coord{x_val, it - s.begin()});
        }
    }
}

std::map<double, std::vector<coord>, cmpByDeg> getAsteroids(std::vector<coord> &coords, std::vector<coord>::iterator station) {
    int asteroids_seen = 0;
    std::vector<coord>::iterator it;
    std::map<double, std::vector<coord>, cmpByDeg> seen;

    for (it = coords.begin(); it != coords.end(); it++) {
        if (it == station) {
            continue;
        }

        double vec_x = it->x - station->x;
        double vec_y = it->y - station->y;
        double arctan_val = atan2(vec_y, vec_x);
        if (seen.count(arctan_val) == 0) {
            std::vector<coord> values;
            values.push_back(*it);
            seen.insert(std::pair<double, std::vector<coord>>(arctan_val, values));
            asteroids_seen++;
        } else {
            seen.find(arctan_val)->second.push_back(*it);
        }
    }

    if (seen.size() == 328) {
        std::cout << asteroids_seen << "\n";
    }
    return seen;
}

double convertToDeg(double arctan_val) {
    double deg = arctan_val * 180 / M_PI;

    // First quadrant
    if (deg >= 0 && deg <= 90) {
        return abs(deg - 90);;
    } else if (deg < 0) {
        // 2nd quadrant and 3rd quadrant
        return abs(deg) + 90;
    } else {
        return 450 - deg;
    }
}
