#include <assert.h> 

#include "Game.h" 

Game::Game(Board& board, const Player& player1, const Player& player2,
        Piece p1Piece, const Piece p2Piece) :
        board(&board), PLAYER1(&player1), PLAYER2(&player2),
        P1_PIECE(p1Piece), P2_PIECE(p2Piece) {}

const Player* Game::play() {
    int size = board->WIDTH * board->HEIGHT;
    
    for(int turn = 0; turn < size; turn++) {
        const Player* player;
        Piece piece;
        if(turn % 2 == 0) {
            player = PLAYER1;
            piece = P1_PIECE;
        } else {
            player = PLAYER2;
            piece = P2_PIECE;
        }
        BoardCoord coord = player->takeTurn(*board);
        board->addPiece(piece, coord);
        if (board->isWinner(piece, true)) {
            return player;
        }
    }
    // Unreachable statement
    assert(false);
}