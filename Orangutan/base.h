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
#define SPACE ' '
#define ZERO 0
#define UINT8_T_MAX 255

// robot library
#include <pololu/orangutan.h>

#endif // BASE_H
