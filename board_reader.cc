#include "board_reader.hh"

#include <iostream>
#include <fstream>
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
        // Stop: end of columns
        if (buffer[i] == 10) {
            clues.push_back(atoi(clue_buffer));
            col_clues.push_back(clues);
            j = 0;
            break; 

        // End of column
        } else if (buffer[i] == ' ') {
            clues.push_back(atoi(clue_buffer));
            col_clues.push_back(clues);
            clues = {};
            j = 0;

        // End of clue
        } else if (buffer[i] == ',') {
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
    clues = {};
    clue_buffer[0] = '\0';
    clue_buffer[1] = '\0';
    clue_buffer[2] = '\0';
    while (true) {
        // Stop: end of rows
        if (buffer[i] == '.') {
            clues.push_back(atoi(clue_buffer));
            row_clues.push_back(clues);
            break; 

        // End of row
        } else if (buffer[i] == 10) {
            clues.push_back(atoi(clue_buffer));
            row_clues.push_back(clues);
            clues = {};
            j = 0;

        // End of clue
        } else if (buffer[i] == ',') {
            clues.push_back(atoi(clue_buffer));
            j = 0;

        // Must be a number
        } else {
            clue_buffer[j] = buffer[i];
            j++;
        }
        i++;
    }

    
    printf("clue size: %d\n", col_clues.size());
    printf("clues:\n");
    for (int i = 0; i < col_clues.size(); i++) {
        for (int j = 0; j < col_clues.at(i).size(); j++) {
            printf("%d ", col_clues.at(i).at(j));
        }
        printf("\n");
    }

    
    printf("clue size: %d\n", row_clues.size());
    printf("clues:\n");
    for (int i = 0; i < row_clues.size(); i++) {
        for (int j = 0; j < row_clues.at(i).size(); j++) {
            printf("%d ", row_clues.at(i).at(j));
        }
        printf("\n");
    }
}