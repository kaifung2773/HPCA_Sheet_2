#include "GameOfLife.hpp"
#include <iostream>
#include <fstream>

GameOfLife::GameOfLife(int width, int height)
    : width(width), height(height), grid(height, std::vector<int>(width, 0))
{
    std::cout << "GameOfLife mit " << width << "x" << height << " erstellt." << std::endl;
}

void GameOfLife::print() const
{
    std::cout << "\033[2J\033[H"; // Bildschirm löschen und Cursor zurücksetzen
    std::cout.flush();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (grid[y][x] == 1)
                std::cout << "\033[1m\033[32m\u2593\u2593\033[0m"; // grün lebendig
            else
                std::cout << "\033[1m\033[90m\u2591\u2591\033[0m"; // grau tot
        }
        std::cout << '\n';
    }
    std::cout.flush();
}

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
    std::vector<std::vector<int>> new_grid = grid; // Kopie für neue Generation

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int alive_neighbors = 0;

            // 8 Nachbarn prüfen (mit toroidalem Verhalten)
            for (int dy = -1; dy <= 1; ++dy)
            {
                for (int dx = -1; dx <= 1; ++dx)
                {
                    if (dx == 0 && dy == 0)
                        continue; // sich selbst überspringen

                    int nx = (x + dx + width) % width;   // toroidal: links/rechts des gitters verbinden
                    int ny = (y + dy + height) % height; // toroidal: oben/unten des gitters verbinden

                    alive_neighbors += grid[ny][nx];
                }
            }

            if (grid[y][x] == 1)
            {
                // lebende Zelle
                new_grid[y][x] = (alive_neighbors == 2 || alive_neighbors == 3) ? 1 : 0;
            }
            else
            {
                // tote Zelle
                new_grid[y][x] = (alive_neighbors == 3) ? 1 : 0;
            }
        }
    }

    grid = new_grid;
}

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
    grid = new_grid;

    std::cout << "Welt aus " << filename << " geladen.\n";
}

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
    // Stillleben: aktuelle == vorherige Generation
    if (grids_equal(grid, previous_generation))
        return true;

    // Oszillator: aktuelle == zwei Generationen vorher
    if (grids_equal(grid, two_generations_ago))
        return true;

    return false;
}
