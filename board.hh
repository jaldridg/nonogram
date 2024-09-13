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

typedef std::vector<int> * Clues;

struct line {
    block * block_head;
    block * block_tail;
    int block_count;
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

    // Stores all of our blocks for easy global access
    block * blocks;
    int num_blocks;
    std::vector<int> open_indices;

    Board();
    // The destructor since ~Board is being weird
    void clear();

    void print();

    void deleteBlock(block * b, line * l);

    void splitBlock(block * b, line * l, int lower_mask_index, int upper_mask_index);

    void mergeBlock(block * block, line * line);

    void setTile(line * line, int index, Tilestate state);

    void setTileRange(line * line, int start_index, int stop_index, Tilestate state);

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