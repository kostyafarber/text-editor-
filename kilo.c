#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <stdio.h>

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


int
main(int argc, char **argv)
{   
    enable_raw_mode();
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if (iscntrl(c)) {
            printf("%d\n", c);
        } else {
            printf("%d: ('%c')\n", c, c);
        }
    }

    return 0;
}
