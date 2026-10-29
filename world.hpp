#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <cstdint>


class World {

    int _width;
    int _height;
    std::vector<std::uint8_t> _cells; // Contiguous storage for the state of each cell.

    public : 
        // ratio is the probability that each cell starts active (1).
        // seed initializes the random number generator for reproducible initialization.
        World(int width , int height ,double ratio , unsigned int seed);

        // Getters
        int getWidth() const;
        int getHeight() const;
        const std::vector<std::uint8_t>& getCells() const;
        // Returns the number of active units among the (up to) 8 surrounding units.
        int countActiveNeighbours(int index) const;


        // Setter
        void setStatus(int index , int status);
        
};

#endif