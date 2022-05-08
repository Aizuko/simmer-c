#include "init.h"

struct state init_state()
{
    struct state State = {
        .program_name = "SimmerC",
        .version = "0.1.0",

        .columns = 80,
        .rows = 24,

        .mode = Normal,

        // Audio start and end time
        .start_time = 0,
        .end_time = 10000,  // TODO: Get ffmpeg to read this

        .markers = new_markers(78, 10000),

        .cursor_start = new_mark(300),
        .cursor_end = new_mark(900),

        .play_head = 0,

        .keys = {
            .exit = 'q',
            .jump_start = 'j',
            .jump_end = 'e',
        },
    };

    return State;
}
