#ifndef BOARD_READER_HH
#define BOARD_READER_HH

#include <stdio.h>
#include <vector>
class BoardReader {
public:
    BoardReader();
    
    std::vector<std::vector<int>> col_clues;
    std::vector<std::vector<int>> row_clues;

private:
    FILE * clues_file;
};

#endif