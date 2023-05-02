
#include <stdio.h>
#include <string>
#include <assert.h>

#include "board.hh"

Board::Board(int size) {
    // Initilize data structures
    Board::size = size;
    rows = new std::vector<Line *>();
    cols = new std::vector<Line *>();
    row_clues = new std::vector<std::vector<int> *>();
    col_clues = new std::vector<std::vector<int> *>();

    // Fill board with unknown tiles which must be solved
    for (int i = 0; i < size; i++) {
        std::vector<Tilestate> * r = new std::vector<Tilestate>();
        std::vector<Tilestate> * c = new std::vector<Tilestate>();
        for (int j = 0; j < size; j++) {
            r->push_back(UNKNOWN);
            c->push_back(UNKNOWN);
        }
        rows->push_back(r);
        cols->push_back(c);

        // Fill in clues
        std::vector<int> * rc = new std::vector<int>();
        std::vector<int> * cc = new std::vector<int>();
        if (i == 1) {
            rc->push_back(3);
            cc->push_back(3);
        } else {
            rc->push_back(1);
            cc->push_back(1);
        }
        row_clues->push_back(rc);
        col_clues->push_back(cc);
    }
}

void Board::clear() {
    for (int i = 0; i < size; i++) {
        delete rows->at(i);
        delete cols->at(i);
        delete row_clues->at(i);
        delete col_clues->at(i);
    }
    delete rows;
    delete cols;
    delete row_clues;
    delete col_clues;
}

void Board::print() {
    // Find largest list of clues for rows and cols
    int max_row_clues = 0;
    int max_col_clues = 0;
    for (int i = 0; i < size; i++) {
        int row_size = row_clues->at(i)->size();
        if (row_size > max_row_clues) {
            max_row_clues = row_size;
        }
        int col_size = col_clues->at(i)->size();
        if (col_size > max_col_clues) {
            max_col_clues = col_size;
        }
    }


    // Print column rules
    for (int i = max_col_clues; i > 0; i--) {
        // Print spaces to account for row rules
        for (int j = 0; j < max_row_clues; j++) {
            printf("  ");
        }
        printf(" ");
        for (int j = 0; j < size; j++) {
            int size = col_clues->at(j)->size();
            if (size >= i) {
                printf("%d ", col_clues->at(j)->at(size - i));
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
        int size = row_clues->at(i)->size();
        for (int j = max_row_clues; j > 0; j--) {
            if (size >= j) {
                printf("%d ", row_clues->at(i)->at(size - j));
            } else {
                printf("  ");
            }
        }
        printf("|");
        // Print board
        for (int j = 0; j < size; j++) {
            printf("%c ", rows->at(i)->at(j));
        }
        printf("\n");
    }
}

// Fills the line with the given state using the limits in the pair
// The first entry is the starting index and the second entry is the stopping index
void Board::setTileRange(Line * line, std::pair<int, int> ids, Tilestate state) {
    assert(ids.first < ids.second);
    for (int i = ids.first; i < ids.second; i++) {
        line->at(i) = state;
    }
}