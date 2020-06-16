//
// Created by Martin Corredor on 2019-04-06.
//
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tile.h"
#include <SFML/Window.hpp>

class Board{

public:
    int flags;
    std::vector<std::vector<Tile>> tiles;
    std::vector<sf::Vector2i> bombs;
    //Board(std::vector<std::vector<Tile>> &_tiles);

    Board();

    void SetBombs(std::vector<std::vector<int>> bombMap);

    void InitalizeBoard(); //scatter bombs and initialize tiles

    void SetBomb(); //sets bomb at random location - checks if bomb is already there first
    //void UpdateBoard();

    void SetNeighbors();

    bool IsPosValid(int r, int c);

    std::vector<sf::Vector2i> &GetBombs();

    int GetNumFlags();

    void AddFlag(int i);
};