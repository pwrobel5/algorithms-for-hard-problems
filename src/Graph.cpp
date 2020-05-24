#include "Graph.h"

#include <iostream>
#include <algorithm>

std::vector<char> Graph::matching_types_horizontal = {
        '-', '+'
};

std::vector<char> Graph::matching_types_vertical = {
        '|', '+'
};

Graph::Graph(int width, int height, const std::vector<std::string> &park_map) {
    // decrement by 1 to have appropriate values to compare with vertex coordinates during finding edges
    this->width = width - 1;
    this->height = height - 1;

    for (std::size_t i = 0; i < park_map.size(); i++) {
        parse_input_string(i, park_map[i]);
    }

    add_edges();
}

void Graph::parse_input_string(int index, std::string input_string) {
    for (std::size_t i = 0; i < input_string.size(); i++) {
        if (input_string[i] != no_vertex_sign) {
            std::pair<int, int> coordinates = std::pair<int, int>(index, i);
            std::shared_ptr<Vertex> vertex(new Vertex(coordinates, input_string[i]));
            vertices.insert(std::pair<std::pair<int, int>, std::shared_ptr<Vertex>>(coordinates, vertex));
        }
    }
}

void Graph::add_edges() {
    // elements in std::map are already sorted by values stored in std::pairs
    // so there is not need to sort it again
    for (const auto &element : vertices) {
        std::pair<int, int> coordinates = element.first;
        std::shared_ptr<Vertex> vertex_ptr = element.second;
        char vertex_type = vertex_ptr->type;

        switch (vertex_type) {
            case '+':
                add_right_neighbour(coordinates, vertex_ptr);
                add_bottom_neighbour(coordinates, vertex_ptr);
                break;
            case '-':
                add_right_neighbour(coordinates, vertex_ptr);
                break;
            case '|':
                add_bottom_neighbour(coordinates, vertex_ptr);
                break;
            default:
                std::cerr << "Unrecognized vertex type in position: " << coordinates.first << ", " << coordinates.second
                          << std::endl;
        }
    }
}

void Graph::add_right_neighbour(std::pair<int, int> coordinates, const std::shared_ptr<Vertex> &vertex_ptr) {
    if (coordinates.second < width) {
        auto it = vertices.find(std::pair<int, int>(coordinates.first, coordinates.second + 1));
        if (it != vertices.end()) {
            std::shared_ptr<Vertex> neighbour = it->second;
            if (std::find(matching_types_horizontal.begin(), matching_types_horizontal.end(), neighbour->type) !=
                matching_types_horizontal.end()) {
                vertex_ptr->add_neighbour(neighbour);
                neighbour->add_neighbour(vertex_ptr);
            }
        }
    }
}

void Graph::add_bottom_neighbour(std::pair<int, int> coordinates, const std::shared_ptr<Vertex> &vertex_ptr) {
    if (coordinates.first < height) {
        auto it = vertices.find(std::pair<int, int>(coordinates.first + 1, coordinates.second));
        if (it != vertices.end()) {
            std::shared_ptr<Vertex> neighbour = it->second;
            if (std::find(matching_types_vertical.begin(), matching_types_vertical.end(), neighbour->type) !=
                matching_types_vertical.end()) {
                vertex_ptr->add_neighbour(neighbour);
                neighbour->add_neighbour(vertex_ptr);
            }
        }
    }
}

void Graph::include_minimal_distance(int distance) {
    for (const auto &element : vertices) {
        for (const auto &neighbour : element.second->neighbours) {
            traverse_neighbours(element.second, element.second, neighbour, distance);
        }
    }
}

void
Graph::traverse_neighbours(const std::shared_ptr<Vertex> &base_vertex, const std::shared_ptr<Vertex> &previous_vertex,
                           const std::shared_ptr<Vertex> &current_vertex, int count) {
    if (count == 0)
        return;

    base_vertex->add_neighbour(current_vertex);
    current_vertex->add_neighbour(base_vertex);

    for (const auto &neighbour : current_vertex->neighbours) {
        if (neighbour != previous_vertex)
            traverse_neighbours(base_vertex, current_vertex, neighbour, count - 1);
    }
}

void Graph::print() {
    for (const auto &vertex : vertices) {
        std::cout << vertex.first.first << ", " << vertex.first.second << std::endl;
        auto neighbours = vertex.second->neighbours;
        for (const auto &neighbour : neighbours) {
            std::cout << "\t" << neighbour->coordinates.first << ", " << neighbour->coordinates.second << std::endl;
        }
    }
}
