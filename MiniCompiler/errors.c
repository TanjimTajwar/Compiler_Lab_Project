/*
 * Team Members:
 * - Tanjim Tajwar Arnab (22701066)
 * - Hafiz Hasnat Sifat Jami (22701068)
 * - Muznabin Ahmed (22701069)
 * - Monir Hossain (21701009)
 *
 * Primary Contributor:
 * Tanjim Tajwar Arnab
 *
 * Contributors:
 * Hafiz Hasnat Sifat Jami
 * Muznabin Ahmed
 * Monir Hossain
 *
 * Centralized error reporting implementation.
 */

#include "errors.h"
#include <stdio.h>
#include <stdarg.h>

static int g_error_count = 0;

void error_report(ErrorKind kind, int line, const char *message, ...) {
    const char *kind_str;
    va_list args;

    switch (kind) {
        case ERR_LEXICAL:  kind_str = "Lexical"; break;
        case ERR_SYNTAX:   kind_str = "Syntax"; break;
        case ERR_SEMANTIC: kind_str = "Semantic"; break;
        default:           kind_str = "Error"; break;
    }

    fprintf(stderr, "%s error at line %d: ", kind_str, line);
    va_start(args, message);
    vfprintf(stderr, message, args);
    va_end(args);
    fprintf(stderr, "\n");
    g_error_count++;
}

int error_get_count(void) {
    return g_error_count;
}

void error_reset(void) {
    g_error_count = 0;
}
