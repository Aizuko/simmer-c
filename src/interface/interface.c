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
    char label[2] = "\0";
    uint64_t rows = 4, cols = State->columns;

    char *buffer[rows][cols];

    for (uint64_t i = 0; i < rows * cols; i++) {
        buffer[0][i] = malloc(sizeof(char) * 3);
        strcpy(buffer[0][i], " ");
    }

    // Cursors and their stems
    for (uint64_t i = 0; i < cols - 2; i++) {
        if (i == State->cursor_start->pos) {
            label[0] = State->cursor_start->label;
            strcpy(buffer[0][i+1], label);
            strcpy(buffer[1][i+1], stem);
            strcpy(buffer[2][i+1], stem);
            strcpy(buffer[3][i+1], stem);
        } else if (i == State->cursor_end->pos) {
            label[0] = State->cursor_end->label;
            strcpy(buffer[0][i+1], label);
            strcpy(buffer[1][i+1], stem);
            strcpy(buffer[2][i+1], stem);
            strcpy(buffer[3][i+1], stem);
        }
    }

    // Labels for marks
    strcpy(buffer[2][0], "0");
    strcpy(buffer[3][0], stem);
    strcpy(buffer[2][State->columns - 1], "$");
    strcpy(buffer[3][State->columns - 1], stem);

    uint64_t index = 0;
    struct markers *marks = State->markers;

    for (uint64_t i = 0; i < cols - 2; i++) {
        if (marks->len > index && marks->buffer[index]->pos == i) {
            label[0] = marks->buffer[index]->label;
            strcpy(buffer[2][i+1], label);
            strcpy(buffer[3][i+1], stem);
            index += 1;
        }
    }

    char *flush = calloc(sizeof(char), cols * 4 + 1);

    for (uint64_t i = 0; i < rows; i++) {
        for (uint64_t j = 0; j < cols; j++) {
            strncat(flush, buffer[i][j], 4);
            free(buffer[i][j]);
        }
        printf("%s\r\n", flush);

        for (uint64_t j = 0; j < cols * 2; j++)
            flush[j] = '\0';
    }
    free(flush);
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
