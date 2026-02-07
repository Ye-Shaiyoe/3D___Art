
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>

#define WIDTH 40
#define HEIGHT 20
#define MAX_LENGTH 800

typedef enum { STOP = 0, LEFT, RIGHT, UP, DOWN } Direction;

typedef struct {
    int x[MAX_LENGTH];
    int y[MAX_LENGTH];
    int length;
    Direction dir;
} Snake;

typedef struct {
    int x, y;
} Food;

typedef struct {
    Snake snake;
    Food food;
    int score;
    int gameOver;
    int speed;
} Game;


struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    printf("\033[?25h"); // Show cursor
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    printf("\033[?25l"); // Hide cursor
}
