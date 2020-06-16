//
// Created by Martin Corredor on 2019-04-06.
//

#include "Board.h"
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <stdio.h>

Board::Board() {
    flags=0;
    tiles=std::vector<std::vector<Tile>>();
    for(int r=0;r<16;r++){
        tiles.push_back(std::vector<Tile>());
        for(int c=0;c<25;c++){
            tiles[r].push_back(Tile(r,c,true,false));
        }
    }
}


void Board::SetBombs(std::vector<std::vector<int>> bombMap){
    std::vector<sf::Vector2i> newBombs;
    for(int r=0;r<16;r++)
        for(int c=0;c<25;c++){
            tiles[r][c].HideAndDisarm();
            if(bombMap[r][c]==1){
                tiles[r][c].SetBomb();
                sf::Vector2i pos(c,r);
                newBombs.push_back(pos);
            }
        }
    bombs=newBombs;
    SetNeighbors();
    flags=0;

}

void Board::InitalizeBoard() {
    flags=0;
    bombs=std::vector<sf::Vector2i>();
    for(int r=0;r<16;r++)
        for(int c=0;c<25;c++)
            tiles[r][c].HideAndDisarm();
    for(int i=0;i<50;i++)
        SetBomb();
    SetNeighbors();
}

void Board::SetBomb() {
    srand(time(NULL));

    int row=0;
    int col=0;
    bool bombPlaced=false;
    while(!bombPlaced){
        row=rand() % 16;
        col=rand() % 25;
        if(!tiles[row][col].IsBomb()){
            tiles[row][col].SetBomb();
            sf::Vector2i pos(col,row);
            bombs.push_back(pos);
            bombPlaced=true;
        }
    }
}

void Board::SetNeighbors() {
    for(int r=0;r<16;r++)
        for(int c=0;c<25;c++){
            Tile &curTile=tiles[r][c];
            if(IsPosValid(r-1,c-1))
                curTile.SetNeighbor(0,tiles[r-1][c-1]);
            if(IsPosValid(r-1,c))
                curTile.SetNeighbor(1,tiles[r-1][c]);
            if(IsPosValid(r-1,c+1))
                curTile.SetNeighbor(2,tiles[r-1][c+1]);
            if(IsPosValid(r,c-1))
                curTile.SetNeighbor(3,tiles[r][c-1]);
            if(IsPosValid(r,c+1))
                curTile.SetNeighbor(4,tiles[r][c+1]);
            if(IsPosValid(r+1,c-1))
                curTile.SetNeighbor(5,tiles[r+1][c-1]);
            if(IsPosValid(r+1,c))
                curTile.SetNeighbor(6,tiles[r+1][c]);
            if(IsPosValid(r+1,c+1))
                curTile.SetNeighbor(7,tiles[r+1][c+1]);
        }
    for(int i=0;i<16;i++) {
        for (int j=0;j<25;j++) {
            tiles[i][j].CountNeighboringBombs();
        }
    }
}

bool Board::IsPosValid(int r, int c) {
    if(r>-1 && c>-1 && r<16 && c<25)
        return true;
    return false;
}

std::vector<sf::Vector2i>& Board::GetBombs() {
    return bombs;
}

int Board::GetNumFlags() {
    return flags;
}

void Board::AddFlag(int i) {
    flags+=i;
}