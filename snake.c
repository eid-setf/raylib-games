#include <raylib.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define SNAKE_LENGTH 256
#define SQUARE_SIZE 24

void InitGame();
void UpdateDrawFrame();
void UpdateGame();
void DrawGame();

typedef struct Snake {
    Vector2 pos;
    Vector2 size;
    Vector2 speed;
    Color color;
} Snake;

typedef struct Food {
    Vector2 pos;
    Vector2 size;
    int active;
    Color color;
} Food;

static int gameOver = false;
static int pause = false;
static int frameCounter = 0;

static int tailCounter = 0;
static Snake snake[SNAKE_LENGTH] = { 0 };
static Vector2 snakePosition[SNAKE_LENGTH] = { 0 };
static Food food = { 0 };
static Vector2 offset = { 0 };

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    InitGame();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    CloseWindow();
    return 0;
}

void InitGame()
{
    gameOver = false;
    pause = false;
    frameCounter = 0;
    tailCounter = 1;

    offset.x = SCREEN_WIDTH % SQUARE_SIZE;
    offset.y = SCREEN_HEIGHT % SQUARE_SIZE;

    for (int i = 0; i < SNAKE_LENGTH; i++) {
        snake[i].pos = (Vector2){ (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2 };
        snake[i].size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
        snake[i].speed = (Vector2){ -SQUARE_SIZE, 0 };

        snake[i].color = i == 0 ? DARKGREEN : GREEN;
    }

    for (int i = 0; i < SNAKE_LENGTH; i++) {
        snakePosition[i] = (Vector2){ 0.0f, 0.0f };
    }

    food.size = (Vector2) { SQUARE_SIZE, SQUARE_SIZE };
    food.active = false;
    food.color = YELLOW;
}

void UpdateGame()
{
    if (!gameOver) {

        if (IsKeyPressed(KEY_P)) pause = !pause;

        if (!pause) {

            if (IsKeyPressed(KEY_D) && snake[0].speed.x == 0) {
                snake[0].speed = (Vector2){ SQUARE_SIZE, 0 };
            }

            if (IsKeyPressed(KEY_A) && snake[0].speed.x == 0) {
                snake[0].speed = (Vector2){ -SQUARE_SIZE, 0 };
            }

            if (IsKeyPressed(KEY_W) && snake[0].speed.y == 0) {
                snake[0].speed = (Vector2){ 0, -SQUARE_SIZE };
            }

            if (IsKeyPressed(KEY_S) && snake[0].speed.y == 0) {
                snake[0].speed = (Vector2){ 0, SQUARE_SIZE };
            }

            for (int i = 0; i < tailCounter; i++) snakePosition[i] = snake[i].pos;

            if (frameCounter % 15 == 0) {
                for (int i = 0; i < tailCounter; i++) {
                    if (i == 0) {
                        snake[i].pos.x += snake[i].speed.x;
                        snake[i].pos.y += snake[i].speed.y;
                    } else {
                        snake[i].pos.x = snakePosition[i-1].x;
                        snake[i].pos.y = snakePosition[i-1].y;
                    }
                }
            }

            // Wall collision
            if (snake[0].pos.x > SCREEN_WIDTH - offset.x ||
                snake[0].pos.y > SCREEN_HEIGHT - offset.y ||
                snake[0].pos.x < 0 || snake[0].pos.y < 0) {
                gameOver = true;
            }

            // Collosion with body
            for (int i = 1; i < tailCounter; i++) {
                if (snake[0].pos.x == snake[i].pos.x && snake[0].pos.y == snake[i].pos.y)
                    gameOver = true;
            }

            // Food position
            if (!food.active) {
                food.active = true;
                food.pos = (Vector2){ GetRandomValue(0, SCREEN_WIDTH / SQUARE_SIZE - 1)*SQUARE_SIZE + offset.x/2, GetRandomValue(0, SCREEN_HEIGHT / SQUARE_SIZE - 1)* SQUARE_SIZE + offset.y/2};

                for (int i = 0; i < tailCounter; i++) {
                    while (food.pos.x == snake[i].pos.x && food.pos.y == snake[i].pos.y) {
                        food.pos = (Vector2){ GetRandomValue(0, SCREEN_WIDTH / SQUARE_SIZE - 1)*SQUARE_SIZE + offset.x/2, GetRandomValue(0, SCREEN_HEIGHT / SQUARE_SIZE - 1)* SQUARE_SIZE + offset.y/2};
                        i = 0;
                    }
                }
            }

            // Food collision
            if ((snake[0].pos.x < (food.pos.x + food.size.x) && (snake[0].pos.x + snake[0].size.x) > food.pos.x) &&
                (snake[0].pos.y < (food.pos.y + food.size.y) && (snake[0].pos.y + snake[0].size.y) > food.pos.y)) {
                snake[tailCounter].pos = snakePosition[tailCounter - 1];
                tailCounter++;
                food.active = false;
            }

        }

        frameCounter++;
    } else {
        if (IsKeyPressed(KEY_ENTER)) {
            InitGame();
        }
    }
}

void DrawGame()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (!gameOver) {
        for (int i = 0; i < tailCounter; i++) {
            DrawRectangleV(snake[i].pos, snake[i].size, snake[i].color);
        }

        DrawRectangleV(food.pos, food.size, food.color);

        if (pause) {
            DrawText("Game Paused!", SCREEN_WIDTH/2 - MeasureText("Game Paused!", 20)/2, SCREEN_HEIGHT/2, 20, BLUE);
        }
    } else {
        DrawText("Game Over! Press [ENTER] to play again!", SCREEN_WIDTH/2 - MeasureText("Game Over! Press [ENTER] to play again!", 20)/2, SCREEN_HEIGHT/2, 20, BLUE);
    }

    EndDrawing();
}

void UpdateDrawFrame()
{
    UpdateGame();
    DrawGame();
}
