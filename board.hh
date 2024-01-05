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

typedef Tilestate * Tiles;
typedef std::vector<int> * Clues;

struct line {
    Tiles tiles;
    Clues clues;
    int unknown_tiles;
    int filled_tiles;
    int line_number; // Lower towards the top left corner
    bool is_row;
};

struct Board {
    int size;

    line * rows;
    line * cols;

    Board(int size);
    // The destructor since ~Board is being weird
    void clear();

    void print();

    void setTileRange(line line, std::pair<int, int> ids, Tilestate state);

    int getClue(std::vector<Clues> * clues, int num, int pos) {
        return clues->at(num)->at(pos);
    }

    void setClue(std::vector<Clues> * clues, int num, int pos, int clue) {
        clues->at(num)->at(pos) = clue;
    }
};

#endif