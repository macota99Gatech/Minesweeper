//
// Created by Martin Corredor on 2019-04-06.
//

#pragma once
#include <SFML/Graphics.hpp>


class Tile{
private:
    float x, y;  // does a tile need to know its location or can the board take care of it?
    bool hidden;   //true if hidden, false if revealed
    bool bomb; //true if bomb
    Tile* neighbors[8];
    int numBombsAround; //must be calculated after initialization of whole board
    bool isFlagged;

public:
    Tile(float _x, float _y, bool _hidden, bool _bomb);

    Tile();

    Tile(bool bomb);

    void Reveal();

    void HideAndDisarm();

    void CountNeighboringBombs();

    sf::Vector2f GetPosition();

    bool IsBomb();

    void SetBomb();

    void SetNeighbor(int index, Tile& tile);

    bool IsHidden();

    int GetNumBombsAround();

    void RevealEmptyNeighbors();


    bool IsFlagged();

    void Flag();
};