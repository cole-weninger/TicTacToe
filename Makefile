compiler = g++
standard = -std=c++11

TARGET = tictactoe

tictactoe: tictactoe.cpp
	${compiler} ${standard} tictactoe.cpp -o tictactoe

clean:
	rm ${TARGET}
	
