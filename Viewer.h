#ifndef HEX_VIEWER_H_
#define HEX_VIEWER_H_

#include "Board.h"

/* This class is used to display a Board and messages on the terminal. The
   called must specify a char to be displayed for the red and black piece. 
   A Board(5, 4) with red as X and black as O might look like:
   
1: . - . - O - O - .
    \ / \ / \ / \ / \
  2: . - X - . - . - .
      \ / \ / \ / \ / \
    3: . - O - X - O - O
        \ / \ / \ / \ / \
      4: X - . - X - X - O
          A   B   C   D   E

*/
class Viewer {
    private:
        const char RED_CHAR;    // The char used for red pieces 
        const char BLACK_CHAR;  // The char used for black pieces
    public:
        /* Creates a BoardView, used to display the board to the terminal.
           const char red : The char used for red pieces 
           const char black: The char used for black pieces                  
        */
        Viewer(const char red, const char black);
        /* Displays the board on the terminal. 
           const Board& b : The Board to be displayed
        */
        void board(const Board& b);
        /* Display a message on the terminal.
        */
        static void message(const std::string& msg);
        /* Return the response from the terminal.
        */
        static std::string response();
};

#endif  // HEX_VIEWER_H_