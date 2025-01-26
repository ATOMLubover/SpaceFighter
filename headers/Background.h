#ifndef _BAKCGROUND_
#define _BAKCGROUND_

#include "PlaneVector.h"
#include "Camera.h"

#include <SDL.h>

#include <cmath>

class Background
{
public:
    Background() = default;
    ~Background() = default;

    void OnUpdate( float delta );
    void OnRender( const Camera& cam );

    void SetTexture( SDL_Texture* texture );

    void SetSpeed( int speed ) { this->speed = speed; }

private:
    SDL_Texture* raw_tex = nullptr;

    float offset = 0; // offset on y axis
    int width;
    int height;

    int speed;
};

#endif