
#ifndef RENDERING_HPP
#define RENDERING_HPP

#include <vector>


class World;

class Rendering {

    public : 
        // Displays the world grid in the terminal.
        void render(const World&) const;
};


#endif