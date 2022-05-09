#include "init.h"

#include <stdio.h>

// Returns true if a help flag was found or args could not be parsed
static bool parse_args(struct state *State, int argc, char **argv)
{
    int option;
    uint64_t time;

    while ((option = getopt(argc, argv, ":hm:e:s:")) != -1) {
        switch (option) {
        case 's':
            printf("Entering s arg\r\n");
            time = (uint64_t) atoll(optarg);
            printf("here1\r\n");
            State->cursor_start->time = time;
            printf("here2 %ld\r\n", time);
            push_mark(State->markers, time);
            printf("here3\r\n");
            break;
        case 'e':
            time = (uint64_t) atoll(optarg);
            State->cursor_end->time = time;
            push_mark(State->markers, time);
            break;
        case 'm':
            time = (uint64_t) atoll(optarg);
            push_mark(State->markers, time);
            break;
        case 'h':
        case ':':
        case '?':
        default:
            return true;
        }
    }

    return false;
}

bool init_state(struct state *State, int argc, char **argv)
{
    State->program_name = PROGRAM_NAME;
    State->version = PROGRAM_VERSION;

    * (char *)&State->columns = 80;
    * (char *)&State->rows = 24;

    State->mode = Normal;

    // Audio start and end time
    *(uint64_t *)&State->start_time = 0;
    *(uint64_t *)&State->end_time = 10000; // TODO: Get ffmpeg to read this

    State->markers = new_markers(78, State->end_time);

    State->cursor_start = new_mark(300);
    State->cursor_end = new_mark(900);

    State->play_head = 0;

    *(struct keybinds *)&State->keys = (struct keybinds) {
        .exit = 'q',
        .jump_start = 'j',
        .jump_end = 'e',
    };

    if (parse_args(State, argc, argv))
        return false;
    return true;
}
