#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdio.h>

#define KEYPRESS(k) ((k) & 0x1f) /* strip 6, 8 bits off */
struct termios orig_termios;

struct editor_config {
    struct termios orig_termios;
    int screen_rows;
    int screen_cols;
};

struct editor_config e;

void sys_error(char* error_type) {
    write(STDOUT_FILENO, "\x1b[2J", 4); /* clear screen */
    write(STDOUT_FILENO, "\x1b[H", 3); /* move cursor to top left corner */

    perror(error_type);
    exit(EXIT_FAILURE);
}

void disable_raw_mode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &e.orig_termios) == -1)
        sys_error("tcsetttr");
}

char editor_readkey()
{
    int nread;
    char c;
    nread = read(STDIN_FILENO, &c, 1);

    if ((nread == -1) && errno == EAGAIN) sys_error("read");
    return c;
}

int get_window_size(int *rows, int *cols)
{
    struct winsize ws;
    if (1 || ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) { /* can assign a zero value so have to check for it*/
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
            return -1;
        editor_readkey();
        return -1;
    } else {
        *rows = ws.ws_row;
        *cols = ws.ws_col;
        return 0;
    }
}

struct abuff {
    char *buff;
    int len;
};

#define INIT_ABUFF {NULL, 0}

void append_abuff(struct abuff *ab, const char *s, int len)
{
    char *new = realloc(ab->buff, ab->len + len);
    if (new == NULL)
        sys_error("realloc");

    memcpy(new + ab->len, s, len);
    ab->buff = new;
    ab->len += len;
};


void init_editor()
{
    if (get_window_size(&e.screen_rows, &e.screen_cols) == -1)
        sys_error("ioctl");
}

void enable_raw_mode()
{

    if (tcgetattr(STDIN_FILENO, &e.orig_termios) == -1)
        sys_error("tcsetttr");

    atexit(disable_raw_mode);
    
    struct termios raw;
    raw = e.orig_termios;

    /*
    Flags to control how the terminal is used in this
    program
    */
    raw.c_iflag &= ~(IXON | ICRNL); /* turn off ctrl-[sq]*/
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); /* flip all bits to not effect other flags. Bitwise and to turn this bit off*/
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        sys_error("tcsetttr");

}


void editor_processkey()
{
    char c; 
    c = editor_readkey();
    switch (c) {
        case KEYPRESS('q'):
            write(STDOUT_FILENO, "\x1b[2J", 4); /* clear screen */
            write(STDOUT_FILENO, "\x1b[H", 3); /* move cursor to top left corner */
            exit(0);
            break;
    }
}

void editor_draw_rows()
{   
    char chars[] = "~\r\n";
    for (int y = 0; y < e.screen_rows; y++) {
        write(STDOUT_FILENO, &chars, 3);
    }
}

void editor_refresh_screen() {
    /* using escape sequences vt100 */
    write(STDOUT_FILENO, "\x1b[2J", 4); /* clear screen */
    write(STDOUT_FILENO, "\x1b[H", 3); /* move cursor to top left corner */
    
    editor_draw_rows();

    write(STDOUT_FILENO, "\x1b[H", 3); /* move cursor to top left corner */

}


int
main()
{   
    enable_raw_mode();
    init_editor();

    while (1) {
        editor_refresh_screen();
        editor_processkey();
    }
    
    return 0;
}
