#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SQUARE_SIZE 24

typedef struct Player {
    Vector2 pos;
    Vector2 size;
    Color color;
    int score;
    int d;
} Player;

typedef struct Ball {
    Vector2 pos;
    Vector2 size;
    Vector2 speed;
    Color color;
} Ball;

static const int screenWidth = 640;
static const int screenHeight = 480;

static int gameOver = false;
static int pause = false;

static Player player1 = { 0 };
static Player player2 = { 0 };
static Ball ball = { 0 };
static char *score = '\0';
static size_t nm = 0;

void InitGame();
void UpdateGame();
void DrawGame();
void UpdateDrawFrame();
void UnloadGame();

int main()
{
    InitWindow(screenWidth, screenHeight, "Pong");
    InitGame();
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    UnloadGame();

    return 0;
}

void InitGame()
{
    gameOver = false;
    pause = false;

    player1.pos = (Vector2) { 10, (float)screenHeight/2 };
    player1.size = (Vector2) { SQUARE_SIZE, SQUARE_SIZE * 3};
    player1.color = BLUE;
    player1.score = 0;
    player1.d = 1;

    player2.pos = (Vector2) { screenWidth - SQUARE_SIZE - 10, (float)screenHeight/2};
    player2.size = (Vector2) { SQUARE_SIZE, SQUARE_SIZE * 3};
    player2.color = BLUE;
    player2.score = 0;
    player2.d = 1;

    ball.pos = (Vector2) {(float)screenWidth/2, (float)screenHeight/2};
    ball.size = (Vector2) { SQUARE_SIZE, SQUARE_SIZE };
    ball.speed = (Vector2) { -SQUARE_SIZE, -SQUARE_SIZE };
    ball.color = YELLOW;

    nm = 4;
    score = malloc(nm);
    sprintf(score, "%d:%d", player1.score, player2.score);
}

void UpdateGame()
{
    if (!gameOver) {

        if (IsKeyPressed(KEY_P))
            pause = !pause;

        if (!pause) {

            if (IsKeyDown(KEY_W)) {
                player1.pos.y -= (float)SQUARE_SIZE/5;
            }

            if (IsKeyDown(KEY_S)) {
                player1.pos.y += (float)SQUARE_SIZE/5;
            }

            if (IsKeyDown(KEY_D)) {
                player2.pos.y -= (float)SQUARE_SIZE/5;
            }

            if (IsKeyDown(KEY_A)) {
                player2.pos.y += (float)SQUARE_SIZE/5;
            }

            /* cpu */
            /* player2.pos.y = ball.pos.y */

            // Collision
            if (player1.pos.y + player1.size.y > screenHeight) {
                player1.pos.y = screenHeight - player1.size.y;
            }

            if (player1.pos.y < 0) {
                player1.pos.y = 0;
            }

            if (player2.pos.y + player2.size.y > screenHeight) {
                player2.pos.y = screenHeight - player2.size.y;
            }

            if (player2.pos.y < 0) {
                player2.pos.y = 0;
            }

            // Ball screen collision
            if (ball.pos.y < 0 || ball.pos.y > screenHeight - ball.size.y) {
                ball.speed.y = -ball.speed.y;
            }

            // Ball touches player
            if ((ball.pos.y + ball.size.y >= player1.pos.y && ball.pos.y <= player1.pos.y + player1.size.y
                 && ball.pos.x == player1.pos.x + player1.size.x) ||
                (ball.pos.y + ball.size.y >= player2.pos.y && ball.pos.y <= player2.pos.y + player2.size.y
                 && ball.pos.x + ball.size.x == player2.pos.x)) {
                ball.speed.x = -ball.speed.x;
            }

            // Scores
            if (ball.pos.x > screenWidth) {
                player1.score++;
                ball.pos = (Vector2) {(float)screenWidth/2, (float)screenHeight/2};
                ball.speed.x = -ball.speed.x;

                player1.d = player1.score == 0 ? 1 : log10(player1.score) + 1;
                player2.d = player2.score == 0 ? 1 : log10(player2.score) + 1;

                nm = player1.d + player2.d + 2;
                score = realloc(score, nm);
                /* score = tmp_score; */
                sprintf(score, "%d:%d", player1.score, player2.score);

            } else if (ball.pos.x + ball.size.x < 0) {
                player2.score++;
                ball.pos = (Vector2) {(float)screenWidth/2, (float)screenHeight/2};
                ball.speed.x = -ball.speed.x;

                player1.d = player1.score == 0 ? 1 : log10(player1.score) + 1;
                player2.d = player2.score == 0 ? 1 : log10(player2.score) + 1;

                nm = player1.d + player2.d + 2;
                score = realloc(score, nm);
                sprintf(score, "%d:%d", player1.score, player2.score);
            }

            // Ball movement
            ball.pos.y += ball.speed.y/ ((float)SQUARE_SIZE / 2);
            ball.pos.x += ball.speed.x/ ((float)SQUARE_SIZE / 2);

        }

    }
}

void DrawGame()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (!gameOver) {
        DrawRectangleV(ball.pos, ball.size, ball.color);

        DrawRectangleV(player1.pos, player1.size, player1.color);
        DrawRectangleV(player2.pos, player2.size, player2.color);

        DrawText(score, screenWidth/2 - MeasureText(score, 40)/2, 10, 40, GREEN);
        if (pause)
            DrawText("GAME PAUSED!", screenWidth/2 - MeasureText("GAME PAUSED!", 40)/2, screenHeight/2-40, 40, BLUE);
    } else {
        DrawText("GAME OVER!", screenWidth/2 - MeasureText("GAME OVER!", 40)/2, screenHeight/2-40, 40, RED);
    }

    EndDrawing();
}

void UpdateDrawFrame()
{
    UpdateGame();
    DrawGame();
}

void UnloadGame()
{
    free(score);
}
