#include "markers.h"

static void swap(struct mark **buffer, uint64_t s1, uint64_t s2)
{
    struct mark *tmp = buffer[s1];
    buffer[s1] = buffer[s2];
    buffer[s2] = tmp;
}

static void isort_marks(struct markers *marks)
{
    if (marks->len <= 1)
        return;

    for (uint64_t i = 1; i < marks->len; i++) {
        for (int64_t j = (int64_t) i-1; j >= 0; j--) {
            if (marks->buffer[j]->time > marks->buffer[j+1]->time)
                swap(marks->buffer, j, j+1);
            else
                break;
        }
    }
}

static void label_marks(struct markers *marks)
{
    isort_marks(marks);

    struct mark *mark;
    int64_t curr, prev = -1;
    char label = 97;

    for (uint64_t i = 0; i < marks->len; i++) {
        mark = marks->buffer[i];
        curr = (int64_t) nearest_pos(marks, mark->time);

        if (prev != -1 && curr == prev) {
            free(marks->buffer[i]);
            marks->len -= 1;
            for (uint64_t j = i; j < marks->len; j++)
                marks->buffer[j] = marks->buffer[j+1];
        } else {
            mark->label = label;
            mark->pos = curr;
            label += 1;
            prev = curr;
        }
    }
}

/*─────────────────────────────────────────────────────────────────────────────╗
│ Pμblic iητεrfαcε fδr mαrks                                                   |
╚─────────────────────────────────────────────────────────────────────────────*/
struct mark *new_mark(uint64_t time)
{
    struct mark *mark = malloc(sizeof(struct mark));
    mark->time = time;
    return mark;
}

uint64_t nearest_pos_m(uint64_t weight, uint64_t positions, uint64_t max)
{
    if (weight >= max) return positions - 1;

    return (uint64_t) ((double) weight * (double) positions / (double) max);
}

/*─────────────────────────────────────────────────────────────────────────────╗
│ Pμblic iητεrfαcε fδr mαrkεrs cδllεcτiδη                                      |
╚─────────────────────────────────────────────────────────────────────────────*/
struct markers *new_markers(uint64_t label_count, uint64_t maximum)
{
    struct markers *marks = malloc(sizeof(struct markers));

    marks->buffer = calloc(sizeof(struct mark *), label_count);
    marks->len = 0;
    marks->label_count = label_count;
    marks->maximum = maximum;

    return marks;
}

bool push_mark(struct markers *marks, uint64_t time)
{
    label_marks(marks);

    uint64_t pos, curr;
    curr = nearest_pos(marks, time);

    for (uint64_t i = 0; i < marks->len; i++) {
        pos = nearest_pos(marks, marks->buffer[i]->time);

        if (curr == pos) {
            marks->buffer[i]->time = time;
            return false;
        }
    }

    marks->buffer[marks->len] = new_mark(time);
    marks->len += 1;

    label_marks(marks);
    return true;
}

bool remove_mark(struct markers *marks, uint64_t time)
{
    for (uint64_t i = 0; i < marks->len; i++) {
        if (marks->buffer[i]->time == time) {
            free(marks->buffer[i]);
            marks->len -= 1;

            for (uint64_t j = i; j < marks->len; j++)
                marks->buffer[j] = marks->buffer[j+1];
            return true;
        }
    }
    return false;
}

void drain_markers(struct markers *marks)
{
    for (uint64_t i = 0; i < marks->len; i++)
        free(marks->buffer[i]);
    marks->len = 0;
}

uint64_t nearest_pos(struct markers *marks, uint64_t time)
{
    return nearest_pos_m(time, marks->label_count, marks->maximum);
}

