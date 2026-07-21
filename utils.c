#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include <ctype.h>
#include "token.h"

int strfndc(char *str, int chr) {

    if (str == NULL) return -1;

    char *charfound = NULL;

    charfound = strchr(str,chr);

    if (charfound == NULL) return -1;

    return (charfound - str);
}

int strfndmathop(char *str, char *chrfound) {

    char *strp = str;
    *chrfound = '\0';
    while (*str != '\0') {
        if (isMathOperator(*str)) {
            *chrfound = *str;
            return strp - str;
        } 
        str++;
    }
    return -1;
}

int strfndCbrace(char *str, char *chrfound) {
    
    char *strp = str;
    *chrfound = '\0';
    while (*str != '\0') {
        if (isCBrace(*str)) {
            *chrfound = *str;
            return strp - str;
        } 
        str++;
    }
    return -1;
}

int isMathOperator(char chr) {
    return (chr == '+' || chr == '-' || chr == '/' || chr == '*' || chr == '%'
    || chr == '>' || chr == '<' || chr == '=' || chr == '^' || chr == '~');
}

int isCBrace(char chr) {
    return (chr == '(' || chr == ')');
}

int isNum(char chr) {
    return (chr >= '0' && chr <= '9');
}

double powto(double base, long power) {
    if (power == 0) return 1;
    double num = 1.0;
    for (long powercycle = 0; powercycle < power; powercycle++) {
        num *= base;
    }
    return num;
}