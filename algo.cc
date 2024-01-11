
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
    
    int step_limit = 100;
    while (queue.size() != 0) {
        line * l = queue.front();
        runCertaintyRules(l);

        queue.pop();
        // Move line to back if line is still incomplete to wait for more clues
        if (l->unknown_tiles > 0) {
            queue.push(l);
        }

        step_limit--;
        if (step_limit == 0) {
            printf("Step limit met! Stopping calculations...\n");
            printf("%d lines were remaining\n", queue.size());
            return;
        }
    }
}

void Algo::runCertaintyRules(line * li) {
    std::vector<int> * clues = li->clues;

    // Run certainty rules on each block in a line based 
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
        printf("i: %d, clues->at(i): %d, bsr/2: %d\n", i, clues->at(i), block_size_range / 2);
        if (clues->at(i) > block_size_range / 2) {
            int edge_uncertainty = block_size_range - clues->at(i);
            int lower_limit = size_before + edge_uncertainty;
            int upper_limit = size_before + block_size_range - 1 - edge_uncertainty;
            printf("line #%d, row?: %d, ll: %d, ul: %d\n", li->line_number, li->is_row, lower_limit, upper_limit);
            board->setTileRange(li, std::make_pair(lower_limit, upper_limit), FILLED);
        }
    }
}