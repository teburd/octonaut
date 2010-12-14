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

#include <octonaut/aio.h>
#include <ev.h>
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

START_TEST (test_octo_aio_create)
{
    octo_aio aio;

    struct ev_loop *loop = EV_DEFAULT;
   
    int fd = fileno(stdin);
    octo_aio_init(&aio, loop, fd); 
    
    fail_unless(aio.fd == fd,
        "fd not set correctly by aio_init");

    octo_aio_destroy(&aio);

    fail_unless(aio.fd == -1,
        "fd not reset by aio_destroy");
}
END_TEST

START_TEST (test_octo_aio_start)
{
    octo_aio aio;

    struct ev_loop *loop = EV_DEFAULT;
    
    octo_aio_init(&aio, loop, 0); 

    octo_aio_start(&aio);

    octo_aio_destroy(&aio);
}
END_TEST

typedef struct mock_ctx
{
    int byte_count;
} mock_ctx;

void mock_rw_cb(void *ctx, void *data, size_t len)
{
    mock_ctx *mctx = (mock_ctx*)ctx;
    mctx->byte_count += len;
}


START_TEST (test_octo_aio_pipe)
{
    int pipefds[2];
    octo_aio aios[2];
    const char* msg = "suck it trabek";
    size_t msg_len = strlen(msg);
    char buffer[msg_len];
    size_t read_len = 0;
    mock_ctx ctx;
    ctx.byte_count = 0;

    struct ev_loop *loop = EV_DEFAULT;

    fail_unless(pipe(pipefds) != -1);

    octo_aio_init(&aios[0], loop, pipefds[0]);
    octo_aio_init(&aios[1], loop, pipefds[1]);
   
    aios[0].read = mock_rw_cb;
    aios[0].read_ctx = &ctx;
    octo_aio_start(&aios[0]);

    write(pipefds[1], msg, msg_len);
    read_len = read(pipefds[0], buffer, msg_len);

    /* sanity check on the pipe */
    fail_unless(read_len == msg_len,
        "message read from pipe not the length of the message, pipe error?");

    /* use the octo_aio as a read watcher on the pipe
     * with our mock_rw_cb which counts bytes
     */
    write(pipefds[1], msg, msg_len);
    ev_run(loop, EVRUN_ONCE);
    fail_unless(ctx.byte_count == msg_len,
        "message read from pipe using aio not hte length of the message.");

    octo_aio_destroy(&aios[0]);
    octo_aio_destroy(&aios[1]);
}
END_TEST

START_TEST (test_octo_aio_eagain)
{
    int pipefds[2];
    octo_aio aios[2];
    char* msg = "suck it trabek";
    size_t msg_len = strlen(msg);
    mock_ctx ctx;
    ctx.byte_count = 0;

    struct ev_loop *loop = EV_DEFAULT;

    fail_unless(pipe(pipefds) != -1);

    octo_aio_init(&aios[0], loop, pipefds[0]);
    octo_aio_init(&aios[1], loop, pipefds[1]);
   
    aios[0].read = mock_rw_cb;
    aios[0].read_ctx = &ctx;
    octo_aio_start(&aios[0]);
    
    octo_aio_start(&aios[1]);

    fail_unless(aios[1].write == octo_aio_direct_write,
        "octo_aio_write is not octo_aio_direct_write.");

    size_t total= 0;
    while(aios[1].write == octo_aio_direct_write && total < 100*1024)
    {
        octo_aio_write(&aios[1], msg, msg_len);
        total += msg_len;
    }

    fail_unless(aios[1].write == octo_aio_buffered_write,
        "octo_aio_write did not switch to buffered writing.");

    fail_unless(ev_is_active(&aios[1].write_watcher), 
        "octo_aio_write did not start the write watcher.");

    total= 0;
    while(total < 100*1024)
    {
        octo_aio_write(&aios[1], msg, msg_len);
        total += msg_len;
    }

    fail_unless(octo_buffer_size(&aios[1].write_buffer) > 99*1024,
        "octo_aio write buffer has not been filled in.");

    size_t count = 0;
    while(octo_buffer_size(&aios[1].write_buffer) != 0 && count < 1000)
    {
        count += 1;
        ev_run(loop, EVRUN_ONCE);
    }

    fail_unless(octo_buffer_size(&aios[1].write_buffer) == 0,
        "octo_aio write_buffer is not empty as it should be.");

    octo_aio_destroy(&aios[0]);
    octo_aio_destroy(&aios[1]);
}
END_TEST


TCase* octo_aio_tcase()
{
    TCase* tc_octo_aio = tcase_create("octo_aio");
    tcase_add_test(tc_octo_aio, test_octo_aio_create);
    tcase_add_test(tc_octo_aio, test_octo_aio_start);
    tcase_add_test(tc_octo_aio, test_octo_aio_pipe);
    tcase_add_test(tc_octo_aio, test_octo_aio_eagain);
    return tc_octo_aio;
}
