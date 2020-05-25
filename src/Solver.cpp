#include "Solver.h"
#include "NoSolutionException.h"

#include <utility>
#include <algorithm>
#include <iostream>


Solver::Solver(Graph graph, int people_number) :
        graph(std::move(graph)) {
    this->people_number = people_number;
}

std::set<std::shared_ptr<Vertex>>
Solver::find_vertex_cover(int size, std::set<std::shared_ptr<Vertex>> &solution) {
    if (size < 0)
        throw NoSolutionException();

    auto edge_list = graph.get_edge_list();
    if (edge_list.empty()) {
        return solution;
    }
    if (size == 0) {
        throw NoSolutionException();
    }

    std::vector<std::shared_ptr<Vertex>> vertices = graph.get_vertices();
    std::vector<std::shared_ptr<Vertex>> one_degrees;
    std::copy_if(vertices.begin(), vertices.end(), std::back_inserter(one_degrees),
                 [](const std::shared_ptr<Vertex> &vertex) { return vertex->get_degree() == 1; });
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> deleted_edges;

    while (!one_degrees.empty()) {
        auto current_vertex = one_degrees.back();
        one_degrees.pop_back();

        std::shared_ptr<Vertex> one_degree_neighbour = *(current_vertex->neighbours.begin());
        solution.insert(one_degree_neighbour);
        size--;

        auto new_deletes = remove_vertex(one_degree_neighbour);
        deleted_edges.reserve(deleted_edges.size() + new_deletes.size());
        deleted_edges.insert(deleted_edges.end(), new_deletes.begin(), new_deletes.end());

        edge_list = graph.get_edge_list();
        if (edge_list.empty()) {
            return solution;
        }
        if (size == 0) {
            restore_graph(deleted_edges);
            solution.erase(one_degree_neighbour);
            throw NoSolutionException();
        }

        one_degrees.clear();
        std::copy_if(vertices.begin(), vertices.end(), std::back_inserter(one_degrees),
                     [](const std::shared_ptr<Vertex> &vertex) { return vertex->get_degree() == 1; });
    }

    std::sort(vertices.begin(), vertices.end(),
              [](const std::shared_ptr<Vertex> &lhs, const std::shared_ptr<Vertex> &rhs) {
                  return lhs->get_degree() > rhs->get_degree();
              });

    std::shared_ptr<Vertex> current_vertex = vertices[0];
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
Solver::remove_vertex(const std::shared_ptr<Vertex> &vertex) {
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
        auto first_vertex = graph.get_vertex(edge.first);
        auto second_vertex = graph.get_vertex(edge.second);

        if (first_vertex == nullptr || second_vertex == nullptr)
            throw NoSolutionException();

        first_vertex->neighbours.insert(second_vertex);
        second_vertex->neighbours.insert(first_vertex);
    }
}

std::set<std::shared_ptr<Vertex>> Solver::find_independent_set() {
    std::set<std::shared_ptr<Vertex>> vertex_cover_solution{};
    find_vertex_cover(graph.size() - people_number, vertex_cover_solution);

    std::vector<std::shared_ptr<Vertex>> vertices = graph.get_vertices();

    std::set<std::shared_ptr<Vertex>> result{};
    for (const auto &vertex : vertices) {
        if (vertex_cover_solution.find(vertex) == vertex_cover_solution.end())
            result.insert(vertex);
    }

    return result;
}

void Solver::print_solution() {
    auto solution = find_independent_set();
    int elements_counter = 0;

    if (solution.size() < people_number)
        throw NoSolutionException();

    // not iterating to the end of the set in case if found solution is bigger than desired
    for (auto it = solution.begin(); elements_counter < people_number; it++) {
        auto coordinates = it->get()->coordinates;
        std::cout << coordinates.first << " " << coordinates.second << std::endl;
        elements_counter++;
    }
}

