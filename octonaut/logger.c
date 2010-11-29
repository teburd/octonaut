#include "logger.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static char LVLSTR[][10]= {"DEBUG", "INFO ", "WARN ", "ERROR"};

#define RESET       0
#define BRIGHT      1
#define DIM         2
#define UNDERLINE   3
#define BLINK       4
#define REVERSE     7
#define HIDDEN      8

#define BLACK       0
#define RED         1
#define GREEN       2
#define YELLOW      3
#define BLUE        4
#define MAGENTA     5
#define CYAN        6
#define WHITE       7

void textcolor(FILE* stream, int attr, int fg, int bg)
{
    fprintf(stream, "%c[%d;%dm", 0x1B, attr, fg+30);
}

/**
 * initialize a octo_logger
 */
void octo_logger_init(octo_logger *lgr, const char *name)
{
    int i = 0;
    lgr->name = malloc(strlen(name) + 1);
    lgr->level = 0;
    strcpy(lgr->name, name);

    for(i = 0; i < 4; ++i)
    {
        INIT_LIST_HEAD(&lgr->outs[i].list);
    }
}

/**
 * destroy a octo_logger
 */
void octo_logger_destroy(octo_logger *lgr)
{
    struct list_head *pos, *q;
    int i = 0;

    for(i = 0; i < 4; ++i)
    {
        list_for_each_safe(pos, q, &lgr->outs[i].list)
        {
            octo_log_output *lo = list_entry(pos, octo_log_output, list);
            list_del(pos);
            fclose(lo->stream);
            free(lo);
        }
    }

    free(lgr->name);
}

/**
 * obtain a static octo_logger struct
 */
octo_logger* octo_logger_static()
{
    static octo_logger log;
    return &log;
}


/**
 * add a octo_logger output
 * @lgr: octo_logger to append an output to
 * @lvl: level the output should be used for
 * @fd: file descriptor to write to
 */
void octo_logger_add_output(octo_logger *lgr, octo_log_level level, int fd, bool colorize)
{
    octo_log_output *out = malloc(sizeof(octo_log_output));
    out->stream = fdopen(dup(fd), "a+");
    out->colorize = colorize;
    INIT_LIST_HEAD(&out->list);
    list_add(&out->list, &lgr->outs[level].list);
}

/**
 * get the current octo_logger filter level
 * @lgr: the octo_logger to get the level from
 */
octo_log_level octo_logger_level(octo_logger *lgr)
{
    return lgr->level;
}

/**
 * set the current octo_logger filter level
 * @lgr: octo_logger to set the filter level
 * @lvl: filter level
 */
void octo_logger_set_level(octo_logger *lgr, octo_log_level level)
{
    lgr->level = level;
}

/**
 * private function to log to a specific level
 * @lgr: octo_logger to log to
 * @lvl: level to log to
 * @where: string containing where the log message is coming from
 * @fmt: formatted log message
 * @args: args for formatted log message
 */
void octo_logger_log(octo_logger *lgr, octo_log_level level, const char *where, const char *fmt, ...)
{
    static char buf[80]; /* allocate a single one time buffer for time printouts */
    time_t now;
    struct tm *ts;
    struct list_head *pos, *q;
    va_list args;
    va_start(args, fmt);

    if(lgr->level <= level)
    {
        int color = YELLOW;
        now = time(NULL);
        ts = localtime(&now);
        strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", ts);

        list_for_each_safe(pos, q, &lgr->outs[level].list)
        {
            octo_log_output *lo = list_entry(pos, octo_log_output, list);
            

            if(lo->colorize)
            {
                fprintf(lo->stream, "[");
                textcolor(lo->stream, BRIGHT, MAGENTA, BLACK);
                fprintf(lo->stream, "%s", lgr->name);
                textcolor(lo->stream, RESET, WHITE, BLACK);
                fprintf(lo->stream, "]");

                fprintf(lo->stream, "[");
                if(level == LOG_DEBUG)
                    color = GREEN;
                else if(level == LOG_INFO)
                    color = CYAN;
                else if(level == LOG_WARN)
                    color = YELLOW;
                else if(level == LOG_ERROR)
                    color = RED;
                textcolor(lo->stream, BRIGHT, color, BLACK);
                fprintf(lo->stream, "%5s", LVLSTR[level]);
                textcolor(lo->stream, RESET, WHITE, BLACK);
                fprintf(lo->stream, " %18s]", where);
            }
            else
            {
                fprintf(lo->stream, "[%5s - %18s]", LVLSTR[level], where);
            }

            fprintf(lo->stream, " :: %s :: ", buf);
            vfprintf(lo->stream, fmt, args);
            fprintf(lo->stream, "\n");
            fflush(lo->stream);
        }
    }
    va_end(args);
}
