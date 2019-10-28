#include <stdio.h>

#define BOARDSIZE 3

class TicTacToe
{
public:
    enum class Square : char
    {
        EMPTY = '-',
        HUMAN = 'X',
        CPU = '0'
    };
    
    TicTacToe();
    ~TicTacToe();
    
    const void printBoard();
    void resetGame();
    
    bool getMove();
    bool isTie();
    bool isWinner( Square );
    bool isValidHumanMove( int , int );
    
    void miniMax();
    int maximizeHuman();
    int minimizeCPU();
    
    bool Play();
    
private:
    Square Board[BOARDSIZE][BOARDSIZE];
    int* move;
    int movesRemaining;
    bool isHuman;
    bool playAgain;
};
