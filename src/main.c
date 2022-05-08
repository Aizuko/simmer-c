#include <stdio.h> // TODO: Remove?

#include "init/init.h"

int main()
{
    struct state State = init_state();

    printf("SegFaultless: %lu\r\n", State.cursor_start->time);
}
