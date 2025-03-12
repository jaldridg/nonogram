#ifndef DEBUG_HH
#define DEBUG_HH

#include "board.hh"

class Debug {
    public:
        static int tempVariable1;
        static int tempVariable2;
        static int tempVariable3;

        static void setBoard(Board * board);

        static void printLines();

        static void printBlocks();

        static void printAvailableBlocks();

        static void checkBlocks();

        static void printBlockClues();

    private:
        static Board * board;
};

#endif