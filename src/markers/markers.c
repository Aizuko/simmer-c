#include "markers.h"

static void swap(struct mark *m1, struct mark *m2)
{
    struct mark *tmp = m1;
    m1 = m2;
    m2 = tmp;
}

static void qsort_marks(struct mark **buffer, uint64_t len)
{
    if (len < 2) return;

    struct mark *pivot = buffer[len - 1];
    uint64_t low = 0;

    for (uint64_t high = 0; high < len - 1; high++) {
        if (buffer[high]->time < pivot->time) {
            low += 1;
            swap(buffer[low], buffer[high]);
        }
    }

    qsort_marks(buffer, low);
    qsort_marks(buffer + low, len - low - 1);

    low += 1;

    swap(buffer[low], pivot);
}

static void label_marks(struct markers *marks)
{
    qsort_marks(marks->buffer, marks->len);

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

