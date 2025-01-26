#ifndef _GAME_H_
#define _GAME_H_

#include "Camera.h"
#include "Background.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <iostream>
#include <chrono>
#include <thread>

class Game
{
public:
    static Game& GetInstance();

public:
    Game( const Game& rhs ) = delete;
    Game& operator=( const Game& rhs ) = delete;
    ~Game();

    void Init();
    void Deinit();

    void Run();

    int GetWidth() const { return WIDTH_WIN; }
    int GetHeight() const { return HEIGHT_WIN; }

    Camera& GetMainCam() { return camera; }

    SDL_Renderer* GetRenderer() { return renderer; }

private:
    explicit Game();

    void HandleEvent( SDL_Event& event );
    void Update( float delta );
    void Render();

    void UpdateBackground( float delta );
    void RenderBackground();

private:
    SDL_Window* window = nullptr;
    static const int WIDTH_WIN = 600;
    static const int HEIGHT_WIN = 800;

    SDL_Renderer* renderer = nullptr;
    Camera camera;
    static const int FPS = 60;

    bool is_initialized = false;
    bool is_cleaned_up = false;
    bool is_quit = false;

    bool is_stars_shown = true;
    Background stars_near;
    Background stars_far;
};

#endif // !_GAME_H_