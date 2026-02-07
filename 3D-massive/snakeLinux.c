
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

int kbhit() {
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

void setColor(int color) {
    printf("\033[%dm", color);
}

void initGame(Game *game) {
    game->snake.length = 3;
    game->snake.dir = RIGHT;

    for (int i = 0; i < game->snake.length; i++) {
        game->snake.x[i] = WIDTH / 2 - i;
        game->snake.y[i] = HEIGHT / 2;
    }

    srand(time(NULL));
    game->food.x = rand() % (WIDTH - 2) + 1;
    game->food.y = rand() % (HEIGHT - 2) + 1;
    
    game->score = 0;
    game->gameOver = 0;
    game->speed = 100;
}

void drawBorder() {
    setColor(36); // Cyan
    for (int i = 0; i < WIDTH; i++) {
        gotoxy(i, 0);
        printf("═");
        gotoxy(i, HEIGHT - 1);
        printf("═");
    }
    for (int i = 0; i < HEIGHT; i++) {
        gotoxy(0, i);
        printf("║");
        gotoxy(WIDTH - 1, i);
        printf("║");
    }
    gotoxy(0, 0); printf("╔");
    gotoxy(WIDTH - 1, 0); printf("╗");
    gotoxy(0, HEIGHT - 1); printf("╚");
    gotoxy(WIDTH - 1, HEIGHT - 1); printf("╝");
}

void clearScreen() {
    printf("\033[2J");
    printf("\033[H");
}


void drawUI(Game *game) {
    setColor(33); // Yellow
    gotoxy(WIDTH + 2, 2);
    printf("🎮 SNAKE GAME 🎮");
    
    setColor(32); // Green
    gotoxy(WIDTH + 2, 4);
    printf("Score: %d  ", game->score);
    
    gotoxy(WIDTH + 2, 5);
    printf("Length: %d  ", game->snake.length);
    
    setColor(35); // Magenta
    gotoxy(WIDTH + 2, 7);
    printf("Controls:");
    setColor(37); // White
    gotoxy(WIDTH + 2, 8);
    printf("W - Up");
    gotoxy(WIDTH + 2, 9);
    printf("S - Down");
    gotoxy(WIDTH + 2, 10);
    printf("A - Left");
    gotoxy(WIDTH + 2, 11);
    printf("D - Right");
    gotoxy(WIDTH + 2, 12);
    printf("Q - Exit");
}

void drawSnake(Snake *snake) {
    setColor(33); // Yellow
    gotoxy(snake->x[0], snake->y[0]);
    printf("◉");
    
    setColor(32); // Green
    for (int i = 1; i < snake->length; i++) {
        gotoxy(snake->x[i], snake->y[i]);
        printf("●");
    }
}

void drawFood(Food *food) {
    setColor(31); // Red
    gotoxy(food->x, food->y);
    printf("★");
}

void handleInput(Game *game) {
    if (kbhit()) {
        char key = getchar();
        switch (key) {
            case 'w': case 'W':
                if (game->snake.dir != DOWN) game->snake.dir = UP;
                break;
            case 's': case 'S':
                if (game->snake.dir != UP) game->snake.dir = DOWN;
                break;
            case 'a': case 'A':
                if (game->snake.dir != RIGHT) game->snake.dir = LEFT;
                break;
            case 'd': case 'D':
                if (game->snake.dir != LEFT) game->snake.dir = RIGHT;
                break;
            case 'q': case 'Q':
            case 'x': case 'X':
                game->gameOver = 1;
                break;
        }
    }
}

void updateSnake(Game *game) {
    int prevX = game->snake.x[game->snake.length - 1];
    int prevY = game->snake.y[game->snake.length - 1];
    
    for (int i = game->snake.length - 1; i > 0; i--) {
        game->snake.x[i] = game->snake.x[i - 1];
        game->snake.y[i] = game->snake.y[i - 1];
    }
    
    switch (game->snake.dir) {
        case UP:    game->snake.y[0]--; break;
        case DOWN:  game->snake.y[0]++; break;
        case LEFT:  game->snake.x[0]--; break;
        case RIGHT: game->snake.x[0]++; break;
        default: break;
    }
    
    gotoxy(prevX, prevY);
    printf(" ");
}

void checkCollision(Game *game) {
    if (game->snake.x[0] <= 0 || game->snake.x[0] >= WIDTH - 1 ||
        game->snake.y[0] <= 0 || game->snake.y[0] >= HEIGHT - 1) {
        game->gameOver = 1;
        return;
    }
    
    for (int i = 1; i < game->snake.length; i++) {
        if (game->snake.x[0] == game->snake.x[i] && 
            game->snake.y[0] == game->snake.y[i]) {
            game->gameOver = 1;
            return;
        }
    }
    
    if (game->snake.x[0] == game->food.x && game->snake.y[0] == game->food.y) {
        game->snake.length++;
        game->score += 10;
        
        do {
            game->food.x = rand() % (WIDTH - 2) + 1;
            game->food.y = rand() % (HEIGHT - 2) + 1;
        } while (game->food.x == game->snake.x[0] && game->food.y == game->snake.y[0]);
        
        if (game->speed > 30) game->speed -= 2;
    }
}

void showGameOver(Game *game) {
    clearScreen();
    setColor(31); // Red
    printf("\n\n");
    printf("    ╔═══════════════════════════════╗\n");
    printf("    ║        GAME OVER!             ║\n");
    printf("    ╚═══════════════════════════════╝\n\n");
    
    setColor(33); // Yellow
    printf("    Final Score: %d\n", game->score);
    printf("    Snake Length: %d\n\n", game->snake.length);
    
    setColor(37); // White
    printf("    Thanks for playing!\n");
    printf("    Press any key to exit...");
    fflush(stdout);
    
    getchar();
}

int main() {
    Game game;
    
    enableRawMode();
    clearScreen();
    
    initGame(&game);
    drawBorder();
    
    while (!game.gameOver) {
        drawUI(&game);
        drawFood(&game.food);
        drawSnake(&game.snake);
        
        handleInput(&game);
        updateSnake(&game);
        checkCollision(&game);
        
        usleep(game.speed * 1000);
        fflush(stdout);
    }
    
    showGameOver(&game);
    setColor(0);
    printf("\033[0m\n");
    
    return 0;
}
