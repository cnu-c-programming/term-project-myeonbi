// file_io.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "student.h"

#define LINE_SIZE 128

static void removeNewline(char* str) {
    str[strcspn(str, "\n")] = '\0';
}

int loadCSV(const char* filename, Student** head) {
    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error: cannot open CSV file.\n");
        return -1;
    }

    char line[LINE_SIZE];

    if (fgets(line, sizeof(line), fp) == NULL) {
        printf("Error: empty CSV file.\n");
        fclose(fp);
        return -1;
    }

    removeNewline(line);

    if (strcmp(line, "id,name,score") != 0) {
        printf("Error: invalid CSV header.\n");
        fclose(fp);
        return -1;
    }

    int count = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        removeNewline(line);

        if (line[0] == '\0') {
            continue;
        }

        char* idStr = strtok(line, ",");
        char* name = strtok(NULL, ",");
        char* scoreStr = strtok(NULL, ",");

        if (idStr == NULL || name == NULL || scoreStr == NULL) {
            printf("Error: invalid CSV format. Skipped line.\n");
            continue;
        }

        int id = atoi(idStr);
        int score = atoi(scoreStr);

        if (id <= 0 || name[0] == '\0' || score < 0 || score > 100) {
            printf("Error: invalid student data. Skipped line.\n");
            continue;
        }

        Student* newStudent = createStudent(id, name, score);

        if (newStudent == NULL) {
            printf("Error: memory allocation failed.\n");
            fclose(fp);
            return -1;
        }

        if (appendStudent(head, newStudent) == 0) {
            printf("Error: duplicate ID. Skipped line.\n");
            free(newStudent);
            continue;
        }

        count++;
    }

    fclose(fp);
    return count;
}

int saveCSV(const char* filename, Student* head) {
    (void)filename;
    (void)head;

    return 0;
}
