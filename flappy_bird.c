#include <raylib.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SQUARE_SIZE 24
#define PIPE_LENGTH (SCREEN_HEIGHT/SQUARE_SIZE)
#define PIPE_NUM 3

typedef struct Player {
    Vector2 pos;
    Vector2 size;
    Vector2 speed;
    int flying;
    Color color;
} Player;

typedef struct Pipe {
    Vector2 pos;
    Vector2 size;
    int active;
    Color color;
} Pipe;

static int gameOver = false;
static int pause = false;
static int spawnPipes = false;
static int flyCounter = 0;

static Player bird = { 0 };
static Pipe pipes[PIPE_NUM][PIPE_LENGTH] = { 0 };
static int score = 0;

void InitGame();
void SpawnPipes(Sound ding);
void DrawGame();

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy bird");
    InitGame();
    InitAudioDevice();
    SetTargetFPS(60);

    Sound ding = LoadSound("resources/ding.mp3");

    while (!WindowShouldClose()) {
        if (!gameOver) {

            if (IsKeyPressed(KEY_P))
                pause = !pause;

            if (!pause) {

                if (!bird.flying) {
                    if (IsKeyPressed(KEY_SPACE)) {
                        bird.speed.y = -SQUARE_SIZE;
                        bird.flying = true;
                    }

                } else {
                    if (IsKeyPressed(KEY_SPACE)) {
                        bird.speed.y *= 1.2;
                        flyCounter = 15;
                    }

                    flyCounter--;
                }

                bird.speed.y *= 1.03;

                if (flyCounter == 0) {
                    bird.flying = false;
                    flyCounter = 15;
                    bird.speed.y = SQUARE_SIZE;
                }

                bird.pos.y += bird.speed.y/15;

                SpawnPipes(ding);

                // Pipe Collision
                for (int i = 0; i < PIPE_NUM; i++) {
                    for (int j = 0; j < PIPE_LENGTH; j++) {
                        if (pipes[i][j].active) {
                            if (bird.pos.x + bird.size.x > pipes[i][j].pos.x && bird.pos.x < pipes[i][j].pos.x + pipes[i][j].size.x &&
                                bird.pos.y + bird.size.y > pipes[i][j].pos.y && bird.pos.y < pipes[i][j].pos.y + pipes[i][j].size.y)
                                gameOver = true;
                        }
                    }
                }

                // Border Collision
                if (bird.pos.y < 0 || bird.pos.y + bird.size.y > SCREEN_HEIGHT)
                    gameOver = true;

            }
        } else {
            if (IsKeyPressed(KEY_ENTER)) {
                InitGame();
            }
        }
        DrawGame();
    }

    UnloadSound(ding);

    // Unload
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void InitGame()
{
    gameOver = false;
    pause = false;

    spawnPipes = true;
    flyCounter = 15;
    score = 0;

    bird.pos = (Vector2) { (float)SCREEN_WIDTH/4, (float)SCREEN_HEIGHT/2 };
    bird.size = (Vector2) { SQUARE_SIZE, SQUARE_SIZE };
    bird.speed = (Vector2) { 0, SQUARE_SIZE };
    bird.flying = false;
    bird.color = YELLOW;

    for (int i = 0; i < PIPE_NUM; i++) {
        for (int j = 0; j < PIPE_LENGTH; j++) {
            pipes[i][j].size = (Vector2) { SQUARE_SIZE, SQUARE_SIZE };
            pipes[i][j].pos = (Vector2) { (float)SCREEN_WIDTH + SQUARE_SIZE * (i * 10), pipes[i][j].size.x*j };
            pipes[i][j].active = true;
            pipes[i][j].color = GREEN;
        }
    }

    for (int i = 0; i < PIPE_NUM; i++) {
        int nonactive = GetRandomValue(0, PIPE_LENGTH);
        pipes[i][nonactive].active = false;

        if (nonactive + 5 > PIPE_LENGTH) {
            for (int k = 1; k <= 5; k++) {
                pipes[i][nonactive-k].active = false;
            }
        } else {
            for (int k = 1; k <= 5; k++) {
                pipes[i][nonactive+k].active = false;
            }
        }
    }
}

void SpawnPipes(Sound ding) {
    // Spawn pipes
    for (int i = 0; i < PIPE_NUM; i++) {
        for (int j = 0; j < PIPE_LENGTH; j++) {
            pipes[i][j].pos.x--;
        }

        if (bird.pos.x == pipes[i][0].pos.x) {
            score++;
            PlaySound(ding);
        }

        if (pipes[i][0].pos.x + pipes[i][0].size.x < 0) {
            for (int j = 0; j < PIPE_LENGTH; j++) {
                /* pipes[i][j].pos.y = pipes[i][j].size.y * j; */
                pipes[i][j].pos.x = SCREEN_WIDTH;
                pipes[i][j].active = true;
            }

            int nonactive = GetRandomValue(0, PIPE_LENGTH);
            pipes[i][nonactive].active = false;

            if (nonactive + 5 > PIPE_LENGTH) {
                for (int k = 1; k < 5; k++) {
                    pipes[i][nonactive-k].active = false;
                }
            } else {
                for (int k = 1; k < 5; k++) {
                    pipes[i][nonactive+k].active = false;
                }
            }
        }
    }

}

void DrawGame()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (!gameOver) {
        DrawRectangleV(bird.pos, bird.size, bird.color);

        for (int i = 0; i < PIPE_NUM; i++) {
            for (int j = 0; j < PIPE_LENGTH; j++) {
                if (pipes[i][j].active) {
                    DrawRectangleV(pipes[i][j].pos, pipes[i][j].size, pipes[i][j].color);
                }
            }
        }

        DrawText(TextFormat("Score: %d", score), SCREEN_WIDTH/2 - MeasureText(TextFormat("Score: %d", score), 20)/2, 10, 20, BLUE);

        if (pause) {
            DrawText("Paused", SCREEN_WIDTH/2 - MeasureText("Paused", 40)/2, SCREEN_HEIGHT/2 - 40, 40, BLUE);
        }

    } else {
        DrawText("Game Over!", SCREEN_WIDTH/2 - MeasureText("Game Over!", 40)/2, SCREEN_HEIGHT/2 - 40, 40, BLUE);
    }

    EndDrawing();
}
