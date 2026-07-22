#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdlib.h>

typedef enum {

    ERROR_NONE,

    ERROR_SYNTAX,
    ERROR_UNEXPECTED_TOKEN,
    ERROR_UNKNOWN_VARIABLE,
    ERROR_UNKNOWN_FUNCTION,
    ERROR_DIVISION_BY_ZERO,
    ERROR_INVALID_ARGUMENT,
    ERROR_ARGUMENT_COUNT,
    ERROR_DOMAIN,
    ERROR_RUNTIME,
    ERROR_MEMORY,
    ERROR_CORRUPTED_MEMORY,

} ErrorCode;

typedef struct {

    enum {
        VALUE_INT,
        VALUE_FLOAT,
        VALUE_VAR,
        VALUE_FUNC,
        VALUE_VOID,
        VALUE_INVALID,
        VALUE_CSTRING,
    } type;

    union {
        long integer;
        double floater;
        void *pfunc;
        char *string;
    } value;

} RuntimeValue;

typedef struct {

    int hasError;
    ErrorCode code;
    char message[256];

} RuntimeContext;

#define INVALID_VALUE ((RuntimeValue){ .type = VALUE_INVALID })
#define VOID_VALUE    ((RuntimeValue){ .type = VALUE_VOID })

typedef RuntimeValue (*BuiltinFunction)(RuntimeValue *args,
                                        size_t argc,
                                        RuntimeContext *ctx);

#endif