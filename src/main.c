#include <stdio.h>

#include "init/init.h"
#include "interface/interface.h"

const char *PROGRAM_NAME = "SimmerC";
const char *PROGRAM_VERSION = "0.1.0";

void print_help()
{
    printf("%s v%s\r\n"
           "Quickly trim audio from the comfort of your shell\r\n\r\n"
           "USAGE:\r\n"
           "\tsimmer [OPTIONS]\r\n\r\n"
           "OPTIONS:\r\n"
           "\t-m <milliseconds>\r\n"
           "\t\tSets a mark at given millisecond timestamp\r\n\r\n"
           "\t-s <milliseconds>\r\n"
           "\t\tPlaces start cursor and mark at given milliseconds\r\n\r\n"
           "\t-e <milliseconds>\r\n"
           "\t\tPlaces end cursor and mark at given milliseconds\r\n",
           PROGRAM_NAME, PROGRAM_VERSION);
}

void raw_terminal(struct termios *original) {
    tcgetattr(STDIN_FILENO, original);

    struct termios raw = *original;

    cfmakeraw(&raw);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(int argc, char **argv)
{
    struct termios original;

    struct state *State = malloc(sizeof(struct state));

    if (!init_state(State, argc, argv)) {
        print_help();
        exit(1);
    } else {
        raw_terminal(&original);
        draw_interface(State);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
    }
}
