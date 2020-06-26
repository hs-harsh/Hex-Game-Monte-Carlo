#include <iostream>
#include <regex>

#include "Viewer.h"
#include "BoardCoord.h"

using namespace std;

Viewer::Viewer(const char red, const char black): RED_CHAR(red), BLACK_CHAR(black) {}

void Viewer::board(const Board& b) {
    // Go through each row
    for(unsigned int row = 0; row < b.HEIGHT; row++) {
        string line = "";
        string next = "";
        // For each row, go through each cell
        for(unsigned int col = 0; col < b.WIDTH; col++) {
            // Find the corresponding piece on the board
            BoardCoord bc = BoardCoord(col, row);
            // Add the corresponding character to line
            if (b.isEmpty(bc)) {
                line += ".";            
            } else {
                Piece p = b.getPiece(bc);
                if (p == RED) {
                    line += RED_CHAR;
                } else {
                    line += BLACK_CHAR;
                }
            }
            // Add the aesthetic padding between pieces and lines
            if (col == b.WIDTH - 1) {
                next += " \\";        
            } else {
                line += " - ";
                next +=  " \\ /";
            }
        } 
        // Display the line padding at the start of the line and coord numbers
        ostringstream iStm;
        iStm << (row + 1);
        string iStr = iStm.str() + ": ";
        string pad((row * 2) + 3 - iStr.size(), ' ');
        string nextPad((row * 2) + 3, ' ');
        cout << pad << iStr << line << endl;
        if (row != b.HEIGHT - 1) {
            cout << nextPad << next << endl;
        } else {
            // Display the coord letters at the bottom    
            int noOfRows = BoardCoord::toColStr(0, b.WIDTH).size();
            for(int col = 0; col < noOfRows; col++) {
                string lettersPad(col + 1, ' ');
                cout << nextPad << lettersPad;
                for(unsigned int k = 0; k < b.WIDTH; k++) {
                    cout << BoardCoord::toColStr(k, b.WIDTH).at(col) << "   ";
                }
                cout << endl;
            }        
        }
    }
}

void Viewer::message(const string& msg) {
    cout << msg << endl;
}

string Viewer::response() {
    string response;
    cin >> response;
    return response;
}
