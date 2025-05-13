#include "GameOfLifeCLI.hpp"
#include "GameOfLife.hpp"

#include <iostream>
#include <sstream>

void GameOfLifeCLI::run() {
    GameOfLife* game = nullptr;
 
    std::string line;
    std::cout << "Welcome to Game of Life. Type 'help' to show all commands." << std::endl;

    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "help") {
            std::cout << "Commands:" << std::endl
                      << "create <width> <height>" << std::endl
                      << "help" << std::endl;
        } else if (cmd == "create") {
            int w, h;
            if (iss >> w >> h) {
                delete game;
                game = new GameOfLife(w, h);
            }
        } else {
            std::cout << "Unknown command." << std::endl;
        }
    }

    delete game;
}
