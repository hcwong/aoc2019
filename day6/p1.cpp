#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct Planet {
    std::vector<std::string> children;
    bool visited;
};

std::vector<std::string> split(std::string);
int dfs(Planet&, std::map<std::string, Planet>, int);

int main() {
    int sum = 0;
    std::ifstream ifs ("input1.txt", std::ifstream::in);
    std::string line;
    std::map<std::string, Planet> planets;

    while (ifs.good()) {
        getline(ifs, line);
        std::vector<std::string> values = split(line);
        
        // First check if said planet exists in the map
        if (planets.count(values[0]) == 0) {
            Planet planet = {std::vector<std::string>(), false};
            planets[values[0]] = planet;
        }
        if (planets.count(values[1]) == 0) {
            Planet planet = {std::vector<std::string>(), false};
            planets[values[1]] = planet;
        }
        planets[values[0]].children.push_back(values[1]);
    }

    int orbits = dfs(planets["COM"], planets, 0);
    std::cout << orbits;
}

int dfs(Planet& p, std::map<std::string, Planet> planets, int count) {
    p.visited = true;
    std::vector<std::string>::iterator it;
    // Direct orbits / Depth of tree
    int sum = count;

    // Add orbits of children in the tree
    for (it = p.children.begin(); it != p.children.end(); it++) {
        if (!planets[*it].visited) {
            sum += dfs(planets[*it], planets, count + 1);
        }
    }

    return sum;
}

std::vector<std::string> split(std::string s) {
    std::string tmp;
    std::vector<std::string> values;
    std::stringstream ss(s);

    while (getline(ss, tmp, ')')) {
       values.push_back(tmp); 
    }
    return values;
}