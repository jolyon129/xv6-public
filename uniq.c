#include "types.h"
#include "user.h"

#define MAX_BUF 512
#define NULL (void *)0
#define DEFAULT 0

char buf[MAX_BUF];
int argidx = 1;
/**
 * 1: normal
 * 2: count and group preﬁx lines by the number of occurrences
 * 3: only print duplicate lines
 */
int output_format = 1;
int ignore_case = 0;

int get_line(char *line_ptr, int max, int fd) {
    memset(line_ptr, DEFAULT, MAX_BUF);
//    int n;
    char *dst = line_ptr;
    while ((read(fd, buf, 1)) > 0) {
        if (buf[0] == '\n') {
            *dst++ = buf[0];
            break;
        } else {
            *dst++ = buf[0];
            if ((dst - line_ptr) + 1 > max) {
                break;
            }
        }

    }
    // If the last line doesn't have `\n`, add it manually
    if (*(dst - 1) != '\n') *dst = '\n';
    return dst - line_ptr;
}

uchar tolower(uchar ch) {
    if (ch >= 'A' && ch <= 'Z')
        ch = 'a' + (ch - 'A');
    return ch;
}

/**
 *  Comprea two case-insensentive strings
 * @param p
 * @param q
 * @return
 */
int strcicmp(const char *p, const char *q) {
    while (*p && tolower(*p) == tolower(*q))
        p++, q++;
    return (uchar) *p - (uchar) *q;
}

void output(int stdout, char *line, int count, int has_prefix) {
    if (has_prefix) {
        printf(stdout, "%d %s", count, line);
    } else {
        printf(stdout, "%s", line);
    }
}

void uniq(int fd, char *name) {
    char *prev = NULL, *cur = NULL;
    char *line_ptr = (char *) malloc(MAX_BUF * sizeof(char));
    int count = 0;
    while ((get_line(line_ptr, MAX_BUF, fd)) > 0) {
        if (prev == NULL) {
            prev = (char *) malloc(MAX_BUF * sizeof(char));
            cur = (char *) malloc(MAX_BUF * sizeof(char));
            memmove(prev, line_ptr, MAX_BUF);
            memmove(cur, line_ptr, MAX_BUF);
            count = 1;
            if (output_format == 1) {
                output(1, cur, count, 0);
            }
        } else {
            memmove(cur, line_ptr, MAX_BUF);
            int cmp_result;
            if (ignore_case) {
                cmp_result = strcicmp(cur, prev);
            } else {
                cmp_result = strcmp(cur, prev);
            }
            if (cmp_result == 0) {
                count++;
                if (ignore_case) {
                    memmove(cur, prev, MAX_BUF);
                }
            } else {
                if (output_format == 1) {
                    output(1, cur, count, 0);
                } else if (output_format == 3 && count > 1) {
                    output(1, prev, count, 0);
                } else if (output_format == 2) {
                    output(1, prev, count, 1);
                }
                count = 1;
            }
        }
        memmove(prev, cur, MAX_BUF);
    }
    if ((output_format == 3 && count > 1)) {
        output(1, cur, count, 0);
    } else if (output_format == 2) {
        output(1, cur, count, 1);
    }
    free(prev);
    free(cur);
    free(line_ptr);
}


/**
 *
 * @param argc
 * @param argv
 * @param opts
 *      Given a string of possible options
 * @return
 *      -1: reach the end
 *      -2: invalid option
 *      others: ASCII
 */
int get_next_opt(int argc, char *argv[], char *opts) {
    if (argidx >= argc - 1) {
        return -1;
    }
    char *arg = argv[argidx];
    if (arg[0] != '-' || (strchr(opts, arg[1]) == 0)) {
        printf(1, "uniq: invalid option %s\n", arg);
        return -2;
    } else {
        argidx++;
        return arg[1];
    }
}


int main(int argc, char *argv[]) {
    int fd;
    int c;
    char *options = "cdi";
    if (argc <= 1) {
        uniq(0, "");
        exit();
    } else if (argc == 2) {
        if ((fd = open(argv[1], 0)) < 0) {
            printf(1, "uniq: cannot open %s\n", argv[1]);
            exit();
        }
        uniq(fd, argv[1]);
        exit();
    } else if (argc >= 3) {
        while ((c = get_next_opt(argc, argv, options)) > 0) {
            switch (c) {
                case 'c':
                    output_format = 2;
                    break;
                case 'd':
                    output_format = 3;
                    break;
                case 'i':
                    ignore_case = 1;
                    break;
            }
        }
        if ((fd = open(argv[argc - 1], 0)) < 0) {
            printf(1, "uniq: cannot open %s\n", argv[1]);
            exit();
        }
        uniq(fd, argv[argc - 1]);
        exit();
    }
    exit();
}
