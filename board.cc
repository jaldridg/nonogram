
#include <stdio.h>
#include <string>
#include <assert.h>

#include "board.hh"
#include "board_reader.hh"

Board::Board(int size) {

    // Initilize data structures
    Board::size = size;

    rows = new line[size];
    cols = new line[size];

    Clues row_clues = new std::vector<int>;
    Clues col_clues = new std::vector<int>;

    // Initialize default line values
    for (int i = 0; i < size; i++) {
        rows[i].filled_tiles = 0;
        rows[i].unknown_tiles = size;
        rows[i].line_number = i;
        rows[i].is_row = true;

        cols[i].filled_tiles = 0;
        cols[i].unknown_tiles = size;
        cols[i].line_number = i;
        cols[i].is_row = false;
    }

    // Fill board with unknown tiles which must be solved
    for (int i = 0; i < size; i++) {
        Tiles row_tiles = new Tilestate[size];
        Tiles col_tiles = new Tilestate[size];
        for (int j = 0; j < size; j++) {
            row_tiles[j] = UNKNOWN;
            col_tiles[j] = UNKNOWN;
        }
        rows[i].tiles = row_tiles;
        cols[i].tiles = col_tiles;
    }

    for (int i = 0; i < size; i++) {
        Clues rc = new std::vector<int>;
        Clues cc = new std::vector<int>;
        if (i == 0) {
            rc->push_back(4);
            cc->push_back(4);
        } else if (i == 1) {
            rc->push_back(6);
            cc->push_back(6);
        } else if (i == 2) {
            rc->push_back(2);
            rc->push_back(5);
            cc->push_back(3);
            cc->push_back(4);
        } else if (i == 3) {
            rc->push_back(10);
            cc->push_back(2);
            cc->push_back(7);
        } else if (i == 4) {
            rc->push_back(2);
            rc->push_back(1);
            rc->push_back(5);
            cc->push_back(4);
            cc->push_back(5);
        } else if (i == 5) {
            rc->push_back(10);
            cc->push_back(10);
        } else if (i == 6) {
            rc->push_back(10);
            cc->push_back(10);
        } else if (i == 7) {
            rc->push_back(8);
            cc->push_back(8);
        } else if (i == 8) {
            rc->push_back(6);
            cc->push_back(6);
        } else if (i == 9) {
            rc->push_back(4);
            cc->push_back(4);
        }
        rows[i].clues = rc;
        cols[i].clues = cc;
    }
}

void Board::clear() {
    for (int i = 0; i < size; i++) {
        delete[] rows[i].tiles;
        delete[] cols[i].tiles;
        delete rows[i].clues;
        delete cols[i].clues;
    }
    delete[] rows;
    delete[] cols;
}

void Board::print() {
    // Find largest list of clues for rows and cols
    int max_row_clues = 0;
    int max_col_clues = 0;
    for (int i = 0; i < size; i++) {
        int row_size = rows[i].clues->size();
        if (row_size > max_row_clues) {
            max_row_clues = row_size;
        }
        int col_size = cols[i].clues->size();
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
            int size = cols[j].clues->size();
            if (size >= i) {
                printf("%x ", cols[j].clues->at(size - i));
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
        int len = rows[i].clues->size();
        for (int j = max_row_clues; j > 0; j--) {
            if (len >= j) {
                printf("%x ", rows[i].clues->at(len - j));
            } else {
                printf("  ");
            }
        }
        printf("|");
        // Print board
        for (int j = 0; j < size; j++) {
            printf("%c ", rows[i].tiles[j]);
        }
        printf("\n");
    }
}

// Fills the line with the given state using the limits in the pair
// The first entry is the starting index and the second entry is the stopping index
void Board::setTileRange(line * line, std::pair<int, int> ids, Tilestate state) {
    assert(ids.first <= ids.second);

    if (line->is_row) {
        // Edit each column affected
        for (int i = ids.first; i <= ids.second; i++) {
            if (cols[i].tiles[line->line_number] != state) {
                cols[i].tiles[line->line_number] = state;
                cols[i].unknown_tiles--;
                cols[i].filled_tiles++;
            }
        }
    } else {
        // Edit each row affected
        for (int i = ids.first; i <= ids.second; i++) {
            if (rows[i].tiles[line->line_number] != state) {
                rows[i].tiles[line->line_number] = state;
                rows[i].unknown_tiles--;
                rows[i].filled_tiles++;
            }
        }
    }

    // Set the range in the line
    int diff_count = 0;
    for (int i = ids.first; i <= ids.second; i++) {
        if (line->tiles[i] != state) {
            line->tiles[i] = state;
            diff_count++;
        }
    }
    line->unknown_tiles -= diff_count;
    line->filled_tiles += diff_count;
}