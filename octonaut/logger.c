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
    lgr->level = 0;
    strncpy(lgr->name, name, sizeof(lgr->name)-1);

    for(i = 0; i < 4; ++i)
    {
        octo_list_init(&lgr->outs[i]);
    }
}

/**
 * destroy a octo_logger
 */
void octo_logger_destroy(octo_logger *lgr)
{
    int i = 0;

    for(i = 0; i < 4; ++i)
    {
        octo_log_output *output;
        octo_log_output *next;

        octo_list_foreach(output, next, &lgr->outs[i], list)
        {
            octo_list_remove(&output->list);
            fclose(output->stream);
            free(output);
        }

    }
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
 * @stream: FILE* stream to write to
 */
void octo_logger_add_output(octo_logger *lgr, octo_log_level level, FILE* stream, bool colorize)
{
    octo_log_output *out = malloc(sizeof(octo_log_output));
    out->stream = stream;
    out->colorize = colorize;
    octo_list_init(&out->list);
    octo_list_add(&lgr->outs[level], &out->list);
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
    va_list args;
    va_start(args, fmt);

    if(lgr->level <= level)
    {
        int color = YELLOW;
        now = time(NULL);
        ts = localtime(&now);
        strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", ts);
    
        octo_log_output *output;
        octo_log_output *next;
        octo_list_foreach(output, next, &lgr->outs[level], list)
        {
            if(output->colorize)
            {
                fprintf(output->stream, "[");
                textcolor(output->stream, BRIGHT, MAGENTA, BLACK);
                fprintf(output->stream, "%s", lgr->name);
                textcolor(output->stream, RESET, WHITE, BLACK);
                fprintf(output->stream, "]");

                fprintf(output->stream, "[");
                if(level == LOG_DEBUG)
                    color = GREEN;
                else if(level == LOG_INFO)
                    color = CYAN;
                else if(level == LOG_WARN)
                    color = YELLOW;
                else if(level == LOG_ERROR)
                    color = RED;
                textcolor(output->stream, BRIGHT, color, BLACK);
                fprintf(output->stream, "%5s", LVLSTR[level]);
                textcolor(output->stream, RESET, WHITE, BLACK);
                fprintf(output->stream, " %18s]", where);
            }
            else
            {
                fprintf(output->stream, "[%5s - %18s]", LVLSTR[level], where);
            }

            fprintf(output->stream, " :: %s :: ", buf);
            vfprintf(output->stream, fmt, args);
            fprintf(output->stream, "\n");
            fflush(output->stream);
        }
    }
    va_end(args);
}
