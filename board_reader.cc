#include "board_reader.hh"

#include <iostream>
#include <fstream>

BoardReader::BoardReader() {
    clues_file = fopen("clues.txt", "r");
    if (!clues_file) {
        printf("File opening failed!");
    }

    fseek(clues_file, 0, SEEK_END);
    int length = ftell(clues_file);
    char * buffer = new char[length];

    rewind(clues_file);
    fread(buffer, length, sizeof(char), clues_file);

    int i = 0;
    while (buffer[i] != '.') { printf("%c - %d\n", buffer[i], buffer[i++]); }
    fclose(clues_file);
}