#ifndef board_hh
#define board_hh

#include <vector>

enum Tilestate {
    UNKNOWN = 63,
    NONE = 88,
    COL_ONE = 65,
    COL_TWO = 66,
    COL_THREE = 67
};

typedef std::vector<Tilestate> Row;
typedef std::vector<Tilestate> Col;

struct Board {
    int size;

    std::vector<Row> rows;
    std::vector<Col> cols;

    // The hints which were given to solve the puzzle
    std::vector<Row> row_clues;
    std::vector<Col> col_clues;

    Board(int size);

    void print();
};

#endif