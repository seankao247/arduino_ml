#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void createNewFile(FILE **currentFile, int *fileIndex) {
    char fileName[300];
    sprintf(fileName, "C:\\Users\\seank\\Desktop\\neatlab用\\arduino_ml\\finalversion\\ARDUINO_ML\\my_project\\demo2\\data%d.txt", *fileIndex);
    *currentFile = fopen(fileName, "w");
    (*fileIndex)++;
}

int main() {
    FILE *inputFile = fopen("C:\\Users\\seank\\Desktop\\neatlab用\\arduino_ml\\final_version\\ARDUINO_ML\\my_project\\demo2\\log.txt", "r");
    if (inputFile == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    char line[256];
    int fileIndex = 1;
    FILE *currentFile = NULL;

    while (fgets(line, sizeof(line), inputFile)) {
        if (strncmp(line, "====================", 20) == 0) {
            if (currentFile != NULL) {
                fclose(currentFile);
            }
            createNewFile(&currentFile, &fileIndex);
        } else {
            fprintf(currentFile, "%s", line);
        }
    }

    if (currentFile != NULL) {
        fclose(currentFile);
    }

    fclose(inputFile);

    printf("Files created successfully.\n");

    return 0;
}



