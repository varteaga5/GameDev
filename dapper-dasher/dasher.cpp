#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    // update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
};

int main()
{
    int windowDimensions[2];
    windowDimensions[0] = 800; // width
    windowDimensions[1] = 450; // height
    // initialize window
    InitWindow(windowDimensions[0], windowDimensions[1], "The Dapper Dasher - sprinting sprucely");

    // acceleration due to gravity (pixels/s)/s
    const int gravity{1100};

    // nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    int sizeOfNebulae{3};
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width / 8;
        nebulae[i].rec.height = nebula.height / 8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height / 8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
        nebulae[i].pos.x = windowDimensions[0] + i * 500;
    }

    float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

    // nebula X velocity(pixels/second)
    int nebVel{-450};

    // scarfy variables
    // using Texture2D, Rectangle, Vector2 from raylib library
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width / 6; // divide by 6 because there are 6 versions of scarfy in the file
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0] / 2 - scarfyData.rec.width / 2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0 / 12.0;
    scarfyData.runningTime = 0.0;

    int velocity{0};

    // is rectangle in the air
    bool isInAir{false};

    // jump velocity pixels per second
    const int jumpVel{-550};

    // texture for the background
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    // texture for the midground
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    // texture for the foreground
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    // tests if a collision occured, put inside the while loop singular instances
    bool collision{};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // delta time (time since last frame)
        const float dT{GetFrameTime()};

        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // background x position, scrolling at 20 pixels per second
        bgX -= 20 * dT;
        if (bgX <= -background.width * 3.15)
        {
            bgX = 0.0;
        }
        // midground x position, scrolling at 40 pixels per second
        mgX -= 40 * dT;
        if (mgX <= -midground.width * 3.0)
        {
            mgX = 0.0;
        }
        // foreground x position, scrolling at 80 pixels per second
        fgX -= 80 * dT;
        if (fgX <= -foreground.width * 2.5)
        {
            fgX = 0.0;
        }

        // draw the background
        Vector2 bg1Pos{bgX, 0.0};
        // 2.75 is the scale, third arg
        DrawTextureEx(background, bg1Pos, 0.0, 3.15, WHITE);
        Vector2 bg2Pos{bgX + background.width * 3.15, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 3.15, WHITE);

        // draw the midground
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 3.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width * 3.0, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 3.0, WHITE);

        // draw the foreground
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.5, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 2.5, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.5, WHITE);

        // perform ground check
        if (isOnGround(scarfyData, windowDimensions[1]))
        {
            // scarfy is on the ground
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // scarfy is in the air
            // apply gravity
            velocity += gravity * dT;
            isInAir = true;
        }
        // using space key to jump, not allowing double jumping
        if (!isInAir && IsKeyPressed(KEY_SPACE))
        {
            velocity += jumpVel;
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            // update nebula position
            nebulae[i].pos.x += nebVel * dT;
        }

        // update scarfy position
        scarfyData.pos.y += velocity * dT;

        // update finishline position
        finishLine += nebVel * dT;

        // updates scarfy animation
        if (!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }
        else
        {
            scarfyData.rec.x = 1 * scarfyData.rec.width;
        }
        // for loop to update nebulae
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            // updates nebula animation
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

        // collision checking
        for (AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - (2 * pad),
                nebula.rec.height - (2 * pad),
            };

            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height};

            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

        if (collision)
        {
            // lose game
            DrawText("Game Over!", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, RED);
            // restart game
            if (IsKeyPressed(KEY_SPACE))
            {
                collision = false;
                sizeOfNebulae = 3;
            }
        }
        else if (scarfyData.pos.x >= finishLine)
        {
            DrawText("You Win!", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, BLUE);
            // restart game stil in development
            if (IsKeyPressed(KEY_SPACE))
            {
                collision = false;
                sizeOfNebulae = 3;
                scarfyData.pos.x = 0;
                finishLine = nebulae[sizeOfNebulae - 1].pos.x;
            }
        }
        else
        {
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                // draw nebulae
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }

            // draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}