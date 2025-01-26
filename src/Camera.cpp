#include "Camera.h"

Camera::Camera()
{
    timer_shake.SetOneshot( true );
    timer_shake.SetOnTimeout(
        [ & ] ()
        {
            is_shaking = false;
            Reset();
        }
    );
}

void Camera::OnUpdate( float delta )
{
    timer_shake.OnUpdate( delta );

    if ( is_shaking )
    {
        position.x = ( -50 + rand() % 100 ) / 50.0f * shaking_strength;
        position.y = ( -50 + rand() % 100 ) / 50.0f * shaking_strength;
    }
}

void Camera::Shake( float strength, float duration )
{
    is_shaking = true;
    shaking_strength = strength;

    timer_shake.SetWaitTime( duration );
    timer_shake.Restart();
}

void Camera::RenderTexture( SDL_Texture* texture, const SDL_Rect* rect_src, const SDL_FRect* rect_dst, double angle, const SDL_FPoint* centre ) const
{
    SDL_FRect rect_dst_win = *rect_dst;
    rect_dst_win.x -= position.x;
    rect_dst_win.y -= position.y;

    SDL_RenderCopyExF( renderer, texture, rect_src, &rect_dst_win,
        angle, centre, SDL_RendererFlip::SDL_FLIP_NONE );
}
