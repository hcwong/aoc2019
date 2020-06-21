#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <math.h>

struct coord {
    long x;
    long y;
    double vec_x;
    double vec_y;

    bool operator <(const coord &b) const {
        double magnitude_a = (vec_x * vec_x) + (vec_y * vec_y);
        double magnitude_b = (b.vec_x * b.vec_x) + (b.vec_y * b.vec_y);

        if (magnitude_a > magnitude_b) {
            return false;
        } else {
            return true;
        }
    }
};

std::vector<coord> getInput(std::string filepath);
void parseLine(std::vector<coord> &vec, std::string s, int x_val);
std::map<double, std::vector<coord>> getAsteroids(std::vector<coord> &coords, std::vector<coord>::iterator station);
double convertToDeg(double arctan_val);
bool magnitudeComparator(coord *a, coord *b);

int main() {
    std::vector<coord> asteroids = getInput("input.txt");
    
    int max_asteroids = 0;
    int idx = 0;
    std::vector<coord>::iterator it;
    std::map<double, std::vector<coord>> asteroid_map;
    coord station;

    for (it = asteroids.begin(); it != asteroids.end(); it++) {
        std::map<double, std::vector<coord>> curr_map = getAsteroids(asteroids, it);
        if (curr_map.size() > max_asteroids) {
            max_asteroids = curr_map.size();
            asteroid_map = curr_map;
            station = *it;
            idx = it - asteroids.begin();
        }
    }

    // Remove the station from vector
    asteroids.erase(asteroids.begin() + idx);
    int asteroids_destroyed = 0;
    std::map<double, std::vector<coord>>::iterator mapit;

    // Sort all the asteroids in each arctan by magnitude
    for (mapit = asteroid_map.begin(); mapit != asteroid_map.end(); mapit++) {
        // Iterate through the asteroids and get the magnitude
        for (it = mapit->second.begin(); it != mapit->second.end(); it++) {
            it->vec_x = it->x - station.x;
            it->vec_y = it->y - station.y;
        }

        std::sort(mapit->second.begin(), mapit->second.end());

        //for (it = mapit->second.begin(); it != mapit->second.end(); it++) {
            //std::cout << it->vec_x * it->vec_x + it->vec_y * it->vec_y << "\n";
        //}
        //std::cout << "Done\n";
    }

    while (true) {
        for (mapit = asteroid_map.begin(); mapit != asteroid_map.end(); mapit++) {
            asteroids_destroyed++;

            if (asteroids_destroyed == 200) {
                // stop and print next asteroid to destroy
                coord target = *mapit->second.begin();
                std::cout << target.x << " " << target.y << "\n";
                return 0;
            }

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

std::map<double, std::vector<coord>> getAsteroids(std::vector<coord> &coords, std::vector<coord>::iterator station) {
    int asteroids_seen = 0;
    std::vector<coord>::iterator it;
    std::map<double, std::vector<coord>> seen;

    for (it = coords.begin(); it != coords.end(); it++) {
        if (it == station) {
            continue;
        }

        double vec_x = it->x - station->x;
        double vec_y = it->y - station->y;
        // NOTE: vec_x and vec_y switch places cos x is the y_value
        // NOTE: -vec_x because index 0 is a the top. FML
        double arctan_val = atan2(-vec_x, vec_y);
        double deg = convertToDeg(arctan_val);
        if (seen.count(deg) == 0) {
            std::vector<coord> values;
            values.push_back(*it);
            seen.insert(std::pair<double, std::vector<coord>>(deg, values));
            asteroids_seen++;
        } else {
            seen.find(deg)->second.push_back(*it);
        }
    }

    return seen;
}

double convertToDeg(double arctan_val) {
    double deg = arctan_val * 180 / M_PI;

    // First quadrant
    if (deg >= 0 && deg <= 90) {
        return 90 - deg;
    } else if (deg < 0) {
        // 2nd quadrant and 3rd quadrant
        return abs(deg) + 90;
    } else {
        return 450 - deg;
    }
}
