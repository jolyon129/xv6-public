#include "types.h"
#include "user.h"

#define MAX_BUF 1024
#define NULL (void *)0
#define DEFAULT 0

char buf[MAX_BUF];

int get_line(char *line_ptr, int max, int fd) {
    memset(line_ptr, DEFAULT, MAX_BUF);
    char *dst = line_ptr;
    while ((read(fd, buf, 1) > 0)) {
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
    return dst - line_ptr;
}


void uniq(int fd, char *name) {
    char *prev = NULL, *cur = NULL;
    char *line_ptr = (char *) malloc(MAX_BUF * sizeof(char));
    while ((get_line(line_ptr, MAX_BUF, fd)) > 0) {
        if (prev == NULL) {
            prev = (char *) malloc(MAX_BUF * sizeof(char));
            cur = (char *) malloc(MAX_BUF * sizeof(char));
            memmove(prev, line_ptr, MAX_BUF);
            memmove(cur, line_ptr, MAX_BUF);
            printf(1, cur);
        } else {
            memmove(cur, line_ptr, MAX_BUF);
            if (strcmp(cur, prev) != 0) {
                printf(1, cur);
            }
        }
        memmove(prev, cur, MAX_BUF);
    }
    free(prev);
    free(cur);
    free(line_ptr);
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
    } else if(argc == 3){

    }
    exit();
}
