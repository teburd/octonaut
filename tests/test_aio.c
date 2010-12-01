#include "aio.h"
#include <stdio.h>

START_TEST (test_octo_aio_create)
{
    octo_aio aio;

    struct ev_loop *loop = EV_DEFAULT;
   
    int fd = fileno(stdin);
    octo_aio_init(&aio, loop, fd, 128); 
    
    fail_unless(aio.buffer_size == 128,
        "buffer size not set correctly by aio_init");
    fail_unless(aio.fd == fd,
        "fd not set correctly by aio_init");

    octo_aio_destroy(&aio);

    fail_unless(aio.buffer_size == 0,
        "buffer_size not reset by aio_destroy");
    fail_unless(aio.fd == -1,
        "fd not reset by aio_destroy");
}
END_TEST

START_TEST (test_octo_aio_start)
{
    octo_aio aio;

    struct ev_loop *loop = EV_DEFAULT;
    
    octo_aio_init(&aio, loop, 0, 128); 

    octo_aio_start(&aio);

    octo_aio_destroy(&aio);
}
END_TEST

typedef struct mock_ctx
{
    int byte_count;
} mock_ctx;

void mock_rw_cb(void *ctx, uint8_t *data, size_t len)
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

    octo_aio_init(&aios[0], loop, pipefds[0], 128);
    octo_aio_init(&aios[1], loop, pipefds[1], 128);
   
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

TCase* octo_aio_tcase()
{
    TCase* tc_octo_aio = tcase_create("octo_aio");
    tcase_add_test(tc_octo_aio, test_octo_aio_create);
    tcase_add_test(tc_octo_aio, test_octo_aio_start);
    tcase_add_test(tc_octo_aio, test_octo_aio_pipe);
    return tc_octo_aio;
}
