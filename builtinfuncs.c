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
#include "error.h"
#include <math.h>

void builtinRegister(Dictionary *functions) {
    dictInsert(functions, "echo", builtin_test);
    dictInsert(functions, "sqrt", builtin_sqrt);
    dictInsert(functions, "exit", builtin_exit);
    dictInsert(functions, "sin", builtin_sin);
    dictInsert(functions, "cos", builtin_cos);
    dictInsert(functions, "randint", builtin_randint);
    dictInsert(functions, "randprob", builtin_randprob);
}

static RuntimeValue builtin_test(RuntimeValue *args, size_t argc, RuntimeContext *ctx) {
    if (argc != 1) {
        runtimeError(ctx,ERROR_ARGUMENT_COUNT,"echo() expects exactly 1 argument.");
        return INVALID_VALUE;
    }
    return args[0];
}

//EXAMPLE
static RuntimeValue builtin_exit(RuntimeValue *args,
                                 size_t argc,
                                 RuntimeContext *ctx) {
    (void)args;
    (void)argc;
    (void)ctx;

    exit(EXIT_SUCCESS);
}

static RuntimeValue builtin_sqrt(RuntimeValue *args,
                                 size_t argc, RuntimeContext *ctx) {

    if (argc != 1) {
        runtimeError(ctx,ERROR_ARGUMENT_COUNT,"sqrt() expects exactly 1 argument.");
        return INVALID_VALUE;
    }

    double value;
    if (args[0].type == VALUE_FLOAT) {
        value = sqrt(args[0].value.floater);
    } else if (args[0].type == VALUE_INT) {
        value = sqrt((double)args[0].value.integer);
    } else {
        runtimeError(ctx,ERROR_ARGUMENT_COUNT,"sqrt() expects exactly 1 argument.");
        return INVALID_VALUE;
    }

    if (isnan(value)) {
        runtimeError(ctx,ERROR_INVALID_ARGUMENT,"sqrt() expects a single non-negative value.");
        return INVALID_VALUE;
    }

    args[0].type = VALUE_FLOAT;
    args[0].value.floater = value;

    return args[0];                              

}

static RuntimeValue builtin_sin(RuntimeValue *args,
                                size_t argc, RuntimeContext *ctx) {

    if (argc != 1) {
        runtimeError(ctx,ERROR_ARGUMENT_COUNT,"sin() expects exactly 1 argument.");
        return INVALID_VALUE;
    }

    double value;
    if (args[0].type == VALUE_FLOAT) {
        value = sin(args[0].value.floater);
    } else if (args[0].type == VALUE_INT) {
        value = sin((double)args[0].value.integer);
    } else {
        runtimeError(ctx,ERROR_INVALID_ARGUMENT,"sin() expects exactly 1 argument (number).");
        return INVALID_VALUE;
    }

    args[0].type = VALUE_FLOAT;
    args[0].value.floater = value;
    return args[0];   

}

static RuntimeValue builtin_cos(RuntimeValue *args,
                                size_t argc, RuntimeContext *ctx) {

    if (argc != 1) {
        runtimeError(ctx,ERROR_ARGUMENT_COUNT,"cos() expects exactly 1 argument.");
        return INVALID_VALUE;
    }

    double value;
    if (args[0].type == VALUE_FLOAT) {
        value = cos(args[0].value.floater);
    } else if (args[0].type == VALUE_INT) {
        value = cos((double)args[0].value.integer);
    } else {
        runtimeError(ctx,ERROR_INVALID_ARGUMENT,"cos() expects exactly 1 argument (number).");
        return INVALID_VALUE;
    }

    args[0].type = VALUE_FLOAT;
    args[0].value.floater = value;
    return args[0];   

}

static RuntimeValue builtin_randint(RuntimeValue *args,
                                size_t argc, RuntimeContext *ctx) {
    if (argc != 2) {
        runtimeError(ctx,ERROR_ARGUMENT_COUNT,"randint() expects exactly 2 arguments (min,max).");
        return INVALID_VALUE;
    }

    long min;
    long max;

    if (args[0].type == VALUE_INT) min = args[0].value.integer;
    else min = (long)args[0].value.floater;

    if (args[1].type == VALUE_INT) max = args[1].value.integer;
    else max = (long)args[1].value.floater;

    if (min > max) {
        runtimeError(ctx,ERROR_INVALID_ARGUMENT,"randint() expects max > min.");
        return INVALID_VALUE;
    }

    RuntimeValue result;
    result.type = VALUE_INT;

    result.value.integer = min + rand() % (max - min + 1);
    
    return result;   
}

static RuntimeValue builtin_randprob(RuntimeValue *args,
                                size_t argc, RuntimeContext *ctx) {
       
    if (argc != 0) {
        runtimeError(ctx,ERROR_ARGUMENT_COUNT,"randprob() expects exactly 0 arguments.");
        return INVALID_VALUE;
    }                      
    
    RuntimeValue result;
    result.type = VALUE_FLOAT;

    result.value.floater = (double)rand() / (double)RAND_MAX;
    return result;
}


