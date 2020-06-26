#ifndef HEX_MONTECARLOPLAYER_H_
#define HEX_MONTECARLOPLAYER_H_

#include "Player.h" 
#include "Board.h" 
#include "BoardCoord.h" 
#include "Viewer.h"

/* An implementation of Player that uses a Monte Carlo algorithm
   to decide on the best move.
*/
class MonteCarloPlayer: public Player {
    public:
        const unsigned int ITERATIONS;
        Piece PIECE;
        Viewer viewer;
        MonteCarloPlayer(Piece piece, unsigned int iterations, Viewer viewer);
        BoardCoord takeTurn(Board& board) const; 
};

#endif  // HEX_MONTECARLOPLAYER_H_