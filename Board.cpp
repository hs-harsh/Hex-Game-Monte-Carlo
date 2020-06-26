#include <stdexcept>

#include "Board.h"

using namespace std;

/*****************************************************************************
Constructors
******************************************************************************/
        
Board::Board(unsigned int width, unsigned int height, Piece player1, Piece player2) : 
        WIDTH(width), HEIGHT(height), PLAYER1(player1), PLAYER2(player2) {
    if (width <= 0u || height <= 0u) {
        throw invalid_argument("Width and height must not be less than 1");
    }
    board = new BoardSquare[WIDTH * HEIGHT];
    // Each players connectivity graph
    p1Graph = new Graph<bool>(false, WIDTH*HEIGHT);
    p2Graph = new Graph<bool>(false, WIDTH*HEIGHT);
    // Connect all starts/ends together to make only 1 of each
    p1Start = coord2graph(BoardCoord(0, 0));
    p1End = coord2graph(BoardCoord(WIDTH - 1, 0));
    for (unsigned int i = 1; i < HEIGHT; i++) {
        p1Graph->setWeight(p1Start, coord2graph(BoardCoord(0, i)), true);
        p1Graph->setWeight(p1End, coord2graph(BoardCoord(WIDTH - 1, i)), true);
    }
    p2Start = coord2graph(BoardCoord(0, 0));
    p2End = coord2graph(BoardCoord(0, HEIGHT - 1));
    for (unsigned int i = 1; i < WIDTH; i++) {
        p2Graph->setWeight(p2Start, coord2graph(BoardCoord(i, 0)), true);
        p2Graph->setWeight(p2End, coord2graph(BoardCoord(i, HEIGHT - 1)), true);
    }
}

Board::Board(const Board& other) : 
        WIDTH(other.WIDTH), HEIGHT(other.HEIGHT), 
        PLAYER1(other.PLAYER1), PLAYER2(other.PLAYER2),
        p1Graph(new Graph<bool>(*other.p1Graph)),
        p2Graph(new Graph<bool>(*other.p2Graph)),
        p1Start(other.p1Start), p1End(other.p1End),
        p2Start(other.p2Start), p2End(other.p2End) {
    
    board = new BoardSquare[WIDTH * HEIGHT];
    for(unsigned int i = 0; i < WIDTH * HEIGHT; i++) {
        board[i] = other.board[i];
    }
}

/*****************************************************************************
Deconstructors
******************************************************************************/

Board::~Board(){
    delete[] board;
}

/*****************************************************************************
Public Methods
******************************************************************************/

bool Board::isValid(BoardCoord coord) const {
    return isValid(coord.col, coord.row);
}

bool Board::isEmpty(BoardCoord coord) const {
    return get(coord.col, coord.row).empty;     
}

Piece Board::getPiece(BoardCoord coord) const {
    BoardSquare square = get(coord.col, coord.row);
    if (square.empty) {
        throw out_of_range("BoardCoord is empty");
    }
    return square.piece;     
}

void Board::addPiece(Piece piece, BoardCoord coord) {
    if(!isValid(coord)) {
        throw out_of_range("Invalid BoardCoord");
    }
    if(!isEmpty(coord)) {
        throw out_of_range("BoardCoord not empty");
    }
    set(coord.col, coord.row, BoardSquare(false, piece));
    // Update the pieces graphs
    Graph<bool>* graph;
    if (piece == PLAYER1) {
        graph = p1Graph;
    } else {
        graph = p2Graph;
    }
    unsigned int index = coord2graph(coord);
    // Adjacent connections
    int adj[12] = {0, -1, 1, -1, -1, 0, 1, 0, -1, 1, 0, 1};
    for (int i = 0; i < 12; i += 2) {
        unsigned int adjCol = coord.col + adj[i];
        unsigned int adjRow = coord.row + adj[i+1];
        BoardCoord adjCoord = BoardCoord(adjCol, adjRow);
        if (isValid(adjCoord) && !isEmpty(adjCoord) &&
                getPiece(adjCoord) == piece) {
            unsigned int adjIndex = coord2graph(adjCoord);
            graph->setWeight(index, adjIndex, true);
        }
    }      
}

/*****************************************************************************
Private Methods
******************************************************************************/

bool Board::isValidCol(unsigned int col) const {
    return col >= 0u && col < this->WIDTH;
}

bool Board::isValidRow(unsigned int row) const {
    return row >= 0u && row < this->HEIGHT;
}

bool Board::isValid(unsigned int col, unsigned int row) const {
    return isValidCol(col) && isValidRow(row);
}

Board::BoardSquare Board::get(unsigned int col, unsigned int row) const {
    if(!isValid(col, row)) {
        throw out_of_range("Invalid col or row");
    }
    return board[row * WIDTH + col];
}

void Board::set(unsigned int col, unsigned int row, BoardSquare square) {
    if(!isValid(col, row)) {
        throw out_of_range("Invalid col or row");
    }
    board[row * WIDTH + col] = square;
}

unsigned int Board::coord2graph(BoardCoord coord) {
    return coord.row * WIDTH + coord.col;
}

bool Board::isWinner(Piece piece, bool cache) {
    if (piece == PLAYER1) {
        return p1Graph->isConnected(p1Start, p1End, cache);
    } else {
        return p2Graph->isConnected(p2Start, p2End, cache);
    }
}
/*****************************************************************************
Private Class BoardSquare
******************************************************************************/

Board::BoardSquare::BoardSquare(): empty(true), piece(RED) {}
Board::BoardSquare::BoardSquare(const bool e, const Piece p): empty(e), piece(p) {}
