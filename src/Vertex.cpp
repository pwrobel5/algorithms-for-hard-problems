#include "Vertex.h"

Vertex::Vertex(std::pair<int, int> coordinates, char type) {
    this->coordinates = coordinates;
    this->type = type;
}

void Vertex::add_neighbour(const std::shared_ptr<Vertex> &neighbour) {
    neighbours.insert(neighbour);
}

void Vertex::add_extra_neighbour(const std::shared_ptr<Vertex> &neighbour) {
    extra_neighbours.insert(neighbour);

}

void Vertex::merge_neighbours_sets() {
    neighbours.insert(extra_neighbours.begin(), extra_neighbours.end());
}

int Vertex::get_degree() const {
    return neighbours.size();
}
