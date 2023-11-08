#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FIELD_WIDTH 80
#define FIELD_HEIGHT 25

void map_choice(int field[FIELD_HEIGHT][FIELD_WIDTH]);
void field_printer(int field[FIELD_HEIGHT][FIELD_WIDTH]);
void input_field(int field[FIELD_HEIGHT][FIELD_WIDTH], FILE* file);
void field_copy(int field[FIELD_HEIGHT][FIELD_WIDTH], int new_field[FIELD_HEIGHT][FIELD_WIDTH]);

int x_cycle_coord(int x_ind);
int y_cycle_coord(int y_ind);
int step_analyzer(int field[FIELD_HEIGHT][FIELD_WIDTH]);
int cell_analysis(int field[FIELD_HEIGHT][FIELD_WIDTH], int x_ind, int y_ind);
int cell_living_neighbour_count(int field[FIELD_HEIGHT][FIELD_WIDTH], int x_ind, int y_ind);
int cell_is_alive_test(int field[FIELD_HEIGHT][FIELD_WIDTH], int x_ind, int y_ind, int* counter);

int main() {
    int field[FIELD_HEIGHT][FIELD_WIDTH], out_flag = 0, sleep_time = 400000;
    char player_input;

    map_choice(field);

    initscr();
    noecho();
    nodelay(stdscr, 1);
    curs_set(0);

    while (!out_flag) {
        player_input = getch();

        if (player_input == 'w' || player_input == 'W') {
            if (sleep_time - 100000 > 100000) {
                sleep_time -= 100000;
            }
        } else if (player_input == 's' || player_input == 'S') {
            if (sleep_time + 100000 < 1000000) {
                sleep_time += 100000;
            }
        } else if (player_input == 'q' || player_input == 'Q') {
            out_flag = 1;
        }
        usleep(sleep_time);
        field_printer(field);

        /* Условие для конца игры. */
        if (step_analyzer(field) == 0) {
            out_flag = 1;
        }
    }
    clear();
    printw("End of game.");
    refresh();
    sleep(4);
    clear();

    endwin();
    return 0;
}

void map_choice(int field[FIELD_HEIGHT][FIELD_WIDTH]) {
    int out = 0, input_lvl;
    FILE* file = NULL;
    
    printf("Enter the level number (from 1 to 5): ");
    scanf("%d", &input_lvl);
    
    switch (input_lvl) {
        case 1:
            file = fopen("maps/1.txt", "r");
            out = 1;
            break;
        case 2:
            file = fopen("maps/2.txt", "r");
            out = 1;
            break;
        case 3:
            file = fopen("maps/3.txt", "r");
            out = 1;
            break;
        case 4:
            file = fopen("maps/4.txt", "r");
            out = 1;
            break;
        case 5:
            file = fopen("maps/5.txt", "r");
            out = 1;
            break;
    }
    if (out == 1) {
        input_field(field, file);
    } else {
        map_choice(field);
    }
}

void input_field(int field[FIELD_HEIGHT][FIELD_WIDTH], FILE* file) {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            fscanf(file, "%d", &field[i][j]);
        }
    }
}

int step_analyzer(int field[FIELD_HEIGHT][FIELD_WIDTH]) {
    int new_field[FIELD_HEIGHT][FIELD_WIDTH], out_flag = 0;

    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            new_field[i][j] = cell_analysis(field, j, i);

            if (new_field[i][j] != field[i][j]) {
                out_flag = 1;
            }
        }
    }
    field_copy(field, new_field);
    return out_flag;
}

int cell_analysis(int field[FIELD_HEIGHT][FIELD_WIDTH], int x_ind, int y_ind) {
    int out = 1, living_neighbours_count = cell_living_neighbour_count(field, x_ind, y_ind);
    
    /* Обработка живой клетки. */
    if (field[y_ind][x_ind] == 1) {
        if (living_neighbours_count < 2 || living_neighbours_count > 3) {
            out = 0;
        }
    } else {
        /* Обработка мертвой клетки. */
        if (living_neighbours_count != 3) {
            out = 0;
        }
    }
    return out;
}

int cell_living_neighbour_count(int field[FIELD_HEIGHT][FIELD_WIDTH], int x_ind, int y_ind) {
    int counter = 0;
    /* Обработка верхних соседей. */
    cell_is_alive_test(field, x_ind - 1, y_ind - 1, &counter);
    cell_is_alive_test(field, x_ind, y_ind - 1, &counter);
    cell_is_alive_test(field, x_ind + 1, y_ind - 1, &counter);

    /* Обработка боковых соседей. */
    cell_is_alive_test(field, x_ind - 1, y_ind, &counter);
    cell_is_alive_test(field, x_ind + 1, y_ind, &counter);

    /* Обработка нижних соседей. */
    cell_is_alive_test(field, x_ind - 1, y_ind + 1, &counter);
    cell_is_alive_test(field, x_ind, y_ind + 1, &counter);
    cell_is_alive_test(field, x_ind + 1, y_ind + 1, &counter);

    return counter;
}

int cell_is_alive_test(int field[FIELD_HEIGHT][FIELD_WIDTH], int x_ind, int y_ind, int* counter) {
    int out = 0;
    if (field[y_cycle_coord(y_ind)][x_cycle_coord(x_ind)] == 1) {
        out = 1;
        *counter += 1;
    }
    return out;
}

void write_to_file() {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        if (i > 0) {
            printf("\n");
        }
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (j != FIELD_WIDTH - 1) {
                printf("%d ", 0);
            } else {
                printf("%d", 0);
            }
        }
    }
}

int x_cycle_coord(int x_ind) {
    if (x_ind < 0) {
        x_ind = 79;
    } else if (x_ind > 79) {
        x_ind = 0;
    }
    return x_ind;
}

int y_cycle_coord(int y_ind) {
    if (y_ind < 0) {
        y_ind = 24;
    } else if (y_ind > 24) {
        y_ind = 0;
    }
    return y_ind;
}

void field_copy(int field[FIELD_HEIGHT][FIELD_WIDTH], int new_field[FIELD_HEIGHT][FIELD_WIDTH]) {
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            field[y][x] = new_field[y][x];
        }
    }
}

void field_printer(int field[FIELD_HEIGHT][FIELD_WIDTH]) {
    clear();
    
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        if (i > 0) printw("\n");
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (field[i][j] == 0) {
                printw(" ");
            } else {
                printw("*");
            }
        }
    }
    refresh();
}
