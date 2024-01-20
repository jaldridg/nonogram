
#include <stdio.h>
#include <string>
#include <assert.h>

#include "board.hh"
#include "board_reader.hh"

Board::Board() {
    BoardReader reader;

    // Initilize data structures
    size = reader.col_clues.size();

    rows = new line[size];
    cols = new line[size];

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
        rows[i].tiles = new Tilestate[size];
        cols[i].tiles = new Tilestate[size];
        for (int j = 0; j < size; j++) {
            rows[i].tiles[j] = UNKNOWN;
            cols[i].tiles[j] = UNKNOWN;
        }
    }

    // Get the clues from our clue file reader
    for (int i = 0; i < size; i++) {
        Clues rc = new std::vector<int>;
        Clues cc = new std::vector<int>;
        for (int j = 0; j < reader.row_clues.at(i).size(); j++) {
            rc->push_back(reader.row_clues.at(i).at(j));
        }
        
        for (int j = 0; j < reader.col_clues.at(i).size(); j++) {
            cc->push_back(reader.col_clues.at(i).at(j));
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
// The first entry is the starting index and the second entry is the stopping index (inclusive)
void Board::setTileRange(line * line, std::pair<int, int> ids, Tilestate state) {
    assert(ids.first <= ids.second);

    if (line->is_row) {
        // Edit each column affected
        for (int i = ids.first; i <= ids.second; i++) {
            if (cols[i].tiles[line->line_number] != state) {
                cols[i].tiles[line->line_number] = state;
                cols[i].unknown_tiles--;
                if (state == FILLED) { cols[i].filled_tiles++; }
            }
        }
    } else {
        // Edit each row affected
        for (int i = ids.first; i <= ids.second; i++) {
            if (rows[i].tiles[line->line_number] != state) {
                rows[i].tiles[line->line_number] = state;
                rows[i].unknown_tiles--;
                if (state == FILLED) { rows[i].filled_tiles++; }
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
    if (state == FILLED) { line->filled_tiles += diff_count; }
}

void Board::completeLine(line * line) {
    if (line->is_row) {
        for (int i = 0; i < size; i++) {
            if (line->tiles[i] == UNKNOWN) {
                line->tiles[i] = NONE;
                cols[i].tiles[line->line_number] = NONE;
                cols[i].unknown_tiles--;
            }
        }
    } else {
        for (int i = 0; i < size; i++) {
            if (line->tiles[i] == UNKNOWN) {
                line->tiles[i] = NONE;
                rows[i].tiles[line->line_number] = NONE;
                rows[i].unknown_tiles--;
            }
        }
    }
    
    line->unknown_tiles = 0;
}