/**
 * Copyright (c) 2010 Tom Burdick <thomas.burdick@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef OCTO_LOGGER_H
#define OCTO_LOGGER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include "list.h"

typedef enum _octo_log_level
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} octo_log_level;

typedef struct octo_log_output
{
    octo_list list;
    bool colorize;
    FILE* stream;
} octo_log_output;

typedef struct octo_logger
{
    char *name;
    octo_log_level level;
    octo_list outs[4];
} octo_logger;

void octo_logger_init(octo_logger *lgr, const char *name);
void octo_logger_destroy(octo_logger *lgr);

void octo_logger_add_output(octo_logger *lgr, octo_log_level level, FILE* stream, bool colorize);

octo_log_level octo_logger_level(octo_logger *lgr);
void octo_logger_set_level(octo_logger *lgr, octo_log_level level);

void octo_logger_log(octo_logger *lgr, octo_log_level level, const char *where, const char *fmt, ...);

#define octo_plogger_debug(logger, ...) \
    octo_logger_log(logger, LOG_DEBUG, __func__, __VA_ARGS__)

#define octo_logger_debug(logger, ...) \
    octo_logger_log(&logger, LOG_DEBUG, __func__, __VA_ARGS__)

#define octo_plogger_info(logger, ...) \
    octo_logger_log(logger, LOG_INFO, __func__, __VA_ARGS__)

#define octo_logger_info(logger, ...) \
    octo_logger_log(&logger, LOG_INFO, __func__, __VA_ARGS__)

#define octo_plogger_warn(logger, ...) \
    octo_logger_log(logger, LOG_WARN, __func__, __VA_ARGS__)

#define octo_logger_warn(logger, ...) \
    octo_logger_log(&logger, LOG_WARN, __func__, __VA_ARGS__)

#define octo_plogger_error(logger, ...) \
    octo_logger_log(logger, LOG_ERROR, __func__, __VA_ARGS__)

#define octo_logger_error(logger, ...) \
    octo_logger_log(&logger, LOG_ERROR, __func__, __VA_ARGS__)

#endif
