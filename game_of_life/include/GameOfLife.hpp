#ifndef GAME_OF_LIFE_HPP
#define GAME_OF_LIFE_HPP

#include <vector>
#include <string>

class GameOfLife
{
public: // class declaration (public) with constructor
    GameOfLife(int width, int height);
    void print() const;                     // Gitter anzeigen
    void set_cell(int x, int y, int state); // set cells
    void evolve();
    int get_cell(int x, int y) const;
    void save(const std::string &filename) const;
    void load(const std::string &filename);
    bool is_stable();
    int get_width() const { return width; }
    int get_height() const { return height; }

private: // private attributes
    int width;
    int height;
    std::vector<std::vector<int>> grid; // vector of vectors
    std::vector<std::vector<int>> previous_generation;
    std::vector<std::vector<int>> two_generations_ago;
};

#endif // GAME_OF_LIFE_HPP
