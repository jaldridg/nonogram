#ifndef BOARD_HH
#define BOARD_HH

#include <vector>

enum Tilestate {
    UNKNOWN = 32,
    NONE = 88,
    FILLED = 510
    //COL_ONE = 65,
    //COL_TWO = 66,
    //COL_THREE = 67
};

typedef Tilestate * Line;
typedef std::vector<int> * Clues;

struct Board {
    int size;

    Line * rows;
    Line * cols;

    Clues * row_clues;
    Clues * col_clues;

    Board(int size);
    // The destructor since ~Board is being weird
    void clear();

    void print();

    void setTileRange(Line line, std::pair<int, int> ids, Tilestate state);

    int getRowNumber(Line line);
    int getColNumber(Line line);

    bool isLineSolved(Line line);

    int getClue(std::vector<Clues> * clues, int num, int pos) {
        return clues->at(num)->at(pos);
    }

    void setClue(std::vector<Clues> * clues, int num, int pos, int clue) {
        clues->at(num)->at(pos) = clue;
    }
};

#endif