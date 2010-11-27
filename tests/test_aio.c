#include "aio.h"


START_TEST (test_octo_aio_create)
{
    octo_aio aio;

    struct ev_loop *loop = EV_DEFAULT;
    
    octo_aio_init(&aio, loop, (size_t)stdin, 128); 

    octo_aio_destroy(&aio);
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
    mock_ctx ctx;
    ctx.byte_count = 0;

    struct ev_loop *loop = EV_DEFAULT;

    printf("creating pipes\n");
    if(pipe(pipefds) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    printf("initialize\n");
    octo_aio_init(&aios[0], loop, pipefds[0], 128);
    octo_aio_init(&aios[1], loop, pipefds[1], 128);
   
    printf("starting 0\n");
    aios[0].read = mock_rw_cb;
    aios[0].read_ctx = &ctx;
    octo_aio_start(&aios[0]);

    printf("writting\n");
    write(pipefds[1], msg, msg_len);
    read(pipefds[0], buffer, msg_len);
    printf("read %s\n", buffer);
    write(pipefds[1], msg, msg_len);

    printf("running\n");
    ev_run(loop, EVRUN_ONCE);

    /* assert that the current read count is greater than 1 */
    printf("pipe got %d bytes\n", ctx.byte_count);

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
