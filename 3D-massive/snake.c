#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

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

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
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
    setColor(11); 
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


void drawUI(Game *game) {
    setColor(14); // Yellow
    gotoxy(WIDTH + 2, 2);
    printf("🎮 SNAKE GAME 🎮");
    
    setColor(10); // Green
    gotoxy(WIDTH + 2, 4);
    printf("Score: %d", game->score);
    
    gotoxy(WIDTH + 2, 5);
    printf("Length: %d", game->snake.length);
    
    setColor(13); // Magenta
    gotoxy(WIDTH + 2, 7);
    printf("Controls:");
    setColor(7);
    gotoxy(WIDTH + 2, 8);
    printf("W - Up");
    gotoxy(WIDTH + 2, 9);
    printf("S - Down");
    gotoxy(WIDTH + 2, 10);
    printf("A - Left");
    gotoxy(WIDTH + 2, 11);
    printf("D - Right");
    gotoxy(WIDTH + 2, 12);
    printf("X - Exit");
}

// Gambar ular
void drawSnake(Snake *snake) {
    // Kepala ular
    setColor(14); // Yellow
    gotoxy(snake->x[0], snake->y[0]);
    printf("◉");
    
    // Badan ular
    setColor(10); // Green
    for (int i = 1; i < snake->length; i++) {
        gotoxy(snake->x[i], snake->y[i]);
        printf("●");
    }
}

// Gambar makanan
void drawFood(Food *food) {
    setColor(12); // Red
    gotoxy(food->x, food->y);
    printf("★");
}

// Handle input
void handleInput(Game *game) {
    if (_kbhit()) {
        char key = _getch();
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
            case 'x': case 'X':
                game->gameOver = 1;
                break;
        }
    }
}

// Update posisi ular
void updateSnake(Game *game) {
    // Simpan posisi lama ekor
    int prevX = game->snake.x[game->snake.length - 1];
    int prevY = game->snake.y[game->snake.length - 1];
    
    // Geser semua segmen
    for (int i = game->snake.length - 1; i > 0; i--) {
        game->snake.x[i] = game->snake.x[i - 1];
        game->snake.y[i] = game->snake.y[i - 1];
    }
    
    // Update kepala
    switch (game->snake.dir) {
        case UP:    game->snake.y[0]--; break;
        case DOWN:  game->snake.y[0]++; break;
        case LEFT:  game->snake.x[0]--; break;
        case RIGHT: game->snake.x[0]++; break;
    }
    
    // Hapus ekor lama
    gotoxy(prevX, prevY);
    printf(" ");
}

// Cek tabrakan
void checkCollision(Game *game) {
    // Tabrakan dengan dinding
    if (game->snake.x[0] <= 0 || game->snake.x[0] >= WIDTH - 1 ||
        game->snake.y[0] <= 0 || game->snake.y[0] >= HEIGHT - 1) {
        game->gameOver = 1;
        return;
    }
    
    // Tabrakan dengan diri sendiri
    for (int i = 1; i < game->snake.length; i++) {
        if (game->snake.x[0] == game->snake.x[i] && 
            game->snake.y[0] == game->snake.y[i]) {
            game->gameOver = 1;
            return;
        }
    }
    
    // Makan makanan
    if (game->snake.x[0] == game->food.x && game->snake.y[0] == game->food.y) {
        game->snake.length++;
        game->score += 10;
        
        // Spawn makanan baru
        do {
            game->food.x = rand() % (WIDTH - 2) + 1;
            game->food.y = rand() % (HEIGHT - 2) + 1;
        } while (game->food.x == game->snake.x[0] && game->food.y == game->snake.y[0]);
        
        // Percepat game
        if (game->speed > 30) game->speed -= 2;
    }
}

// Game over screen
void showGameOver(Game *game) {
    system("cls");
    setColor(12);
    printf("\n\n");
    printf("    ╔═══════════════════════════════╗\n");
    printf("    ║        GAME OVER!             ║\n");
    printf("    ╚═══════════════════════════════╝\n\n");
    
    setColor(14);
    printf("    Final Score: %d\n", game->score);
    printf("    Snake Length: %d\n\n", game->snake.length);
    
    setColor(7);
    printf("    Thanks for playing!\n");
    printf("    Press any key to exit...");
    _getch();
}

// Main game loop
int main() {
    Game game;
    
    system("cls");
    hideCursor();
    
    initGame(&game);
    drawBorder();
    
    while (!game.gameOver) {
        drawUI(&game);
        drawFood(&game.food);
        drawSnake(&game.snake);
        
        handleInput(&game);
        updateSnake(&game);
        checkCollision(&game);
        
        Sleep(game.speed);
    }
    
    showGameOver(&game);
    return 0;
}