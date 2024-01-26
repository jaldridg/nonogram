
#include "algo.hh"
#include "board.hh"

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

        bool line_updated = false;
        if (runGrowthStrategyBeginning(l) || runGrowthStrategyEnd(l) /* || strategy 3 || strategy 4 */) {
            line_updated = true;
        } 
        
        // See if our strategies completed the line
        if (attemptLineCompletion(l)) {
            line_updated = true;
        } else {
            queue.push(l);
        }       

        if (line_updated) {
            no_solution_counter = 0;         
        } else {
            no_solution_counter++;
        }

        total_steps++;
        if (no_solution_counter == queue.size()) {
            printf("Algorithm has exhausted its techniques over %d steps!\n", total_steps);
            printf("%d lines were remaining\n", queue.size());
            return;
        }
    }
}

void Algo::runCertaintyStrategy(line * l) {
    std::vector<int> * clues = l->clues;

    // Run certainty rule on each block in a line based 
    // on the space taken by blocks before and after
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
            we can be certain that parts of the block are in some squares
        */
        if (clues->at(i) > block_size_range / 2) {
            int edge_uncertainty = block_size_range - clues->at(i);
            int lower_limit = size_before + edge_uncertainty;
            int upper_limit = size_before + block_size_range - 1 - edge_uncertainty;
            board->setTileRange(l, std::make_pair(lower_limit, upper_limit), FILLED);
        }
    }
}

// Attempt growth from start of the line
bool Algo::runGrowthStrategyBeginning(line * l) {
    if (l->unknown_tiles == 0) { return false; }

    // Traverse to the first filled block
    int i = 0;
    while (l->tiles[i] == NONE) { i++; }

    // Only works if the next cell is FILLED
    // (we haven't dealt with floating cells with unknown cells in between)
    bool can_fill = l->tiles[i] == FILLED;
    int initial_filled = l->filled_tiles;
    if (can_fill) {
        int upper_cell = i + l->clues->at(0) - 1;
        board->setTileRange(l, std::make_pair(i, upper_cell), FILLED);
        if (upper_cell + 1 < board->size) {
            board->setTileRange(l, std::make_pair(upper_cell + 1, upper_cell + 1), NONE);
        }
    }
    // Check if we actually updated something and made progress
    bool updated = l->filled_tiles - initial_filled > 0;
    return can_fill && updated;
}

// Attempt grwoth from the end of the line
bool Algo::runGrowthStrategyEnd(line * l) {
    if (l->unknown_tiles == 0) { return false; }

    // Traverse to the first filled block
    int i = board->size - 1;
    while (l->tiles[i] == NONE) { i--; }

    // Only works if the next cell is FILLED
    // (we haven't dealt with floating cells with unknown cells in between)
    bool can_fill = l->tiles[i] == FILLED;
    int initial_filled = l->filled_tiles;
    if (can_fill) {
        int lower_cell = i - (l->clues->at(l->clues->size() - 1));
        board->setTileRange(l, std::make_pair(lower_cell, i), FILLED);
        if (lower_cell - 1 >= 0) {
            board->setTileRange(l, std::make_pair(lower_cell - 1, lower_cell - 1), NONE);
        }
    }
    // Check if we actually updated something and made progress
    bool updated = l->filled_tiles - initial_filled > 0;
    return can_fill && updated;
}

bool Algo::attemptLineCompletion(line * l) {
    // Find the total cells which should be filled by clues
    int tile_total = 0;
    for (int i = 0; i < l->clues->size(); i++) {
        tile_total += l->clues->at(i);
    }

    // We know we're done if we've filled the number of tiles given by clues
    bool done = tile_total == l->filled_tiles;
    if (done) { board->completeLine(l); }
    return done;
}