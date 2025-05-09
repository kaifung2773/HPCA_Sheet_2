#include "GameOfLife.hpp"
#include <iostream>
#include <fstream>

GameOfLife::GameOfLife(int width, int height)
    : width(width), height(height), grid(height, std::vector<int>(width, 0))
{
    std::cout << "GameOfLife mit " << width << "x" << height << " erstellt." << std::endl;
}

// print out grid
void GameOfLife::print() const
{
    for (const auto &row : grid)
    {
        for (int cell : row)
        {
            std::cout << (cell ? "O" : ".") << " ";
        }
        std::cout << "\n";
    }
}

// for later use
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

void GameOfLife::evolve()
{
    two_generations_ago = previous_generation;
    previous_generation = grid;
    std::vector<std::vector<int>> new_grid = grid; // copy for next generation

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int alive_neighbors = 0;

            // Check all 8 neighbouring cells
            for (int dy = -1; dy <= 1; ++dy)
            {
                for (int dx = -1; dx <= 1; ++dx)
                {
                    if (dx == 0 && dy == 0)
                        continue; // sich selbst überspringen

                    // Maybe "+ width" and "+ height" can be removed
                    int nx = (x + dx + width) % width;   // toroidal: connect right/left parts of grig
                    int ny = (y + dy + height) % height; // toroidal: connect top/bottom

                    alive_neighbors += grid[ny][nx];
                }
            }

            if (grid[y][x] == 1)
            {
                // living cell
                new_grid[y][x] = (alive_neighbors == 2 || alive_neighbors == 3) ? 1 : 0;
            }
            else
            {
                // dead cell
                new_grid[y][x] = (alive_neighbors == 3) ? 1 : 0;
            }
        }
    }

    grid = new_grid;
}

// for later use
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

void GameOfLife::save(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Fehler beim Öffnen der Datei zum Speichern.\n";
        return;
    }

    file << width << " " << height << "\n";
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

void GameOfLife::load(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Fehler beim Öffnen der Datei zum Laden.\n";
        return;
    }

    int w, h;
    file >> w >> h;
    std::vector<std::vector<int>> new_grid(h, std::vector<int>(w));

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            file >> new_grid[y][x];
        }
    }

    width = w;
    height = h;
    // grid can be initialized without new_grid
    grid = new_grid;

    std::cout << "Welt aus " << filename << " geladen.\n";
}

// Usesd for is_stable, still need to work on is_stagble
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

// still need to work on is_stagble
bool GameOfLife::is_stable()
{
    // Stillleben: aktuelle == vorherige Generation
    if (grids_equal(grid, previous_generation))
        return true;

    // Oszillator: aktuelle == zwei Generationen vorher
    if (grids_equal(grid, two_generations_ago))
        return true;

    return false;
}