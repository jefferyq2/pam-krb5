/*
 * logging.c
 *
 * Logging functions for pam_krb5.
 *
 * Logs errors and debugging messages from pam_krb5 functions.  The debug
 * versions only log anything if debugging was enabled; the error versions
 * always log.
 */

#include <com_err.h>
#include <krb5.h>
#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>

#include "pam_krb5.h"

/*
 * Basic error logging.  Log a message with LOG_NOTICE priority.
 */
void
error(struct context *ctx, const char *fmt, ...)
{
    const char *name;
    char msg[256];
    va_list args;

    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);
    name = (ctx != NULL && ctx->name != NULL) ? ctx->name : "none";
    syslog(LOG_NOTICE, "(pam_krb5): %s: %s", name, msg);
}


/*
 * Log a generic debugging message only if debug is enabled.
 */
void
debug(struct context *ctx, struct pam_args *pargs, const char *fmt, ...)
{
    const char *name;
    char msg[256];
    va_list args;

    if (!pargs->debug)
        return;

    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);
    name = ctx && ctx->name ? ctx->name : "none";
    syslog(LOG_DEBUG, "(pam_krb5): %s: %s", name, msg);
}


/*
 * Log a PAM failure if debugging is enabled.
 */
void
debug_pam(struct context *ctx, struct pam_args *args, const char *msg,
          int status)
{
    debug(ctx, args, "%s: %s", msg, pam_strerror(ctx->pamh, status));
}


/*
 * Log a Kerberos v5 failure if debugging is enabled.
 */
void
debug_krb5(struct context *ctx, struct pam_args *args, const char *msg,
           int status)
{
    debug(ctx, args, "%s: %s", msg, error_message(status));
}
