
#include "algo.hh"
#include "board.hh"

#include <stdio.h>

Algo::Algo(Board * board) {
    Algo::board = board;
}

void Algo::run() {
    // Fill the queue with all rows and cols
    for(int i = 0; i < board->size; i++) {
        lineinfo row_li;
        row_li.line = board->rows[i];
        row_li.clues = board->row_clues[i];
        queue.push(row_li);
        lineinfo col_li;
        col_li.line = board->cols[i];
        col_li.clues = board->col_clues[i];
        queue.push(col_li);
    }
    
    int step_limit = 1000; 
    while (queue.size() != 0) {
        lineinfo li = queue.front();
        runCertaintyRules(li);

        queue.pop();
        // Move line to back if line is still imcomplete to wait for more clues
        if (!board->isLineSolved(li.line)) {
            queue.push(li);
        }

        step_limit--;
        if (step_limit == 0) {
            printf("Step limit met! Stopping calculations...\n");
            printf("%d lines were remaining\n", queue.size());
            return;
        }
    }
}

void Algo::runCertaintyRules(lineinfo li) {
    std::vector<int> * clues = li.clues;

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
        // If the size of the block is at least half the size of the range,
        // we can be certain that parts of the block are in some squares
        if (clues->at(i) > block_size_range / 2) {
            int edge_uncertainty = block_size_range - clues->at(i);
            int lower_limit = size_before + edge_uncertainty;
            int upper_limit = size_before + block_size_range - 1 - edge_uncertainty;
            board->setTileRange(li.line, std::make_pair(lower_limit, upper_limit), FILLED);
        }
    }
    /*
    // Level 1: One block of over (size / 2) is in the line
    if (clues->size() == 1 && clues->at(0) > board->size / 2) {
        int edge_uncertainty = board->size - clues->at(0);
        int lower_limit = edge_uncertainty;
        int upper_limit = board->size - 1 - edge_uncertainty;
        board->setTileRange(li.line, std::make_pair(lower_limit, upper_limit), FILLED);
    }
    */
}