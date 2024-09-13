#include "board_reader.hh"

#include <assert.h>
#include <vector>

BoardReader::BoardReader() {
    // Read file
    clues_file = fopen("clues.txt", "r");
    if (!clues_file) {
        printf("File opening failed!");
    }

    fseek(clues_file, 0, SEEK_END);
    int length = ftell(clues_file);
    char * buffer = new char[length];

    rewind(clues_file);
    fread(buffer, sizeof(char), length, clues_file);
    fclose(clues_file);

    int i = 0;
    int j = 0;
    std::vector<int> clues = {};
    char * clue_buffer = new char[3];
    // Parse columns
    while (true) {
        // End of column
        if (buffer[i] == ' ' || buffer[i] == 10) {
            clue_buffer[j] = '\0';
            clues.push_back(atoi(clue_buffer));
            col_clues.push_back(clues);
            clues = {};
            j = 0;
            
            // End of all columns
            if (buffer[i] == 10) { 
                i++;
                break; 
            }

        // End of clue
        } else if (buffer[i] == ',') {
            clue_buffer[j] = '\0';
            clues.push_back(atoi(clue_buffer));
            j = 0;

        // Must be a number
        } else {
            clue_buffer[j] = buffer[i];
            j++;
        }
        i++;
    }

    // Parse row clues
    // Reset the buffer location which may give us problems if not reset
    clue_buffer[1] = '\0';
    while (true) {
        // End of row
        if (buffer[i] == 10 || buffer[i] == '.') {
            clue_buffer[j] = '\0';
            clues.push_back(atoi(clue_buffer));
            row_clues.push_back(clues);
            clues = {};
            j = 0;
            
            // End of all rows
            if (buffer[i] == '.') { break; }

        // End of clue
        } else if (buffer[i] == ',') {
            clue_buffer[j] = '\0';
            clues.push_back(atoi(clue_buffer));
            j = 0;

        // Must be a number
        } else {
            clue_buffer[j] = buffer[i];
            j++;
        }
        i++;
    }

    delete clue_buffer;  

    assert(row_clues.size() == col_clues.size());
}