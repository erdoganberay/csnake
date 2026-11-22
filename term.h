// term.h
#ifndef TERM_H
#define TERM_H

#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>

static struct termios old_termios, new_termios;
static struct winsize w;
static int exit_loop;

static void reset_terminal(void) {
    write(STDOUT_FILENO, "\x1b[?25h", 6);
    write(STDOUT_FILENO, "\x1b[?1049l", 8);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_termios);
}

static void configure_terminal(void) {
    tcgetattr(STDIN_FILENO, &old_termios);
    new_termios = old_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_termios);

    write(STDOUT_FILENO, "\x1b[?1049h", 8);
    write(STDOUT_FILENO, "\x1b[?25l", 6);
    write(STDOUT_FILENO, "\x1b[H", 3);
    atexit(reset_terminal);
}
static unsigned short get_term_col(void){
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}
static unsigned short get_term_row(void) {
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}

static void signal_handler(__attribute__((unused)) int signum) {  
    exit_loop = 1;
}
#endif
