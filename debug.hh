#ifndef DEBUG_HH
#define DEBUG_HH

#include "board.hh"

class Debug {
    public:
        int tempVariable1 = 0;
        int tempVariable2 = 0;
        int tempVariable3 = 0;

        Debug(Board * board);

        static void printLines();

        static void printBlocks();

        static void printAvailableBlocks();

        static void checkBlocks();

    private:
        static Board * board;
};

#endif