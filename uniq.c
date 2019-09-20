#include "types.h"
#include "user.h"

#define MAX_BUF 1024
#define NULL (void *)0

char buf[MAX_BUF];

char *getline(char *lineptr, int max, int fd) {
    int i, n;
    char *dst = lineptr;
    int stop = 0;
    while ((n = read(fd, buf, 1) > 0) && stop != 1) {
        for (i = 0; i < n; i++) {
            if (buf[i] == '\n') {
                *lineptr = buf[i];
                break;
            } else {
                *lineptr++ = buf[i];
                if ((lineptr - dst)+1>max){
                    stop = 1;
                    break;
                }

            }
        }
    }
    return dst;
}


void uniq(int fd, char *name) {
    int i, n;
    int start_i;
    int line_len;
//    int cur_line_is_end;
//    int resume_line = 0;
    char *prev = NULL, *cur = NULL;
    char *tmp = NULL;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        start_i = 0;
        i = 0;
        for (; i < n; i++) {
            if (buf[i] == '\n') {
                line_len = i - start_i + 1;
                tmp = &buf[start_i];
                if (!prev) {
                    prev = (char *) malloc(MAX_BUF * sizeof(char));
                    cur = (char *) malloc(MAX_BUF * sizeof(char));
                    memmove(prev, tmp, line_len * sizeof(char));
                    memmove(cur, tmp, line_len * sizeof(char));
                    printf(1, prev);
                } else {
                    cur = (char *) malloc(MAX_BUF * sizeof(char));
                    memmove(cur, tmp, line_len * sizeof(char));
                    if (strcmp(prev, cur) != 0) {
                        printf(1, cur);
                    }
                }
                memmove(prev, cur, line_len * sizeof(char));
                free(cur);
                // Point to the next i
                start_i = i + 1;
            }
        }


    }
    free(prev);
    free(cur);
}


int main(int argc, char *argv[]) {
    int fd;
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
    }
    exit();
}
