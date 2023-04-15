
#include "nonogram.hh"
#include "board.hh"
#include "algo.hh"

#include <stdio.h>

int main() {
    Board b = Board(5);
    Algo algo = Algo(&b);
    algo.run();
    b.print();
    b.clear();
    return 0;
}