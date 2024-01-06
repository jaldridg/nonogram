#ifndef BOARD_READER_HH
#define BOARD_READER_HH

#include <stdio.h>
#include <vector>

typedef std::vector<std::vector<int>> clues;

class BoardReader {
public:
    BoardReader();

private:
    FILE * clues_file;

    clues col_clues;
    clues row_clues;
};

#endif