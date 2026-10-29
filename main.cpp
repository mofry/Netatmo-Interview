
#include <iostream>
#include <vector>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <thread>
#include <limits>


#include "world.hpp"
#include "engine.hpp"
#include "rendering.hpp"

int main(int argc, char* argv[]) {

    try {

        const int width = 30;
        const int height = 15;
        const double ratio = 0.8;

        const std::vector<std::uint8_t> inactive_to_active_Rules = {3, 6, 7, 8};
        const std::vector<std::uint8_t> remain_active_Rules = {3, 4, 6, 7, 8};

        //pass a seed as first argument to replay a simulation, otherwise a random one is used
        const unsigned int seed = (argc > 1)
            ? static_cast<unsigned int>(std::stoul(argv[1]))
            : std::random_device{}();

        World w(width, height, ratio, seed);
        Engine engine;
        Rendering r;

        const int iterations = 10;

        // 2. Render initial state
        r.render(w);
        std::cout << "Seed: " << seed << std::endl;
        std::cout << "Iteration: 0 (Initial state)" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // 3. Run simulation loop
        for (int step = 1; step <= iterations; ++step) {
            engine.updateCells(w, inactive_to_active_Rules, remain_active_Rules);

            r.render(w);
            std::cout << "Iteration: " << step << " / " << iterations << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}