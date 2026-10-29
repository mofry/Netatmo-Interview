#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <vector>
#include <cstdint>


//Forward declaration of world class
class World; 

class Engine {

    public :
        // Applies one synchronous generation using the provided neighbor rules.
        void updateCells(World& w , const std::vector<std::uint8_t>& inactive_to_active_Rules  , const std::vector<std::uint8_t>& remain_active_Rules);
   
};

#endif