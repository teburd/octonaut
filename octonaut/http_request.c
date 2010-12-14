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

#include <stdio.h>

#include "http_request.h"
#include "common.h"

enum octo_http_parser_state
{
    PARSER_INIT,
    PARSER_MESSAGE_BEGIN,
    PARSER_PATH,
    PARSER_QUERY,
    PARSER_URL,
    PARSER_FRAGMENT,
    PARSER_FIELD,
    PARSER_VALUE,
    PARSER_HEADERS_COMPLETE,
    PARSER_BODY,
    PARSER_MESSAGE_COMPLETE,
    error
};

/**
 * http_parser callbacks
 */
static int request_message_begin(http_parser *parser)
{
    octo_http_request *request = ptr_offset(parser, octo_http_request, parser);
    request->parser_state = PARSER_MESSAGE_BEGIN;
    return 0;
}

static int request_path(http_parser *parser,
    const char *path, const size_t len)
{
    octo_http_request *request = ptr_offset(parser, octo_http_request, parser);
    request->parser_state = PARSER_PATH;
    return 0;
}

static int request_query(http_parser *parser,
    const char *query, const size_t len)
{
    octo_http_request *request = ptr_offset(parser, octo_http_request, parser);
    request->parser_state = PARSER_QUERY;
    return 0;
}

static int request_url(http_parser *parser,
    const char *url, const size_t len)
{
    octo_http_request *request = ptr_offset(parser, octo_http_request, parser);
    request->parser_state = PARSER_URL;
    return 0;
}

static int request_fragment(http_parser *parser,
    const char *fragment, const size_t len)
{
    octo_http_request *request = ptr_offset(parser, octo_http_request, parser);
    request->parser_state = PARSER_FRAGMENT;
    return 0;
}

static int request_header_field(http_parser *parser,
    const char *field, const size_t len)
{
    octo_http_request *request = ptr_offset(parser, octo_http_request, parser);
    request->parser_state = PARSER_FIELD;
    return 0;
}

static int request_header_value(http_parser *parser,
    const char *value, const size_t len)
{
    octo_http_request *request = ptr_offset(parser, octo_http_request, parser);
    request->parser_state = PARSER_VALUE;
    return 0;
}

static int request_headers_complete(http_parser *parser)
{

    octo_http_request *request = ptr_offset(parser, octo_http_request, parser);
    request->parser_state = PARSER_HEADERS_COMPLETE;
    return 0;
}

static int request_body(http_parser *parser,
    const char *body, const size_t len)
{
    octo_http_request *request = ptr_offset(parser, octo_http_request, parser);
    request->parser_state = PARSER_BODY;
    return 0;
}

static int request_message_complete(http_parser *parser)
{
    octo_http_request *request = ptr_offset(parser, octo_http_request, parser);
    request->parser_state = PARSER_MESSAGE_COMPLETE;
    return 0;
}

static http_parser_settings
parser_settings = {
    .on_message_begin       = request_message_begin,
    .on_path                = request_path,
    .on_query_string        = request_query,
    .on_url                 = request_url,
    .on_fragment            = request_fragment,
    .on_header_field        = request_header_field,
    .on_header_value        = request_header_value,
    .on_headers_complete    = request_headers_complete,
    .on_body                = request_body,
    .on_message_complete    = request_message_complete
};

void octo_http_request_init(octo_http_request *request)
{
    octo_list_init(&request->message_queue);
    http_parser_init(&request->parser, HTTP_REQUEST);
    request->parser_state = PARSER_INIT;
}

void octo_http_request_destroy(octo_http_request *request)
{
    octo_list_destroy(&request->message_queue);
    request->parser_state = PARSER_INIT;
}

size_t octo_http_request_parse(octo_http_request *request, const char *data, size_t len)
{
    size_t parsed = http_parser_execute(&request->parser, &parser_settings, data, len);
    return parsed;
}

bool octo_http_request_valid(octo_http_request *request)
{
    if(request->parser_state != error)
    {
        return true;
    }
    return false;
}
