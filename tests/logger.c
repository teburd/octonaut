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

#include <octonaut/logger.h>
#include <check.h>

START_TEST (test_octo_logger)
{
    octo_logger logger;
    octo_logger_init(&logger, "octonaut");
    octo_logger_add_output(&logger, LOG_ERROR, stdout, true);
    octo_logger_add_output(&logger, LOG_WARN, stdout, true);
    octo_logger_add_output(&logger, LOG_INFO, stdout, true);
    octo_logger_add_output(&logger, LOG_DEBUG, stdout, true);
    octo_logger_debug(logger, "log debug");
    octo_logger_info(logger, "log info");
    octo_logger_warn(logger, "log warn");
    octo_logger_error(logger, "log error");
    octo_logger_destroy(&logger);
}
END_TEST

TCase* octo_logger_tcase()
{
    TCase* tc_octo_logger= tcase_create("octo_logger");
    tcase_add_test(tc_octo_logger, test_octo_logger);
    return tc_octo_logger;
}
