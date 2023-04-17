#include <SDL.h>
#include <cstdlib>
#include <string.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int DOT_WIDTH = 50;
const int DOT_HEIGHT = 50;
const int DOT_VELOCITY = -150;
const int GRAVITY = 500;
const int DOT_ACCELERATION = 100;
const int FLOOR_HEIGHT = 0;
const int DOT_FRICTION = 200;

using namespace std;

class Dot
{
public:
    // Constructor
    Dot(float x, float y, SDL_Renderer* renderer);

    // Handle input events
    void handleEvent(SDL_Event& e);

    // Move dot using physics equations
    void move(float deltaTime);

    // Render dot to screen
    void render(SDL_Renderer* renderer);

private:
    // Dot's position
    float mPosX, mPosY;

    // Dot's velocity
    float mVelX, mVelY;

    // Dot's acceleration
    float mAccX, mAccY;

    // Texture for rendering dot
    SDL_Texture* mTexture;

    // Renderer for rendering dot
    SDL_Renderer* mRenderer;
};

Dot::Dot(float x, float y, SDL_Renderer* renderer)
{
    // Initialize position and velocity
    mPosX = x;
    mPosY = y;
    mVelX = 0;
    mVelY = 0;

    // Initialize acceleration
    mAccX = 0;
    mAccY = GRAVITY;

    // Load dot texture
    SDL_Surface* surface = SDL_LoadBMP("./example/images/dot.bmp");
    mTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (mTexture == NULL)
    {
        cerr << "Failed to create dot texture: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    // Set renderer
    mRenderer = renderer;
}

void Dot::handleEvent(SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            mVelY = DOT_VELOCITY;
            break;
        case SDLK_LEFT:
            mAccX = -DOT_ACCELERATION;
            break;
        case SDLK_RIGHT:
            mAccX = DOT_ACCELERATION;
            break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT:
            mAccX = 0;
            break;
        case SDLK_RIGHT:
            mAccX = 0;
            break;
        }
    }
}

void Dot::move(float deltaTime)
{
    // Apply gravity to vertical acceleration
    mAccY = GRAVITY;

    // Update velocity using acceleration
    mVelX += mAccX * deltaTime;
    mVelY += mAccY * deltaTime;

    if (mAccX == 0)
    {
        if (mVelX > 0)
        {
            mVelX -= DOT_FRICTION * deltaTime;
            if (mVelX < 0)
            {
                mVelX = 0;
            }
        }
        else if (mVelX < 0)
        {
            mVelX += DOT_FRICTION * deltaTime;
            if (mVelX > 0)
            {
                mVelX = 0;
            }
        }
    }

    // Update position using velocity
    mPosX += mVelX * deltaTime;
    mPosY += mVelY * deltaTime;

    // Keep dot on the screen
    if (mPosX < 0)
    {
        mPosX = 0;
        mVelX = 0;
    }
    else if (mPosX > SCREEN_WIDTH - DOT_WIDTH)
    {
        mPosX = SCREEN_WIDTH - DOT_WIDTH;
        mVelX = 0;
    }

    if (mPosY < 0)
    {
        mPosY = 0;
        mVelY = 0;
    }
    else if (mPosY > SCREEN_HEIGHT - DOT_HEIGHT)
    {
        mPosY = SCREEN_HEIGHT - DOT_HEIGHT;
        mVelY = 0;
    }
}

void Dot::render(SDL_Renderer* renderer)
{
    // Set rendering space and render to screen
    SDL_Rect renderQuad = { (int)mPosX, (int)mPosY, DOT_WIDTH, DOT_HEIGHT };
    SDL_RenderCopy(renderer, mTexture, nullptr, &renderQuad);
}

int main(int argc, char* args[])
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create window
    SDL_Window* window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create dot
    Dot dot(SCREEN_WIDTH / 2, SCREEN_HEIGHT - DOT_HEIGHT, renderer);

    // Event loop flag
    bool quit = false;

    // Event object
    SDL_Event e;

    // Time variables
    Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime, elapsedTime;

    // Game loop
    while (!quit)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            // Handle input for the dot
            dot.handleEvent(e);
        }

        // Compute elapsed time since last frame
        currentTime = SDL_GetTicks();
        elapsedTime = currentTime - lastTime;
        lastTime = currentTime;

        // Move dot using physics equations
        dot.move(elapsedTime / 1000.0f);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Render floor
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT - FLOOR_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - FLOOR_HEIGHT);

        // Render dot
        dot.render(renderer);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Free resources and close SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}