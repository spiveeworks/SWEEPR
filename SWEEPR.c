#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned char byte;

int _tile(int width, int height, int x, int y) {
    // because C handles (%) stupidly
    x += width;
    y += height;
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
    for (int ip = -1; ip <= 1; ip++)
        for (int jp = -1; jp <= 1; jp++)
            adjacent_marks[tile(i+ip, j+jp)]++;
}

#define place_mine(i, j) _mark(width, height, mines, adjacent_mines, i, j)
#define flag(i, j) _mark(width, height, flags, adjacent_flags, i, j)
#define clear(i, j) _mark(width, height, cleared, adjacent_cleared, i, j)

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
            if (i < 3 && j < 3) {
                // we clear these separately
            } else if (random_bool(flag_num, cleared_num)) {
                flag_num--;

                place_mine(i, j);
            } else {
                cleared_num--;
            }
        }

    clear(1, 1);

    bool made_progress = true;
    while (made_progress && flag_num < mine_num) {
        made_progress = false;
        for (int focus_i = 0; focus_i < width; focus_i++)
            for (int focus_j = 0; focus_j < height; focus_j++) {
                int t = tile(focus_i, focus_j);
                if (cleared[t]) {
                    bool clear_strat = adjacent_flags[t] == adjacent_mines[t];
                    bool flag_strat =
                        adjacent_cleared[t] + adjacent_mines[t] == 9;
                    if (flag_strat && clear_strat) {
                        // if both strategies are possible,
                        // then they have already been done.
                        continue;
                    }
                    if(flag_strat) {
                        // do flag strat
                        for (int ip = -1; ip <= 1; ip++)
                            for (int jp = -1; jp <= 1; jp++) {
                                int tp = tile(focus_i+ip, focus_j+jp);
                                if (!cleared[tp] && !flags[tp]) {
                                    flag(focus_i+ip, focus_j+jp);
                                    flag_num++;
                                }
                            }
                        made_progress = true;
                        continue;
                    }
                    if (clear_strat) {
                        // do clear strat
                        for (int ip = -1; ip <= 1; ip++)
                            for (int jp = -1; jp <= 1; jp++) {
                                int tp = tile(focus_i+ip, focus_j+jp);
                                if (!cleared[tp] && !flags[tp]) {
                                    clear(focus_i+ip, focus_j+jp);
                                    cleared_num++;
                                }
                            }
                        made_progress = true;
                        continue;
                    }
                    // try advanced strats?
                    // or should i do more advanced checks for the above two
                }
            }
    }

    printf("Flags found: %d/%d\n", flag_num, mine_num);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int t = tile(i, j);

            int case_num = 0;
            if (cleared[t]) {
                case_num += 1;
            }
            if (flags[t]) {
                case_num += 2;
            }
            if (mines[t]) {
                case_num += 4;
            }
            switch (case_num) {
                case 0:  // untouched, empty
                    putchar('+');
                    break;
                case 1:  // cleared, empty
                    if (adjacent_mines[t] == 0) {
                        putchar(' ');
                    } else {
                        printf("%d", adjacent_mines[t]);
                    }
                    break;
                case 4:  // untouched, mine
                    putchar('*');
                    break;
                case 5:  // cleared, mine
                    putchar('X');
                    break;
                case 6:  // flagged, mine
                    putchar('!');
                    break;
                case 2:  // flagged, empty
                case 3:  // cleared, flagged, empty
                case 7:  // cleared, flagged, mine
                    putchar('X');
                    break;
            }
        }
        putchar('\n');
    }

}



