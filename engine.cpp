#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>


#include "engine.hpp"
#include "world.hpp"


void Engine::updateCells(World& w , const std::vector<std::uint8_t>& inactive_to_active_Rules  , const std::vector<std::uint8_t>& remain_active_Rules) {


    // Read every cell and its neighbors from the same snapshot while updating w.
    World temp = w; 
    const std::vector<std::uint8_t>& cells = temp.getCells();
    int size = static_cast<int>(cells.size());

    for(int i = 0 ; i < size ; i++) {

        const int activeNeighbours = temp.countActiveNeighbours(i);
        if (cells[i] == 0 ) {
            
            if (std::find(inactive_to_active_Rules.begin(), inactive_to_active_Rules.end(), activeNeighbours) != inactive_to_active_Rules.end()) {
                    w.setStatus(i , 1 );
                } 
        } else {

            if (std::find(remain_active_Rules.begin(), remain_active_Rules.end(), activeNeighbours) == remain_active_Rules.end()) {
                    w.setStatus(i , 0 );
                } 
        }

    }

}
