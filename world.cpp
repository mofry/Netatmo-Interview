#include <iostream>
#include <vector>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <cstdint>

#include "world.hpp"

World::World(int width , int height , double ratio , unsigned int seed) : _width(width), _height(height) {

    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("width and height must be strictly positive");
    }
    // Ensure the total number of cells fits in an int, as indices are ints.
    if (width > std::numeric_limits<int>::max() / height) {
        throw std::invalid_argument("width * height is too large");
    }
    
    if (!(ratio >= 0.0 && ratio <= 1.0)) {
        throw std::invalid_argument("ratio must be between 0.0 and 1.0");
    }
    
    _cells.resize(static_cast<std::size_t>(width) * static_cast<std::size_t>(height));

    // Seed the generator with the provided value for repeatable initialization.
    std::mt19937 gen(seed);
    std::bernoulli_distribution b(ratio);

    for (auto& cell : _cells) {
        cell = b(gen);
    }
}



int World::getWidth() const {

    return _width;
}


int World::getHeight() const {

    return _height;
}


const std::vector<std::uint8_t>& World::getCells() const {
    return _cells;
}

int World::countActiveNeighbours(int index) const {

    if (index < 0 || index >= static_cast<int>(_cells.size())) {
        throw std::out_of_range("cell index is outside the world");
    }

    // Cells are stored row by row 
    const int row = index / _width;
    const int column = index % _width;

    int count = 0;

    // Visit the 3x3 square centred on the unit.
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {

            // Skip the unit itself
            if (dy == 0 && dx == 0) {
                continue;
            }

            const int neighbourRow = row + dy;
            const int neighbourColumn = column + dx;

            // Positions beyond the grid are ignored
            if (neighbourRow < 0 || neighbourRow >= _height ||
                neighbourColumn < 0 || neighbourColumn >= _width) {
                continue;
            }

            count += _cells[neighbourRow * _width + neighbourColumn];
        }
    }

    return count;
}


void World::setStatus(int index , int status) {

    if (index < 0 || index >= static_cast<int>(_cells.size())) {
        throw std::out_of_range("cell index is outside the world");
    }
    if (status != 0 && status != 1) {
        throw std::invalid_argument("status must be 0 or 1");
    }

    _cells[index] = status;

}
