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

// Set terminal into raw mode
void raw_terminal(struct termios *original) {
    tcgetattr(STDIN_FILENO, original);

    struct termios raw = *original;

    cfmakeraw(&raw);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Moves the start or end cursor by the given amount
void move_cursor(bool is_start, struct state *State, int64_t amount)
{
    struct mark *mark = is_start ? State->cursor_start : State->cursor_end;

    int64_t curr = (int64_t) mark->time;
    int64_t new = curr + amount;

    if (is_start && (int64_t) State->cursor_end->time < new)
        mark->time = State->cursor_end->time;
    else if (!is_start && new < (int64_t) State->cursor_start->time)
        mark->time = State->cursor_start->time;
    else if (new < 0)
        mark->time = 0;
    else if (new > (int64_t) State->end_time)
        mark->time = State->end_time;
    else
        mark->time = new;

    mark->pos = nearest_pos_m(mark->time, State->columns - 2, State->end_time);
}

// Main event loop. Responds to all key presses as the program runs
void event_loop(struct state *State)
{
    draw_interface(State);

    char key;
    struct keybinds k = State->keys;

    while (read(STDIN_FILENO, &key, 1) != -1) {
        eval_mode: switch (State->mode) {
        case Normal:
            if (key == k.exit) {
                State->mode = Exiting;
                goto eval_mode;
            } else if (key == k.start_left) {
                move_cursor(true, State, -1 * State->move_amount);
            } else if (key == k.start_right) {
                move_cursor(true, State, State->move_amount);
            } else if (key == k.end_left) {
                move_cursor(false, State, -1 * State->move_amount);
            } else if (key == k.end_right) {
                move_cursor(false, State, State->move_amount);
            }
            break;
        case JumpStart:
            break;
        case JumpEnd:
            break;
        case Exiting:
            return;
        }

        draw_interface(State);
    }
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
        event_loop(State);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
    }
}
