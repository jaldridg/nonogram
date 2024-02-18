#ifndef BOARD_HH
#define BOARD_HH

#include <vector>

enum Tilestate {
    UNKNOWN = 32,
    NONE = 88,
    FILLED = 510
    //COL_ONE = 65,
    //COL_TWO = 66,
    //COL_THREE = 67
};
typedef Tilestate * Tiles;

// Doubly linked list style
struct block {
    int first_tile; // The index of the first tile in the block
    int last_tile;  // The index of the last tile in the block
    int block_length;
    Tilestate tile_state;
    block * prev;
    block * next;
};
typedef std::vector<block> * Blocks;

typedef std::vector<int> * Clues;

struct line {
    Blocks blocks;
    Tiles tiles;
    Clues clues;
    int unknown_tiles;
    int filled_tiles;
    int line_number; // Lower towards the top left corner
    bool is_row;
};

class Board {
public:
    int size;

    line * rows;
    line * cols;

    Board();
    // The destructor since ~Board is being weird
    void clear();

    void print();

    void setTileRange(line * line, std::pair<int, int> ids, Tilestate state);

    void completeLine(line * line);

    int getClue(std::vector<Clues> * clues, int num, int pos) {
        return clues->at(num)->at(pos);
    }

    void setClue(std::vector<Clues> * clues, int num, int pos, int clue) {
        clues->at(num)->at(pos) = clue;
    }

private:

};

#endif