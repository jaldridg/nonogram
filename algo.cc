
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
        queue.push_back(row_li);
        lineinfo col_li = lineinfo(std::make_pair(board->cols[i], board->col_clues[i]));
        queue.push_back(col_li);
    }
    while (queue.size() != 0) {
        lineinfo li = queue.back();
        Line state = li.first;
        std::vector<int> * clues = li.second;
        printf("state loc: %d\n", state);
        // Super hard coded clue == n
        if (clues->size() == 1 && clues->at(0) == board->size) {
            board->setTileRange(state, std::make_pair(0, board->size), FILLED);
        }
        queue.pop_back();
    }
}