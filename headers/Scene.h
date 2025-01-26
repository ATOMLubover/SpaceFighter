#ifndef _SCENE_H_
#define _SCENE_H_

#include "Camera.h"

#include <SDL.h>

#include <functional>

class Scene
{
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;

    virtual void OnHandle( SDL_Event& event ) = 0;
    virtual void OnUpdate( float delta ) = 0;
    virtual void OnRender( const Camera& cam ) = 0;

    // void SetOnEnd( const std::function<void()>& on_end ) { this->on_end = on_end; }

protected:
    virtual void Refresh() = 0; // reset every data member if necessary when OnEnter

    virtual void LoadResources() = 0; // load dynamic resources when constructed
    virtual void UnloadResources() = 0; // unload ... when destroyed

    // std::function<void()> on_end;
};

#endif // !_SCENE_H_