
#include <stdio.h>
#include <string>
#include <assert.h>

#include "board.hh"

Board::Board(int size) {
    // Initilize data structures
    Board::size = size;
    rows = new Line[size];
    cols = new Line[size];
    row_clues = new Clues[size];
    col_clues = new Clues[size];

    // Fill board with unknown tiles which must be solved
    for (int i = 0; i < size; i++) {
        Line rl = new Tilestate[size];
        Line cl = new Tilestate[size];
        for (int j = 0; j < size; j++) {
            rl[j] = UNKNOWN;
            cl[j] = UNKNOWN;
        }
        rows[i] = rl;
        cols[i] = cl;
    }

    for (int i = 0; i < size; i++) {
        Clues rc = new std::vector<int>;
        Clues cc = new std::vector<int>;
        if (i == 0) {
            rc->push_back(3);
            cc->push_back(3);
        } else {
            rc->push_back(1);
            cc->push_back(1);
            rc->push_back(1);
            cc->push_back(1);
        }
        row_clues[i] = rc;
        col_clues[i] = cc;
    }
}

void Board::clear() {
    for (int i = 0; i < size; i++) {
        delete[] rows[i];
        delete[] cols[i];
        delete row_clues[i];
        delete col_clues[i];
    }
    delete[] rows;
    delete[] cols;
    delete[] row_clues;
    delete[] col_clues;
}

void Board::print() {
    // Find largest list of clues for rows and cols
    int max_row_clues = 0;
    int max_col_clues = 0;
    for (int i = 0; i < size; i++) {
        int row_size = row_clues[i]->size();
        if (row_size > max_row_clues) {
            max_row_clues = row_size;
        }
        printf("row loc: %d\n", rows[i]);
        int col_size = col_clues[i]->size();
        if (col_size > max_col_clues) {
            max_col_clues = col_size;
        }
        printf("col loc: %d\n", cols[i]);
    }


    // Print column rules
    for (int i = max_col_clues; i > 0; i--) {
        // Print spaces to account for row rules
        for (int j = 0; j < max_row_clues; j++) {
            printf("  ");
        }
        printf(" ");
        for (int j = 0; j < size; j++) {
            int size = col_clues[j]->size();
            if (size >= i) {
                printf("%d ", col_clues[j]->at(size - i));
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }

    // Prints the contents of the board
    for (int i = 0; i < max_row_clues; i++) {
        printf("  ");
    }
    printf(" ");
    for(int i = 0; i < 2 * size - 1; i++) {
        printf("-");
    }
    printf("\n");
    for (int i = 0; i < size; i++) {
        // Print clues
        int size = row_clues[i]->size();
        for (int j = max_row_clues; j > 0; j--) {
            if (size >= j) {
                printf("%d ", row_clues[i]->at(size - j));
            } else {
                printf("  ");
            }
        }
        printf("|");
        // Print board
        for (int j = 0; j < size; j++) {
            printf("%c ", rows[j][i]);
        }
        printf("\n");
    }
}

// Fills the line with the given state using the limits in the pair
// The first entry is the starting index and the second entry is the stopping index
void Board::setTileRange(Line line, std::pair<int, int> ids, Tilestate state) {
    assert(ids.first < ids.second);
    for (int i = ids.first; i < ids.second; i++) {
        line[i] = state;
    }
}