#ifndef DEBUG_HH
#define DEBUG_HH

#include "board.hh"

class Debug {
    public:
        int tempVariable1 = 0;
        int tempVariable2 = 0;
        int tempVariable3 = 0;

        Debug(Board  * board);

        void printLines();

        void printBlocks();

        void printAvailableBlocks();

        void checkBlocks();

    private:
        Board * board;
};

#endif