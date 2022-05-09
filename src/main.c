#include <stdio.h>

#include "init/init.h"

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

int main(int argc, char **argv)
{
    struct state *State = malloc(sizeof(struct state));

    if (!init_state(State, argc, argv)) {
        print_help();
        exit(1);
    } else {
        struct mark *mark;
        for (uint64_t i = 0; i < State->markers->len; i++) {
            mark = State->markers->buffer[i];
            printf("%c @ %lu\r\n", mark->label, mark->time);
        }
    }

    printf("SegFaultless: %lu\r\n", State->cursor_start->time);
}
