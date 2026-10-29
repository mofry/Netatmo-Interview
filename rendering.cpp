#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "rendering.hpp"
#include "world.hpp"


 void Rendering::render(const World& w) const {

    // Clear the terminal before drawing the current grid.
    std::cout << "\033[2J" << "\033[1;1H";

    const std::vector<std::uint8_t>& cells = w.getCells();
    std::size_t size = cells.size();

    for(std::size_t i = 0 ; i < size ; i++) {

        if(cells[i] == 1) {
            std::cout << 'O';
        } else {std::cout << ' ';}
        
        // Start a new line after each row.
        if(!((i+1) % w.getWidth()) ){
            std::cout << '\n';
        }  
    }
 }
