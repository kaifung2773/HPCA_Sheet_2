#ifndef GAME_OF_LIFE_CLI_HPP
#define GAME_OF_LIFE_CLI_HPP

#include "GameOfLife.hpp"
#include <string>
#include <thread> // für sleep

class GameOfLifeCLI
{
public:
    void run();       // Haupt-Loop
    ~GameOfLifeCLI(); // Destruktor

private:
    GameOfLife *game = nullptr;
    bool print_enabled = true;
    int delay_ms = 0;                    // Verzögerung in ms zwischen Generationen
    bool stability_check_enabled = true; // aktiviert/deaktiviert is_stable()-Prüfung
};

#endif // GAME_OF_LIFE_CLI_HPP
