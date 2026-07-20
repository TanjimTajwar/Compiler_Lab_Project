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
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

int semantic_analyze(ASTNode *root);
int semantic_get_error_count(void);

#endif /* SEMANTIC_H */
