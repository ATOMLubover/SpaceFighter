#ifndef _SCENE_MENU_H_
#define _SCENE_MENU_H_

#include "Scene.h"
#include "Timer.h"

#include <SDL_ttf.h>

class SceneMenu : public Scene
{
public:
    SceneMenu();
    ~SceneMenu();

    void OnEnter() override;
    void OnExit() override;

    void OnHandle( SDL_Event& event ) override;
    void OnUpdate( float delta ) override;
    void OnRender( const Camera& cam ) override;

protected:
    void Refresh() override;

    void LoadResources() override;
    void UnloadResources() override;

private:
    void InitTimers();

private:
    TTF_Font* font_title = nullptr;
    TTF_Font* font_text = nullptr;
    TTF_Font* font_tip = nullptr;
    Timer timer_text_blink;
    bool is_text_visible;
};

#endif // !_SCENE_MENU_H_