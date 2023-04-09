
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

        // Fill in clues
        std::vector<int> rc = std::vector<int>();
        std::vector<int> cc = std::vector<int>();
        rc.push_back(5);
        cc.push_back(5);
        row_clues = std::vector<std::vector<int>>();
        col_clues = std::vector<std::vector<int>>();
        row_clues.push_back(rc);
        col_clues.push_back(cc);
    }
}

void Board::print() {
      // Find largest list of clues for rows and cols
    int max_row_clues = 0;
    int max_col_clues = 0;
    for (int i = 0; i < size; i++) {
        if (row_clues[i].size() > max_row_clues) {
            max_row_clues = row_clues[i].size();
        }
        if (col_clues[i].size() > max_col_clues) {
            max_col_clues = col_clues[i].size();
        }
    }

    // Print column rules
    for (int i = max_col_clues; i > 0; i--) {
        for (int j = 0; j < size; j++) {
            if (col_clues[j].size() >= i) {
                printf("%d ", max_col_clues - i);
            }
        }
        printf("\n");
    }

    printf(" ");
    for(int i = 0; i < 2 * size - 1; i++) {
        printf("-");
    }
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("|");
        for (int j = 0; j < size; j++) {
            printf("%c ", rows[i][j]);
        }
        printf("\n");
    }
}