#include "board.hh"
#include "debug.hh"

#include <assert.h>
#include <stdio.h>

Board * Debug::board;

void Debug::setBoard(Board * b) {
    board = b;
}

void Debug::printLines() {
    // Horizontal lines
    printf("HORIZONTAL LINES\n");
    for (int i = 0; i < board->size; i++) {
        line l = board->rows[i];
        printf("Line #%d:\n", i + 1);
        block * b = l.block_head;
        do {
            int block_number = (int) (b - board->blocks);
            printf("\tBlock #%d: (%x) - %d\n", block_number + 1, b, b->block_length);
            printf("\t\t%d [%c] tiles ranging from tiles %d to %d\n", b->block_length, b->tile_state, b->first_tile, b->last_tile);
            printf("\t\tprev: %x\t\tnext: %x\n", b->prev, b->next);
            b = b->next;
        } while (b);
    }

    // Vertical lines
    printf("\nVERTICAL LINES\n");
    for (int i = 0; i < board->size; i++) {
        line l = board->cols[i];
        printf("Line #%d:\n", i + 1);
        block * b = l.block_head;
        do {
            int block_number = (int) (b - board->blocks);
            printf("\tBlock #%d: (%x) - %d\n", block_number + 1, b, b->block_length);
            printf("\t\t%d [%c] tiles ranging from tiles %d to %d\n", b->block_length, b->tile_state, b->first_tile, b->last_tile);
            printf("\t\tprev: %x\t\tnext: %x\n", b->prev, b->next);
            b = b->next;
        } while (b);
    }
}

void Debug::printBlocks() {
    for (int i = 0; i < board->size * board->size * 2; i++) {
        block b = board->blocks[i];
        printf("Block #%d (%x) - %d\n", i + 1, &(board->blocks[i]), b.block_length);
        printf("\t%d [%c] tiles ranging from tiles %d to %d\n", b.block_length, b.tile_state, b.first_tile, b.last_tile);
        printf("\tprev: %x\t\tnext: %x\n", b.prev, b.next);
    }
    printAvailableBlocks();
}

void Debug::printAvailableBlocks() {
    printf("Avilable blocks: ");
    for (int i = 0; i < board->open_indices.size(); i++) {
        printf("%d ", board->open_indices.at(i) + 1);
    }
    printf("\n");
}

void Debug::checkBlocks() {
    for (int i = 0; i < board->size; i++) {
        line l = board->rows[i];
        block * b = l.block_head;
        int total_count = 0;
        do {
            // Make sure count matches the range presented
            assert(b->last_tile - b->first_tile + 1 == b->block_length);
            // Make sure there are no loops
            assert(b->prev != b);
            assert(b->next != b);
            // Make sure first <= last
            assert(b->first_tile <= b->last_tile);
            total_count += b->block_length;
            // Make sure the next block comes right after the current one
            if (b->next) {
                assert(b->next->first_tile == b->last_tile + 1);
            }
            b = b->next;
        } while (b);
        // Make sure block total count is the size of board
        assert(total_count == board->size);
    }
    for (int i = 0; i < board->size; i++) {
        line l = board->cols[i];
        block * b = l.block_head;
        int total_count = 0;
        do {
            // Make sure count matches the range presented
            assert(b->last_tile - b->first_tile + 1 == b->block_length);
            // Make sure there are no loops
            assert(b->prev != b);
            assert(b->next != b);
            total_count += b->block_length;
            // Make sure the next block comes right after the current one
            if (b->next) {
                assert(b->next->first_tile == b->last_tile + 1);
            }
            b = b->next;
        } while (b);
        // Make sure block total count is the size of board
        assert(total_count == board->size);
    }
}