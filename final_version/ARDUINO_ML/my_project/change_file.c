//此只是為了把兩個空格的data改成一個空格
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000

void processFile(const char *inputFileName, const char *outputFileName) {
    FILE *inputFile = fopen(inputFileName, "r");
    FILE *outputFile = fopen(outputFileName, "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening files.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        char *current = line;
        char *next;

        while (*current != '\0') {
            while (*current == ' ' && *(current + 1) == ' ') {
                next = current + 1;

                while (*next != '\0') {
                    *current = *next;
                    current++;
                    next++;
                }

                *current = '\0';
                current = line;
            }

            current++;
        }

        fprintf(outputFile, "%s", line);
    }

    fclose(inputFile);
    fclose(outputFile);
}

int main() {

    //const char *inputFileName = "C:\\Users\\seank\\Desktop\\work\\ARDUINO_ML\\my_test\\train_data\\DATA3.txt";  // 輸入檔案名稱
    //const char *outputFileName = "C:\\Users\\seank\\Desktop\\work\\ARDUINO_ML\\my_test\\temp\\DATA3.txt";  // 輸出檔案名稱
    int i;
    for (i = 3; i <= 20;i++){
        char filename_a[100];
        char filename_b[100];
        sprintf(filename_a, "C:\\Users\\seank\\Desktop\\work\\ARDUINO_ML\\my_test\\train_data\\DATA%d.txt", i); //設定檔案路徑名稱
        sprintf(filename_b, "C:\\Users\\seank\\Desktop\\work\\ARDUINO_ML\\my_test\\temp\\DATA%d.txt", i); //設定檔案路徑名稱
        processFile(filename_a, filename_b);
    }
    printf("File processed successfully.\n");

    return 0;
}
