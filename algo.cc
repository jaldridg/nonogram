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
            line_updated = true;
        } else {
            queue.push(l);
        }
        
        // Run strategies
        // if (runGrowthStrategyBeginning(l)) { line_updated = true; } 
        // if (runGrowthStrategyEnd(l)) { line_updated = true; }
            

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
    }

    curr_block = l->block_head;
    block * prev_filled_block = NULL;
    int prev_first_possible_clue = -1;
    int prev_last_possible_clue = -1;
    // The indices defining a range of the clues that are possible with the given block
    int first_possible_clue = 0;
    int last_possible_clue = l->clues->size() - 1;
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
        if(first_possible_clue <= last_possible_clue) {
            float i = 0 / 0; // doing it this way so I dont have to import assert.h
        }

        // You know the block's clue if the range is only one number
        if (first_possible_clue == last_possible_clue) {
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

// Attempt growth from start of the line
bool Algo::runGrowthStrategyBeginning(line * l) {
    if (l->unknown_tiles == 0) { return false; }
    
    // Traverse until the first unknown tile
    int i = 0;
    int filled_count = 0;
    int clue_index = 0;
    while (l->tiles[i] != UNKNOWN) {
        // Count filled tiles to see if we're running over a clue
        if (l->tiles[i] == FILLED) {
            filled_count++;
            // If we've counted all tiles in a clue, we've exhausted it so move onto the next one for counting
            if (filled_count == l->clues->at(clue_index)) { 
                clue_index++;
                filled_count = 0;

                // If the tiles are as long as the clue we need to set NONE to "cap" it off
                if (l->tiles[i + 1] == UNKNOWN) {
                    board->setTile(l, i + 1, NONE);
                    // Edge case where we complete the line by completing the NONE tile
                    if (i + 1 == board->size - 1) { return true; }
                }
            }
        }
        i++;
    }

    // Look at the tile before the unknown space
    // If filled, grow the block based on the clue's given size
    int initial_filled = l->filled_tiles;
    if (i - 1 >= 0 && l->tiles[i - 1] == FILLED) {
        int upper_cell = i + l->clues->at(clue_index) - filled_count - 1;
        board->setTileRange(l, i, upper_cell, FILLED);
        if (upper_cell + 1 <= board->size - 1) {
            board->setTileRange(l, upper_cell + 1, upper_cell + 1, NONE);
        }

    // If none (remaining case), try to find a hanging filled tile that we can grow
    } else {
        // Search for a filled tile next to our first unknown space
        int gap_length = 0;
        bool can_fill = false;
        for (int j = 0; j < l->clues->at(clue_index); j++, i++) {
            // Break if we hit a none tile because there's some weird edge cases
            if (l->tiles[i] == NONE) { break; }
            // If we find a tile, we're clear to grow it out until the clue size
            if (l->tiles[i] == FILLED) { 
                can_fill = true;
                break;
            }
            gap_length++;
        }

        if (can_fill) {
            int upper_cell = i + l->clues->at(clue_index) - gap_length - 1;
            board->setTileRange(l, i, upper_cell, FILLED);
        }
    }

    // Check if we actually updated something and made progress
    bool updated = l->filled_tiles - initial_filled > 0;
    return updated;
}

// Attempt grwoth from the end of the line
bool Algo::runGrowthStrategyEnd(line * l) {
    if (l->unknown_tiles == 0) { return false; }

    // Traverse until the first unknown tile
    int i = board->size - 1;
    int filled_count = 0;
    int clue_index = l->clues->size() - 1;
    while (l->tiles[i] != UNKNOWN) {
        // Count filled tiles to see if we're running over a clue
        if (l->tiles[i] == FILLED) { 
            filled_count++;
            // If we've counted all tiles in a clue, we've exhausted it so move onto the next one for counting
            if (filled_count == l->clues->at(clue_index)) { 
                clue_index--;
                filled_count = 0;

                // If the next cell is unknown, it must actually be NONE
                // Grow it out so we can try to grow other blocks - and avoid edge cases :)
                if (l->tiles[i - 1] == UNKNOWN) {
                    board->setTileRange(l, i - 1, i - 1, NONE);
                    // Edge case where we complete the line by completing the NONE tile
                    if (i - 1 == 0) { return true; }
                }
            }
        }
        i--;
    }

    // Look at the tile before the unknown space
    // If filled, grow the block based on the clue's given size
    int initial_filled = l->filled_tiles;
    if (i + 1 <= board->size - 1 && l->tiles[i + 1] == FILLED) {
        int lower_cell = i - (l->clues->at(clue_index)) + 1 + filled_count;
        board->setTileRange(l, lower_cell, i, FILLED);
        if (lower_cell - 1 >= 0) {
            board->setTileRange(l, lower_cell - 1, lower_cell - 1, NONE);
        }

    // If none (remaining case), try to find a hanging filled tile that we can grow
    } else {
        // Search for a filled tile next to our first unknown space
        int gap_length = 0;
        bool can_fill = false;
        for (int j = 0; j < l->clues->at(clue_index); j++, i--) {
            // Break if we hit a none tile because there's some weird edge cases
            if (l->tiles[i] == NONE) { break; }
            // If we find a tile, we're clear to grow it out until the clue size
            if (l->tiles[i] == FILLED) { 
                can_fill = true;
                break;
            }
            gap_length++;
        }

        if (can_fill) {
            int lower_cell = i - l->clues->at(clue_index) + gap_length + 1;
            board->setTileRange(l, lower_cell, i, FILLED);
        }
    }

    // Check if we actually updated something and made progress
    bool updated = l->filled_tiles - initial_filled > 0;
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