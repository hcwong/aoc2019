#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "../utils.hpp"

struct Position {
    long x;
    long y;
    long z;
};

struct Velocity {
    long x;
    long y;
    long z;
};

struct Moon {
    Position position;
    Velocity velocity;
};

std::vector<std::string> splitString(std::string s, char delim);
void parseLine(std::vector<Position> &vec, std::string s);
void compareMoons(Moon *moon1, Moon *moon2);
std::vector<Position> getInputPositions(std::string filepath);
void applyTurn(std::vector<Moon> &moons);
bool areMoonsSame(std::vector<Moon> &original, std::vector<Moon> &current, char dimension);
long lcm(long a, long b);

int main() {
    std::vector<Moon> moons;
    std::vector<Moon> originals;
    auto positions = getInputPositions("input.txt");

    for (std::vector<Position>::iterator pos_it = positions.begin(); pos_it != positions.end(); pos_it++) {
        Moon moon = Moon{*pos_it, Velocity{0, 0, 0}};
        Moon original = Moon{*pos_it, Velocity{0, 0, 0}};
        moons.push_back(moon);
        originals.push_back(original);
    }
    
    long x_steps = 0, y_steps = 0, z_steps = 0;

    while (true) {
        x_steps++;
        applyTurn(moons);
        if (areMoonsSame(originals, moons, 'x')) {
            break;
        }
    } 

    std::vector<Moon>::iterator original_it = originals.begin();
    std::vector<Moon>::iterator current_it = moons.begin();
    while (original_it != originals.end()) {
        *current_it = *original_it;
        original_it++;
        current_it++;
    }

    while (true) {
        y_steps++;
        applyTurn(moons);
        if (areMoonsSame(originals, moons, 'y')) {
            break;
        }
    } 

    original_it = originals.begin();
    current_it = moons.begin();
    while (original_it != originals.end()) {
        *current_it = *original_it;
        original_it++;
        current_it++;
    }

    while (true) {
        z_steps++;
        applyTurn(moons);
        if (areMoonsSame(originals, moons, 'z')) {
            break;
        }
    } 

    std::cout << x_steps << " " << y_steps << " " << z_steps << '\n';

    std::cout << lcm(x_steps, lcm(y_steps, z_steps)) << '\n';
}

void applyTurn(std::vector<Moon> &moons) {
    std::vector<Moon>::iterator it, inner_it;
    for (it = moons.begin(); it != moons.end(); it++) {
        for (inner_it = it + 1; inner_it != moons.end(); inner_it++) {
            compareMoons(&(*it), &(*inner_it));
        }
    }

    for (it = moons.begin(); it != moons.end(); it++) {
        it->position.x += it->velocity.x;
        it->position.y += it->velocity.y;
        it->position.z += it->velocity.z;
    }
}

long gcd(long a, long b) {
    if (a == 0) {
        return b;
    }
    return gcd(b % a, a);
}

long lcm(long a, long b) {
    return (a * b) / gcd(a, b);
}

void compareMoons(Moon *moon1, Moon *moon2) {
    if (moon1 == moon2) {
        return;
    }

    if (moon1->position.x > moon2->position.x) {
        moon1->velocity.x -= 1;
        moon2->velocity.x += 1;
    } else if (moon1->position.x < moon2->position.x) {
        moon1->velocity.x += 1;
        moon2->velocity.x -= 1;
    }

    if (moon1->position.y > moon2->position.y) {
        moon1->velocity.y -= 1;
        moon2->velocity.y += 1;
    } else if (moon1->position.y < moon2->position.y) {
        moon1->velocity.y += 1;
        moon2->velocity.y -= 1;
    }

    if (moon1->position.z > moon2->position.z) {
        moon1->velocity.z -= 1;
        moon2->velocity.z += 1;
    } else if (moon1->position.z < moon2->position.z) {
        moon1->velocity.z += 1;
        moon2->velocity.z -= 1;
    }
}

// First must extract the positions
std::vector<Position> getInputPositions(std::string filepath) {
    std::ifstream ifs(filepath, std::ifstream::in);
    std::vector<Position> positions;
    std::string line;

    while (getline(ifs, line)) {
        parseLine(positions, line);
    }

    return positions;
}

void parseLine(std::vector<Position> &vec, std::string s) {
    s.erase(std::remove(s.begin(), s.end(), '<'), s.end());
    s.erase(std::remove(s.begin(), s.end(), '>'), s.end());
    Position position = Position{};

    auto coords = splitString(s, ',');
    for (std::vector<std::string>::iterator it = coords.begin(); it != coords.end(); it++) {
        auto coord = splitString(*it, '=');

        if (it == coords.begin()) {
            position.x = std::stoi(coord.at(1));
        } else if (it == coords.begin() + 1) {
            position.y = std::stoi(coord.at(1));
        } else {
            position.z = std::stoi(coord.at(1));
        }
    }
    vec.push_back(position);
}

std::vector<std::string> splitString(std::string s, char delim) {
    std::string tmp;
    std::vector<std::string> strs;
    std::stringstream ss(s);

    while (getline(ss, tmp, delim)) {
       strs.push_back(tmp); 
    }
    return strs;
}

bool isStateSame(Moon *original, Moon *current, char dimension) {
    if (dimension == 'x') {
        return original->velocity.x == current->velocity.x && original->position.x == current->position.x;
    } else if (dimension == 'y') {
        return original->velocity.y == current->velocity.y && original->position.y == current->position.y;
    } else {
        return original->velocity.z == current->velocity.z && original->position.z == current->position.z;
    }
}

bool areMoonsSame(std::vector<Moon> &original, std::vector<Moon> &current, char dimension) {
    std::vector<Moon>::iterator original_it = original.begin();
    std::vector<Moon>::iterator current_it = current.begin();
    while (original_it != original.end()) {
        if (!isStateSame(&(*original_it), &(*current_it), dimension)) {
           return false; 
        }
        original_it++;
        current_it++;
    }
    return true;
}
