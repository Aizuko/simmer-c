#ifndef INIT_H
#define INIT_H

extern const char *PROGRAM_NAME;
extern const char *PROGRAM_VERSION;

#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>

#include "../markers/markers.h"

#define CTRL_KEY(k) ((k) & 0x1f)

enum mode {
    Normal,
    JumpStart,
    JumpEnd,
    Exiting,
};

struct keybinds {
    char exit;
    char jump_start;
    char jump_end;

    char start_left;
    char start_right;
    char end_left;
    char end_right;

    char trim;
};

struct state {
    const char *title;
    const char *version;

    const uint64_t columns;
    const uint64_t rows;

    enum mode mode;

    // Audio start and end time
    const uint64_t start_time;
    const uint64_t end_time;

    struct markers *markers;

    struct mark *cursor_start;
    struct mark *cursor_end;

    uint64_t play_head;

    const struct keybinds keys;
    int64_t move_amount;
};


// Creates a new state. TODO: Read commandline arguments for marks
bool init_state(struct state *State, int argc, char **argv);

// Sets the terminal into raw mode
void set_raw(struct termios *raw);

#endif
