#ifndef BOARD_HH
#define BOARD_HH

#include <vector>

enum Tilestate {
    UNKNOWN = 32,
    NONE = 88,
    COL_ONE = 65,
    COL_TWO = 66,
    COL_THREE = 67
};

typedef std::vector<Tilestate> Line;

struct Board {
    int size;

    std::vector<Line *> * rows;
    std::vector<Line *> * cols;

    // The hints which were given to solve the puzzle
    std::vector<std::vector<int> *> * row_clues;
    std::vector<std::vector<int> *> * col_clues;

    Board(int size);
    // The destructor since ~Board is being weird
    void clear();

    void print();
};

#endif