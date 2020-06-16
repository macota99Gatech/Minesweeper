//
// Created by Martin Corredor on 2019-04-06.
//

#include "Tile.h"
#include <iostream>

Tile::Tile(float _x, float _y, bool _hidden, bool _bomb) {
    x=_x;
    y=_y;
    hidden=_hidden;
    bomb=_bomb;
    for(int i=0;i<8;i++)
        neighbors[i]=nullptr;
    numBombsAround=0;
}

Tile::Tile() {
    x=0;
    y=0;
    hidden=true;
    bomb=false;
    for(int i=0;i<8;i++)
        neighbors[i]=nullptr;
    numBombsAround=0;
}

Tile::Tile(bool _bomb){
    x=0;
    y=0;
    hidden=true;
    for(int i=0;i<8;i++)
        neighbors[i]=nullptr;
    numBombsAround=0;
    bomb=_bomb;
}

sf::Vector2f Tile::GetPosition() {
    sf::Vector2f pos(x,y);
    return pos;
}

void Tile::Reveal() {
    hidden=false;
    if(numBombsAround==0 && !bomb)
        RevealEmptyNeighbors();
}

void Tile::HideAndDisarm() {
    hidden=true;
    bomb=false;
    numBombsAround=0;
    isFlagged=false;
}
//should only be called if not a bomb itself

void Tile::CountNeighboringBombs() {
    for(int i=0;i<8;i++){//TODO add prints and debug why its not incrementing number of bombs around
        if(neighbors[i]!=nullptr) {
            Tile curNeighbor=*neighbors[i];
            if (curNeighbor.IsBomb()) {
                numBombsAround++;
            }
        }
    }
}

bool Tile::IsBomb() {
    return bomb;
}

void Tile::SetBomb() {
    bomb=true;
}

void Tile::SetNeighbor(int index, Tile &tile) {
    neighbors[index]=&tile;
}

bool Tile::IsHidden() {
    return hidden;
}

int Tile::GetNumBombsAround(){
    return numBombsAround;
}

void Tile::RevealEmptyNeighbors(){
    for(int i=0;i<8;i++){
        if(neighbors[i]!=nullptr){
            int bombsAroundNeighbor=neighbors[i]->GetNumBombsAround();
            bool neighborIsHidden=neighbors[i]->IsHidden();
            bool neighborIsNotBomb=!neighbors[i]->IsBomb();
            bool neighborIsFlagged=neighbors[i]->IsFlagged();
            if (bombsAroundNeighbor==0 && neighborIsHidden && neighborIsNotBomb && !neighborIsFlagged)
                neighbors[i]->Reveal();
            else if(neighborIsHidden && neighborIsNotBomb && !neighborIsFlagged)
                neighbors[i]->Reveal();
        }
    }
}


bool Tile::IsFlagged() {
    return isFlagged;
}

void Tile::Flag() {
    isFlagged=!isFlagged;
}

