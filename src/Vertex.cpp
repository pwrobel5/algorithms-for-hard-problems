#include "Vertex.h"

Vertex::Vertex(std::pair<int, int> coordinates, char type) {
    this->coordinates = coordinates;
    this->type = type;
}

void Vertex::add_neighbour(const std::shared_ptr<Vertex> &neighbour) {
    neighbours.insert(neighbour);
}
