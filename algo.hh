
#ifndef ALGO_HH
#define ALGO_HH

#include "board.hh"

class Algo {
public:
    Algo(Board * board);

    void run();

private:
    Board * board;
};

#endif