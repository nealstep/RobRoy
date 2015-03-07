#ifndef BASE_H
#define BASE_H

// set CPU speed
#define F_CPU 20000000UL

// not in stdlib.h
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

// some useful constants
#define FALSE 0
#define TRUE 1
#define ZERO 0

// number conversion
#define UINT8_T_MAX 255
#define BYTE_LENGTH 8
#define ROUND_SUM 5
#define ROUND_DIVIDE 10

// character and string values
#define SPACE ' '
#define LF "\n"
#define STRING_TERMINATOR 0


// robot library
#include <pololu/orangutan.h>

#endif // BASE_H
