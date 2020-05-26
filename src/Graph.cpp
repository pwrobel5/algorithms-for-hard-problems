#include "Graph.h"

#include <iostream>
#include <algorithm>

std::vector<char> Graph::matching_types_horizontal = {
        '-', '+'
};

std::vector<char> Graph::matching_types_vertical = {
        '|', '+'
};

Graph::Graph(int width, int height, const std::vector<std::string> &park_map, int minimal_distance) {
    // decrement by 1 to have appropriate values to compare with vertex coordinates during finding edges
    this->width = width - 1;
    this->height = height - 1;

    for (std::size_t i = 0; i < park_map.size(); i++) {
        parse_input_string(i, park_map[i]);
    }

    add_edges();
    include_minimal_distance(minimal_distance);
}

void Graph::parse_input_string(int index, std::string input_string) {
    for (std::size_t i = 0; i < input_string.size(); i++) {
        if (input_string[i] != no_vertex_sign) {
            std::pair<int, int> coordinates = std::pair<int, int>(index, i);
            auto* vertex = new Vertex(coordinates, input_string[i]);
            vertices.insert(std::pair<std::pair<int, int>, Vertex*>(coordinates, vertex));
        }
    }
}

void Graph::add_edges() {
    // elements in std::map are already sorted by values stored in std::pairs
    // so there is not need to sort it again
    for (const auto &element : vertices) {
        std::pair<int, int> coordinates = element.first;
        Vertex* vertex_ptr = element.second;
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

void Graph::add_right_neighbour(std::pair<int, int> coordinates, Vertex* vertex_ptr) {
    if (coordinates.second < width) {
        auto it = vertices.find(std::pair<int, int>(coordinates.first, coordinates.second + 1));
        if (it != vertices.end()) {
            Vertex* neighbour = it->second;
            if (std::find(matching_types_horizontal.begin(), matching_types_horizontal.end(), neighbour->type) !=
                matching_types_horizontal.end()) {
                vertex_ptr->add_neighbour(neighbour);
                neighbour->add_neighbour(vertex_ptr);
            }
        }
    }
}

void Graph::add_bottom_neighbour(std::pair<int, int> coordinates, Vertex* vertex_ptr) {
    if (coordinates.first < height) {
        auto it = vertices.find(std::pair<int, int>(coordinates.first + 1, coordinates.second));
        if (it != vertices.end()) {
            Vertex* neighbour = it->second;
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
        auto neighbours = element.second->neighbours;
        for (const auto &neighbour : neighbours) {
            traverse_neighbours(element.second, element.second, neighbour, distance);
        }
    }

    for (const auto &element : vertices) {
        element.second->merge_neighbours_sets();
    }
}

void Graph::traverse_neighbours(Vertex* base_vertex, Vertex* previous_vertex, Vertex* current_vertex, int count) {
    if (count == 0)
        return;

    base_vertex->add_extra_neighbour(current_vertex);
    current_vertex->add_extra_neighbour(base_vertex);

    for (const auto &neighbour : current_vertex->neighbours) {
        if (neighbour != previous_vertex)
            traverse_neighbours(base_vertex, current_vertex, neighbour, count - 1);
    }
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> Graph::get_edge_list() {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> result;
    for (const auto &vertex : vertices) {
        std::pair<int, int> first_coordinates = vertex.first;

        for (const auto &neighbour : vertex.second->neighbours) {
            std::pair<int, int> second_coordinates = neighbour->coordinates;
            if (first_coordinates < second_coordinates)
                result.emplace_back(first_coordinates, second_coordinates);
        }
    }

    return result;
}

int Graph::size() {
    return vertices.size();
}

std::vector<Vertex*> Graph::get_vertices() {
    std::vector<Vertex*> result;
    for (const auto &element : vertices) {
        result.push_back(element.second);
    }

    return result;
}

Vertex* Graph::get_vertex(std::pair<int, int> coordinates) {
    auto find_result = vertices.find(coordinates);

    if (find_result != vertices.end())
        return find_result->second;

    return nullptr;
}
