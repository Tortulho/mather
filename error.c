#include "error.h"
#include "runtime.h"
#include <stdarg.h>

void runtimeError(RuntimeContext *ctx,
                  ErrorCode code,
                  const char *fmt,
                  ...)
{
    if (ctx == NULL)
        return;

    if (ctx->hasError)
        return;

    ctx->hasError = 1;
    ctx->code = code;

    va_list args;
    va_start(args, fmt);

    vsnprintf(ctx->message,
              sizeof(ctx->message),
              fmt,
              args);

    va_end(args);
}

void runtimeClearError(RuntimeContext *ctx) {
    if (ctx == NULL)
        return;

    ctx->hasError = 0;
    ctx->code = ERROR_NONE;
    ctx->message[0] = '\0';
}