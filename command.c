// command.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "command.h"

static ShellResult handle_find(char* args, Student** head);
static ShellResult handle_list(char* args, Student** head);
static ShellResult handle_stats(char* args, Student** head);
static ShellResult handle_help(char* args, Student** head);
static ShellResult handle_clear(char* args, Student** head);
static ShellResult handle_exit(char* args, Student** head);

static Command commands[] = {
    {"find", handle_find, "find <id>", "Find student by ID"},
    {"list", handle_list, "list", "List all students"},
    {"stats", handle_stats, "stats", "Show statistics"},
    {"help", handle_help, "help", "Show help"},
    {"clear", handle_clear, "clear", "Clear screen"},
    {"exit", handle_exit, "exit", "Exit shell"}
};

static int commandCount(void) {
    return sizeof(commands) / sizeof(commands[0]);
}

static char* skipSpaces(char* str) {
    while (str != NULL && *str != '\0' && isspace((unsigned char)*str)) {
        str++;
    }

    return str;
}

static int parseInt(const char* str, int* value) {
    char* end;

    if (str == NULL || value == NULL) {
        return 0;
    }

    while (isspace((unsigned char)*str)) {
        str++;
    }

    if (*str == '\0') {
        return 0;
    }

    long result = strtol(str, &end, 10);

    while (isspace((unsigned char)*end)) {
        end++;
    }

    if (*end != '\0') {
        return 0;
    }

    *value = (int)result;
    return 1;
}

ShellResult executeCommand(char* input, Student** head) {
    if (input == NULL) {
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    input[strcspn(input, "\n")] = '\0';

    char* commandName = strtok(input, " \t");

    if (commandName == NULL) {
        return SHELL_OK;
    }

    char* args = strtok(NULL, "");
    args = skipSpaces(args);

    for (int i = 0; i < commandCount(); i++) {
        if (strcmp(commandName, commands[i].name) == 0) {
            return commands[i].handler(args, head);
        }
    }

    printf("Unknown command or permission denied.\n");
    return SHELL_ERR_UNKNOWN_COMMAND;
}

static ShellResult handle_find(char* args, Student** head) {
    int id;

    if (parseInt(args, &id) == 0 || id <= 0) {
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    Student* target = findStudent(*head, id);

    if (target == NULL) {
        printf("Error: student not found.\n");
        return SHELL_ERR_STUDENT_NOT_FOUND;
    }

    printf("ID: %d\n", target->id);
    printf("Name: %s\n", target->name);
    printf("Score: %d\n", target->score);

    return SHELL_OK;
}

static ShellResult handle_list(char* args, Student** head) {
    (void)args;

    if (head == NULL || *head == NULL) {
        printf("No students found.\n");
        return SHELL_OK;
    }

    printf("ID Name Score\n");

    Student* current = *head;

    while (current != NULL) {
        printf("%d %s %d\n", current->id, current->name, current->score);
        current = current->next;
    }

    return SHELL_OK;
}

static ShellResult handle_stats(char* args, Student** head) {
    (void)args;

    if (head == NULL || *head == NULL) {
        printf("No student data available.\n");
        return SHELL_OK;
    }

    int count = 0;
    int sum = 0;
    int max = (*head)->score;
    int min = (*head)->score;

    Student* current = *head;

    while (current != NULL) {
        count++;
        sum += current->score;

        if (current->score > max) {
            max = current->score;
        }

        if (current->score < min) {
            min = current->score;
        }

        current = current->next;
    }

    printf("Count: %d\n", count);
    printf("Average: %.1f\n", (double)sum / count);
    printf("Max: %d\n", max);
    printf("Min: %d\n", min);

    return SHELL_OK;
}

static ShellResult handle_help(char* args, Student** head) {
    (void)args;
    (void)head;

    printf("Commands:\n");

    for (int i = 0; i < commandCount(); i++) {
        printf("%s %s\n", commands[i].usage, commands[i].description);
    }

    return SHELL_OK;
}

static ShellResult handle_clear(char* args, Student** head) {
    (void)args;
    (void)head;

    printf("\033[2J\033[H");

    return SHELL_OK;
}

static ShellResult handle_exit(char* args, Student** head) {
    (void)args;
    (void)head;

    printf("Goodbye.\n");

    return SHELL_EXIT;
}