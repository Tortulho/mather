#include "token.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"

void tokenize(char *text, Token *tokenarr, int tokensize, int *tokencount) {

    int tokencounter = 0;
    int chrcounter = 0;
    char *startstrcpy = NULL;

    //ciclo

    /*
    SE FOR LETRA
    CONSOME TODAS AS LETRAS
    FAZ MALLOC E COLOCA A PALAVRA LA
    */
    while (1) { 
        if (tokencounter >= tokensize) break;
        if (isalpha(*text)) {
            tokenarr[tokencounter].type = TOKEN_STRING;
            chrcounter = 0;
            startstrcpy = text;
            while (isalpha(*text)) {
                chrcounter++;
                text++;
            }
            tokenarr[tokencounter].value.string = calloc(chrcounter+1,1);
            if (tokenarr[tokencounter].value.string == NULL) exit(EXIT_FAILURE);
            strncpy(tokenarr[tokencounter].value.string,startstrcpy,chrcounter);
            tokencounter++;
        }
        /*
        SE FOR NUM
        CONSOME TODOS OS NUM
        SE ENCONTRAR PONTO VIRA FLOAT
        SE NAO É INTEIRO
        */
        else if (isdigit(*text)) {
            tokenarr[tokencounter].type = TOKEN_INT;
            chrcounter = 0;
            startstrcpy = text;
            while (isdigit(*text)) {
                chrcounter++;
                text++;
            }
            if (*text == '.') {
                tokenarr[tokencounter].type = TOKEN_FLOAT;
                tokenarr[tokencounter].value.numFLOAT = strtod(startstrcpy,&text);
            } else {
                tokenarr[tokencounter].value.numINT = strtol(startstrcpy,&text,10);
            }
            //tokenarr[tokencounter].value.numINT = strtol(startstrcpy,&text,10);
            tokencounter++;
        }
        /*
        SE FOR OPERADOR
        GUARDAR OPERADOR
        */
        else if (isMathOperator(*text)) {
            tokenarr[tokencounter].type = TOKEN_MATH_OPERATOR;
            chrcounter = 0;
            startstrcpy = text;
            tokenarr[tokencounter].value.numINT = *text;
            tokencounter++;
            text++;
        } 
        /*
        SE FOR ' '
        ignorar
        */
        else if (isspace(*text)) {
            text++;
        }
        /*
        SE FOR (
        GUARDAR
        */
        else if (*text == '(') {
            tokenarr[tokencounter].type = TOKEN_LEFT_CBRACE;
            chrcounter = 0;
            startstrcpy = text;
            tokencounter++;
            text++;
        /*
        SE FOR )
        GUARDAR
        */
        } else if (*text == ')') {
            tokenarr[tokencounter].type = TOKEN_RIGHT_CBRACE;
            chrcounter = 0;
            startstrcpy = text;
            tokencounter++;
            text++;
        } else if (*text == '\0') {
            //END OF STRING / INVALID TOKEN
            break;
        } else {
            exit(EXIT_FAILURE);
        }
    }   
    *tokencount = tokencounter;
}

void freeTokens(Token *tokens, int count) {

    for (int i = 0; i < count; i++) {

        if (tokens[i].type == TOKEN_STRING) {
            free(tokens[i].value.string);
            tokens[i].value.string = NULL;
        }
    }
}