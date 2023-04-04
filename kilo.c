#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>

#define KEYPRESS(k) ((k) & 0x1f)
struct termios orig_termios;

void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);
    
    struct termios raw;
    raw = orig_termios;

    /*
    Flags to control how the terminal is used in this
    program
    */
    raw.c_iflag &= ~(IXON | ICRNL); /* turn off ctrl-[sq]*/
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); /* flip all bits to not effect other flags. Bitwise and to turn this bit off*/
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

}

char editor_readkey()
{
    int nread;
    char c;
    nread = read(STDIN_FILENO, &c, 1);

    if ((nread == -1) && errno == EAGAIN) exit(0);
    return c;
}

void editor_processkey()
{
    char c; 
    c = editor_readkey();
    switch (c) {
        case KEYPRESS('q'):
            exit(0);
            break;
    }
}

void editor_refresh_screen() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
}

int
main()
{   
    enable_raw_mode();
    while (1) {
        editor_refresh_screen();
        editor_processkey();
    }
    
    return 0;
}
