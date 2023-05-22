
#ifndef ALGO_HH
#define ALGO_HH

#include "board.hh"
#include <queue>

class Algo {
public:
    Algo(Board * board);

    void run();

private:
    Board * board;
    
    struct lineinfo {
        Line line;
        Clues clues;
    };

    // A queue of the pointers of the rows/cols which need to be analyzed next
    std::queue<lineinfo> queue;

    void runCertaintyRules(lineinfo li);
};

#endif