#ifndef HEX_BOARD_H_
#define HEX_BOARD_H_

#include "Piece.h" 
#include "BoardCoord.h" 
#include "Graph.h"

class Board {  
    public:
        const unsigned int WIDTH;
        const unsigned int HEIGHT;
        const Piece PLAYER1;
        const Piece PLAYER2;
    private:
        Graph<bool>* p1Graph;
        Graph<bool>* p2Graph;
        unsigned int p1Start;
        unsigned int p1End;
        unsigned int p2Start;
        unsigned int p2End;
        class BoardSquare {
            public:
                bool empty;
                Piece piece;
                BoardSquare();
                BoardSquare(const bool edge, const Piece piece);
        };
        BoardSquare* board;
    public:
        Board(unsigned int width, unsigned int height, Piece player1, Piece player2);
        /* Creates a deep copy of Board board.
           const Board& board: the Board to be copied.
        */
        Board(const Board& other);
        ~Board();
        bool isValid(BoardCoord coord) const;
        /* Returns true if this BoardCoord is empty.
           BoardCoord coord: The BoardCoord to be checked
           throws: out_of_range if the BoardCoord does not lie on the board
        */
        bool isEmpty(BoardCoord coord) const;
        /* Returns the Piece at this BoardCoord.
           BoardCoord coord: The BoardCoord to be checked
           throws: out_of_range if the BoardCoord is empty or not on the board
        */
        Piece getPiece(BoardCoord coord) const;
        void addPiece(Piece piece, BoardCoord coord);
        // If cache is true then this will increase performance if called
        // repeatedivly
        bool isWinner(Piece piece, bool cache);
    private:
        bool isValidCol(unsigned int col) const;
        bool isValidRow(unsigned int row) const;
        bool isValid(unsigned int col, unsigned int row) const;
        /* Returns the BoardSquare for the specified col and row.
           throws: out_of_range if the col or row is outwith the bounds of this 
           Board
        */
        BoardSquare get(unsigned int col, unsigned int row) const;
        void set(unsigned int col, unsigned int row, BoardSquare square);
        // Returns a graph index
        unsigned int coord2graph(BoardCoord coord);

};

#endif  // HEX_BOARD_H_