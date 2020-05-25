#include <iostream>
#include <vector>

#include "src/Graph.h"
#include "src/Solver.h"
#include "src/NoSolutionException.h"

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

    Graph graph = Graph(park_width, park_height, park_map, minimum_distance);
    Solver solver = Solver(graph, people_number);

    try {
        solver.print_solution();
    } catch (NoSolutionException &ex) {
        std::cout << "No solution found!" << std::endl;
    }

    return 0;
}
