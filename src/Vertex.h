#ifndef ALGORITHMS_FOR_HARD_PROBLEMS_VERTEX_H
#define ALGORITHMS_FOR_HARD_PROBLEMS_VERTEX_H

#include <utility>
#include <set>

class Vertex {
public:
    std::pair<int, int> coordinates;
    char type;
    std::set<Vertex *> neighbours;
    std::set<Vertex *> extra_neighbours;

    Vertex(std::pair<int, int> coordinates, char type);

    void add_neighbour(Vertex *&neighbour);

    void add_extra_neighbour(Vertex *&neighbour);

    void merge_neighbours_sets();

    int get_degree() const;
};


#endif
