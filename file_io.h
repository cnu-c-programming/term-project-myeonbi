// file_io.h
#ifndef FILE_IO_H
#define FILE_IO_H

#include "student.h"

int loadCSV(const char* filename, Student** head);
int saveCSV(const char* filename, Student* head);

#endif