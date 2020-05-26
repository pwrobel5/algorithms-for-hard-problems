#ifndef ALGORITHMS_FOR_HARD_PROBLEMS_GRAPH_H
#define ALGORITHMS_FOR_HARD_PROBLEMS_GRAPH_H

#include <map>
#include <utility>
#include <memory>
#include <vector>

#include "Vertex.h"

class Graph {
private:
    static std::vector<char> matching_types_horizontal;
    static std::vector<char> matching_types_vertical;
    static const char no_vertex_sign = '.';

    int width;
    int height;
    std::map<std::pair<int, int>, Vertex*> vertices;

    void parse_input_string(int index, std::string input_string);

    void add_edges();

    void add_right_neighbour(std::pair<int, int> coordinates, Vertex *vertex_ptr);

    void add_bottom_neighbour(std::pair<int, int> coordinates, Vertex* vertex_ptr);

    void traverse_neighbours(Vertex* base_vertex, Vertex* previous_vertex, Vertex* current_vertex, int count);

    void include_minimal_distance(int distance);

public:
    Graph(int width, int height, const std::vector<std::string> &park_map, int minimal_distance);

    int size();

    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> get_edge_list();

    std::vector<Vertex*> get_vertices();

    Vertex* get_vertex(std::pair<int, int> coordinates);
};

#endif
