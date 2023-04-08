#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 50

struct str_buffer {
    char *str;
    int size;
};

void add_to_buff(struct str_buffer *buf, char *s, int len)
{
    char *new = NULL;
    if ((buf->size + len) >= BUFSIZE) {
        printf("Buffer is over %d, re-allocating space\n", BUFSIZE);
        new = realloc(buf->str, buf->size + len);

        if (new == NULL) {
            printf("realloc error");
            exit(0);
        }

        memcpy(new + buf->size, s, len);
        buf->str = new;
        buf->size += len;

    } else {
        strcat(buf->str, s);
        buf->size += len;
    }

}

int 
main(int argc, char **argv)
{
    char *init_buff; 
    init_buff = malloc(BUFSIZE);
    if (init_buff == NULL) {
        printf("malloc error");
        exit(0);
    }

    struct str_buffer buff = {init_buff, 0};
    
    while (1) {
        printf("Enter string for buffer: \n");
        char msg[BUFSIZE];
        fgets(msg, BUFSIZE, stdin);
        if (msg == EOF) break;
        add_to_buff(&buff, msg, strlen(msg));
        printf("Buffer is now: %s\n", buff.str);
        printf("Buffer size is: %d\n", buff.size);
    }
}


