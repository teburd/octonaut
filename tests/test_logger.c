#include "logger.h"

START_TEST (test_octo_logger)
{
    octo_logger logger;
    octo_logger_init(&logger, "octonaut");
    octo_logger_add_output(&logger, LOG_ERROR, fileno(stdout), true);
    octo_logger_add_output(&logger, LOG_WARN, fileno(stdout), true);
    octo_logger_add_output(&logger, LOG_INFO, fileno(stdout), true);
    octo_logger_add_output(&logger, LOG_DEBUG, fileno(stdout), true);
    octo_logger_debug(&logger, "log debug");
    octo_logger_info(&logger, "log info");
    octo_logger_warn(&logger, "log warn");
    octo_logger_error(&logger, "log error");
    octo_logger_destroy(&logger);
}
END_TEST

TCase* octo_logger_tcase()
{
    TCase* tc_octo_logger= tcase_create("octo_logger");
    tcase_add_test(tc_octo_logger, test_octo_logger);
    return tc_octo_logger;
}
