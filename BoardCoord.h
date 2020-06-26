#ifndef HEX_BOARDCOORD_H_
#define HEX_BOARDCOORD_H_

#include <string>
#include <iostream>

/* A row and col coordinate corresponding to a position on the board */
class BoardCoord {
    public:
        int col;
        int row;
        BoardCoord();
        BoardCoord(int col, int row);
        /* Given a string which is a uppercase alphabetic string with a
           positive number appended, creates the corresponding BoardCoord.
           E.g. A7 -> BoardCoord(0, 6), E3 -> BoardCoord(5, 2)
           throws: invalid_argument if coord is invalid.
           Note: A's are like 0's. They can precede the column without changing
           it's value.
        */
        BoardCoord(const std::string& coord);
        /* Given a column number and the total number of columns, this method
           returns an uppercase alphabetic string representing the number to
           base 26. This method is the inverse of toColInt(string)
           throws: out_of_range if col < 0 or col >= noOfCols
           noOfCols: if <=0 then leading A's (0's) are emitted
        */
        static std::string toColStr(int col, int noOfCols = 0);
        /* Given an uppercase alphabetic string representing, this method
           returns an int representing the string. This method is the inverse
           of toColInt(int, int)
           throws: invalid_argument if col is invalid.
        */
        static int toColInt(const std::string& col);
        friend std::ostream& operator<<(std::ostream& strm, const BoardCoord& coord);
};

#endif  // HEX_BOARDCOORD_H_