#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "RandomPlayer.h" 

BoardCoord RandomPlayer::takeTurn(Board& board) const {
    // Count all the empty squares
    int empty = 0;
    for(unsigned int x = 0; x < board.WIDTH; x++) {
        for(unsigned int y = 0; y < board.HEIGHT; y++) {
            if(board.isEmpty(BoardCoord(x, y))) {
                empty++;
            }
        } 
    }
    // Randomly Select one of the empty squares
    srand (time(NULL));
    int index = rand() % empty;
    int count = 0;
    for(unsigned int x = 0; x < board.WIDTH; x++) {
        for(unsigned int y = 0; y < board.HEIGHT; y++) {
            if(board.isEmpty(BoardCoord(x, y))) {
                if(index == count) {
                    // Return the random square
                    return BoardCoord(x, y);
                }
                count++;
            }
        } 
    }
    // Unreachable statement
    assert(false);
}