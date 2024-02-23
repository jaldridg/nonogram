
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

    blocks = new block[2 * size * size];
    block_count = 0;

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

    for (int i = 0; i < size; i++) {
        // Initilize clues
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
        // This involves our block storage, initial blocks, and their linked lists
        block * block_head;
        block initial_row_block = block { 0, size - 1, size, UNKNOWN, NULL, NULL };
        block initial_col_block = block { 0, size - 1, size, UNKNOWN, NULL, NULL };
        blocks->push_back(initial_row_block);
        block * initial_row_block_ptr = &(blocks->back());
        blocks->push_back(initial_col_block);
        block * initial_col_block_ptr = &(blocks->back());
        rows[i].block_head = initial_row_block_ptr;
        cols[i].block_head = initial_col_block_ptr;
        rows[i].block_count = 1;
        cols[i].block_count = 1;

        // Initilize tiles
        rows[i].tiles = new tile[size];
        cols[i].tiles = new tile[size];
        for (int j = 0; j < size; j++) {
            rows[i].tiles[j].state = UNKNOWN;
            cols[i].tiles[j].state = UNKNOWN;
            rows[i].tiles[j].block = initial_row_block_ptr;
            cols[i].tiles[j].block = initial_col_block_ptr;
        }
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

void Board::mergeBlock(block * b, line * l) {
    // Get compatible blocks before this block
    block * before_block = b;
    while (before_block->prev) {
        // Stop if blocks are different types
        if (before_block->tile_state != before_block->prev->tile_state) { break; }
        before_block = b->prev;
    }
    // Get compatible blocks after this block
    // Remove all compatible blocks
    // Update current block
}

// Sets a tile to a given tilestate by splitting blocks if necessary
// Note: This does not correct the other dimension but should be used as a helper
void Board::setTile(line * line, int index, Tilestate state) {
    // TODO: Need to find the block which belongs to a certain tile
        // This could be a data structure thing by adding the a block pointer to each tile
        // This could also be a function which loops over the blocks and sees which one is at the index
    // Split block 
    // set new block to state
    // Attempt to merge new block
    line->unknown_tiles--;
}

// Fills the line with the given state using the limits in the pair
// The first entry is the starting index and the second entry is the stopping index (inclusive)
void Board::setTileRange(line * line, std::pair<int, int> ids, Tilestate state) {
    assert(ids.first <= ids.second);

    for (int i = ids.first; i <= ids.second; i++) {
        struct line l = line->is_row ? cols[i] : rows[i];
        if (l.tiles[line->line_number].state != state) {
            l.tiles[line->line_number].state = state;
            l.unknown_tiles--;
            if (state == FILLED) { l.filled_tiles++; }
        }
    }

    // Set the range in the line
    int diff_count = 0;
    for (int i = ids.first; i <= ids.second; i++) {
        if (line->tiles[i].state != state) {
            line->tiles[i].state = state;
            diff_count++;
        }
    }
    line->unknown_tiles -= diff_count;
    if (state == FILLED) { line->filled_tiles += diff_count; }
}

void Board::completeLine(line * l) {
    // TILE APPROACH
    /*
    for (int i = 0; i < size; i++) {
        if (line->tiles[i].state == UNKNOWN) {
            line->tiles[i].state = NONE;

            struct line l = line->is_row ? cols[i] : rows[i];
            l.tiles[line->line_number].state = NONE;
            l.unknown_tiles--;
        }
    }
    */

    // BLOCK APPROACH
    // Loop over blocks
    block * curr_block = l->block_head;
    for (int i = 0; i < l->block_count; i++) {
        if (curr_block->tile_state == UNKNOWN) {
            curr_block->tile_state = NONE;
            // Loop over each tile in the block and correct the opposite dimension
            for (int j = curr_block->first_tile; j <= curr_block->last_tile; j++) {
                line * opposite_line = l->is_row ? (cols + i) : (rows + i);
                setTile(opposite_line, j, NONE);
            }
        }
        curr_block = curr_block->next;
    }
    // Attempt to merge (we have to check the whole line since we updated mutiple blocks)
    for (int i = 0; i < l->block_count; i+=2) {

    }
    
    l->unknown_tiles = 0;
}