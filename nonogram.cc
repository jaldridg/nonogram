#include "nonogram.hh"
#include "board.hh"
#include "board_reader.hh"
#include "algo.hh"
#include "debug.hh"

int main() {
    Board b = Board();
    Debug::setBoard(&b);
    Algo algo = Algo(&b);
    algo.run();
    b.print();
    Debug::printLines();
    b.clear();
    return 0;
}