#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Timer.h"
#include "PlaneVector.h"

#include <SDL.h>

class Camera
{
public:
    Camera();
    ~Camera() = default;

    const PlaneVector& GetPosition() const { return position; }

    void SetRenderer( SDL_Renderer* renderer ) { this->renderer = renderer; }

    void Reset() { position.x = 0.0, position.y = 0.0; }

    void OnUpdate( float delta );

    void Shake( float strength, float duration );

    // rect_dst is the relevant coord to camera
    void RenderTexture(
        SDL_Texture* texture, const SDL_Rect* rect_src, const SDL_FRect* rect_dst,
        double angle, const SDL_FPoint* centre ) const;

private:
    PlaneVector position;

    Timer timer_shake;
    bool is_shaking = false;
    float shaking_strength = 0;

    SDL_Renderer* renderer = nullptr;
};

#endif