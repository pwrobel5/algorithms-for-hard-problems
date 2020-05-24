#include <iostream>
#include <vector>

#include "src/Graph.h"

int main() {
    int park_width, park_height, minimum_distance, people_number;
    std::cin >> park_width >> park_height >> minimum_distance >> people_number;

    std::string park_name;
    std::cin >> park_name;

    std::vector<std::string> park_map(park_height);
    for (auto &i : park_map) {
        std::cin >> i;
        if (i.size() != park_width) {
            std::cerr << "Incorrect line length!: " << std::endl;
            return 1;
        }
    }

    Graph graph = Graph(park_width, park_height, park_map);

    return 0;
}
