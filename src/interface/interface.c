#include <string.h>
#include <stdio.h>

#include "interface.h"

static const char *fill_block = "█";
static const char *void_block = "░";
static const char *stem = "│";

static char *format_milli(uint64_t milli)
{
    uint8_t hour = 0, mins = 0, secs = 0;

    while (milli > 3600*1000) { hour++; milli -= 3600*1000; }
    while (milli >   60*1000) { mins++; milli -=   60*1000; }
    while (milli >      1000) { secs++; milli -=      1000; }

    char *fmt = calloc(sizeof(char), 14);
    sprintf(fmt, "%02d:%02d:%02d.%03d", hour, mins, secs, (uint8_t) milli);
    return fmt;
}

static void print_title(const struct state *State)
{
    int title_len, padding_left;

    title_len = strlen(State->title) + strlen(State->version) + 2;
    padding_left = (int) ((State->columns - title_len) / 2) + strlen(State->title);

    printf("%*s v%s\r\n\r\n", padding_left, State->title, State->version);
}

static void print_playtime(const struct state *State)
{
    char *start = format_milli(State->cursor_start->time);
    char *play = format_milli(State->play_head);
    char *end = format_milli(State->cursor_end->time);

    printf("\
                ┌──────────────┬──────────────┬──────────────┐\r\n\
                │  Start Time  │  Play  Head  │   End Time   │\r\n\
                ├──────────────┼──────────────┼──────────────┤\r\n\
                │"  " %12s "  "│"  " %12s "  "│"  " %12s "  "│\r\n\
                └──────────────┴──────────────┴──────────────┘\r\n",
            start, play, end);

    free(start);
    free(play);
    free(end);
}

static void print_timeline(const struct state *State)
{
    uint64_t bar = State->columns - 2;

    // Labels for marks
    printf("0");

    uint64_t index = 0;
    struct markers *marks = State->markers;

    for (uint64_t i = 0; i < bar; i++) {
        if (marks->len > index && marks->buffer[index]->pos == i) {
            printf("%c", marks->buffer[index]->label);
            index += 1;
        } else {
            printf(" ");
        }
    }

    printf("$\r\n%s", stem);

    // Stems for marks
    index = 0;

    for (uint64_t i = 0; i < bar; i++) {
        if (marks->len > index && marks->buffer[index]->pos == i) {
            printf("%s", stem);
            index += 1;
        } else {
            printf(" ");
        }
    }

    printf("%s\r\n", stem);
}

static void print_bar(const struct state *State)
{
    printf("%s", fill_block);

    uint64_t start = State->cursor_start->pos;
    uint64_t end   = State->cursor_end->pos;
    uint64_t bar   = State->columns - 2;

    for (uint64_t i = 0; i < start; i++)       printf("%s", void_block);
    for (uint64_t i = 0; i < end - start; i++) printf("%s", fill_block);
    for (uint64_t i = 0; i < bar - end; i++)   printf("%s", void_block);

    printf("%s\r\n", fill_block);
}

static void print_times(const struct state *State)
{
    char *start_time   = format_milli(State->start_time);
    char *cursor_start = format_milli(State->cursor_start->time);
    char *cursor_end   = format_milli(State->cursor_end->time);
    char *end_time     = format_milli(State->end_time);

    printf("%12s              %12s -> %12s              %12s\r",
        start_time, cursor_start, cursor_end, end_time);

    free(start_time);
    free(cursor_start);
    free(cursor_end);
    free(end_time);
}

static void clear_screen()
{
    write(STDOUT_FILENO, "\x1b[2J", 4);    // Clear screen
    write(STDOUT_FILENO, "\x1b[1;1H", 6);  // Move cursor to top left
}

/*─────────────────────────────────────────────────────────────────────────────╗
│ Pμblic iητεrfαcε sταrτs hεrε                                                 |
╚─────────────────────────────────────────────────────────────────────────────*/
void draw_interface(const struct state *State)
{
    clear_screen();

    print_title(State);
    print_playtime(State);
    print_timeline(State);
    print_bar(State);
    print_times(State);
}
