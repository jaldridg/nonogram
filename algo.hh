
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

    // A queue of the pointers of the rows/cols which need to be analyzed next
    std::queue<line *> queue;

    void runCertaintyStrategy(line * l);

    void findBlockClues(line * l);

    bool runGrowthStrategyBeginning(line * l);
    bool runGrowthStrategyEnd(line * l);

    bool attemptLineCompletion(line * l);
};

#endif