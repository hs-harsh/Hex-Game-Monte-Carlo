#include <stdlib.h>
#include <assert.h>
#include <random>
#include <chrono>

#include "MonteCarloPlayer.h" 

using namespace std;

MonteCarloPlayer::MonteCarloPlayer(Piece piece, unsigned int iterations, Viewer viewer) : 
    ITERATIONS(iterations), PIECE(piece), viewer(viewer) {}

BoardCoord MonteCarloPlayer::takeTurn(Board& board) const {
    // Setup random number generator
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine gen(seed);
    uniform_real_distribution<long double> dis(0.0,1.0);
    // Display a random "Thinking" message
    const string messages[] = {"Let me think.", "Thinking...", "Just a moment.", "Hmmm.....", 
            "I'll need a moment.", "Just a sec.", "Calculating...", "I be a mo."};
    viewer.message(messages[(int) (dis(gen)*8)]);
    // Array of empty BoardCoords
    BoardCoord* empties = new BoardCoord[board.WIDTH * board.HEIGHT];
    unsigned int emptiesLength = 0;
    for (unsigned int col = 0; col < board.WIDTH; col++) {
        for (unsigned int row = 0; row < board.HEIGHT; row++) {
            BoardCoord coord = BoardCoord(col, row);
            if (board.isEmpty(coord)) {
                empties[emptiesLength] = coord;
                emptiesLength++;
            }
        }
    }
    unsigned int iterPerJ = ITERATIONS/emptiesLength;
    // Update the cache before copies are made
    board.isWinner(PIECE, true);
    // Iterate to find the best position
    BoardCoord best;
    unsigned int bestScore = 0;
    for(unsigned int i = 0; i < emptiesLength; i++) {
        // Copy the board and add the piece
        BoardCoord coord_i = empties[i];
        Board* board_i = new Board(board);
        board_i->addPiece(PIECE, coord_i);
        // Check if winner and update the cache
        if (board_i->isWinner(PIECE, true)) {
            return coord_i;
        }
        // Add up the number of wins
        unsigned int score = 0;
        for(unsigned int j = 0; j < iterPerJ; j++) {
            // Copy the board for this iteration
            Board* board_j = new Board(*board_i);
            // Assign the remaining coords
            unsigned int rem = emptiesLength - 1;
            unsigned int places = (emptiesLength - 1) / 2;
            for(unsigned int k = 0; k < emptiesLength - 1; k++) {
                BoardCoord coord_k;
                if (k < i) {
                    coord_k = empties[k];
                } else {
                    coord_k = empties[k+1];
                }
                if (places == rem || (places != 0 && dis(gen) < places/((long double)rem))) {
                    board_j->addPiece(PIECE, coord_k);
                    places--;
                }
                rem--;
            }
            if (board_j->isWinner(PIECE, true)) {
                score++;
            }
        }
        if (score >= bestScore) {
            best = coord_i;
            bestScore = score;
        }
    }
    return best;
}