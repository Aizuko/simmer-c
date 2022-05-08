#ifndef INIT_H
#define INIT_H

#include <stdint.h>
#include <stdlib.h>

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
};

struct state {
    const char *program_name;
    const char *version;

    const uint64_t columns;
    const uint64_t rows;

    enum mode mode;

    // Audio start and end time
    const uint64_t start_time;
    const uint64_t end_time;

    struct markers markers;

    struct mark *cursor_start;
    struct mark *cursor_end;

    uint64_t play_head;

    const struct keybinds keys;
};

// Creates a new state. TODO: Read commandline arguments for marks
struct state init_state();

#endif
