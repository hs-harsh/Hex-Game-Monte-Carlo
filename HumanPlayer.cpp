#include <stdexcept>
#include "UserAbort.h"
#include "HumanPlayer.h" 


using namespace std;

HumanPlayer::HumanPlayer(Viewer& viewer): viewer(&viewer) {}

BoardCoord HumanPlayer::takeTurn(Board& board) const {
    viewer->board(board);
    while (true) {
        viewer->message("Enter your move or type \"exit\" to quit: ");
        try {
            string response = viewer->response();
            if(response == "exit") {
                throw UserAbort();
            }
            BoardCoord boardCoord = BoardCoord(response);
            if (!board.isValid(boardCoord)) {
                viewer->message("That's not on the board! Try again...");
            } else if (!board.isEmpty(boardCoord)) {
                viewer->message("That's already occupied! Try again...");
            } else { 
                return boardCoord;
            }
        } catch (const invalid_argument&) {
            viewer->message("That's not a coordinate! Try again...");
        }
    }
}