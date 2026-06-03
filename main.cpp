#include "Game.hpp"
#include <iostream>
#include <exception>
#include <cstdlib>

int main() {

    try {
        std::cout << "[SYSTEM] Initializing...\n";
//creating instance
        Game game;

        std::cout << "[SYSTEM] Starting game loop\n";
//turn main loop on
        game.run();
    }
    //standard error catch
    catch (const std::exception& e) {
        std::cerr << "[ERROR] Code: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    //untracked error catch
    catch(...) {
        std::cerr << "[ERROR] Unknown error\n";
        return EXIT_FAILURE;
    }

    std::cout << "[SYSTEM] Game closed properly\n";
    return EXIT_SUCCESS;
}