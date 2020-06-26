#include "Board.h"
#include "Viewer.h"
#include "HumanPlayer.h"
#include "RandomPlayer.h"
#include "Game.h"
#include "MonteCarloPlayer.h"
#include "Piece.h"
#include "UserAbort.h"


using namespace std;

int main() {
    const Piece piece1 = RED;
    const Piece piece2 = BLACK;
    Viewer viewer = Viewer('O', 'X');
    Board board = Board(7, 7, piece1, piece2);
    HumanPlayer player1 = HumanPlayer(viewer);
    //RandomPlayer player2 = RandomPlayer();
    MonteCarloPlayer player2 = MonteCarloPlayer(piece2, 10000u, viewer);
    Game game = Game(board, player1, player2, piece1, piece2);
    string howto = "";
    howto += "How to play: Players take turns to place their piece on the\n";
    howto += "board. Player 1's piece is an 'O' and player 2's an 'X'. The\n";
    howto += "first player to link their edges of the board wins. Player 1\n";
    howto += "must link horizontally and player 2 vertically. Coordinates\n";
    howto += "should be entered as the column letter(s) with the row number\n";
    howto += "appended, e.g. 'A7'\n";
    howto += "\n";
    viewer.message(howto);
    try {
        const Player* winner = game.play();
        viewer.board(board);
        if (winner == &player1) {
            viewer.message("Well done, you win!");
        } else {
            viewer.message("Bad luck, you lose!");
        }
    } catch (UserAbort e) {
        viewer.message("Goodbye");
    }
}