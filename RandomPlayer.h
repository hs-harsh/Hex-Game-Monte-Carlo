#ifndef HEX_RANDOMPLAYER_H_
#define HEX_RANDOMPLAYER_H_

#include "Player.h" 
#include "Board.h" 
#include "BoardCoord.h" 

/* An implementation of Player that makes random moves on the board */
class RandomPlayer: public Player {
    public:
        BoardCoord takeTurn(Board& board) const; 
};

#endif  // HEX_RANDOMPLAYER_H_