#ifndef PARSER_H
#define PARSER_H

#include "task.h"

typedef enum ParserError {
    PARSER_OK = 0,
    PARSER_FAILED,
    PARSER_EMPTY_LINE,
    PARSER_ERR_SPLIT_LINE,
    PARSER_ERR_INSUFFICIENT_TOKEN_COUNT,
    PARSER_ERR_PARSE_TOKENS,
} ParserError;

ParserError parse_line_into_task(char *line, Task *task);

#endif //PARSER_H
