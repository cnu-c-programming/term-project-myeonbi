/*
 * main.c  –  Mini Student Shell
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "student.h"
 #include "file_io.h"
 #include "command.h"
 
 #define INPUT_SIZE 256
 
 static void print_usage(void) {
 #ifdef ADMIN_MODE
     printf("Usage: ./admin_shell <csv_file> [-f command_file]\n");
 #else
     printf("Usage: ./client_shell <csv_file> [-f command_file]\n");
 #endif
 }
 

 void run_shell(const char *csv_path) {
     Student* head = NULL;
     char input[INPUT_SIZE];
     int count;
 
     setCSVPath(csv_path);
 
     count = loadCSV(csv_path, &head);
     if (count < 0) {
         return;
     }
 
 #ifdef ADMIN_MODE
     printf("[Admin Program]\n");
     printf("Loaded %d students from %s.\n", count, csv_path);
 #else
     printf("[Client Program]\n");
     printf("Loaded %d students from %s.\n", count, csv_path);
 #endif
 
     while (1) {
 #ifdef ADMIN_MODE
         printf("admin> ");
 #else
         printf("client> ");
 #endif
 
         if (fgets(input, sizeof(input), stdin) == NULL) {
             break;
         }
 
         ShellResult result = executeCommand(input, &head);
 
         if (result == SHELL_EXIT) {
             break;
         }
     }
 
     freeStudents(head);
 }
 
 void run_command_file(const char *cmd_file, const char *csv_path) {
    Student* head = NULL;
    FILE* fp;
    char input[INPUT_SIZE];
    int lineNumber = 0;
    int count;

    setCSVPath(csv_path);

    count = loadCSV(csv_path, &head);
    if (count < 0) {
        return;
    }

#ifdef ADMIN_MODE
    printf("[Admin Program]\n");
#else
    printf("[Client Program]\n");
#endif
    printf("Loaded %d students from %s.\n", count, csv_path);

    fp = fopen(cmd_file, "r");

    if (fp == NULL) {
        printf("Error: cannot open command file.\n");
        freeStudents(head);
        return;
    }

    while (fgets(input, sizeof(input), fp) != NULL) {
        lineNumber++;

        input[strcspn(input, "\n")] = '\0';

        char* line = input;
        while (*line == ' ' || *line == '\t') {
            line++;
        }

        if (*line == '\0' || *line == '#') {
            continue;
        }

        printf("[command file:%d] %s\n", lineNumber, line);

        ShellResult result = executeCommand(line, &head);

        if (result == SHELL_EXIT) {
            break;
        }

        if (result != SHELL_OK) {
            printf("Skipped line %d.\n", lineNumber);
        }
    }

    fclose(fp);
    freeStudents(head);
}
 
 int main(int argc, char *argv[]) {
     const char *csv_path = NULL;
     const char *cmd_file = NULL;
 
     if (argc < 2) {
         print_usage();
         return 1;
     }
 
     for (int i = 1; i < argc; i++) {
         if (strcmp(argv[i], "-f") == 0) {
             if (i + 1 >= argc) {
                 print_usage();
                 return 1;
             }
             cmd_file = argv[++i];
         } else {
             csv_path = argv[i];
         }
     }
 
     if (csv_path == NULL) {
         print_usage();
         return 1;
     }
 
     if (cmd_file != NULL) {
         run_command_file(cmd_file, csv_path);
     } else {
         run_shell(csv_path);
     }
 
     return 0;
 }