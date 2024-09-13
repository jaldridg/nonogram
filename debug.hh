#ifndef DEBUG_HH
#define DEBUG_HH

#include "board.hh"

class Debug {
    public:
        Debug(Board  * board);

        void printLines();

        void printBlocks();

        void printAvailableBlocks();

        void checkBlocks();

    private:
        Board * board;
};

#endif