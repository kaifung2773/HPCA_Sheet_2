#ifndef GAME_OF_LIFE_CLI_HPP
#define GAME_OF_LIFE_CLI_HPP

#include "GameOfLife.hpp"
#include <string>
#include <thread> // forsleep

class GameOfLifeCLI
{
public:
    void run();       // main-Loop for CLI
    ~GameOfLifeCLI(); // Destructor

private:
    GameOfLife *game = nullptr;
    bool print_enabled = true;
    int delay_ms = 0;                    // Delay between generations
    bool stability_check_enabled = true; // activate is_stable
};

#endif // GAME_OF_LIFE_CLI_HPP
