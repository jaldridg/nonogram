
#include "algo.hh"
#include "board.hh"

#include <stdio.h>

Algo::Algo(Board * board) {
    Algo::board = board;
}

void Algo::run() {
    // Fill the queue with all rows and cols
    for(int i = 0; i < board->size; i++) {
        lineinfo * row_li = new lineinfo(std::make_pair(board->rows->at(i), board->row_clues->at(i)));
        queue.push_back(row_li);
        lineinfo * col_li = new lineinfo(std::make_pair(board->cols->at(i), board->col_clues->at(i)));
        queue.push_back(col_li);
    }
    while (queue.size() != 0) {
        lineinfo * li = queue.back();
        Line * state = li->first;
        std::vector<int> * clues = li->second;
        // Super hard coded clue == n
        if (clues->size() == 1 && clues->at(0) == board->size) {
            board->setTileRange(state, std::make_pair(0, board->size), FILLED);
        }
        queue.pop_back();
    }
}