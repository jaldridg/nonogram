
#include "algo.hh"
#include "board.hh"

#include <stdio.h>

Algo::Algo(Board * board) {
    Algo::board = board;
}

void Algo::run() {
    // Fill the queue with all rows and cols
    for(int i = 0; i < board->size; i++) {
        lineinfo row_li = lineinfo(std::make_pair(board->rows[i], board->row_clues[i]));
        queue.push(row_li);
        lineinfo col_li = lineinfo(std::make_pair(board->cols[i], board->col_clues[i]));
        queue.push(col_li);
    }
    
    int step_limit = 1000; 
    while (queue.size() != 0) {
        lineinfo li = queue.front();
        runCertaintyRules(li);

        queue.pop();
        // Move line to back if line is still imcomplete to wait for more clues
        if (!board->isLineSolved(li.first)) {
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
    Line state = li.first;
    std::vector<int> * clues = li.second;

    // Level 1: One block of size / 2 + 1 is in the line
    if (clues->size() == 1 && clues->at(0) > board->size / 2) {
        int edge_uncertainty = board->size - clues->at(0);
        int lower_limit = edge_uncertainty;
        int upper_limit = board->size - 1 - edge_uncertainty;
        board->setTileRange(state, std::make_pair(lower_limit, upper_limit), FILLED);
    }
}