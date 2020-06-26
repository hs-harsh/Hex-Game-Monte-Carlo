#ifndef HEX_HUMANPLAYER_H_
#define HEX_HUMANPLAYER_H_

#include "Player.h" 
#include "Board.h" 
#include "BoardCoord.h" 
#include "Viewer.h" 


class HumanPlayer: public Player {
    private:
        Viewer* viewer;
    public:
        HumanPlayer(Viewer& viewer);
        BoardCoord takeTurn(Board& board) const; 
};

#endif  // HEX_HUMANPLAYER_H_