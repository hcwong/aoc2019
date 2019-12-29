#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <queue>

struct Planet {
    std::vector<std::string> children;
    bool visited;
    std::string name;
};

std::vector<std::string> split(std::string);
int bfs(Planet&, std::map<std::string, Planet>, int);

int main() {
    int sum = 0;
    std::ifstream ifs ("input1.txt", std::ifstream::in);
    std::map<std::string, Planet> planets;

    while (ifs.good()) {
        std::string line;
        getline(ifs, line);
        std::vector<std::string> values = split(line);
        std::string planet1 = values[0];
        std::string planet2 = values[1];
        
        // First check if said planet exists in the map
        if (planets.count(planet1) == 0) {
            Planet planet = {std::vector<std::string>(), false, planet1};
            planets[planet1] = planet;
        }
        if (planets.count(planet2) == 0) {
            Planet planet = {std::vector<std::string>(), false, planet2};
            planets[planet2] = planet;
        }
        planets[planet1].children.push_back(planet2);
        planets[planet2].children.push_back(planet1);
    }

    int orbits = bfs(planets["YOU"], planets, 0);
    std::cout << orbits;
}

int bfs(Planet& p, std::map<std::string, Planet> planets, int count) {
    std::queue<Planet> queue;
    queue.push(p);
    p.visited = true;

    while (queue.size() > 0) {
        int currQueueSize = queue.size();

        for (int i = 0; i < currQueueSize; i++) {
            Planet curr = queue.front();
            queue.pop();
            if (curr.name == "SAN") {
                return count - 2;
            }
            std::vector<std::string>::iterator it;
            for (it = curr.children.begin(); it != curr.children.end(); it++) {
                if (!planets[*it].visited) {
                    planets[*it].visited = true;
                    queue.push(planets[*it]);
                }
            }
        }

        count++;
    }

    return -1;
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