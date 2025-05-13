#include "GameOfLifeCLI.hpp"
#include "GameOfLife.hpp"

#include <iostream>
#include <sstream>
#include <sstream>
#include <chrono>
#include <thread>

void GameOfLifeCLI::run() {
    GameOfLife* game = nullptr;
    bool print_enabled = false;
    bool stability_check = false;
    int delay_ms = 300; // standard verzögerung
 
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
            } else {
                std::cout << "Usage: create <width> <height>" << std::endl;
            }
        } 
         else if (cmd == "load") {
            std::string filename;
            if (iss >> filename) {
                delete game;
                game = new GameOfLife(filename);
            } else {
                std::cout << "Usage: load <filename>" << std::endl;
            }
        }
        else if (cmd == "save") {
            if (game == nullptr) {
                std::cout << "No world created yet!" << std::endl;
                continue;
            }
            std::string filename;
            if (iss >> filename) {
                game->save(filename);
            } else {
                std::cout << "Usage: save <filename>" << std::endl;
            }
        }
        else if (cmd == "print") {
            // print an/aus schalten
            int value;
            if (iss >> value) {
                print_enabled = (value == 1);
                std::cout << "Printing " << (print_enabled ? "enabled" : "disabled") << std::endl;
            }
        }
        else if (cmd == "run") {
            if (game == nullptr) {
                std::cout << "No world created yet!" << std::endl;
                continue;
            }
            int generations;
            if (iss >> generations) {
                // zeit messung start
                auto start = std::chrono::high_resolution_clock::now();
                
                for (int i = 0; i < generations; ++i) {
                    if (print_enabled) {
                        std::cout << "\033[2J\033[H"; // Clear screen
                        std::cout << "Generation " << i << ":" << std::endl;
                        game->print();
                        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
                    }
                    
                    game->evolve(); // nächste generation berechnen
                    // stabilität checken falls aktiviert
                    if (stability_check && game->is_stable()) {
                        std::cout << "World reached stable state at generation " << i << std::endl;
                        break;
                    }
                }
                
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            }
        }
        else if (cmd == "set") {
            if (game == nullptr) {
                std::cout << "No world created yet!" << std::endl;
                continue;
            }
            int x, y, state;
            if (iss >> x >> y >> state) {
                game->set_cell(x, y, state);
                std::cout << "Cell (" << x << "," << y << ") set to " << state << std::endl;
            }
        }
        else if (cmd == "get") {
            if (game == nullptr) {
                std::cout << "No world created yet!" << std::endl;
                continue;
            }
            int x, y;
            if (iss >> x >> y) {
                int state = game->get_cell(x, y);
                if (state != -1) {
                    std::cout << "Cell (" << x << "," << y << ") = " << state << std::endl;
                }
            }
        }
        else if (cmd == "stability") {
            // stabilität check an/aus
            int value;
            if (iss >> value) {
                stability_check = (value == 1);
                std::cout << "Stability check " << (stability_check ? "enabled" : "disabled") << std::endl;
            }
        }
        else if (cmd == "exit") {
            break;
        }
        else {
            std::cout << "Unknown command." << std::endl;
        }
    }

    delete game;
}
