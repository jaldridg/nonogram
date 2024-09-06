
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
    num_blocks = 0;

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
        block initial_row_block = block { 0, size - 1, size, UNKNOWN, NULL, NULL };
        block initial_col_block = block { 0, size - 1, size, UNKNOWN, NULL, NULL };
        block * initial_row_block_ptr = blocks + num_blocks;
        block * initial_col_block_ptr = blocks + num_blocks + 1;
        blocks[num_blocks++] = initial_row_block;
        blocks[num_blocks++] = initial_col_block;
        rows[i].block_head = initial_row_block_ptr;
        rows[i].block_tail = initial_row_block_ptr;
        cols[i].block_head = initial_col_block_ptr;
        cols[i].block_tail = initial_col_block_ptr;
        rows[i].block_count = 1;
        cols[i].block_count = 1;

        // Initilize tiles
        rows[i].tiles = new Tilestate[size];
        cols[i].tiles = new Tilestate[size];
        for (int j = 0; j < size; j++) {
            rows[i].tiles[j]= UNKNOWN;
            cols[i].tiles[j] = UNKNOWN;
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
        block * curr_block = rows[i].block_head;
        while (curr_block) {
            Tilestate t = curr_block->tile_state;
            for (int k = 0; k < curr_block->block_length; k++) { printf("%c ", t); }
            curr_block = curr_block->next;
        }
        
       /*
        for (int j = 0; j < size; j++) {
            printf("%c ", rows[i].tiles[j]);
        }
        */
        printf("\n");
    }
}

void Board::deleteBlock(block * b) {
    // Update pointers of previous and next blocks
    if (b->prev) {
        block * prev_block = b->prev;
        prev_block->next = b->next;
    }
    if (b->next) {
        block * next_block = b->next;
        next_block->prev = b->prev;
    }

    // Move last block to open space
    int new_index = (b - blocks) / sizeof(block);
    blocks[new_index] = blocks[num_blocks - 1];

    num_blocks--;
}

// Splits a block as if the mask indecies are a block which is being cut out of the given block 
void Board::splitBlock(block * b, line * l, int lower_mask_index, int upper_mask_index) {
    assert(lower_mask_index <= upper_mask_index);

    // Make a new block which comes before the current one
    if (lower_mask_index > b->first_tile) {
        // Make new block
        int first = b->first_tile;
        int last = lower_mask_index - 1;
        int length = last - first + 1;
        Tilestate ts = b->tile_state;
        block * prev = b->prev;
        block * next = b;
        block split_block = block { first, last, length, ts, prev, next };
        
        // Modify current block to link to new block
        b->first_tile = lower_mask_index;
        b->block_length -= length;
        b->prev = blocks + num_blocks;
        // Set new block head if necessary
        if (!split_block.prev) {
            l->block_head = blocks + num_blocks;
        }

        blocks[num_blocks++] = split_block;
        l->block_count++;

    }
    // Make a new block which comes after the current one
    if (upper_mask_index < b->last_tile) {
        // Make new block
        int first = upper_mask_index + 1;
        int last = b->last_tile;
        int length = last - first + 1;
        Tilestate ts = b->tile_state;
        block * prev = b;
        block * next = b->next;
        block split_block = block { first, last, length, ts, prev, next };
        
        // Modify current block to link to new block
        b->last_tile = upper_mask_index;
        b->block_length -= length;
        b->next = blocks + num_blocks;
        // Set new block tail if necessary
        if (!split_block.next) {
            l->block_tail = blocks + num_blocks;
        }

        blocks[num_blocks++] = split_block;
        l->block_count++;
    }
}

void Board::mergeBlock(block * b, line * l) {
    // Get compatible blocks before this block
    int first_index = b->first_tile;
    block * first_block = b;
    while (first_block->prev) {
        // Stop if blocks are different types
        if (first_block->tile_state != first_block->prev->tile_state) {
            first_index = first_block->first_tile;
            break;
        }
        // Grow block to encompass the next block
        first_block = first_block->prev;
        first_block->block_length += first_block->next->block_length;
        first_block->last_tile = first_block->next->last_tile;
        // Set tail if the merged block was the tail
        if (first_block->next == l->block_tail) {
            l->block_tail = first_block;
        }

        deleteBlock(first_block->next);
        l->block_count--;
    }
    // Get compatible blocks after this block
    // Since we've been removing blocks, we start at before_block->next
    block * last_block = first_block;
    while (last_block->next) {
        // Stop if blocks are different types
        if (last_block->tile_state != last_block->next->tile_state) {
            break; 
        }
        // Grow block to encompass the prevous block
        last_block = last_block->next;
        last_block->block_length += last_block->prev->block_length;
        last_block->first_tile = last_block->prev->first_tile;
        // Set head if the merged block was the head
        if (last_block->prev == l->block_head) {
            l->block_head = last_block;
        }

        deleteBlock(last_block->prev);
        l->block_count--;
    }

    // If we merged the first block, we need to update the head
    if (first_index == 0) {
        l->block_head = last_block;
    }
    // Last block should now encompass all of the blocks that were removed
    last_block->first_tile = first_index;
}

// Sets a tile to a given tilestate by splitting blocks if necessary
// Note: This does not correct the opposite dimension but should be used as a helper
void Board::setTile(line * l, int index, Tilestate state) {
    assert(index < size);

    // Find index by looping over blocks
    block * curr_block = l->block_head;
    while (index > curr_block->last_tile) {
        curr_block = curr_block->next;
    }

    splitBlock(curr_block, l, index, index);
    curr_block->tile_state = state;
    mergeBlock(curr_block, l);
    l->unknown_tiles--;
}

void Board::printLines() {
    // Horizontal lines
    printf("HORIZONTAL LINES\n");
    for (int i = 0; i < size; i++) {
        line l = rows[i];
        printf("Line #%d:\n", i + 1);
        block * b = l.block_head;
        do {
            int block_number = (int) (b - blocks);
            printf("\tBlock #%d:(%x)\n", block_number + 1, b);
            printf("\t\t%d [%c] tiles ranging from tiles %d to %d\n", b->block_length, b->tile_state, b->first_tile, b->last_tile);
            printf("\t\tprev: %x\t\tnext: %x\n", b->prev, b->next);
            b = b->next;
        } while (b);
    }

    // Vertical lines
    printf("\nVERTICAL LINES\n");
    for (int i = 0; i < size; i++) {
        line l = cols[i];
        printf("Line #%d:\n", i + 1);
        block * b = l.block_head;
        do {
            int block_number = (int) (b - blocks);
            printf("\tBlock #%d:(%x)\n", block_number + 1, b);
            printf("\t\t%d [%c] tiles ranging from tiles %d to %d\n", b->block_length, b->tile_state, b->first_tile, b->last_tile);
            printf("\t\tprev: %x\t\tnext: %x\n", b->prev, b->next);
            b = b->next;
        } while (b);
    }
    printf("\n\n");
}

void Board::printBlocks() {
    printf("Printing blocks...\n");
    for (int i = 0; i < size * size * 2; i++) {
        block b = blocks[i];
        printf("Block #%d (%x)\n", i + 1, &blocks[i]);
        printf("\t%d [%c] tiles ranging from tiles %d to %d\n", b.block_length, b.tile_state, b.first_tile, b.last_tile);
        printf("\tprev: %x\t\tnext: %x\n", b.prev, b.next);
    }
    printf("\n");
}

// Fills the line with the given state using the limits
// Starting index and stopping index are inclusive
void Board::setTileRange(line * l, int start_index, int stop_index, Tilestate state) {
    assert(start_index <= stop_index);

    // TILE APPROACH
    /*
    for (int i = start_index; i <= stop_index; i++) {
        struct line opposite_line = l->is_row ? cols[i] : rows[i];
        if (opposite_line.tiles[l->line_number] != state) {
            opposite_line.tiles[l->line_number] = state;
            opposite_line.unknown_tiles--;
            if (state == FILLED) { opposite_line.filled_tiles++; }
        }
    }

    // Set the range in the line
    int diff_count = 0;
    for (int i = start_index; i <= stop_index; i++) {
        if (l->tiles[i] != state) {
            l->tiles[i] = state;
            diff_count++;
        }
    }
    l->unknown_tiles -= diff_count;
    if (state == FILLED) { l->filled_tiles += diff_count; }
    */

   // Find the first block which intersects with the indices we're interested in

    // Try to split all blocks in the line
    block * curr_block = l->block_head;
    while (curr_block) {
        splitBlock(curr_block, l, start_index, stop_index);
        curr_block = curr_block->next;
    }

    // Blocks now line up with indices
    curr_block = l->block_head;
    while (curr_block) {
        // Blocks above start_index
        if (curr_block->first_tile >= start_index) {
            // Blocks below stop_index
            if (curr_block->last_tile <= stop_index) {
                curr_block->tile_state = state;
                // Fix the blocks in the opposite line
                for (int i = curr_block->first_tile; i <= curr_block->last_tile; i++) {
                    line * opposite_line = l->is_row ? (cols + i) : (rows + i);
                    setTile(opposite_line, l->line_number, state);
                }
            } else {
                // Stop when we pass the desired range
                break;
            }
        }
        if (curr_block->next) {
            curr_block = curr_block->next;
        } else {
            break;
        }
    }
    // Handle case when there's only one block
    block * to_merge = curr_block->prev ? curr_block->prev : curr_block;
    mergeBlock(to_merge, l);
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
    // Attempt to merge by looping over the blocks
    // We have to check the whole line since we updated mutiple blocks)
    // After merging we should jump two more blocks and try to merge more
    curr_block = l->block_head;
    while (curr_block->next && curr_block->next->next) {
        curr_block = curr_block->next->next;
        mergeBlock(curr_block, l);
    }
    
    l->unknown_tiles = 0;
}