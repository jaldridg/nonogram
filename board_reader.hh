#ifndef BOARD_READER_HH
#define BOARD_READER_HH

#include <stdio.h>

class BoardReader {
public:
    BoardReader();

private:
    FILE * clues_file;
};

#endif