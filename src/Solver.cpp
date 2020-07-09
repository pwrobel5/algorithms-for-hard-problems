#include "Solver.h"
#include "NoSolutionException.h"

#include <utility>
#include <algorithm>
#include <iostream>


Solver::Solver(Graph *graph, int people_number) {
    this->people_number = people_number;
    this->graph = graph;
    this->vertices = graph->get_vertices();
}

void Solver::print_solution() {
    auto solution = find_independent_set();
    int elements_counter = 0;

    if (solution.size() < (unsigned) people_number)
        throw NoSolutionException();

    // not iterating to the end of the set in case if found solution is bigger than desired
    for (auto it = solution.begin(); elements_counter < people_number; it++) {
        auto coordinates = (*it)->coordinates;
        std::cout << coordinates.first << " " << coordinates.second << std::endl;
        elements_counter++;
    }
}

std::set<Vertex *> Solver::find_independent_set() {
    std::set<Vertex *> vertex_cover_solution{};
    find_vertex_cover(graph->size() - people_number, vertex_cover_solution);

    std::set<Vertex *> result{};

    for (const auto &vertex : vertices) {
        if (vertex_cover_solution.find(vertex) == vertex_cover_solution.end())
            result.insert(vertex);
    }

    return result;
}

std::set<Vertex *> Solver::find_vertex_cover(int size, std::set<Vertex *> &solution) {
    if (size < 0)
        throw NoSolutionException();

    auto edge_list = graph->get_edge_list();
    if (edge_list.empty()) {
        return solution;
    }
    if (size == 0) {
        throw NoSolutionException();
    }

    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> deleted_edges;
    std::sort(vertices.begin(), vertices.end(),
              [](const Vertex *lhs, const Vertex *rhs) {
                  return lhs->get_degree() > rhs->get_degree();
              });

    auto one_degree_it = std::find_if(vertices.begin(), vertices.end(), [](const Vertex *vertex) {
                                          return vertex->get_degree() == 1;
                                      }
    );

    while (one_degree_it != vertices.end()) {
        int index = one_degree_it - vertices.begin();
        auto current_vertex = vertices[index];

        Vertex *one_degree_neighbour = *(current_vertex->neighbours.begin());
        solution.insert(one_degree_neighbour);
        size--;

        auto new_deletes = remove_vertex(one_degree_neighbour);
        deleted_edges.reserve(deleted_edges.size() + new_deletes.size());
        deleted_edges.insert(deleted_edges.end(), new_deletes.begin(), new_deletes.end());

        edge_list = graph->get_edge_list();
        if (edge_list.empty()) {
            return solution;
        }
        if (size == 0) {
            restore_graph(deleted_edges);
            solution.erase(one_degree_neighbour);
            throw NoSolutionException();
        }

        std::sort(vertices.begin(), vertices.end(),
                  [](const Vertex *lhs, const Vertex *rhs) {
                      return lhs->get_degree() > rhs->get_degree();
                  });

        one_degree_it = std::find_if(vertices.begin(), vertices.end(), [](const Vertex *vertex) {
                                         return vertex->get_degree() == 1;
                                     }
        );
    }

    Vertex *current_vertex = vertices[0];
    auto new_deletes = remove_vertex(current_vertex);
    solution.insert(current_vertex);

    try {
        return find_vertex_cover(size - 1, solution);
    } catch (NoSolutionException &ex) {
        restore_graph(new_deletes);
        solution.erase(current_vertex);
    }

    auto neighbours = current_vertex->neighbours;
    for (const auto &neighbour : neighbours) {
        new_deletes = remove_vertex(neighbour);
        deleted_edges.reserve(deleted_edges.size() + new_deletes.size());
        deleted_edges.insert(deleted_edges.end(), new_deletes.begin(), new_deletes.end());

        solution.insert(neighbour);
    }

    try {
        return find_vertex_cover(size - (int) neighbours.size(), solution);
    } catch (NoSolutionException &ex) {
        restore_graph(deleted_edges);
        for (const auto &neighbour : neighbours) {
            solution.erase(neighbour);
        }
    }

    throw NoSolutionException();
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>
Solver::remove_vertex(Vertex *vertex) {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> deleted_edges;

    auto neighbours_copy = vertex->neighbours;

    deleted_edges.reserve(neighbours_copy.size());
    for (const auto &neighbour : neighbours_copy) {
        deleted_edges.emplace_back(vertex->coordinates, neighbour->coordinates);

        neighbour->neighbours.erase(vertex);
        vertex->neighbours.erase(neighbour);
    }

    return deleted_edges;
}

void Solver::restore_graph(const std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &deleted_edges) {
    for (const auto &edge : deleted_edges) {
        auto first_vertex = graph->get_vertex(edge.first);
        auto second_vertex = graph->get_vertex(edge.second);

        if (first_vertex == nullptr || second_vertex == nullptr)
            throw NoSolutionException();

        first_vertex->neighbours.insert(second_vertex);
        second_vertex->neighbours.insert(first_vertex);
    }
}
