#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Assignment-2
 * Username: _______ (fill out your username here)
 * Class: UCSD CSE30-S224
 */


#define USAGE_STRING "usage: %s -c incols [-s] col1 [col2 ...]\n"
#define INCOLS_INVALID_RANGE "Err: incols must be >= 1 and <= %d\n"
#define INCOLS_MISSING "Err: input columns required\n"
#define OUTCOLS_MISSING "Err: at least one output column must be provided\n"
#define COL_OUT_OF_RANGE "Err: output column number must be >= 1 and <= %d\n"
#define TOO_MANY_COLS_ERR "Err: too many columns in row. Dropping row...\n"
#define TOO_FEW_COLS_ERR "Err: too few columns in row. Dropping row...\n"
#define STATS_STRING "%d valid lines\n%d dropped lines\nlongest field = %d characters\n"
#define MAX_COLS 100

void usage(char *argv0) {
    fprintf(stderr, USAGE_STRING, argv0);
}

int main(int argc, char **argv) {
    int incols = 0; // number of columns in input file
    int outcols = 0; // number of columns in output

    char *linebuf = NULL; // buffer for reading in a single line
    char **colptrs = NULL; // array of pointers to start of each col
    int *outcols_buf = NULL; // buffer for list of output cols (1-indexed)

    unsigned int linecnt = 0; // number of valid lines
    unsigned int dropcnt = 0; // number of invalid (dropped) lines
    unsigned int longest_field = 0; // longest field length
    int stats_flag = 0; // was -s provided
    int c_flag = 0; // was -c provided

    int c; // for reading options with getopt
    while ((c = getopt(argc, argv, "c:s")) != -1) {
        switch (c) {
            case 'c':
                c_flag = 1;
                incols = atoi(optarg);
                if (incols < 1 || incols > MAX_COLS) {
                    fprintf(stderr, INCOLS_INVALID_RANGE, MAX_COLS);
                    return EXIT_FAILURE;
                }
                break;
            case 's':
                stats_flag = 1;
                break;
            default:
                usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (!c_flag) {
        fprintf(stderr, INCOLS_MISSING);
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    outcols = argc - optind;
    if (outcols < 1) {
        fprintf(stderr, OUTCOLS_MISSING);
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    outcols_buf = malloc(outcols * sizeof(int));
    if (outcols_buf == NULL) {
        fprintf(stderr, "malloc failure\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < outcols; i++) {
        outcols_buf[i] = atoi(argv[optind + i]);
        if (outcols_buf[i] < 1 || outcols_buf[i] > incols) {
            fprintf(stderr, COL_OUT_OF_RANGE, incols);
            free(outcols_buf);
            return EXIT_FAILURE;
        }
    }

    colptrs = malloc(incols * sizeof(char *));
    if (colptrs == NULL) {
        fprintf(stderr, "malloc failure\n");
        free(outcols_buf);
        return EXIT_FAILURE;
    }

    size_t buffer_len = 0;
    ssize_t nread;
    while ((nread = getline(&linebuf, &buffer_len, stdin)) != -1) {
        int col_count = 0;
        char *p = linebuf;
        while (*p && col_count < incols) {
            while (*p && (*p == ' ' || *p == '\t')) p++;
            colptrs[col_count++] = p;
            while (*p && *p != ' ' && *p != '\t' && *p != '\n') p++;
            if (*p) *p++ = '\0';
        }

        if (col_count != incols) {
            if (col_count < incols) {
                fprintf(stderr, TOO_FEW_COLS_ERR);
            } else {
                fprintf(stderr, TOO_MANY_COLS_ERR);
            }
            dropcnt++;
            continue;
        }

        int longest_in_line = 0;
        for (int i = 0; i < col_count; i++) {
            int field_len = strlen(colptrs[i]);
            if (field_len > longest_in_line) {
                longest_in_line = field_len;
            }
        }

        if (longest_in_line > longest_field) {
            longest_field = longest_in_line;
        }

        for (int i = 0; i < outcols; i++) {
            printf("%s", colptrs[outcols_buf[i] - 1]);
            if (i < outcols - 1) printf(" ");
        }
        printf("\n");

        linecnt++;
    }

    if (stats_flag) {
        fprintf(stderr, STATS_STRING, linecnt, dropcnt, longest_field);
    }

    free(outcols_buf);
    free(colptrs);
    free(linebuf);

    return EXIT_SUCCESS;
}
