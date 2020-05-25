#ifndef ALGORITHMS_FOR_HARD_PROBLEMS_SOLVER_H
#define ALGORITHMS_FOR_HARD_PROBLEMS_SOLVER_H

#include "Graph.h"

class Solver {
private:
    Graph graph;
    int people_number;

    static std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>
    remove_vertex(const std::shared_ptr<Vertex> &vertex);

    void restore_graph(const std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &deleted_edges);

    std::set<std::shared_ptr<Vertex>> find_vertex_cover(int size, std::set<std::shared_ptr<Vertex>> &solution);

    std::set<std::shared_ptr<Vertex>> find_independent_set();

public:
    Solver(Graph graph, int people_number);

    void print_solution();
};


#endif
