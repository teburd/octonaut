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

#include <octonaut/hash.h>
#include <octonaut/aio.h>
#include <octonaut/logger.h>
#include <sys/types.h>
#include <unistd.h>

static octo_logger _logger;
static octo_logger *logger;


void stats(EV_P_ ev_timer *w, int revents)
{
    octo_logger_info(logger, "timer fired for %d", getpid());
}

void worker_loop(int worker_id, int port)
{
    octo_logger_info(logger, "worker_loop %d starting up to listen on port %d", worker_id, port);
    
    struct ev_loop *loop = EV_DEFAULT;
    
    ev_timer timer;
    ev_timer_init(&timer, stats, 5, 5);
    ev_timer_start(loop, &timer);

    ev_run(loop, 0);

    octo_logger_info(logger, "worker_loop %d quitting", worker_id);

}

void server_loop()
{
    octo_logger_info(logger, "server loop starting");
    for(int i = 0; i < 4; ++i)
    {
        int port = 8000;
        pid_t server_pid = getpid();
        fork();
        if(getpid() != server_pid)
        {
            octo_logger_debug(logger, "forked process %d", getpid());
            worker_loop(i, port + i);
            return;
        }
    }

    struct ev_loop *loop = EV_DEFAULT;

    ev_timer timer;
    ev_timer_init(&timer, stats, 5, 5);
    ev_timer_start(loop, &timer);

    ev_run(loop, 0);

    octo_logger_debug(logger, "quitting");
}


int main(int argc, char **argv)
{
    logger = &_logger;
    octo_logger_init(logger, "httpfork");
    octo_logger_add_output(logger, LOG_DEBUG, stdout, true);
    octo_logger_add_output(logger, LOG_INFO, stdout, true);
    octo_logger_add_output(logger, LOG_WARN, stdout, true);
    octo_logger_add_output(logger, LOG_ERROR, stdout, true);
    octo_logger_info(logger, "octonaut forking http example");

    server_loop();

    return 0;
}
