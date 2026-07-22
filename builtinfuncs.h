#ifndef BUILTIN_H
#define BUILTIN_H

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
#include "runtime.h"

void builtinRegister(Dictionary *functions);
static RuntimeValue builtin_test(RuntimeValue *args, size_t argc, RuntimeContext *ctx);

static RuntimeValue builtin_sqrt(RuntimeValue *args,
                                 size_t argc, RuntimeContext *ctx);

static RuntimeValue builtin_sin(RuntimeValue *args,
                                size_t argc, RuntimeContext *ctx);

static RuntimeValue builtin_cos(RuntimeValue *args,
                                size_t argc, RuntimeContext *ctx);

static RuntimeValue builtin_exit(RuntimeValue *args,
                                size_t argc, RuntimeContext *ctx);

static RuntimeValue builtin_clearvars(RuntimeValue *args,
                                size_t argc, RuntimeContext *ctx);

static RuntimeValue builtin_hex(RuntimeValue *args,
                                size_t argc, RuntimeContext *ctx);

static RuntimeValue builtin_bin(RuntimeValue *args,
                                size_t argc, RuntimeContext *ctx);

static RuntimeValue builtin_randint(RuntimeValue *args,
                                size_t argc, RuntimeContext *ctx);

static RuntimeValue builtin_randf(RuntimeValue *args,
                                size_t argc, RuntimeContext *ctx);

static RuntimeValue builtin_randprob(RuntimeValue *args,
                                size_t argc, RuntimeContext *ctx);

#endif