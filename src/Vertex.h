#ifndef ALGORITHMS_FOR_HARD_PROBLEMS_VERTEX_H
#define ALGORITHMS_FOR_HARD_PROBLEMS_VERTEX_H

#include <utility>
#include <set>
#include <memory>

class Vertex {
public:
    std::pair<int, int> coordinates;
    char type;
    std::set<std::shared_ptr<Vertex>> neighbours;
    std::set<std::shared_ptr<Vertex>> extra_neighbours;

    Vertex(std::pair<int, int> coordinates, char type);

    void add_neighbour(const std::shared_ptr<Vertex> &neighbour);

    void add_extra_neighbour(const std::shared_ptr<Vertex> &neighbour);

    void merge_neighbours_sets();

    int get_degree() const;
};


#endif
