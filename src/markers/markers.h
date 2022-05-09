#ifndef MARKERS_H
#define MARKERS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// A mark on the timeline, delimited by a letter
struct mark {
    uint64_t time;
    uint64_t pos;
    char label;
};

// Creates a new mark at given time
struct mark *new_mark(uint64_t time);

// Finds the nearest integral position of a mark. Similiar to the progress
// position of a loading bar
// Return: Nearest position, starting at index 0
uint64_t nearest_pos_m(uint64_t weight, uint64_t positions, uint64_t max);

// A collection of marks
struct markers {
    struct mark **buffer;
    uint64_t len;
    uint64_t label_count;
    uint64_t maximum;
};

// Returns a new empty collection of marks
struct markers *new_markers(uint64_t label_count, uint64_t maximum);

// Adds a new mark to the collection. The new mark will overwrite any old mark
// with the same position
// Returns: True when the mark didn't overwrite any other mark
bool push_mark(struct markers *marks, uint64_t time);

// Removes a mark with the given time from the collection
// Returns: True if the marks was removed, false if it wasn't found
bool remove_mark(struct markers *marks, uint64_t time);

// Removes all the marks in the collection
void drain_markers(struct markers *marks);

// Find the nearest position of a time in a collection of marks
uint64_t nearest_pos(struct markers *marks, uint64_t time);

#endif
