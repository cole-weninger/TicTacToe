#include <iostream>
#include <limits.h>
#include "Tictactoe.h"


using std::cin;
using std::cout;

TicTacToe::TicTacToe()
{
    resetGame();
}

TicTacToe::~TicTacToe()
{

}

const void TicTacToe::printBoard()
{
    for( int y = 0; y < BOARDSIZE; y++ ){
        for( int x = 0; x < BOARDSIZE; x++ )
            cout << (char)Board[x][y] << " ";
        cout << "\n";
    }
    return;
}

void TicTacToe::resetGame()
{
    for( int y = 0; y < BOARDSIZE; y++ )
        for( int x = 0; x < BOARDSIZE; x++ )
            Board[x][y] = Square::EMPTY;
    movesRemaining = 9;
    isHuman = true;
    playAgain = false;
    move = nullptr;
}

bool TicTacToe::getMove()
{
    move = new int[2];
    move[0] = move[1] = -1;
    Square player;
    if( isHuman )
    {
        int x, y;
        player = Square::HUMAN;
        do
        {
            cin >> x;
            cin >> y;
        }while( ! isValidHumanMove(y,x) );
        move[0] = y-1;
        move[1] = x-1;
        Board[move[0]][move[1]] = player;
        isHuman = false;			//next turn is CPU's
    }
    else
    {
        player = Square::CPU;
        miniMax();
        isHuman = true;				//next turn is Human's
    }
    delete[] move;
    move = nullptr;
    movesRemaining--;
    return isWinner(player);
}


bool TicTacToe::isTie()
{
    return (movesRemaining <= 0);
}

bool TicTacToe::isWinner( Square player )
{
    for(int i=0; i < BOARDSIZE; i++){
        if( (player == Board[i][0] && player == Board[i][1] && player == Board[i][2] ) || // verticals
       
            (player == Board[0][i] && player == Board[1][i] && player == Board[2][i] ) || // horizontals
       
            (player == Board[0][0] && player == Board[1][1] && player == Board[2][2] ) || // diagonals
            (player == Board[0][2] && player == Board[1][1] && player == Board[2][0] ) )
        {
            return true;
        }
    }
    return false;
}

bool TicTacToe::isValidHumanMove( int x_move, int y_move )
{
    if( x_move > 3 || x_move < 1 || y_move > 3 || y_move < 1 ) {
		  cout << "Put in coordinates between 1 and 3\n";
        return false;
	 }
	 else if( Board[x_move-1][y_move-1] != Square::EMPTY ){
		  cout << "Selected space is occupied, select an empty space\n";
		  return false;
	 }
    return true;
}

/*
 MINIMAX LOGIC

 - end of game states are: Human wins   -> score = +10
                           Nobody wins  -> score = +0
                           CPU wins     -> score = -10
 - initially set the CPU to the highest possible score as the CPU is seeking minimum possible score
 - CPU assumes that human opponent will be playing "ideally" (the same method) and therefore forces each game to end with a tie in the worst case scenario
 - In Minimax() call:
    - loop through all empty board spaces, inserting CPU player into each EMPTY space and simulating rest of game
    - record result of the score that is generated
    - if a score is found that is less than the existing score (ie minimizes the result of the game state), that will become the new best acceptable move
    - return only A move that has a minimum score, to optimize could add alpha-beta pruning or another form of early breaking when seeking a minimized score
    - this will guarentee always at least a tie, but not neccesarily the fastest route to end-of-game possible (ie some moves are faster game-ending but just as effective)
 
 - In maximizeHuman() call:
    - the function does as the name implies, first search for a winner and if one is found (human or CPU) return appropriate end-of-game state
    - otherwise, continue the double-nested looping through all remaining EMPTY spaces until one is found the fill with Human player
    - simulate next move of game by calling another optimal "CPU" move given by minimizeCPU()
 
 - In minimizeCPU() call:
    - the function again does as the name implies, first seek a winner and return end-of-game state
    - if no winner, continue searching for available EMPTY spaces until the most optimal one is found
    - simulate next move of game by calling another optimal "human" move given maximizeHuman()
 
 - In minimax with alpha-beta call:
 
 */


void TicTacToe::miniMax()
{
    int CPUScore = std::numeric_limits<int>::max();
    for(int y = 0; y < BOARDSIZE; y++){
        for(int x = 0; x < BOARDSIZE; x++){
            if(Board[x][y] == Square::EMPTY){
                Board[x][y] = Square::CPU;
                movesRemaining--;
                int simScore = maximizeHuman();
                if(simScore < CPUScore){
                    CPUScore = simScore;
                    move[0] = x;
                    move[1] = y;
                }
                Board[x][y] = Square::EMPTY;
                movesRemaining++;
            }
        }
    }
    Board[move[0]][move[1]] = Square::CPU;
	
    return;
}

int TicTacToe::maximizeHuman()
{
    if( isWinner(Square::HUMAN) ) return 10;
    else if( isWinner(Square::CPU) ) return -10;
    else if( isTie() ) return 0;
    int score = std::numeric_limits<int>::min();
    
    for(int y = 0; y < BOARDSIZE; y++){
        for( int x = 0; x < BOARDSIZE; x++){
            if( Board[x][y] == Square::EMPTY){
            	 movesRemaining--;
            	 
                Board[x][y] = Square::HUMAN;
                score = std::max(score, minimizeCPU());
                Board[x][y] = Square::EMPTY;
					
                movesRemaining++;
            }
        }
    }
    
    return score;
}

int TicTacToe::minimizeCPU()
{
    if( isWinner(Square::HUMAN) ) return 10;
    else if( isWinner(Square::CPU) ) return -10;
    else if( isTie() ) return 0;
    int score = std::numeric_limits<int>::max();
    
    for(int y = 0; y < BOARDSIZE; y++){
        for( int x = 0; x < BOARDSIZE; x++){
            if( Board[x][y] == Square::EMPTY){
            	 movesRemaining--;
                Board[x][y] = Square::CPU;
                score = std::min( score, maximizeHuman() );
                Board[x][y] = Square::EMPTY;
                movesRemaining++;
            }
        }
    }
    
    return score;
}

bool TicTacToe::Play()
{
    resetGame();
    
    cout << "WELCOME TO TICTACTOE!\n\nPLEASE INPUT YOUR FIRST MOVE IN THE FORM \"row[1,2,3] column[1,2,3]\"\n";
	
	printBoard();
	
    while( !(getMove()) && movesRemaining >= 0 )
    {
		printBoard();
        cout << "Moves Remaining: " << movesRemaining << std::endl;
    }
	
    printBoard();
	if(isWinner(Square::HUMAN)){
		cout << "Winner is X's, congratulations!\n\n";
	}
	else if(isWinner(Square::CPU)){
		cout << "Winner is O's, tough loss!\n\n";
	}
	else if(isTie()){
		cout << "It's a draw!\n\n";
	}
    cout << "GAMEOVER\n\nPLAYAGAIN?(y/n)";
    char replay;
    do
    {
        cin >> replay;
    } while( replay != 'y' && replay != 'n' && replay != 'Y' && replay != 'N');
	
	if( replay == 'Y' || replay == 'y' ){
        playAgain = true;
	}
		
    return playAgain;
}

int main()
{
    TicTacToe Game;
    while( Game.Play() )
    {
        cout << "\n\n\n\n";
    }
    cout << "THANKS FOR PLAYING !!!\n";
    return 0;
}

 
