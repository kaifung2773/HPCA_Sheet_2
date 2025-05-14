#include "GameOfLifeCLI.hpp"
#include "GameOfLife.hpp"
#include <iostream>
#include <sstream>
#include <chrono>
#include <cstdlib>
#include <ctime>

void GameOfLifeCLI::run()
{
    std::string line; // declare line as string for input

    std::cout << "Game of Life CLI gestartet. Tippe 'exit' zum Beenden.\n";
    while (true)
    {
        std::cout << "> ";            // await input
        std::getline(std::cin, line); // userinput

        std::istringstream iss(line); // transfomr to stream (iss) words separated by whitspce
        std::string command;
        iss >> command; // fist word ist command // following items will be parameters

        if (command == "exit")
        {
            break;
        }
        else if (command == "help") // CLI help menu
        {
            std::cout << "Verfügbare Befehle:\n";
            std::cout << "  help                     - Zeigt diese Hilfe\n";
            std::cout << "  create <w> <h>           - Erstellt ein neues Spielfeld\n";
            std::cout << "  set <x> <y> <0|1>        - Setzt eine Zelle (2D)\n";
            std::cout << "  set <pos> <0|1>          - Setzt eine Zelle (1D)\n";
            std::cout << "  get <x> <y>              - Zeigt Zellstatus (2D)\n";
            std::cout << "  get <pos>                - Zeigt Zellstatus (1D)\n";
            std::cout << "  print [0|1]              - Druckt das Grid bzw. aktiviert/deaktiviert Liveanzeige\n";
            std::cout << "  delay <ms>               - Setzt Verzögerung zwischen Generationen (ms)\n";
            std::cout << "  run <n>                  - Führt n Generationen aus\n";
            std::cout << "  stability <0|1>          - Aktiviert/deaktiviert Stabilitätsprüfung\n";
            std::cout << "  save <datei>             - Speichert Welt\n";
            std::cout << "  load <datei>             - Lädt Welt\n";
            std::cout << "  glider <x> <y>           - Setzt einen Glider\n";
            std::cout << "  toad <x> <y>             - Setzt einen Toad (Oszillator)\n";
            std::cout << "  beacon <x> <y>           - Setzt einen Beacon (Oszillator)\n";
            std::cout << "  methuselah <x> <y>       - Setzt ein R-Pentomino (Methuselah)\n";
            std::cout << "  random <n>               - Platziert n zufällige Muster\n";
            std::cout << "  exit                     - Beendet das Programm\n";
        }

        else if (command == "create") // create world x,y
        {
            int w, h;
            if (iss >> w >> h)
            {
                if (game)
                    delete game; // delete prev game
                game = new GameOfLife(w, h);
                std::cout << "Welt mit " << w << "x" << h << " erstellt.\n";
            }
            else
            {
                std::cout << "Fehler: Benutze 'create <breite> <höhe>'\n";
            }
        }
        else if (command == "set") // set singe cell
        {
            int x, y, state; // 2d
            if (iss >> x >> y >> state)
            {
                if (game)
                {
                    game->set_cell(x, y, state);
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else // 1d with pos
            {
                iss.clear();
                iss.seekg(0);
                iss >> command; // skip "set"
                int p;
                if (iss >> p >> state)
                {
                    if (game)
                    {
                        int w = game->get_width();
                        int h = game->get_height();
                        if (p >= 0 && p < w * h)
                        {
                            int x1 = p % w;
                            int y1 = p / w;
                            game->set_cell(x1, y1, state);
                        }
                        else
                        {
                            std::cout << "Fehler: Ungültige 1D-Zellposition.\n";
                        }
                    }
                    else
                    {
                        std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                    }
                }
                else
                {
                    std::cout << "Fehler: Benutze 'set <x> <y> <0|1>' oder 'set <position> <0|1>'\n";
                }
            }
        }
        else if (command == "print") // display world, deafult print on
        {
            int flag;
            if (iss >> flag)
            {
                print_enabled = (flag != 0);
                std::cout << "Automatisches Anzeigen nach Generationen ist "
                          << (print_enabled ? "aktiviert." : "deaktiviert.") << "\n";
            }
            else if (game)
            {
                game->print(); // if no argument directly print
            }
            else
            {
                std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
            }
        }

        else if (command == "stability") // uses is_stable, default stability on
        {
            int flag;
            if (iss >> flag)
            {
                stability_check_enabled = (flag != 0);
                std::cout << "Stabilitätsprüfung ist jetzt "
                          << (stability_check_enabled ? "aktiviert." : "deaktiviert.") << "\n";
            }
            else
            {
                std::cout << "Fehler: Benutze 'stability <0|1>'\n";
            }
        }

        else if (command == "run") // run for round
        {
            int generations;
            if (iss >> generations)
            {
                if (game)
                {
                    auto start = std::chrono::high_resolution_clock::now(); // timing addedd

                    for (int i = 0; i < generations; ++i)
                    {
                        game->evolve();

                        if (print_enabled)
                        {
                            game->print();
                            // std::cout << "[Debug] Generation " << i << " gezeichnet\n";
                            if (delay_ms > 0)
                                std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
                        }

                        // debug dcheck stability after every round
                        if (stability_check_enabled && game->is_stable())
                        {
                            std::cout << "Welt ist stabil nach " << (i + 1) << " Generation(en).\n";
                            break;
                        }
                    }

                    auto end = std::chrono::high_resolution_clock::now(); // stop timing
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

                    std::cout << generations << " Generation(en) in " << duration.count() << " ms.\n";
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                std::cout << "Fehler: Benutze 'run <anzahl_generationen>'\n";
            }
        }
        else if (command == "delay") // add dealy after each round, deafult = 0
        {
            int ms;
            if (iss >> ms)
            {
                delay_ms = ms;
                std::cout << "Verzögerung nach Generationen auf " << ms << " ms gesetzt.\n";
            }
            else
            {
                std::cout << "Fehler: Benutze 'delay <millisekunden>'\n";
            }
        }
        else if (command == "get") // get single cell value
        {
            int x, y; // 2d <- like set
            if (iss >> x >> y)
            {
                if (game)
                {
                    int value = game->get_cell(x, y);
                    if (value != -1)
                        std::cout << "Zelle (" << x << "," << y << ") = " << value << "\n";
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else // 1d pos
            {
                iss.clear();
                iss.seekg(0);
                iss >> command; // skip "get"
                int p;
                if (iss >> p)
                {
                    if (game)
                    {
                        // counter for 1d
                        int w = game->get_width();
                        int h = game->get_height();
                        if (p >= 0 && p < w * h)
                        {
                            int x1 = p % w;
                            int y1 = p / w;
                            int value = game->get_cell(x1, y1);
                            std::cout << "Zelle #" << p << " = Zelle(" << x1 << "," << y1 << ") = " << value << "\n";
                        }
                        else
                        {
                            std::cout << "Fehler: Ungültige 1D-Zellposition.\n";
                        }
                    }
                    else
                    {
                        std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                    }
                }
                else
                {
                    std::cout << "Fehler: Benutze 'get <x> <y>' oder 'get <position>'\n";
                }
            }
        }
        else if (command == "save") // save world
        {
            std::string filename;
            if (iss >> filename)
            {
                if (game)
                {
                    game->save(filename);
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                std::cout << "Fehler: Benutze 'save <filename>'\n";
            }
        }
        else if (command == "load") // load world
        {
            std::string filename;
            if (iss >> filename)
            {
                if (game)
                    delete game;

                game = new GameOfLife(1, 1); // for testing
                game->load(filename);
            }
            else
            {
                std::cout << "Fehler: Benutze 'load <filename>'\n";
            }
        }
        else if (command == "glider") // create glider https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
        {
            int x, y;
            if (iss >> x >> y)
            {
                if (game)
                {
                    game->set_cell((x + 1) % game->get_width(), y % game->get_height(), 1);
                    game->set_cell((x + 2) % game->get_width(), (y + 1) % game->get_height(), 1);
                    game->set_cell(x % game->get_width(), (y + 2) % game->get_height(), 1);
                    game->set_cell((x + 1) % game->get_width(), (y + 2) % game->get_height(), 1);
                    game->set_cell((x + 2) % game->get_width(), (y + 2) % game->get_height(), 1);
                    std::cout << "Glider bei (" << x << "," << y << ") gesetzt.\n";
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                std::cout << "Fehler: Benutze 'glider <x> <y>'\n";
            }
        }
        else if (command == "toad") // crewate toad: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
        {
            int x, y;
            if (iss >> x >> y)
            {
                if (game)
                {
                    int w = game->get_width();
                    int h = game->get_height();

                    game->set_cell((x + 1) % w, y % h, 1);
                    game->set_cell((x + 2) % w, y % h, 1);
                    game->set_cell((x + 3) % w, y % h, 1);
                    game->set_cell(x % w, (y + 1) % h, 1);
                    game->set_cell((x + 1) % w, (y + 1) % h, 1);
                    game->set_cell((x + 2) % w, (y + 1) % h, 1);

                    std::cout << "Toad bei (" << x << "," << y << ") gesetzt.\n";
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                std::cout << "Fehler: Benutze 'toad <x> <y>'\n";
            }
        }
        else if (command == "beacon") // create beacon: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
        {
            int x, y;
            if (iss >> x >> y)
            {
                if (game)
                {
                    int w = game->get_width();
                    int h = game->get_height();

                    // lower left corner
                    game->set_cell((x + 0) % w, (y + 0) % h, 1);
                    game->set_cell((x + 1) % w, (y + 0) % h, 1);
                    game->set_cell((x + 0) % w, (y + 1) % h, 1);
                    game->set_cell((x + 1) % w, (y + 1) % h, 1);

                    // upper right corner
                    game->set_cell((x + 2) % w, (y + 2) % h, 1);
                    game->set_cell((x + 3) % w, (y + 2) % h, 1);
                    game->set_cell((x + 2) % w, (y + 3) % h, 1);
                    game->set_cell((x + 3) % w, (y + 3) % h, 1);

                    std::cout << "Beacon bei (" << x << "," << y << ") gesetzt.\n";
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                std::cout << "Fehler: Benutze 'beacon <x> <y>'\n";
            }
        }
        else if (command == "methuselah") // create methusela https://en.wikipedia.org/wiki/Methuselah_(cellular_automaton)
        // https://conwaylife.com/wiki/R-pentomino
        {
            int x, y;
            if (iss >> x >> y)
            {
                if (game)
                {
                    int w = game->get_width();
                    int h = game->get_height();

                    game->set_cell((x + 1) % w, y % h, 1);
                    game->set_cell((x + 2) % w, y % h, 1);
                    game->set_cell(x % w, (y + 1) % h, 1);
                    game->set_cell((x + 1) % w, (y + 1) % h, 1);
                    game->set_cell((x + 1) % w, (y + 2) % h, 1);

                    std::cout << "Methuselah (R-Pentomino) bei (" << x << "," << y << ") gesetzt.\n";
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                std::cout << "Fehler: Benutze 'methuselah <x> <y>'\n";
            }
        }
        else if (command == "random") // add n random figures
        {
            int n;
            if (iss >> n)
            {
                if (game)
                {
                    int w = game->get_width();
                    int h = game->get_height();

                    for (int i = 0; i < n; ++i)
                    {
                        int pattern = rand() % 4; // 0=glider, 1=toad, 2=beacon, 3=methuselah
                        int x = rand() % w;
                        int y = rand() % h;

                        switch (pattern)
                        {
                        case 0: // glider
                            game->set_cell((x + 1) % w, y % h, 1);
                            game->set_cell((x + 2) % w, (y + 1) % h, 1);
                            game->set_cell(x % w, (y + 2) % h, 1);
                            game->set_cell((x + 1) % w, (y + 2) % h, 1);
                            game->set_cell((x + 2) % w, (y + 2) % h, 1);
                            break;
                        case 1: // toad
                            game->set_cell((x + 1) % w, y % h, 1);
                            game->set_cell((x + 2) % w, y % h, 1);
                            game->set_cell((x + 3) % w, y % h, 1);
                            game->set_cell(x % w, (y + 1) % h, 1);
                            game->set_cell((x + 1) % w, (y + 1) % h, 1);
                            game->set_cell((x + 2) % w, (y + 1) % h, 1);
                            break;
                        case 2: // beacon
                            game->set_cell((x + 0) % w, (y + 0) % h, 1);
                            game->set_cell((x + 1) % w, (y + 0) % h, 1);
                            game->set_cell((x + 0) % w, (y + 1) % h, 1);
                            game->set_cell((x + 1) % w, (y + 1) % h, 1);
                            game->set_cell((x + 2) % w, (y + 2) % h, 1);
                            game->set_cell((x + 3) % w, (y + 2) % h, 1);
                            game->set_cell((x + 2) % w, (y + 3) % h, 1);
                            game->set_cell((x + 3) % w, (y + 3) % h, 1);
                            break;
                        case 3: // methuselah (R-pentomino)
                            game->set_cell((x + 1) % w, y % h, 1);
                            game->set_cell((x + 2) % w, y % h, 1);
                            game->set_cell(x % w, (y + 1) % h, 1);
                            game->set_cell((x + 1) % w, (y + 1) % h, 1);
                            game->set_cell((x + 1) % w, (y + 2) % h, 1);
                            break;
                        }
                    }

                    std::cout << n << " zufällige Muster platziert.\n";
                }
                else
                {
                    std::cout << "Fehler: Es wurde noch keine Welt erstellt.\n";
                }
            }
            else
            {
                std::cout << "Fehler: Benutze 'random <anzahl>'\n";
            }
        }

        else
        {
            std::cout << "Unbekannter Befehl: " << command << "\n";
        }
    }

    std::cout << "Beendet.\n";
}

GameOfLifeCLI::~GameOfLifeCLI() // destructor
{
    if (game)
    {
        delete game;
    }
}
