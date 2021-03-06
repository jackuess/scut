#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>

#include "scut.h"

char *t_error_message(const char *file_name,
                      long line_number,
                      const char *test_name,
                      const char *first,
                      const char *second) {
    static const char *format = "%s: `%s` doesn't equal `%s`\n\t%s:%ld";
    size_t message_len = strlen(format);
    char *message = malloc(message_len + 1);

    int required_len = snprintf(message, message_len, format, test_name, first, second, file_name, line_number);
    if (required_len >= 0 && (unsigned int)required_len > message_len) {
        message = realloc(message, required_len + 1);
        snprintf(message, required_len + 1, format, test_name, first, second, file_name, line_number);
    }
    return message;
}

struct TestResult *t_run(size_t n_test_functions,
                         void (**test_functions)(struct TestResult *)) {
    struct TestResult *tr = calloc(1, sizeof *tr);
    for (size_t i = 0; i < n_test_functions; i++) {
        test_functions[i](tr);
        tr->n_tests++;
    };
    return tr;
}

void tr_append_error_message(struct TestResult *tr, char *message) {
    if (tr->n_error_messages < MAX_ERROR_MESSAGES) {
        tr->error_messages[tr->n_error_messages++] = message;
    }
}

void tr_print(const struct TestResult *tr) {
    for (unsigned int i = 0; i < tr->n_error_messages; i++) {
        printf("%s\n", tr->error_messages[i]);
    }
    printf("--\nCompleted %zu assertion%s in %zu test%s in XXs\n",
           tr->n_assertions,
           tr->n_assertions == 1 ? "" : "s",
           tr->n_tests,
           tr->n_tests == 1 ? "" : "s");
    printf("%u test%s failed!\n",
           tr->n_error_messages,
           tr->n_error_messages == 1 ? "" : "s");
    printf("%s\n", tr->n_error_messages == 0 ? "Pass" : "Faiure");
}

_Bool tr_success(const struct TestResult *tr) {
    return tr->n_error_messages == 0;
}

void tr_free(struct TestResult *tr) {
    for (unsigned int i = 0; i < tr->n_error_messages; i++) {
        free(tr->error_messages[i]);
    }
    free(tr);
}
