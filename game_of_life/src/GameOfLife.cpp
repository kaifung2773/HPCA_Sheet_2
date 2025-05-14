#include "GameOfLife.hpp"
#include <iostream>
#include <fstream>

// Constructor: Initialize grid with width, height
GameOfLife::GameOfLife(int width, int height)
    : width(width), height(height), grid(height, std::vector<int>(width, 0))
{
    std::cout << "GameOfLife mit " << width << "x" << height << " erstellt." << std::endl;
}

void GameOfLife::print() const
{
    std::cout << "\033[2J\033[H"; // Delete output and reset cursor (Hint on Exercisesheet)
    std::cout.flush();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (grid[y][x] == 1)
                std::cout << "\033[1m\033[32m\u2593\u2593\033[0m"; // As recommended: green for living cells
            else
                std::cout << "\033[1m\033[90m\u2591\u2591\033[0m"; // : grey for dead cells
        }
        std::cout << '\n';
    }
    std::cout.flush();
}

// Set state of one cell with x and y coordinates
void GameOfLife::set_cell(int x, int y, int state)
{
    if (y >= 0 && y < height && x >= 0 && x < width)
    {
        grid[y][x] = state;
    }
    else
    {
        std::cerr << "Fehler: Ungültige Position (" << x << ", " << y << ")" << std::endl;
    }
}

// Run one round of the game
void GameOfLife::evolve()
{
    // tracking generations
    two_generations_ago = previous_generation;     // used for is_stable: two generations
    previous_generation = grid;                    // used in is_stable
    std::vector<std::vector<int>> new_grid = grid; // Copy for new generation

    // Loop over all cells: Y for rows, X for columns
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // Counter for living cells
            int alive_neighbors = 0;

            // Second loop to check all neighboring cells
            for (int dy = -1; dy <= 1; ++dy)
            {
                for (int dx = -1; dx <= 1; ++dx)
                {
                    if (dx == 0 && dy == 0)
                        continue; // skip current cell
                    // Toroidal issue:
                    // https://stackoverflow.com/questions/40430803/conway-game-of-life-toroidal-approach-rim-and-corners
                    int nx = (x + dx + width) % width;   // connecting edges left and right of the grid with mod
                    int ny = (y + dy + height) % height; // connecting top and bottom of the grid with mod height

                    alive_neighbors += grid[ny][nx];
                }
            }

            if (grid[y][x] == 1) // living cell:  survivces with 2-3 neighbours
            {
                new_grid[y][x] = (alive_neighbors == 2 || alive_neighbors == 3) ? 1 : 0;
            }
            else // dead cell: lives with 3 living neighbours
            {
                new_grid[y][x] = (alive_neighbors == 3) ? 1 : 0;
            }
        }
    }

    grid = new_grid; // replace grid
}

// Get Value of a cell with x and y coordinates.
int GameOfLife::get_cell(int x, int y) const
{
    if (y >= 0 && y < height && x >= 0 && x < width)
    {
        return grid[y][x];
    }
    else
    {
        std::cerr << "Fehler: Ungültige Position (" << x << ", " << y << ")" << std::endl;
        return -1;
    }
}

// save game with param string filename
void GameOfLife::save(const std::string &filename) const
{
    std::ofstream file(filename); // open file
    if (!file)                    // (debug) check if able to open file
    {
        std::cerr << "Fehler beim Öffnen der Datei zum Speichern.\n";
        return;
    }

    file << width << " " << height << "\n"; // Write Size into file
    for (const auto &row : grid)
    {
        for (int cell : row)
        {
            file << cell << " ";
        }
        file << "\n";
    }

    std::cout << "Welt wurde in " << filename << " gespeichert.\n";
}

// load file
void GameOfLife::load(const std::string &filename)
{
    std::ifstream file(filename); // read
    if (!file)
    {
        std::cerr << "Fehler beim Öffnen der Datei zum Laden.\n";
        return;
    }

    int w, h; // read first line with parameters for x,y
    file >> w >> h;
    std::vector<std::vector<int>> new_grid(h, std::vector<int>(w)); // init new grid

    // iterate over rows/ columns and write new grid
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            file >> new_grid[y][x];
        }
    }

    width = w;
    height = h;
    grid = new_grid;

    std::cout << "Welt aus " << filename << " geladen.\n";
}

// compare two grids used in is stable funtion
static bool grids_equal(const std::vector<std::vector<int>> &a,
                        const std::vector<std::vector<int>> &b)
{
    if (a.size() != b.size())
        return false;
    for (size_t y = 0; y < a.size(); ++y)
    {
        if (a[y] != b[y])
            return false;
    }
    return true;
}

bool GameOfLife::is_stable()
{
    // still: aktuelle == vorherige Generation
    if (grids_equal(grid, previous_generation))
        return true;

    // oscillating: aktuelle == zwei Generationen vorher
    if (grids_equal(grid, two_generations_ago))
        return true;

    return false;
}
