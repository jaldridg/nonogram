
#ifndef ALGO_HH
#define ALGO_HH

#include "board.hh"

class Algo {
public:
    Algo(Board * board);

    void run();

private:
    Board * board;

    typedef std::pair<Line *, std::vector<int> *> lineinfo;

    // A queue of the pointers of the rows/cols which need to be analyzed next
    std::vector<lineinfo *> queue;
};

#endif