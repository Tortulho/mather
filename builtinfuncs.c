#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "ast.h"
#include "token.h"
#include "utils.h"
#include "parser.h"
#include "interpreter.h"
#include "dict.h"
#include "builtinfuncs.h"
#include "runtime.h"
#include <math.h>

void builtinRegister(Dictionary *functions) {
    dictInsert(functions, "test", builtin_test);
    dictInsert(functions, "sqrt", builtin_sqrt);
    dictInsert(functions, "exit", builtin_exit);
    dictInsert(functions, "sin", builtin_sin);
    dictInsert(functions, "cos", builtin_cos);
    dictInsert(functions, "randint", builtin_randint);
    dictInsert(functions, "randprob", builtin_randprob);
}

static RuntimeValue builtin_test(RuntimeValue *args, size_t argc) {
    if (argc != 1) {
        printf("Error: test() expects exactly 1 argument.\n");
        exit(EXIT_FAILURE);
    }
    return args[0];
}

//EXAMPLE
static RuntimeValue builtin_exit(RuntimeValue *args, size_t argc) {
    if (argc != 0) {
        printf("Error: exit() expects exactly 0 arguments.\n");
        exit(EXIT_FAILURE);
    } else {
        exit(EXIT_SUCCESS);
    }
    RuntimeValue value;
    value.type = VALUE_VOID;
    return value;
}

static RuntimeValue builtin_sqrt(RuntimeValue *args,
                                 size_t argc) {

    if (argc != 1) {
        printf("Error: sqrt() expects exactly 1 argument.\n");
        exit(EXIT_FAILURE);
    }

    double value;
    if (args[0].type == VALUE_FLOAT) {
        value = sqrt(args[0].value.floater);
    } else if (args[0].type == VALUE_INT) {
        value = sqrt((double)args[0].value.integer);
    } else {
        printf("Error: sqrt() expects exactly 1 argument of type float/int.\n");
        exit(EXIT_FAILURE);
    }

    if (isnan(value)) {
        printf("Error: sqrt() domain error.\n");
        exit(EXIT_FAILURE);
    }

    args[0].type = VALUE_FLOAT;
    args[0].value.floater = value;

    return args[0];                              

}

static RuntimeValue builtin_sin(RuntimeValue *args,
                                size_t argc) {

    if (argc != 1) {
        printf("Error: sin() expects exactly 1 argument.\n");
        exit(EXIT_FAILURE);
    }

    double value;
    if (args[0].type == VALUE_FLOAT) {
        value = sin(args[0].value.floater);
    } else if (args[0].type == VALUE_INT) {
        value = sin((double)args[0].value.integer);
    } else {
        printf("Error: sin() expects exactly 1 argument of type float/int.\n");
        exit(EXIT_FAILURE);
    }

    args[0].type = VALUE_FLOAT;
    args[0].value.floater = value;
    return args[0];   

}

static RuntimeValue builtin_cos(RuntimeValue *args,
                                size_t argc) {

    if (argc != 1) {
        printf("Error: cos() expects exactly 1 argument.\n");
        exit(EXIT_FAILURE);
    }

    double value;
    if (args[0].type == VALUE_FLOAT) {
        value = cos(args[0].value.floater);
    } else if (args[0].type == VALUE_INT) {
        value = cos((double)args[0].value.integer);
    } else {
        printf("Error: cos() expects exactly 1 argument of type float/int.\n");
        exit(EXIT_FAILURE);
    }

    args[0].type = VALUE_FLOAT;
    args[0].value.floater = value;
    return args[0];   

}

static RuntimeValue builtin_clearvars(RuntimeValue *args,
                                size_t argc);

static RuntimeValue builtin_hex(RuntimeValue *args,
                                size_t argc);

static RuntimeValue builtin_bin(RuntimeValue *args,
                                size_t argc);

static RuntimeValue builtin_randint(RuntimeValue *args,
                                size_t argc) {
    if (argc != 2) {
        printf("Error: randint() expects exactly 2 arguments (min,max).\n");
        exit(EXIT_FAILURE);
    }

    long min;
    long max;

    if (args[0].type == VALUE_INT) min = args[0].value.integer;
    else min = (long)args[0].value.floater;

    if (args[1].type == VALUE_INT) max = args[1].value.integer;
    else max = (long)args[1].value.floater;

    if (min > max) {
        printf("Error: randint(): min > max.\n");
        exit(EXIT_FAILURE);
    }

    RuntimeValue result;
    result.type = VALUE_INT;

    result.value.integer = min + rand() % (max - min + 1);
    
    return result;   
}

static RuntimeValue builtin_randprob(RuntimeValue *args,
                                size_t argc) {
       
    if (argc != 0) {
        printf("Error: randprob() expects exactly 0 arguments.\n");
        exit(EXIT_FAILURE);
    }                      
    
    RuntimeValue result;
    result.type = VALUE_FLOAT;

    result.value.floater = (double)rand() / (double)RAND_MAX;
    return result;
}

