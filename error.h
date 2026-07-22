#ifndef ERRORS_H
#define ERRORS_H

#include "runtime.h"
#include <stdarg.h>

#define MEMERRORMSG runtimeError(ctx,ERROR_MEMORY,"Cannot allocate more memory to the program.")

//NOTE: ERROR CODES ARE IN RUNTIME.H

void runtimeError(RuntimeContext *ctx,
                  ErrorCode code,
                  const char *fmt,
                  ...);

void runtimeClearError(RuntimeContext *ctx);

#endif