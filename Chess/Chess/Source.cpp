#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace std;

//Since the board is 600x600 for an 8x8, each square is a 75x75 piece

vector<vector<int>> board = { {2,2,2,2,-2,2,2,2},
                            {2,2,2,2,2,2,2,2},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {1,1,1,1,1,1,1,1},
                            {1,1,1,1,-1,1,1,1} };

int vectX = 0;
int vectY = 0;
int check = 0;//Count # of people who are "checking" the king.
int checkColor = 0;
int checkX = 0;
int checkY = 0;
int colorP = 0;
bool blockableP = false;

int wKingPosX = 4;
int wKingPosY = 7;

int bKingPosX = 4;
int bKingPosY = 0;
//6 == Rook
//5 == Horse
//4 == Bishop
//3 == Queen
//2 == King
//1 == Pawns

vector<vector<int>> boardPi ={{-6,-5,-4,-2,-3,-4,-5,-6},
                            {-1,-1,-1,-1,-1,-1,-1,-1},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {1,1,1,1,1,1,1,1},
                            {6,5,4,3,2,4,5,6} };

class chessPieces;

class movementStrat {
public:
    virtual void move(chessPieces* piece) = 0;
};

chessPieces* pieceThatIsChecking;

class chessPieces : public sf::Sprite {
private:
    int color; //0 = white, 1 = black
    bool firstMove = false;
    bool isAlive = true;
    bool isPawn = false;
    bool isRook = false;
    movementStrat* strat = nullptr;
    vector<vector<int>> availSpaces;
    vector<vector<int>> specialSpaces;
    int boardX;
    int boardY;
    bool blockable = true;

public:
    chessPieces(int x, int y, int c) {
        color = c;
        boardX = x;
        boardY = y;
    }
    void firstMoveMade() {
        firstMove = true;
    }
    bool getFirstMove() {
        return firstMove;
    }
    bool checkAlive() {
        return isAlive;
    }
    void setPawn() {
       isPawn = true;
    }
    bool getPawn() {
        return isPawn;
    }
    void setRook() {
        isRook = true;
    }
    bool getRook() {
        return isRook;
    }
    void setMoveStrat(movementStrat* strat_) {
        strat = strat_;
    }
    movementStrat* getMovementStrat() {
        return strat;
    }
    void movement() {
        strat->move(this);
    }
    void pushAvailSpaces(vector<int> temp) {
        availSpaces.push_back(temp);
    }
    void pushSpecialSpaces(vector<int> temp) {
        specialSpaces.push_back(temp);
    }
    void clearAvailSpaces() {
        availSpaces.clear();
    }
    void clearspecialSpaces() {
        specialSpaces.clear();
    }
    vector<vector<int>> getAvailSpaces() {
        return availSpaces;
    }
    vector<vector<int>> getSpecialSpaces() {
        return specialSpaces;
    }
    void setNewPosition(int x, int y) {
        board[y][x] = board[vectY][vectX];
        board[vectY][vectX] = 0;
        cout << "changing board position from (" << vectX << ", " << vectY << ") to (" << x << " , " << y << ")" << endl;
        boardX = x;
        boardY = y;
    }
    void setNewPositionCastle(int x, int y, int vX, int vY) {
        board[y][x] = board[vY][vX];
        board[vY][vX] = 0;
        cout << "changing board position from (" << vX << ", " << vY << ") to (" << x << " , " << y << ")" << endl;
        boardX = x;
        boardY = y;
    }
    int getPositionX() {
        return boardX;
    }
    int getPositionY() {
        return boardY;
    }
    void killPiece() {
        board[this->getPositionY()][this->getPositionX()] = 0;
        isAlive = false;
    }
    int getColor() {
        return color;
    }
    void setBlockable() {
        blockable = false;
    }
    bool getBlockable() {
        return blockable;
    }
    void checkerino(int color, int positionX, int positionY) {
        ++check;
        checkX = positionX;
        checkY = positionY;
        blockableP = true;
        colorP = color;
        if (color == 1) {
            checkColor = 2;
        }
        else {
            checkColor = 1;
        }
        pieceThatIsChecking = this;
    }
};

vector<chessPieces> holder;



chessPieces findPiece(int x, int y) {
    for (int i = 0; i < holder.size(); i++) {
        if (holder[i].getPositionX() == x && holder[i].getPositionY() == y) {
            return holder[i];
        }
    }
}



class pawnWhiteMovement : public movementStrat {
public:
    virtual void move(chessPieces* pawn) {
        pawn->setBlockable();
        pawn->setPawn();//Setting isPawn to  true
        bool fMove = pawn->getFirstMove();
        int positionX = pawn->getPositionX();
        int positionY = pawn->getPositionY();

        int color = pawn->getColor();

        if (positionY != 0 && positionX != 0 && board[positionY - 1][positionX - 1] != 0) {
            pawn->pushAvailSpaces(vector<int> {positionY - 1, positionX - 1});
            pawn->pushSpecialSpaces(vector<int> {positionY - 1, positionX - 1});
            if (board[positionY - 1][positionX - 1] != color * -1 && board[positionY - 1][positionX- 1] < 0) {
                pawn->checkerino(color, positionX, positionY);
            }
        }
        if (positionY != 0 && positionX != 7 && board[positionY - 1][positionX + 1] != 0) {
            pawn->pushAvailSpaces(vector<int> {positionY - 1, positionX + 1});
            pawn->pushSpecialSpaces(vector<int> {positionY - 1, positionX + 1});
            if (board[positionY - 1][positionX + 1] != color * -1 && board[positionY - 1][positionX + 1] < 0) {
                pawn->checkerino(color, positionX, positionY);
            }
        }

        if (board[positionY - 1][positionX] != 0 || positionY == 0) {
            return;
        }
        else if (!fMove) {//No piece in front, but you haven't made your first move yet
            if (board[positionY - 2][positionX] != 0) {
                
                pawn->pushAvailSpaces(vector<int> {positionY - 1, positionX});
            }
            else {//If there isnt as piece two moves ahead, then both spaces are free
                pawn->pushAvailSpaces(vector<int> {positionY - 1, positionX});
                pawn->pushAvailSpaces(vector<int> {positionY - 2, positionX});
            }
        }
        else {//There isnt a piece in front, but you also made your first move already
            pawn->pushAvailSpaces(vector<int> {positionY - 1, positionX});
        }
    }
    ~pawnWhiteMovement() {

    }
};

class pawnBlackMovement : public movementStrat {
public:
    virtual void move(chessPieces* pawn) {
        pawn->setBlockable();
        pawn->setPawn();
        bool fMove = pawn->getFirstMove();
        int positionX = pawn->getPositionX();
        int positionY = pawn->getPositionY();

        int color = pawn->getColor();

        if (positionY != 7 && positionX != 0 && board[positionY + 1][positionX - 1] != 0) {
            pawn->pushAvailSpaces(vector<int> {positionY + 1, positionX - 1});
            pawn->pushSpecialSpaces(vector<int> {positionY + 1, positionX - 1});
            if (board[positionY + 1][positionX - 1] != color * -1 && board[positionY + 1][positionX - 1] < 0) {
                pawn->checkerino(color, positionX, positionY);
            }
        }
        if (positionY != 7 && positionX != 7 && board[positionY + 1][positionX + 1] != 0) {
            pawn->pushAvailSpaces(vector<int> {positionY + 1, positionX + 1});
            pawn->pushSpecialSpaces(vector<int> {positionY + 1, positionX + 1});
            if (board[positionY + 1][positionX + 1] != color * -1 && board[positionY + 1][positionX + 1] < 0) {
                pawn->checkerino(color, positionX, positionY);
            }
        }

        if (board[positionY + 1][positionX] != 0 || positionY == 7) {
            return;
        }
        else if (!fMove) {//No piece in front, but you haven't made your first move yet
            if (board[positionY + 2][positionX] != 0) {
                pawn->pushAvailSpaces(vector<int> {positionY + 1, positionX});
            }
            else {//If there isnt as piece two moves ahead, then both spaces are free

                pawn->pushAvailSpaces(vector<int> {positionY + 1, positionX});
                pawn->pushAvailSpaces(vector<int> {positionY + 2, positionX});
            }
        }
        else {//There isnt a piece in front, but you also made your first move already
            pawn->pushAvailSpaces(vector<int> {positionY + 1, positionX});
        }
    }
    ~pawnBlackMovement() {

    }
};


class rookMovement : public movementStrat {
public:
    virtual void move(chessPieces* rook) {
        bool fMove = rook->getFirstMove();
        rook->setRook();//SEtting rook to true.
        int positionX = rook->getPositionX();
        int positionY = rook->getPositionY();

        int left = positionX;//Look for left values
        int right = positionX;//Look for right values

        int color = rook->getColor();

        while (left-1 >= 0 && board[positionY][left-1] == 0) {
            --left;
        }
        
        if (left != 0 && board[positionY][left - 1] != color) {
            --left;
            if (board[positionY][left] != color * -1 && board[positionY][left] < 0) {
                rook->checkerino(color, positionX, positionY);
            }
        }

        while (right+1 <= 7 && board[positionY][right+1] == 0) {
            ++right;
        }
        
        if (right != 7 && board[positionY][right + 1] != color) {
            ++right;
            if (board[positionY][right] != color * -1 && board[positionY][right] < 0) {
                rook->checkerino(color, positionX, positionY);
            }
        }
        
        //cout << "left: " << left << endl;
        //cout << "right: " << right << endl;

        for (int i = left; i <= right; i++) {
            if (i == positionX) {
                continue;
            }
            rook->pushAvailSpaces(vector<int> {positionY, i});
        }

        int top = positionY;
        int bot = positionY;
        

        while (top-1 >= 0 && board[top-1][positionX] == 0) {
            --top;
        }
        if (top != 0 && board[top-1][positionX] != color) {
            --top;
            if (board[top][positionX] != color * -1 && board[top][positionX] < 0) {
                rook->checkerino(color, positionX, positionY);
            }
        }
        
        while (bot+1 <= 7 && board[bot+1][positionX] == 0) {
            ++bot;
            
        }
        if (bot != 7 && board[bot+1][positionX] != color) {
            ++bot;
            if (board[bot][positionX] != color * -1 && board[bot][positionX] < 0) {
                rook->checkerino(color, positionX, positionY);
            }
        }
        

        //cout << "top: " << top << endl;
        //cout << "bot: " << bot << endl;
        for (int i = top; i <= bot; i++) {
            if (i == positionY) {
                continue;
            }
            rook->pushAvailSpaces(vector<int> {i, positionX});
        }
    }
    ~rookMovement() {

    }
};

class knightMovement : public movementStrat {
public:
    virtual void move(chessPieces* knight) {
        knight->setBlockable();
        bool fMove = knight->getFirstMove();
        int positionX = knight->getPositionX();
        int positionY = knight->getPositionY();

        int pointX = positionX - 2;
        int pointY = positionY - 1;

        int color = knight->getColor();
        for (int i = 0; i < 2; i++) {//top left section
            if (pointX >= 0 && pointY >= 0) {
                knight->pushAvailSpaces(vector<int> {pointY, pointX});
                if (board[pointY][pointX] != color * -1 && board[pointY][pointX] < 0) {
                    knight->checkerino(color, positionX, positionY);
                    blockableP = false;
                }
            }
            pointX += 1;
            pointY -= 1;
        }

        pointX = positionX + 1;
        pointY = positionY - 2;
        for (int i = 0; i < 2; i++) {//top right section
            if (pointX <= 7  && pointY >= 0) {
                knight->pushAvailSpaces(vector<int> {pointY, pointX});
                if (board[pointY][pointX] != color * -1 && board[pointY][pointX] < 0) {
                    knight->checkerino(color, positionX, positionY);
                    blockableP = false;
                }
            }
            pointX += 1;
            pointY += 1;
        }

        pointX = positionX + 2;
        pointY = positionY + 1;
        for (int i = 0; i < 2; i++) {//bottom right section
            if (pointX <= 7 && pointY <= 7) {
                knight->pushAvailSpaces(vector<int> {pointY, pointX});
                if (board[pointY][pointX] != color * -1 && board[pointY][pointX] < 0) {
                    knight->checkerino(color, positionX, positionY);
                    blockableP = false;
                }
            }
            pointX -= 1;
            pointY += 1;
        }

        pointX = positionX - 1;
        pointY = positionY + 2;
        for (int i = 0; i < 2; i++) {//top right section
            if (pointX >= 0 && pointY <= 7) {
                knight->pushAvailSpaces(vector<int> {pointY, pointX});
                if (board[pointY][pointX] != color * -1 && board[pointY][pointX] < 0) {
                    knight->checkerino(color, positionX, positionY);
                    blockableP = false;
                }
            }
            pointX -= 1;
            pointY -= 1;
        }

    }
    ~knightMovement() {

    }
};

class bishopMovement : public movementStrat {
public:
    virtual void move(chessPieces* bishop) {
        bool fMove = bishop->getFirstMove();
        int positionX = bishop->getPositionX();
        int positionY = bishop->getPositionY();

        int color = bishop->getColor();

        int left = positionX;//Look for left values
        
        int top = positionY;
        
        while (left - 1 >= 0 && top - 1 >= 0 && board[top-1][left - 1] == 0) {//Top Left
            --left;
            --top;
            bishop->pushAvailSpaces(vector<int> {top, left});
        }
        if (left != 0 && top != 0 && board[top-1][left - 1] != color) {
            --left;
            --top;
            if (board[top][left] != color * -1 && board[top][left] < 0) {
                bishop->checkerino(color, positionX, positionY);
            }
            bishop->pushAvailSpaces(vector<int> {top, left});
        }

        left = positionX;
        int bot = positionY;
        while (left - 1 >= 0 && bot + 1 <= 7 && board[bot + 1][left - 1] == 0) {//Bottom Left
            --left;
            ++bot;
            bishop->pushAvailSpaces(vector<int> {bot, left});
        }
        if (left != 0 && bot != 7 && board[bot + 1][left - 1] != color) {
            --left;
            ++bot;
            if (board[bot][left] != color * -1 && board[bot][left] < 0) {
                bishop->checkerino(color, positionX, positionY);
            }
            bishop->pushAvailSpaces(vector<int> {bot, left});
        }

        int right = positionX;//Look for right values
        top = positionY;

        while (right + 1 <= 7 && top - 1 >= 0 && board[top - 1][right + 1] == 0) {//Top Right
            ++right;
            --top;
            bishop->pushAvailSpaces(vector<int> {top, right});
        }
        if (right != 7 && top != 0 && board[top - 1][right + 1] != color) {
            ++right;
            --top;
            if (board[top][right] != color * -1 && board[top][right] < 0) {
                bishop->checkerino(color, positionX, positionY);
            }
            bishop->pushAvailSpaces(vector<int> {top, right});
        }

        right = positionX;
        bot = positionY;

        while (right + 1 <= 7 && bot + 1 <= 7 && board[bot + 1][right + 1] == 0) {//Bot Right
            ++right;
            ++bot;
            bishop->pushAvailSpaces(vector<int> {bot, right});
        }
        if (right != 7 && bot != 7 && board[bot + 1][right + 1] != color) {
            ++right;
            ++bot;
            if (board[bot][right] != color * -1 && board[bot][right] < 0) {
                bishop->checkerino(color, positionX, positionY);
            }
            bishop->pushAvailSpaces(vector<int> {bot, right});
        }

    }
    ~bishopMovement() {

    }
};

class queenMovement : public movementStrat {
public:
    virtual void move(chessPieces* queen) {
        bool fMove = queen->getFirstMove();
        int positionX = queen->getPositionX();
        int positionY = queen->getPositionY();

        int color = queen->getColor();

        int left = positionX;//Look for left values

        int top = positionY;

        while (left - 1 >= 0 && top - 1 >= 0 && board[top - 1][left - 1] == 0) {//Top Left
            --left;
            --top;
            queen->pushAvailSpaces(vector<int> {top, left});
        }
        if (left != 0 && top != 0 && board[top - 1][left - 1] != color) {
            --left;
            --top;
            if (board[top][left] != color * -1 && board[top][left] < 0) {
                queen->checkerino(color, positionX, positionY);
            }
            queen->pushAvailSpaces(vector<int> {top, left});
        }

        left = positionX;
        int bot = positionY;
        while (left - 1 >= 0 && bot + 1 <= 7 && board[bot + 1][left - 1] == 0) {//Bottom Left
            --left;
            ++bot;
            queen->pushAvailSpaces(vector<int> {bot, left});
        }
        if (left != 0 && bot != 7 && board[bot + 1][left - 1] != color) {
            --left;
            ++bot;
            if (board[bot][left] != color * -1 && board[bot][left] < 0) {
                queen->checkerino(color, positionX, positionY);
            }
            queen->pushAvailSpaces(vector<int> {bot, left});
        }

        int right = positionX;//Look for right values
        top = positionY;

        while (right + 1 <= 7 && top - 1 >= 0 && board[top - 1][right + 1] == 0) {//Top Right
            ++right;
            --top;
            queen->pushAvailSpaces(vector<int> {top, right});
        }
        if (right != 7 && top != 0 && board[top - 1][right + 1] != color) {
            ++right;
            --top;
            if (board[top][right] != color * -1 && board[top][right] < 0) {
                queen->checkerino(color, positionX, positionY);
            }
            queen->pushAvailSpaces(vector<int> {top, right});
        }

        right = positionX;
        bot = positionY;
        

        while (right + 1 <= 7 && bot + 1 <= 7 && board[bot + 1][right + 1] == 0) {//Bottom Right
            ++right;
            ++bot;
            queen->pushAvailSpaces(vector<int> {bot, right});
        }
        if (right != 7 && bot != 7 && board[bot + 1][right + 1] != color) {
            ++right;
            ++bot;
            if (board[bot][right] != color * -1 && board[bot][right] < 0) {
                queen->checkerino(color, positionX, positionY);
            }
            queen->pushAvailSpaces(vector<int> {bot, right});
        }
        //=========================================================================================================================
        left = positionX;//Look for left values
        right = positionX;//Look for right values

        while (left - 1 >= 0 && board[positionY][left - 1] == 0) { //Left
            --left;
        }
        if (left != 0 && board[positionY][left - 1] != color) {
            --left;
            if (board[positionY][left] != color * -1 && board[positionY][left] < 0) {
                queen->checkerino(color, positionX, positionY);
            }
        }
        while (right + 1 <= 7 && board[positionY][right + 1] == 0) { //Right
            ++right;
        }
        if (right != 7 && board[positionY][right + 1] != color) {
            ++right;
            if (board[positionY][right] != color * -1 && board[positionY][right] < 0) {
                queen->checkerino(color, positionX, positionY);
            }
        }
        //cout << "left: " << left << endl;
        //cout << "right: " << right << endl;

        for (int i = left; i <= right; i++) {
            if (i == positionX) {
                continue;
            }
            queen->pushAvailSpaces(vector<int> {positionY, i});
        }

        top = positionY;
        bot = positionY;
        while (top - 1 >= 0 && board[top - 1][positionX] == 0) { //Top
            --top;
        }
        if (top != 0 && board[top - 1][positionX] != color) {
            --top;
            if (board[top][positionX] != color * -1 && board[top][positionX] < 0) {
                queen->checkerino(color, positionX, positionY);
            }
        }
        while (bot + 1 <= 7 && board[bot + 1][positionX] == 0) { //Bot
            ++bot;
        }
        if (bot != 7 && board[bot + 1][positionX] != color) {
            ++bot;
            if (board[bot][positionX] != color * -1 && board[bot][positionX] < 0) {
                queen->checkerino(color, positionX, positionY);
            }
        }
        
        for (int i = top; i <= bot; i++) {
            if (i == positionY) {
                continue;
            }
            queen->pushAvailSpaces(vector<int> {i, positionX});
        }
    }
    ~queenMovement() {

    }
};

class kingMovement : public movementStrat {
public:
    virtual void move(chessPieces* king) {
        king->setBlockable();
        bool fMove = king->getFirstMove();
        int positionX = king->getPositionX();
        int positionY = king->getPositionY();

        int color = king->getColor();


        int top = positionY; 
        int point = positionX-1;
        for (int i = 0; i < 3; i++) { //Top 3
            if (top - 1 == -1) {
                break;
            }
            if (point >= 0 && point <= 7 && board[top - 1][point] != king->getColor()) {
                king->pushAvailSpaces(vector<int> {top - 1, point});
            }
            ++point;
        }

        int bot = positionY; 
        point = positionX-1;
        for (int i = 0; i < 3; i++) { //Bottom 3
            if (bot + 1 == 8) {
                break;
            }
            if (point >= 0 && point <= 7 && board[bot + 1][point] != king->getColor()) {
                king->pushAvailSpaces(vector<int> {bot + 1, point});
            }
            ++point;
        }

        //TO DO: Fix the castle system, we can castle when there's still pieces inbetween the rook and the king!!
        if (positionX - 1 >= 0 && board[positionY][positionX - 1] != king->getColor()) { //Left
            king->pushAvailSpaces(vector<int> {positionY, positionX - 1});
            chessPieces rook1 = findPiece(positionX - 4, positionY);
            if (!rook1.getFirstMove() && !king->getFirstMove() && positionX - 3 >= 0 && board[positionY][positionX - 3] != king->getColor() && rook1.getRook()) {//We can Castle
                cout << "we can castle left" << endl;
                king->pushSpecialSpaces(vector<int> {positionY, positionX - 3});//To do: fix this
            }
            
        }
        if (positionX + 1 <= 7 && board[positionY][positionX + 1] != king->getColor()) { //Right
            king->pushAvailSpaces(vector<int> {positionY, positionX + 1});
            chessPieces rook1 = findPiece(positionX + 3, positionY);
            if (!rook1.getFirstMove() && !king->getFirstMove() && positionX + 2 <= 7 && board[positionY][positionX + 2] != king->getColor()  && rook1.getRook()) {//We can Castle
                cout << "we can castle right" << endl;
                king->pushSpecialSpaces(vector<int> {positionY, positionX + 2});//To do: fix this
            }
        }

    }
    ~kingMovement() {

    }
};



chessPieces bSpritePawn1(0, 1, 2);
sf::Texture bTextPawn1;

chessPieces bSpritePawn2(1, 1, 2);
sf::Texture bTextPawn2;

chessPieces bSpritePawn3(2, 1, 2);
sf::Texture bTextPawn3;

chessPieces bSpritePawn4(3, 1, 2);
sf::Texture bTextPawn4;

chessPieces bSpritePawn5(4, 1, 2);
sf::Texture bTextPawn5;

chessPieces bSpritePawn6(5, 1, 2);
sf::Texture bTextPawn6;

chessPieces bSpritePawn7(6, 1, 2);
sf::Texture bTextPawn7;

chessPieces bSpritePawn8(7, 1, 2);
sf::Texture bTextPawn8;

chessPieces bSpriteRook1(0, 0, 2);
sf::Texture bTextRook1;

chessPieces bSpriteRook2(7, 0, 2);
sf::Texture bTextRook2;

chessPieces bSpriteKnight1(1, 0, 2);
sf::Texture bTextKnight1;

chessPieces bSpriteKnight2(6, 0, 2);
sf::Texture bTextKnight2;

chessPieces bSpriteBishop1(2, 0, 2);
sf::Texture bTextBishop1;

chessPieces bSpriteBishop2(5, 0, 2);
sf::Texture bTextBishop2;

chessPieces bSpriteQueen(3, 0, 2);
sf::Texture bTextQueen;

chessPieces bSpriteKing(4, 0, -2);
sf::Texture bTextKing;

chessPieces wSpritePawn1(0 , 6, 1);
sf::Texture wTextPawn1;

chessPieces wSpritePawn2(1, 6, 1);
sf::Texture wTextPawn2;

chessPieces wSpritePawn3(2, 6, 1);
sf::Texture wTextPawn3;

chessPieces wSpritePawn4(3, 6, 1);
sf::Texture wTextPawn4;

chessPieces wSpritePawn5(4, 6, 1);
sf::Texture wTextPawn5;

chessPieces wSpritePawn6(5, 6, 1);
sf::Texture wTextPawn6;

chessPieces wSpritePawn7(6, 6, 1);
sf::Texture wTextPawn7;

chessPieces wSpritePawn8(7, 6, 1);
sf::Texture wTextPawn8;

chessPieces wSpriteRook1(0, 7, 1);
sf::Texture wTextRook1;

chessPieces wSpriteRook2(7, 7, 1);
sf::Texture wTextRook2;

chessPieces wSpriteKnight1(1, 7, 1);
sf::Texture wTextKnight1;

chessPieces wSpriteKnight2(6, 7, 1);
sf::Texture wTextKnight2;

chessPieces wSpriteBishop1(2, 7, 1);
sf::Texture wTextBishop1;

chessPieces wSpriteBishop2(5, 7, 1);
sf::Texture wTextBishop2;

chessPieces wSpriteQueen(3, 7, 1);
sf::Texture wTextQueen;

chessPieces wSpriteKing(4, 7, -1);
sf::Texture wTextKing;

//chessPieces greySpriteDot(4, 3, 0);
//sf::Texture greyTextDot;


pawnWhiteMovement* whiteP = new pawnWhiteMovement;
pawnBlackMovement* blackP = new pawnBlackMovement;
rookMovement* rook = new rookMovement;
knightMovement* knight = new knightMovement;
bishopMovement* bishop = new bishopMovement;
queenMovement* queen = new queenMovement;
kingMovement* king = new kingMovement;

int findPositionX(int pX) {
    int num = 0;
    int num2 = 0;

    for (int i = 0; i < 7; i++) {
        num2 = num + 75;
        if (num <= pX && pX <= num2) {
            return num;
        }
        num += 75;
    }
    return 525;
}

int findPositionY(int pY) {
    int num = 0;
    int num2 = 0;

    for (int i = 0; i < 7; i++) {
        num2 = num + 75;
        if (num <= pY && pY <= num2) {
            return num;
        }
        num += 75;
    }
    return 525;
}

void buildBoard(sf::Texture& textureBoard, sf::Texture& texturePiece, vector<sf::Sprite>& bPieces, vector<sf::Sprite>& wPieces, sf::Sprite& spriteBoard, sf::Sprite& spritePiece) {
    
    textureBoard.loadFromFile("ChessBoard.png");
    spriteBoard.setTexture(textureBoard);

    int y1 = 0;
    int y2 = 75;

    int y3 = 450;
    int y4 = 525;

    /*
    greyTextDot.loadFromFile("pi/greyDot1.png");
    greySpriteDot.setTexture(greyTextDot);
    greySpriteDot.setPosition(3, 4);
    holder.push_back(greySpriteDot);
    */

    bTextPawn1.loadFromFile("pi/bPawn.png");
    bSpritePawn1.setTexture(bTextPawn1);
    bSpritePawn1.setPosition(0, y2);
    bSpritePawn1.setMoveStrat(blackP);
    holder.push_back(bSpritePawn1);

    
    bTextPawn2.loadFromFile("pi/bPawn.png");
    bSpritePawn2.setTexture(bTextPawn2);
    bSpritePawn2.setPosition(75, y2);
    bSpritePawn2.setMoveStrat(blackP);
    holder.push_back(bSpritePawn2);

    bTextPawn3.loadFromFile("pi/bPawn.png");
    bSpritePawn3.setTexture(bTextPawn3);
    bSpritePawn3.setPosition(150, y2);
    bSpritePawn3.setMoveStrat(blackP);
    holder.push_back(bSpritePawn3);


    bTextPawn4.loadFromFile("pi/bPawn.png");
    bSpritePawn4.setTexture(bTextPawn4);
    bSpritePawn4.setPosition(225, y2);
    bSpritePawn4.setMoveStrat(blackP);
    holder.push_back(bSpritePawn4);

    bTextPawn5.loadFromFile("pi/bPawn.png");
    bSpritePawn5.setTexture(bTextPawn5);
    bSpritePawn5.setPosition(300, y2);
    bSpritePawn5.setMoveStrat(blackP);
    holder.push_back(bSpritePawn5);


    bTextPawn6.loadFromFile("pi/bPawn.png");
    bSpritePawn6.setTexture(bTextPawn6);
    bSpritePawn6.setPosition(375, y2);
    bSpritePawn6.setMoveStrat(blackP);
    holder.push_back(bSpritePawn6);

    bTextPawn7.loadFromFile("pi/bPawn.png");
    bSpritePawn7.setTexture(bTextPawn7);
    bSpritePawn7.setPosition(450, y2);
    bSpritePawn7.setMoveStrat(blackP);
    holder.push_back(bSpritePawn7);


    bTextPawn8.loadFromFile("pi/bPawn.png");
    bSpritePawn8.setTexture(bTextPawn8);
    bSpritePawn8.setPosition(525, y2);
    bSpritePawn8.setMoveStrat(blackP);
    holder.push_back(bSpritePawn8);

    bTextRook1.loadFromFile("pi/bRook.png");
    bSpriteRook1.setTexture(bTextRook1);
    bSpriteRook1.setPosition(0, y1);
    bSpriteRook1.setMoveStrat(rook);
    holder.push_back(bSpriteRook1);

    bTextRook2.loadFromFile("pi/bRook.png");
    bSpriteRook2.setTexture(bTextRook2);
    bSpriteRook2.setPosition(525, y1);
    bSpriteRook2.setMoveStrat(rook);
    holder.push_back(bSpriteRook2);

    bTextKnight1.loadFromFile("pi/bKnight.png");
    bSpriteKnight1.setTexture(bTextKnight1);
    bSpriteKnight1.setPosition(75, y1);
    bSpriteKnight1.setMoveStrat(knight);
    holder.push_back(bSpriteKnight1);

    bTextKnight2.loadFromFile("pi/bKnight.png");
    bSpriteKnight2.setTexture(bTextKnight2);
    bSpriteKnight2.setPosition(450, y1);
    bSpriteKnight2.setMoveStrat(knight);
    holder.push_back(bSpriteKnight2);

    bTextBishop1.loadFromFile("pi/bBishop.png");
    bSpriteBishop1.setTexture(bTextBishop1);
    bSpriteBishop1.setPosition(150, y1);
    bSpriteBishop1.setMoveStrat(bishop);
    holder.push_back(bSpriteBishop1);

    bTextBishop2.loadFromFile("pi/bBishop.png");
    bSpriteBishop2.setTexture(bTextBishop2);
    bSpriteBishop2.setPosition(375, y1);
    bSpriteBishop2.setMoveStrat(bishop);
    holder.push_back(bSpriteBishop2);

    bTextQueen.loadFromFile("pi/bQueen.png");
    bSpriteQueen.setTexture(bTextQueen);
    bSpriteQueen.setPosition(225, y1);
    bSpriteQueen.setMoveStrat(queen);
    holder.push_back(bSpriteQueen);

    bTextKing.loadFromFile("pi/bKing.png");
    bSpriteKing.setTexture(bTextKing);
    bSpriteKing.setPosition(300, y1);
    bSpriteKing.setMoveStrat(king);
    holder.push_back(bSpriteKing);




    //--------------------------------------------


    wTextPawn1.loadFromFile("pi/wPawn.png");
    wSpritePawn1.setTexture(wTextPawn1);
    wSpritePawn1.setPosition(0, y3);
    wSpritePawn1.setMoveStrat(whiteP);
    holder.push_back(wSpritePawn1);


    wTextPawn2.loadFromFile("pi/wPawn.png");
    wSpritePawn2.setTexture(wTextPawn2);
    wSpritePawn2.setPosition(75, y3);
    wSpritePawn2.setMoveStrat(whiteP);
    holder.push_back(wSpritePawn2);

    wTextPawn3.loadFromFile("pi/wPawn.png");
    wSpritePawn3.setTexture(wTextPawn3);
    wSpritePawn3.setPosition(150, y3);
    wSpritePawn3.setMoveStrat(whiteP);
    holder.push_back(wSpritePawn3);


    wTextPawn4.loadFromFile("pi/wPawn.png");
    wSpritePawn4.setTexture(wTextPawn4);
    wSpritePawn4.setPosition(225, y3);
    wSpritePawn4.setMoveStrat(whiteP);
    holder.push_back(wSpritePawn4);

    wTextPawn5.loadFromFile("pi/wPawn.png");
    wSpritePawn5.setTexture(wTextPawn5);
    wSpritePawn5.setPosition(300, y3);
    wSpritePawn5.setMoveStrat(whiteP);
    holder.push_back(wSpritePawn5);


    wTextPawn6.loadFromFile("pi/wPawn.png");
    wSpritePawn6.setTexture(wTextPawn6);
    wSpritePawn6.setPosition(375, y3);
    wSpritePawn6.setMoveStrat(whiteP);
    holder.push_back(wSpritePawn6);

    wTextPawn7.loadFromFile("pi/wPawn.png");
    wSpritePawn7.setTexture(wTextPawn7);
    wSpritePawn7.setPosition(450, y3);
    wSpritePawn7.setMoveStrat(whiteP);
    holder.push_back(wSpritePawn7);

    wTextPawn8.loadFromFile("pi/wPawn.png");
    wSpritePawn8.setTexture(wTextPawn8);
    wSpritePawn8.setPosition(525, y3);
    wSpritePawn8.setMoveStrat(whiteP);
    holder.push_back(wSpritePawn8);

    wTextRook1.loadFromFile("pi/wRook.png");
    wSpriteRook1.setTexture(wTextRook1);
    wSpriteRook1.setPosition(0, y4);
    wSpriteRook1.setMoveStrat(rook);
    holder.push_back(wSpriteRook1);

    wTextRook2.loadFromFile("pi/wRook.png");
    wSpriteRook2.setTexture(wTextRook2);
    wSpriteRook2.setPosition(525, y4);
    wSpriteRook2.setMoveStrat(rook);
    holder.push_back(wSpriteRook2);

    wTextKnight1.loadFromFile("pi/wKnight.png");
    wSpriteKnight1.setTexture(wTextKnight1);
    wSpriteKnight1.setPosition(75, y4);
    wSpriteKnight1.setMoveStrat(knight);
    holder.push_back(wSpriteKnight1);

    wTextKnight2.loadFromFile("pi/wKnight.png");
    wSpriteKnight2.setTexture(wTextKnight2);
    wSpriteKnight2.setPosition(450, y4);
    wSpriteKnight2.setMoveStrat(knight);
    holder.push_back(wSpriteKnight2);

    wTextBishop1.loadFromFile("pi/wBishop.png");
    wSpriteBishop1.setTexture(wTextBishop1);
    wSpriteBishop1.setPosition(150, y4);
    wSpriteBishop1.setMoveStrat(bishop);
    holder.push_back(wSpriteBishop1);

    wTextBishop2.loadFromFile("pi/wBishop.png");
    wSpriteBishop2.setTexture(wTextBishop2);
    wSpriteBishop2.setPosition(375, y4);
    wSpriteBishop2.setMoveStrat(bishop);
    holder.push_back(wSpriteBishop2);

    wTextQueen.loadFromFile("pi/wQueen.png");
    wSpriteQueen.setTexture(wTextQueen);
    wSpriteQueen.setPosition(225, y4);
    wSpriteQueen.setMoveStrat(queen);
    holder.push_back(wSpriteQueen);

    wTextKing.loadFromFile("pi/wKing.png");
    wSpriteKing.setTexture(wTextKing);
    wSpriteKing.setPosition(300, y4);
    wSpriteKing.setMoveStrat(king);
    holder.push_back(wSpriteKing);
    }

    bool checkIfSameColor(chessPieces temp1, chessPieces temp2) {
        if (temp1.getColor() == temp2.getColor() || temp1.getColor() == temp2.getColor() * -1) {
            cout << "Same Color!!!" << endl;
            return true;
        }
        return false;
    }

    int checkCollision(int x, int y, int z) {
        
        for (int i = 0; i < holder.size(); i++) {
            if (i == z || !holder[i].checkAlive()) {
                continue;
            }
            if (holder[i].getPositionX() == x && holder[i].getPositionY() == y) { 
                return i;
            }
        }
        return -1;
    }

    vector<vector<int>> path;

    void findPathKing(int kingPosX, int kingPosY) {
        int pointX = checkX;
        int pointY = checkY;

        int point = pointY;

        bool blockable = blockableP;

        path.clear();
        if (!blockable) {
            cout << "not blockable" << endl;
        }
        while (blockable && pointX == kingPosX && point != kingPosY) {//If they are on the same column
            if (kingPosY < pointY) {//White King is above
                --point;
            }
            else {//White King is below
                ++point;
            }
            path.push_back(vector<int> {point, pointX});
        }

        point = pointX;
        while (blockable && pointY == kingPosY && point != kingPosX) {//If they are on the same row
            
            if (kingPosX < pointX) {//White King is left
                --point;
            }
            else {//White King is right
                ++point;
            }
            path.push_back(vector<int> {pointY, point});
        }

        int x = pointX;
        int y = pointY;
        while (blockable && x < kingPosX && y < kingPosY) {//Top Left
            ++x;
            ++y;
            path.push_back(vector<int> {y, x});
        }

        x = pointX;
        y = pointY;
        while (blockable && x > kingPosX && y < kingPosY) {//Top Right
            --x;
            ++y;
            path.push_back(vector<int> {y, x});
        }

        x = pointX;
        y = pointY;
        while (blockable && x > kingPosX && y > kingPosY) {//Bot Right
            --x;
            --y;
            path.push_back(vector<int> {y, x});
        }

        x = pointX;
        y = pointY;
        while (blockable && x < kingPosX && y < kingPosY) {//Bot Right
            ++x;
            --y;
            path.push_back(vector<int> {y, x});
        }
        cout << "Path Coord: << ";
        for (int i = 0; i < path.size(); i++) {
            cout << "(" << path[i][1] << " , " << path[i][0] << ")" << " >>";
        }
        cout << endl;
    }

    vector<vector<int>> getCollectionSpaces(int color) {
        vector<vector<int>> temp;
        vector<vector<int>> p;
        vector<vector<int>> q;
        for (int i = 0; i < holder.size(); i++) {
            if (holder[i].getColor() == color || holder[i].getColor() < 0) {//Dont check the spaces of your own color
                continue;
            }

            
            if (holder[i].getPawn()) {
                q = holder[i].getSpecialSpaces();//Special spaces are the spaces that are diagonal to pawns (so we don't get their avail spaces)
                for (int j = 0; j < q.size(); j++) {
                    temp.push_back(q[j]);
                }
            }
            else {
                p = holder[i].getAvailSpaces();//For every other piece, get the collection of the spaces you are looking at
                for (int j = 0; j < p.size(); j++) {
                    temp.push_back(p[j]);
                }
            }
            
        }
        return temp;
    }

    bool checkValidMove(vector<vector<int>> spaces, int posX, int posY, chessPieces temp) {
        int tempPosX = temp.getPositionX();
        int tempPosY = temp.getPositionY();
        int color = temp.getColor();
        int save = 0;

        bool legalSpot = false;

        for (int i = 0; i < spaces.size(); i++) {
            //cout << "checking X: " << posX << " , and y: " << posY << ", with (" << spaces[i][1] << ", " << spaces[i][0] << ")" << endl;
            if (spaces[i][0] == posY && spaces[i][1] == posX) {
                legalSpot = true;
            }
        }

        vector<vector<int>> temp2 = temp.getSpecialSpaces(); //Check if its a castle
        if (color < 0 && temp2.size() != 0) {
            for (int i = 0; i < temp2.size(); i++) {
                if (temp2[i][0] == posY && temp2[i][1] == posX) {
                    //Manually move the rook
                    if (posX > 4) {
                        for (int j = 0; j < holder.size(); j++) {
                            if (holder[j].getPositionX() == 7 && holder[j].getPositionY() == tempPosY) {
                                holder[j].setNewPositionCastle(posX - 1, posY, 7, posY);
                                holder[j].setPosition((posX-1)* 75, posY * 75);
                                holder[j].firstMoveMade();
                            }
                        }
                    }
                    else {
                        for (int j = 0; j < holder.size(); j++) {
                            if (holder[j].getPositionX() == 0 && holder[j].getPositionY() == tempPosY) {
                                holder[j].setNewPositionCastle(posX + 2, posY, 0, posY);
                                holder[j].setPosition((posX + 2) * 75, posY * 75);
                                holder[j].firstMoveMade();
                            }
                        }
                    }
                    return true;
                }
            }
        }
        else if (check) {

            if (colorP == 1) {//Get the path towards the king to check if its blockable
                findPathKing(bKingPosX, bKingPosY);
            }
            else {
                findPathKing(wKingPosX, wKingPosY);
            }
            bool onPath = false;

            for (int j = 0; j < path.size(); j++) {
                if (posY == path[j][0] && posX == path[j][1]) {
                    onPath = true;
                }
            }

            //cout << "posX: " << posX << " , posY: " << posY << " , checkX: " << checkX << " , checkY: " << checkY << endl;
            if (color < 0 && color != colorP * -1) {//We are holding the king opposite color of the piece that is checking it
                if (onPath) { //ALSO CHECK if once off path, if it is still in check
                    cout << "on path" << endl;
                    return false;
                }

                if (legalSpot){
                    //If we are hovering over the checking target, we have to check if we can even move to that location
                    //cout << spaces[i][0] << " , " << spaces[i][1] << " , " << path[j][0] << " , " << path[j][1] << "." << endl;
                    //Reminder: Just go through the path's vector and check if its in it, if it is in it, then its invalid
                    cout << color * -1 << " dfadskfadsk" << endl;
                    vector<vector<int>> p = getCollectionSpaces(color * -1); //Check this function. TO DO. I think the color is wrong
                    for (int i = 0; i < p.size(); i++) {
                        cout << "checking X: " << posX << " , and y: " << posY << ", with (" << p[i][1] << ", " << p[i][0] << ")" << endl; 
                        if (p[i][0] == posY && p[i][1] == posX) {
                            cout << "You'll end up getting checked again" << endl;
                            return false;
                        }
                    }
                            
                         check = 0; //We've moved out of the way
                         cout << "===================================== NO CHECK =====================================12345" << endl;
                         checkX = 10;
                         checkY = 10;
                         return true;    
                }
            }

            

            for (int i = 0; i < path.size(); i++) {
                cout << "checking: (" << posX << " with " << path[i][1] << ") and (" << posY << " with " << path[i][0] << ")" << endl;
                if (posX == path[i][1] && posY == path[i][0] && legalSpot) {//If the spot we are going to is in between the checking target and the king, then we say it is valid spot
                    cout << "blockable!" << endl;
                    check = 0;
                    cout << "===================================== NO CHECK =====================================" << endl;
                    checkX = 10;
                    checkY = 10;
                    path.clear();
                    cout << "path cleared" << endl;
                    return true;
                }
            }
            if (posX != checkX || posY != checkY || check > 1) {//If we are in check but we are not "killing" the checking target, return false
                return false;
            }
            for (int i = 0; i < spaces.size(); i++) {//If we are hovering over the checking target, we have to check if we can even move to that location
                if (spaces[i][0] == posY && spaces[i][1] == posX) {
                    //cout << "HAS TO BE X: " << posX << " , and y: " << posY << ", with (" << spaces[i][1] << ", " << spaces[i][0] << ")" << endl;
                    check = 0; //We've killed the checking piece
                    cout << "===================================== NO CHECK =====================================" << endl;
                    checkX = 10;
                    checkY = 10;
                    path.clear();
                    return true;
                }
            }
            path.clear();
            return false;
        }
        else if(legalSpot) {
            board[tempPosY][tempPosX] = 0;
            save = board[posY][posX]; //Saves the old spot, turns the "predicted" spot into the new color
            board[posY][posX] = color;
            for (int i = 0; i < holder.size(); i++) { //Pinning. Will return false if the piece we are moving causes a check
                if (holder[i].getPositionX() == tempPosX && holder[i].getPositionY() == tempPosY) {
                    continue;
                }
                holder[i].movement();
                
                if (check && color == checkColor) {
                    board[tempPosY][tempPosX] = color;
                    check = 0;
                    cout << "Pinned!" << endl;
                    return false;
                }
            }
            board[tempPosY][tempPosX] = color;//Return everything to the way it was
            board[posY][posX] = save;

            return legalSpot;
            
        }
        return false;
    }

int main()
{
    
    sf::RenderWindow window(sf::VideoMode(600, 600), "Chess");

    window.setFramerateLimit(60);

    window.setKeyRepeatEnabled(false);

    //Booleans//
    bool play = true;
    bool mouseClicked = false;
    bool mouseInsideReact = false;
    bool dragging = false;

    //Locations//
    int mouseX = 0;
    int mouseY = 0;
    int posX = 0;
    int posY = 0;
    int turn = 3;

    sf::Vector2f mouse;
    
    sf::Texture textureBoard;
    sf::Texture texturePiece;
    sf::Sprite spriteBoard;
    sf::Sprite spritePiece;

    vector<sf::Sprite> bPieces;
    vector<sf::Sprite> wPieces;

    int prevX = 0;
    int prevY = 0;

    buildBoard(textureBoard, texturePiece, bPieces, wPieces, spriteBoard, spritePiece);

    int z = -1;

    while (play == true)
    {
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            //cout << "holder size: " << holder.size() << endl;
            if (event.type == sf::Event::Closed)
                window.close();

            
            for (int i = 0; i < holder.size(); i++) {
                if (!holder[i].checkAlive() || holder[i].getColor() == (turn % 2) + 1 || holder[i].getColor() == ((turn % 2) + 1) * -1) {
                    continue;
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    mouseClicked = true;

                    if (holder[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        cout << "Found at :" << event.mouseButton.x << endl;
                        holder[i].movement();
                        vectX = holder[i].getPositionX();
                        vectY = holder[i].getPositionY();
                        prevX = event.mouseButton.x;
                        prevY = event.mouseButton.y;
                        z = i;
                        dragging = true;
                        mouse.x = event.mouseButton.x - holder[i].getGlobalBounds().left - holder[i].getOrigin().x;
                        mouse.y = event.mouseButton.y - holder[i].getGlobalBounds().top - holder[i].getOrigin().y;
                        break;
                    }
                    else {
                        z = -1;
                    }
                }
            }


            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && z != -1) {
                //z = piece we are holding onto
                //b = potential piece that we are "colliding" into
                    mouseClicked = false;
                    dragging = false;
                    posX = holder[z].getLocalBounds().left + mouseX;
                    posY = holder[z].getLocalBounds().top + mouseY;
                    vector<vector<int>> spaces = holder[z].getAvailSpaces();
                    //cout << "posX: " << posX << endl;
                    //cout << "posY: " << posY << endl;
                    int b = checkCollision(posX / 75, posY / 75, z);

                    
                    if (!checkValidMove(spaces, posX/75 , posY/75, holder[z])) {//The place we are going to is not valid
                        cout << "Invalid move, try again" << endl;
                        //cout << "prevX: " << prevX << ", prevY: " << prevY << ", posX: " << posX << ", posY: " << posY << endl;
                        holder[z].setPosition(findPositionX(prevX), findPositionY(prevY));
                    }
                    else if (b != -1) {
                        if (checkIfSameColor(holder[z], holder[b])) {
                            holder[z].setPosition(findPositionX(prevX), findPositionY(prevY));
                        }
                        else{
                            holder[b].killPiece();
                            holder[z].setNewPosition(posX / 75, posY / 75);
                            holder[z].setPosition(findPositionX(posX), findPositionY(posY));
                            ++turn;
                            holder[z].clearAvailSpaces();
                            holder[z].firstMoveMade();
                        }
                    }
                    else {
                        holder[z].setNewPosition(posX / 75, posY / 75);
                        if (holder[z].getColor() < 0) { //Check if we're holding a king
                            if (holder[z].getColor() == -1) { //Update White King's Coordiantes
                                wKingPosX = posX / 75;
                                wKingPosY = posY / 75;
                            }
                            else { //Update Black King's Coordinates
                                bKingPosX = posX / 75;
                                bKingPosY = posY / 75;
                            }
                        }
                        holder[z].setPosition(findPositionX(posX), findPositionY(posY));
                        //cout << "prevX: " << prevX << ", prevY: " << prevY << ", posX: " << posX << ", posY: " << posY << endl;
                        if (prevX != posX / 75 && prevY != posY / 75) { //If you just place the piece in the same position, dont skip your turn
                            ++turn;
                            
                            holder[z].firstMoveMade();
                        }
                    }
                    holder[z].clearAvailSpaces();
                    holder[z].movement();
                    if (check == true) {
                        cout << "===================================== CHECK =====================================" << endl;
                    }
                    holder[z].clearAvailSpaces();
            }

            if (event.type == sf::Event::MouseMoved) {
                    mouseX = event.mouseMove.x;
                    mouseY = event.mouseMove.y;
            }
        }

        if (dragging == true && z != -1) {
            //cout << "z: " << z << endl;
            holder[z].setPosition(mouseX- holder[z].getLocalBounds().width/2, mouseY- holder[z].getLocalBounds().height/2);
        }

        window.clear();
        window.draw(spriteBoard);
        for (int j = 0; j < holder.size(); j++) {
            if (!holder[j].checkAlive()) {
                continue;
            }
            window.draw(holder[j]);
        }
        window.display();

        if (event.type == sf::Event::Closed) {
            play = false;
        }
    }
    window.close();

    delete whiteP;
    delete blackP;
    delete rook;
    delete knight;
    delete bishop;
    delete queen;
    delete king;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}