#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "Timer.h"
#include "PlaneVector.h"
#include "Camera.h"
#include "Atlas.h"

#include <SDL.h>

#include <vector>
#include <functional>

class Animation
{
    // logic frame
    struct Frame
    {
        SDL_Rect rect_src;
        SDL_Texture* texture;

        Frame() = default;
        Frame( SDL_Texture* texture, const SDL_Rect& rect_src )
            : texture( texture ), rect_src( rect_src )
        { }
        ~Frame() = default;
    };

public:
    Animation();
    ~Animation() = default;

    void OnUpdate( float delta );
    void OnRender( const Camera& camera );

    void Reset();

    // get frames from long pic
    void LoadAtlas( SDL_Texture* texture_atlas, int num_h );
    // get frames from atlas
    void LoadAtlas( Atlas& atlas );

    void SetLogicCenter( const PlaneVector& pos ) { pos_center = pos; }
    void SetRotation( double angle ) { this->angle = angle; }
    void SetCenter( const SDL_FPoint& center ) { this->centre = center; }
    void SetLoop( bool is_loop ) { this->is_loop = is_loop; }

    // is called during OnUpdate
    void SetOnStarted( std::function<void()> on_started ) { this->on_started = on_started; }
    void SetOnFinished( std::function<void()> on_finished ) { this->on_finished = on_finished; }

    void SetInterval( float interval ) { timer.SetWaitTime( interval ); }

    void SetRenderScale( float ratio ) { scale_ratio = ratio; }

private:
    PlaneVector pos_center;
    double angle = 0;
    SDL_FPoint centre = { 0 };

    Timer timer;
    bool is_loop = true;
    size_t idx_frame = 0;
    std::vector<Frame> frame_list;
    std::function<void()> on_started;
    std::function<void()> on_finished;

    float scale_ratio = 1.0f;
};

#endif