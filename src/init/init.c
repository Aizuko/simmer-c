#include "init.h"

// Returns true if a help flag was found or args could not be parsed
static bool parse_args(struct state *State, int argc, char **argv)
{
    int option;
    uint64_t time;

    while ((option = getopt(argc, argv, ":hm:e:s:")) != -1) {
        switch (option) {
        case 's':
            time = (uint64_t) atoll(optarg);
            State->cursor_start->time = time;
            push_mark(State->markers, time);
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
    State->title = PROGRAM_NAME;
    State->version = PROGRAM_VERSION;

    * (char *)&State->columns = 80;
    * (char *)&State->rows = 24;

    State->mode = Normal;

    // Audio start and end time
    *(uint64_t *)&State->start_time = 0;
    *(uint64_t *)&State->end_time = 10000; // TODO: Get ffmpeg to read this

    State->markers = new_markers(78, State->end_time);

    // TODO: Don't hardcode cursor times
    struct mark *start = new_mark(300);
    start->pos = nearest_pos_m(start->time, State->columns - 2, State->end_time);
    start->label = '<';
    State->cursor_start = start;

    struct mark *end = new_mark(3104);
    end->pos = nearest_pos_m(end->time, State->columns - 2, State->end_time);
    end->label = '>';
    State->cursor_end = end;

    State->play_head = 0;

    State->move_amount = 12;

    *(struct keybinds *)&State->keys = (struct keybinds) {
        .exit = 'Q',

        .jump_start = 'j',
        .jump_end = 'k',
        .set_mark_start = '1',
        .set_mark_end = '2',

        .start_left = 'q',
        .start_right = 'w',
        .end_left = 'e',
        .end_right = 'r',

        .trim = 'R',
    };

    if (parse_args(State, argc, argv))
        return false;
    return true;
}


void set_raw(struct termios *raw)
{
    cfmakeraw(raw);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, raw);
}
