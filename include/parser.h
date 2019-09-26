//
// Created by starman on 26.09.2019.
//

#ifndef LAB2_PARSER_H_
#define LAB2_PARSER_H_

#include "include/utils.h"

/**
 * Parse arguments from command line
 * @param {char**} path
 * @param {int} argc
 * @param {char**} argv
 * @return {status} stat
 */
status parse_args(char **path, int argc, char **argv);

#endif // LAB2_PARSER_H_
