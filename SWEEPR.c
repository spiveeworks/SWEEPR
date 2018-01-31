#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned char byte;

int _tile(int width, int height, int x, int y) {
    x %= width;
    y %= height;

    return y * width + x;
}

#define tile(i, j) _tile(width, height, i, j)

void _mark(
    int width,
    int height,
    bool* marks,
    byte* adjacent_marks,
    int i,
    int j
) {
    marks[tile(i, j)] = true;
    for (int ip = -1; i < 2; i++)
        for (int jp = -1; j < 2; j++)
            adjacent_marks[tile(ip, jp)]++;
}

#define flag(i, j) _mark(width, height, mines, adjacent_mines, i, j)
#define clear(i, j) _mark(width, height, cleared, adjecent_cleared, i, j)

int rand_lim(int limit) {
/* return a random number between 0 and limit inclusive.
 * https://stackoverflow.com/questions/2999075/generate-a-random-number-within-range
 */

    int divisor = RAND_MAX/(limit+1);
    int retval;

    do {
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

bool random_bool(int true_freq, int false_freq) {
    int r = rand_lim(true_freq + false_freq);
    return r < true_freq;
}

int main() {
    int width = 10,
        height = 10,
        mine_num = 10,

        flag_num = mine_num,
        cleared_num = (width * height) - mine_num;

    bool* mines;
    bool* cleared;
    bool* flags;

    byte* adjacent_mines;
    byte* adjacent_cleared;
    byte* adjacent_flags;

    mines = (bool*)malloc(sizeof(bool) * width * height);
    cleared = (bool*)malloc(sizeof(bool) * width * height);
    flags = (bool*)malloc(sizeof(bool) * width * height);

    adjacent_mines = (byte*)malloc(sizeof(byte) * width * height);
    adjacent_cleared = (byte*)malloc(sizeof(byte) * width * height);
    adjacent_flags = (byte*)malloc(sizeof(byte) * width * height);

    for (int t = 0; t < width * height; t++) {
        mines[t] = false;
        adjacent_mines[t] = 0;

        cleared[t] = false;
        flags[t] = false;

        adjacent_cleared[t] = 0;
        adjacent_flags[t] = 0;
    }

    cleared_num -= 9;

    srand(time(NULL));
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            if (!((i < 3) && (j < 3)) && random_bool(flag_num, cleared_num)) {
                flag_num--;

                mines[tile(i, j)] = true;
                for (int ip = -1; i < 2; i++)
                    for (int jp = -1; j < 2; j++)
                        adjacent_mines[tile(ip, jp)]++;
            } else {
                cleared_num--;
            }
        }

    cleared[tile(1, 1)] = true;

    while (flag_num < mine_num) {
        for (int focus_i = 0; focus_i < width; focus_i++)
            for (int focus_j = 0; focus_j < height; focus_j++) {
                if (cleared[tile(focus_i, focus_j)]) {

                }
            }
    }
}



