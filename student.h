// student.h
#ifndef STUDENT_H
#define STUDENT_H

// 학생 정보는 linked list로 관리
typedef struct Student {
    int id;
    char name[32];
    int score;
    struct Student* next;
} Student;

Student* createStudent(int id, const char* name, int score);
void freeStudents(Student* head);

Student* findStudent(Student* head, int id);
int appendStudent(Student** head, Student* newStudent);
int deleteStudent(Student** head, int id);
int updateStudent(Student* head, int id, int score);

#endif
