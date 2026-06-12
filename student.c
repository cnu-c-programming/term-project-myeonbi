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

Student* findStudent(Student* head, int id) {
    Student* current = head;

    while (current != NULL) {
        if (current->id == id) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

int appendStudent(Student** head, Student* newStudent) {
    if (head == NULL || newStudent == NULL) {
        return 0;
    }

    if (findStudent(*head, newStudent->id) != NULL) {
        return 0;
    }

    if (*head == NULL) {
        *head = newStudent;
        return 1;
    }

    Student* current = *head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newStudent;
    return 1;
}

int deleteStudent(Student** head, int id) {
    if (head == NULL || *head == NULL) {
        return 0;
    }

    Student* current = *head;
    Student* prev = NULL;

    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }

            free(current);
            return 1;
        }

        prev = current;
        current = current->next;
    }

    return 0;
}

int updateStudent(Student* head, int id, int score) {
    Student* target = findStudent(head, id);

    if (target == NULL) {
        return 0;
    }

    target->score = score;
    return 1;
}
