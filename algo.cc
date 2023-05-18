
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
        Line state = li.first;
        std::vector<int> * clues = li.second;

        // Super hard coded clue == n
        bool progress = true;
        if (clues->size() == 1 && clues->at(0) == board->size) {
            board->setTileRange(state, std::make_pair(0, board->size), FILLED);
        } else {
            progress = false;
        }

        queue.pop();
        // Move line to back if no progress is made to wait for more clues
        if (!progress) {
            queue.push(li);
        }

        step_limit--;
        if (step_limit == 0) {
            printf("Step limit exceeded! Stopping calculations...\n");
            break;
        }
    }
}