// Done by Monir Hossain
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
 * Centralized error reporting utilities.
 */

#ifndef ERRORS_H
#define ERRORS_H

typedef enum {
    ERR_LEXICAL,
    ERR_SYNTAX,
    ERR_SEMANTIC
} ErrorKind;

void error_report(ErrorKind kind, int line, const char *message, ...);
int error_get_count(void);
void error_reset(void);

#endif /* ERRORS_H */
