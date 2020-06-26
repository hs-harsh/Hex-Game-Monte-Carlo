#ifndef HEX_GAME_H_
#define HEX_GAME_H_

#include "Board.h" 
#include "Player.h"
#include "Piece.h" 

class Game {
    private:
        Board* board;
        const Player* PLAYER1;
        const Player* PLAYER2;
        const Piece P1_PIECE;
        const Piece P2_PIECE;
    public: 
        Game(Board& board, const Player& player1, const Player& player2, 
                const Piece p1Piece, const Piece p2Piece);
        const Player* play();
};

#endif  // HEX_GAME_H_