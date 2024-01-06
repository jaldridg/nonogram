#include "nonogram.hh"
#include "board.hh"
#include "board_reader.hh"
#include "algo.hh"

int main() {
    Board b = Board();
    Algo algo = Algo(&b);
    algo.run();
    b.print();
    b.clear();
    return 0;
}