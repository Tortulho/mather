#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdlib.h>

int strfndc(char *str, int chr);
int strfndmathop(char *str, char *chrfound);
int strfndCbrace(char *str, char *chrfound);
int isMathOperator(char chr);
int isCBrace(char chr);
int isNum(char chr);
double powto(double base, long power);

#endif