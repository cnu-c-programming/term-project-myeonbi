// student.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

Student* createStudent(int id, const char* name, int score) {
    Student* newStudent = (Student*)malloc(sizeof(Student));

    if (newStudent == NULL) {
        return NULL;
    }

    newStudent->id = id;
    strncpy(newStudent->name, name, sizeof(newStudent->name) - 1);
    newStudent->name[sizeof(newStudent->name) - 1] = '\0';
    newStudent->score = score;
    newStudent->next = NULL;

    return newStudent;
}

void freeStudents(Student* head) {
    Student* current = head;

    while (current != NULL) {
        Student* next = current->next;
        free(current);
        current = next;
    }
}