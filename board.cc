
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
        rows[i].tiles = new tile[size];
        cols[i].tiles = new tile[size];
        for (int j = 0; j < size; j++) {
            rows[i].tiles[j] = UNKNOWN;
            cols[i].tiles[j] = UNKNOWN;
        }
    }

    for (int i = 0; i < size; i++) {
        Clues rc = new std::vector<int>;
        Clues cc = new std::vector<int>;

        // Get the clues from our clue file reader
        for (int j = 0; j < reader.row_clues.at(i).size(); j++) {
            rc->push_back(reader.row_clues.at(i).at(j));
        }
        for (int j = 0; j < reader.col_clues.at(i).size(); j++) {
            cc->push_back(reader.col_clues.at(i).at(j));
        }
        rows[i].clues = rc;
        cols[i].clues = cc;

        // Initialize our blocks
        Blocks row_blocks = new std::vector<block>;
        Blocks col_blocks = new std::vector<block>;
        block initial_block = block { 0, size - 1, size, UNKNOWN };
        row_blocks->push_back(initial_block);
        col_blocks->push_back(initial_block);
        rows[i].blocks = row_blocks;
        cols[i].blocks = col_blocks;
    }
}

void Board::clear() {
    for (int i = 0; i < size; i++) {
        delete[] rows[i].tiles;
        delete[] cols[i].tiles;
        delete rows[i].clues;
        delete cols[i].clues;
        delete rows[i].blocks;
        delete cols[i].blocks;
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
        // Print 
        /*
        for (int j = 0; j < rows[i].blocks->size(); j++) {
            Tilestate t = rows[i].blocks->at(j).tile_state;
            int size = rows[i].blocks->at(j).block_length;
            for (int k = 0; k < size; k++) {
                printf("%c ", t);
            }
        }
        */
        for (int j = 0; j < size; j++) {
            printf("%c ", rows[i].tiles[j]);
        }
        printf("\n");
    }
}

// Sets a tile to a given tilestate by splitting blocks if necessary
// Note: This does not correct the other dimension but should be used as a helper
void Board::setTile(line * line, int index, Tilestate state) {
    // Split block 
    // set new block to state
    // Attempt to merge new block
}

// Fills the line with the given state using the limits in the pair
// The first entry is the starting index and the second entry is the stopping index (inclusive)
void Board::setTileRange(line * line, std::pair<int, int> ids, Tilestate state) {
    assert(ids.first <= ids.second);

    for (int i = ids.first; i <= ids.second; i++) {
        struct line l = line->is_row ? cols[i] : rows[i];
        if (l.tiles[line->line_number] != state) {
            l.tiles[line->line_number] = state;
            l.unknown_tiles--;
            if (state == FILLED) { l.filled_tiles++; }
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
    for (int i = 0; i < size; i++) {
        if (line->tiles[i] == UNKNOWN) {
            line->tiles[i] = NONE;

            struct line l = line->is_row ? cols[i] : rows[i];
            l.tiles[line->line_number] = NONE;
            l.unknown_tiles--;
        }
    }

    // BLOCK APPROACH
    // Loop over blocks
    for (int i = 0; i < line->blocks->size(); i++) {
        if (line->blocks->at(i).tile_state == UNKNOWN) {
            line->blocks->at(i).tile_state = NONE;
            // Loop over each tile in the block and correct the opposite dimension
            Blocks blocks = line->is_row ? cols[i].blocks : rows[i].blocks;
            // TODO: Need to find the block which belongs to a certain tile
                // This could be a data structure thing by adding the a block pointer to each tile
                // This could also be a function which loops over the blocks and sees which one is at the index
            for () {
                setTile(line, ???, NONE);
            }
        }
    }
    // Attempt to merge
    // unknowntiles --;
    
    line->unknown_tiles = 0;
}