#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdlib.h>

typedef struct {

    enum {
        VALUE_INT,
        VALUE_FLOAT,
        VALUE_VAR,
        VALUE_FUNC,
        VALUE_VOID,
    } type;

    union {
        long integer;
        double floater;
        void *pfunc;
    } value;

} RuntimeValue;

typedef RuntimeValue (*BuiltinFunction)(RuntimeValue *args,
                                        size_t argc);

#endif