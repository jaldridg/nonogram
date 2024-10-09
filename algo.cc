#include "algo.hh"
#include "board.hh"
#include "debug.hh"

#include <stdio.h>

Algo::Algo(Board * board) {
    Algo::board = board;
}

void Algo::run() {
    // Fill the queue with all rows and cols
    for(int i = 0; i < board->size; i++) {
        queue.push(board->rows + i);
        queue.push(board->cols + i);
    }

    // Do a one time certainty sweep through the puzzle
    int total_steps = queue.size();
    for (int i = 0; i < queue.size(); i++) {
        line * l = queue.front();
        runCertaintyStrategy(l);
        queue.pop();
        queue.push(l);
    }

    // Main algorithm loop
    int no_solution_counter = 0;
    while (queue.size() != 0) {
        line * l = queue.front();
        queue.pop();
        // See if our strategies completed the line
        bool line_updated = false;
                    
        if (attemptLineCompletion(l)) {
            printf("attemptLineCompletion\n");
            board->print();
            line_updated = true;
        } else {
            queue.push(l);
        }
        
        // Run strategies
        findBlockClues(l);
        if (runGrowthStrategy(l)) {
            printf("runGrowthStrategy\n");
            board->print();
            line_updated = true; 
        } 

        no_solution_counter += !line_updated;
        if (line_updated) {
            no_solution_counter = 0;
        }

        total_steps++;
        if (no_solution_counter == queue.size()) {
            printf("Algorithm has exhausted its techniques over %d steps!\n", total_steps);
            printf("%d lines remain unsolved\n", queue.size());
            return;
        }
    }
}

void Algo::findBlockClues(line * l) {
    if (l->unknown_tiles == 0) { return; }

    /*
    Check if there's only one clue
	If so all filled blocks belong to that clue

    Loop over blocks - 
        Keeping track of the following
            Prev block if there is one
            The lowest and highest possible clue number based on if the clues can fit in the space before or after the current block
        First thing to check for each block
                If the block in question is smaller than only one clue - it must be the large clue
        You can determine that two blocks belong to separate clues if
            The connection of two blocks would exceed the max clue size
            There's a none block between the two blocks
    */

    // Only one clue -> set all blocks to that clue
    block * curr_block = l->block_head;
    if (l->clues->size() == 1) {
        while (curr_block) {
            if (curr_block->tile_state == FILLED) {
                curr_block->belongs_to = 0;
            }
            curr_block = curr_block->next;
        }
        return;
    }

    curr_block = l->block_head;
    block * prev_filled_block = NULL;
    int prev_first_possible_clue = -1;
    int prev_last_possible_clue = -1;
    bool none_block_between = false;
    /*
    // TODO: Use these variables to check: "If the block in question is smaller than only one clue - it must be the large clue"
    int largest_unmatched_clue_index = 
    int second_largest_unmatched_clue_index = 
    */
    // 
    while (curr_block) {
        if (curr_block->tile_state != FILLED) {
            if (curr_block->tile_state == NONE) {
                none_block_between = true;
            }
            curr_block = curr_block->next;
            continue;
        }
        /*
        if (block is smaller than only only one clue) {
            block must belong to large clue
        }
        */

        /* Keep track of lowest and highest possible clue based on wheter clues can fit in the spots before after the current block */

        int clue_length_before = 0;
        int last_possible_clue = l->clues->size() - 1;
        int space_before = curr_block->first_tile;
        for (int i = 0; i < l->clues->size(); i++) {
            clue_length_before += l->clues->at(i) + 1;
            // Stop if clues cannot fit in the space before
            if (clue_length_before > space_before) {
                last_possible_clue = i;
                break;
            }
        }
        int clue_length_after = 0;
        int first_possible_clue = 0;
        int space_after = board->size - curr_block->last_tile - 1;
        for (int i = l->clues->size() - 1; i >= 0; i--) {
            clue_length_after += l->clues->at(i) + 1;
            // Stop if clues cannot fit in the space after
            if (clue_length_after > space_after) {
                first_possible_clue = i;
                break;
            }
        }

        // TODO: Remove after debugging
        if(first_possible_clue > last_possible_clue) {
            float i = 0 / 0; // doing it this way so I dont have to import assert.h
        }

        // You know the block's clue if the range is only one number
        if (first_possible_clue == last_possible_clue) {
            // See if we can figure out the previous block's clue
            if (prev_filled_block && prev_first_possible_clue + 1 == last_possible_clue) {
                // Only apply rule if the blocks can't possibly belong to the same
                if (false /* determine ^^ */) {
                    prev_filled_block->belongs_to = prev_first_possible_clue;
                }
            }
            curr_block->belongs_to = first_possible_clue;
            prev_filled_block = curr_block;
            prev_first_possible_clue = first_possible_clue;
            prev_last_possible_clue = last_possible_clue;
            curr_block = curr_block->next;
            none_block_between = false;
            continue;
        }


        /*
        You can determine that two blocks belong to separate clues if
            The connection of two blocks would exceed the max clue size
            There's a none block between the two blocks
        */
        if (prev_filled_block == NULL) {
            prev_filled_block = curr_block;
            prev_first_possible_clue = first_possible_clue;
            prev_last_possible_clue = last_possible_clue;
            curr_block = curr_block->next;
            none_block_between = false;
            continue;
        }
        if (last_possible_clue - first_possible_clue == 1) {
            // For now only worry about case when current block has two possible clues to choose from - x and y
            // This makes the prev block be clue x and the second block be clue y
            if ((first_possible_clue == prev_first_possible_clue) && (last_possible_clue == prev_last_possible_clue)) {
                // If there's a known NONE block between, the blocks cannot be connected
                if (none_block_between) {
                    prev_filled_block->belongs_to = first_possible_clue;
                    curr_block->belongs_to = last_possible_clue;
                }
                // TODO: If connecting the blocks would be too long, the blocks cannot be connected
                /*
                if (connecting the blocks would be too long) {
                    prev_filled_block->belongs_to = first_possible_clue;
                    curr_block->belongs_to = last_possible_clue;
                }
                */
            }
        }

        prev_filled_block = curr_block;
        prev_first_possible_clue = first_possible_clue;
        prev_last_possible_clue = last_possible_clue;
        curr_block = curr_block->next;
        none_block_between = false;
    }

}

void Algo::runCertaintyStrategy(line * l) {
    std::vector<int> * clues = l->clues;

    // Run certainty rule on each block in a line based...
    // ...on the space taken by blocks before and after
    for (int i = 0; i < clues->size(); i++) {
        // Calculate the size of blocks before current block
        int size_before = 0;
        for (int j = 0; j < i; j++) {
            // Add size plus one for the X between blocks
            size_before += clues->at(j) + 1;
        }

        // Calculate the size of blocks above current block
        int size_after = 0;
        for (int j = i + 1; j < clues->size(); j++) {
            // Add size plus one for the X between blocks
            size_after += clues->at(j) + 1;
        }

        // The size that the block must be in
        int block_size_range = board->size - size_before - size_after;

        /* 
        Certainty Rule:
            If the size of the block is at least half the size of the range,
            we can be certain that parts of the block are in some cells
        */
        if (clues->at(i) > block_size_range / 2) {
            int edge_uncertainty = block_size_range - clues->at(i);
            int lower_limit = size_before + edge_uncertainty;
            int upper_limit = size_before + block_size_range - 1 - edge_uncertainty;
            board->setTileRange(l, lower_limit, upper_limit, FILLED);
        }
    }
}

bool Algo::runGrowthStrategy(line * l) {
    if (l->unknown_tiles == 0) { return false; }

    int initial_unknown = l->unknown_tiles;
    block * curr_block = l->block_head;
    block * prev_known_block = curr_block->prev;
    while (curr_block) {
        // Skip if there's no information about the block
        if (curr_block->belongs_to == -1) {
            curr_block = curr_block->next;
            continue;
        }

        int clue_length = l->clues->at(curr_block->belongs_to);

        // Connect blocks which belong to the same clue
        if (prev_known_block && prev_known_block->belongs_to == curr_block->belongs_to) {
            board->setTileRange(l, prev_known_block->first_tile, curr_block->last_tile, FILLED);
        }

        // See if block is already the right length and add Xs to "cap" it off
        if (curr_block->block_length == clue_length) {
            // Try to put an X before block
            if (curr_block->first_tile > 0) {
                int none_tile_index = curr_block->first_tile - 1;
                board->setTile(l, none_tile_index, NONE);
                line * opposite_line = l->is_row ? (board->cols + none_tile_index) : (board->rows + none_tile_index);
                board->setTile(opposite_line, l->line_number, NONE);
            }
            // Try to pat an X after block
            if (curr_block->last_tile < board->size - 1) {
                int none_tile_index = curr_block->last_tile + 1;
                board->setTile(l, none_tile_index, NONE);
                line * opposite_line = l->is_row ? (board->cols + none_tile_index) : (board->rows + none_tile_index);
                board->setTile(opposite_line, l->line_number, NONE);
            }
        }
        
        prev_known_block = curr_block;
        curr_block = curr_block->next;
    }

    bool updated = initial_unknown - l->unknown_tiles > 0;
    return updated;
}

bool Algo::attemptLineCompletion(line * l) {
    // Find the total cells which should be filled by clues
    int tile_total = 0;
    for (int i = 0; i < l->clues->size(); i++) {
        tile_total += l->clues->at(i);
    }

    // We know we're done if we've filled the number of tiles given by clues
    bool done = tile_total == l->filled_tiles;
    if (done) { 
        board->completeLine(l);
    }
    return done;
}