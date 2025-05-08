#include "GameOfLife.hpp"
#include <iostream>

int main()
{
    GameOfLife game(5, 5); // Create instance of GameOfLife
    // "Blinker"-Pattern (ein einfacher periodischer Oszillator)
    game.set_cell(2, 1, 1);
    game.set_cell(2, 2, 1);
    game.set_cell(2, 3, 1);

    // loop to test is_stable
    for (int i = 0; i < 5; ++i)
    {
        std::cout << "Generation " << i << ":\n";
        game.print();
        if (game.is_stable())
        {
            std::cout << "Welt ist stabil.\n";
        }
        game.evolve();
    }

    // feel free to uncomment
    // std::cout << "Generation 0:" << std::endl;
    // game.print();

    // game.evolve();

    // std::cout << "Zelle (2,2): " << game.get_cell(2, 2) << std::endl;
    // std::cout << "Zelle (0,0): " << game.get_cell(0, 0) << std::endl;

    // std::cout << "Generation 1:" << std::endl;
    // game.print();

    // game.evolve();

    // std::cout << "Generation 2:" << std::endl;
    // game.print();

    // game.save("testwelt.txt");

    // GameOfLife random(1, 1); // Dummy-Größe
    // random.load("testwelt.txt");
    // random.print();

    return 0;
}
