
#include <stdio.h>
#include "board.hh"

Board::Board(int size) {
    Board::size = size;
    // Fill board with unknown tiles which must be solved
    for (int i = 0; i < size; i++) {
        std::vector<Tilestate> r = std::vector<Tilestate>();
        std::vector<Tilestate> c = std::vector<Tilestate>();
        for (int j = 0; j < size; j++) {
            r.push_back(UNKNOWN);
            c.push_back(UNKNOWN);
        }
        rows.push_back(r);
        cols.push_back(c);
    }
}

void Board::print() {
    printf("Hello there!!\n");
}