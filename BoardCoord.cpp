#include <string>
#include <stdexcept>
#include <iostream>

#include "BoardCoord.h"

using namespace std;

BoardCoord::BoardCoord(): col(0), row(0) {}
BoardCoord::BoardCoord(int col, int row): col(col), row(row) {}

BoardCoord::BoardCoord(const string& coord) {
    string s_col = "";
    row = 0;
    // Go through the coord string
    bool numbers = false; // Indicates if reached the row numbers yet
    for (auto it = coord.cbegin(); it != coord.cend(); ++it) {
        int asc = static_cast<int>(*it);
        if(asc >= 65 && asc <= 90 && !numbers) {
            // Uppercase column letter
            s_col += string(1, *it);
        } else if (asc >= 48 && asc <= 57) {
            // Row number
            numbers = true;
            row = row * 10 + (asc - 48);
        } else {
            // Invalid input
            throw invalid_argument("Not a valid coordinate");
        }
    }
    col = toColInt(s_col);
    row = row - 1;
}

string BoardCoord::toColStr(int col, int noOfCols) {
    if(col < 0 || (noOfCols > 0 && col >= noOfCols)) {
        throw out_of_range("Required that 0 <= col < noOfCols");
    }
    string str = "";
    int maxCols = 1;
    do {
        int index = col % 26;    
        str = string(1, char(65 + index)) + str;
        col = col / 26;
        maxCols *= 26;
    } while (noOfCols > maxCols || (noOfCols <= 0 && col > 0));
    return str;
}

int BoardCoord::toColInt(const string& col) {
    int i = 0;
    for (auto it = col.cbegin(); it != col.cend(); ++it) {
        int asc = static_cast<int>(*it);
        if(asc < 65 || asc > 90) {
            throw invalid_argument("Not a valid column letter");
        }
        i = (i * 26) +  (asc - 65);
    }
    return i;
}

std::ostream& operator<<(std::ostream& strm, const BoardCoord& coord) {
  return strm << coord.toColStr(coord.col) << (coord.row + 1);
}