#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Board.h"
#include "Tile.h"
#include <map>
#include <string>
#include <fstream>


void LoadTextures();
void DrawInitialBoard();
void DrawBottomButtons(std::string emotion);
void DrawFlagCounter();
void DrawFace(std::string emotion);
void DrawTests();
std::string ButtonClicked(sf::Vector2i position);
Tile &GetTileClicked(sf::Vector2i position);
void UpdateState(bool debug, std::string emotion);
void DrawBombs();
void GameOver();
void Restart(bool debug);
std::vector<std::vector<int>> ReadBoard(std::string fileName);

std::map<std::string,sf::Texture> textures;
sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper");
Board board;

int main() {

    board.InitalizeBoard();
    LoadTextures();

    UpdateState(false, "HappyFace");

    bool debug=false;
    bool gameOver=false;

    while(window.isOpen()){
        sf::Event event;

        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button == sf::Mouse::Left && !gameOver){
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::string buttonClicked=ButtonClicked(mousePosition);
                    if(buttonClicked == "Tile"){
                        Tile &tileClicked=GetTileClicked(mousePosition);
                        if(!tileClicked.IsFlagged()) {
                            if (tileClicked.IsBomb()) {
                                GameOver();
                                gameOver = true;
                            } else if (tileClicked.IsHidden()) {
                                tileClicked.Reveal();
                                UpdateState(debug, "HappyFace");
                            }
                        }
                    }
                    if(buttonClicked == "Debug"){
                        debug=!debug;
                        UpdateState(debug, "HappyFace");
                    }
                    if(buttonClicked == "Face"){
                        Restart(debug);

                        gameOver=false;
                    }
                    if(buttonClicked == "Test1"){
                        std::vector<std::vector<int>> TestBoardBombs=ReadBoard("boards/testboard1.brd");
                        board.SetBombs(TestBoardBombs);
                        UpdateState(debug, "HappyFace");
                    }
                    if(buttonClicked == "Test2"){
                        std::vector<std::vector<int>> TestBoardBombs=ReadBoard("boards/testboard2.brd");
                        board.SetBombs(TestBoardBombs);
                        UpdateState(debug, "HappyFace");
                    }
                    if(buttonClicked == "Test3"){
                        std::vector<std::vector<int>> TestBoardBombs=ReadBoard("boards/testboard3.brd");
                        board.SetBombs(TestBoardBombs);
                        UpdateState(debug, "HappyFace");
                    }
                }
                if(event.mouseButton.button == sf::Mouse::Left && gameOver){
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::string buttonClicked=ButtonClicked(mousePosition);
                    if(buttonClicked == "Face"){
                        Restart(debug);
                        gameOver=false;
                    }
                }
                if(event.mouseButton.button == sf::Mouse::Right && !gameOver){
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                   // std::cout << mousePosition.x << std::endl << mousePosition.y << std::endl;
                    std::string buttonClicked=ButtonClicked(mousePosition);
                    if(buttonClicked == "Tile") {
                        Tile &tileClicked = GetTileClicked(mousePosition);
                        if(tileClicked.IsHidden()) {
                            tileClicked.Flag();
                            if (tileClicked.IsFlagged())
                                board.AddFlag(1);
                            else
                                board.AddFlag(-1);
                        }
                        UpdateState(debug, "HappyFace");
                    }
                }
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }

    }
    return 0;
}

std::string ButtonClicked(sf::Vector2i position){
    int x=position.x;
    int y=position.y;
    if(y<512)
        return "Tile";
    if(y>511 & y<576){
        if(x>367 && x<433)
            return "Face";

//        if(x>543 && x<608)
//            return "Debug";
//        if(x>607 && x<672)
//            return "Test1";    right aligned buttons
//        if(x>671 && x<736)
//            return "Test2";
//        if(x>735)
//            return "Test3";

        if(x>495 && x<560)
            return "Debug";
        if(x>559 && x<624)
            return "Test1";
        if(x>623 && x<688)
            return "Test2";
        if(x>687 && x<752)
            return "Test3";

    }
    return "";
}

Tile &GetTileClicked(sf::Vector2i position){
    int x=position.x/32;
    int y=position.y/32;
    return board.tiles[y][x];
}
void DrawInitialBoard(){
    for(int r=0;r<25;r++)
        for(int c=0;c<16;c++){
            sf::Sprite defaultTile;
            defaultTile.setTexture(textures["DefaultTexture"]);
            defaultTile.move(r*32,c*32);
            window.draw(defaultTile);
        }
    DrawBottomButtons("HappyFace");
}



void DrawBottomButtons(std::string emotion){
    sf::Sprite digits;
    DrawFlagCounter();
    DrawFace(emotion);
    DrawTests();
}

void DrawFlagCounter(){
    int flags=board.GetNumFlags();
    int tens=0;
    int ones=0;
    int hundreds=0;
    int drawFlags=0;

//    if(flags<51)
//        drawFlags=50-flags;
//    else{
//        drawFlags=flags-50;
//        hundreds=10;
//    }
    int numBombs=board.bombs.size();
    if(flags<numBombs+1)
        drawFlags=numBombs-flags;
    else{
        drawFlags=flags-numBombs;
        hundreds=10;
    }

    tens=drawFlags/10;
    ones=drawFlags%10;
    sf::Sprite tensSprite;
    tensSprite.setTexture(textures["Digits"]);
    tensSprite.setTextureRect(sf::IntRect(0+tens*21,0,21,32));

    sf::Sprite onesSprite;
    onesSprite.setTexture(textures["Digits"]);
    onesSprite.setTextureRect(sf::IntRect(0+21*ones,0,21,32));

    sf::Sprite hundredsSprite;
    hundredsSprite.setTexture(textures["Digits"]);
    hundredsSprite.setTextureRect(sf::IntRect(0+21*hundreds,0,21,32));

    hundredsSprite.move(0,512);
    tensSprite.move(21,512);
    onesSprite.move(42,512);

    window.draw(hundredsSprite);
    window.draw(tensSprite);
    window.draw(onesSprite);
}

void DrawFace(std::string emotion){
    sf::Sprite faceSprite;
    faceSprite.setTexture(textures[emotion]);

    faceSprite.move(368,512);
    window.draw(faceSprite);
}

void DrawTests(){
    sf::Sprite test1;
    sf::Sprite test2;
    sf::Sprite test3;
    sf::Sprite debug;

    test1.setTexture(textures["Test1"]);
    test2.setTexture(textures["Test2"]);
    test3.setTexture(textures["Test3"]);
    debug.setTexture(textures["Debug"]);

//    debug.move(544,512);
//    test1.move(608,512);         right aligned buttons
//    test2.move(672,512);
//    test3.move(736,512);

    debug.move(496,512);
    test1.move(560,512);
    test2.move(624,512);
    test3.move(688,512);

    window.draw(test1);
    window.draw(test2);
    window.draw(test3);
    window.draw(debug);

}

void UpdateState(bool debug, std::string emotion){
    DrawInitialBoard();

    int clearedTiles=0;
    int numBombs=0;

    for(int r=0;r<16;r++)
        for(int c=0;c<25;c++){
            if(board.tiles[r][c].IsBomb())
                numBombs++;
            if(!board.tiles[r][c].IsHidden()){ //only draw once ?
                //std::cout << r << std::endl << c << std::endl;
                sf::Sprite revealedTile;
                revealedTile.setTexture(textures["RevealedTile"]);
                revealedTile.move(c*32,r*32);
                window.draw(revealedTile);

                int numOnTile=board.tiles[r][c].GetNumBombsAround();
                //std::cout << numOnTile << std::endl;
                if(numOnTile!=0 && !board.tiles[r][c].IsBomb()){
                    sf::Sprite numTile;
                    numTile.setTexture(textures[std::to_string(numOnTile)]);
                    numTile.move(c*32,r*32);
                    window.draw(numTile);
                }
                clearedTiles++;
            }
            if(board.tiles[r][c].IsHidden() && board.tiles[r][c].IsFlagged()){
                sf::Sprite flag;
                flag.setTexture(textures["Flag"]);
                flag.move(c*32,r*32);
                window.draw(flag);
            }
        }
    bool win=false;
    if(clearedTiles==400-numBombs){
        win=true;
        emotion="WinFace";
        std::vector<sf::Vector2i> &bombs=board.GetBombs();
        for(int i=0;i<bombs.size();i++){
            int r=bombs[i].x;
            int c=bombs[i].y;
            if(!board.tiles[c][r].IsFlagged())
                board.tiles[c][r].Flag();
            sf::Sprite flag;
            flag.setTexture(textures["Flag"]);
            flag.move(r*32,c*32);
            window.draw(flag);
        }
    }

    DrawBottomButtons(emotion);
    if(debug && !win)
        DrawBombs();


    window.display();
}

void GameOver(){
    std::vector<sf::Vector2i> &bombs=board.GetBombs();
    for(int i=0;i<board.bombs.size();i++){
        int r=bombs[i].x;
        int c=bombs[i].y;
        board.tiles[c][r].Reveal();
    }
    DrawBombs();
    UpdateState(true, "LoseFace");
}

std::vector<std::vector<int>> ReadBoard(std::string fileName){
    std::ifstream boardFile;
    boardFile.open(fileName);
    std::vector<std::vector<int>> boardMap;
    for(int i=0;i<16;i++){
        std::vector<int> row;
        std::string rowLine;
        std::getline(boardFile, rowLine);
        for(int j=0;j<25;j++){
            int a=0;
            a=stoi(rowLine.substr(j,1));
            row.push_back(a);
        }
        boardMap.push_back(row);
    }
    boardFile.close();
    return boardMap;
}

void DrawBombs(){
    for(int r=0;r<16;r++){
        for(int c=0;c<25;c++){
            if(board.tiles[r][c].IsBomb()){
                sf::Sprite bomb;
                bomb.setTexture(textures["Bomb"]);
                bomb.move(c*32,r*32);
                window.draw(bomb);
            }
        }
    }
}

void Restart(bool debug){

    board.InitalizeBoard();
    UpdateState(debug,"HappyFace");

}

void LoadTextures(){
    sf::Texture defaultTexture;
    defaultTexture.loadFromFile("images/tile_hidden.png");
    textures.emplace("DefaultTexture",defaultTexture);

    sf::Texture revealedTile;
    revealedTile.loadFromFile("images/tile_revealed.png");
    textures.emplace("RevealedTile",revealedTile);

    sf::Texture digits;
    digits.loadFromFile("images/digits.png");
    textures.emplace("Digits",digits);

    sf::Texture happyFace;
    happyFace.loadFromFile("images/face_happy.png");
    textures.emplace("HappyFace",happyFace);

    sf::Texture loseFace;
    loseFace.loadFromFile("images/face_lose.png");
    textures.emplace("LoseFace",loseFace);

    sf::Texture winFace;
    winFace.loadFromFile("images/face_win.png");
    textures.emplace("WinFace",winFace);

    sf::Texture test1;
    test1.loadFromFile("images/test_1.png");
    textures.emplace("Test1",test1);

    sf::Texture test2;
    test2.loadFromFile("images/test_2.png");
    textures.emplace("Test2",test2);

    sf::Texture test3;
    test3.loadFromFile("images/Test_3.png");
    textures.emplace("Test3",test3);

    sf::Texture debug;
    debug.loadFromFile("images/debug.png");
    textures.emplace("Debug",debug);

    sf::Texture num1;
    num1.loadFromFile("images/number_1.png");
    textures.emplace("1",num1);

    sf::Texture num2;
    num2.loadFromFile("images/number_2.png");
    textures.emplace("2",num2);

    sf::Texture num3;
    num3.loadFromFile("images/number_3.png");
    textures.emplace("3",num3);

    sf::Texture num4;
    num4.loadFromFile("images/number_4.png");
    textures.emplace("4",num4);

    sf::Texture num5;
    num5.loadFromFile("images/number_5.png");
    textures.emplace("5",num5);

    sf::Texture num6;
    num6.loadFromFile("images/number_6.png");
    textures.emplace("6",num6);

    sf::Texture num7;
    num7.loadFromFile("images/number_7.png");
    textures.emplace("7",num7);

    sf::Texture num8;
    num8.loadFromFile("images/number_8.png");
    textures.emplace("8",num8);

    sf::Texture bomb;
    bomb.loadFromFile("images/mine.png");
    textures.emplace("Bomb",bomb);

    sf::Texture flag;
    flag.loadFromFile("images/flag.png");
    textures.emplace("Flag",flag);
}