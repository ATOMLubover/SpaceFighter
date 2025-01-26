#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "SceneMenu.h"
#include "SceneGaming.h"
#include "SceneEnd.h"
#include "PlaneVector.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>

class SceneManager
{
public:
    enum class EnumSceneState
    {
        MENU, GAMING, END
    };

public:
    static SceneManager& GetInstance();

    static PlaneVector RenderText(
        const Camera& cam,
        const std::string& text, TTF_Font* font,
        float* x, float* y, SDL_Color* color = nullptr,
        bool based_on_left = true );

public:
    SceneManager( const SceneManager& rhs ) = delete;
    SceneManager& operator=( const SceneManager& rhs ) = delete;
    ~SceneManager() = default;

    void Init();
    void Deinit();

    void Refresh();

    void SwitchScene( EnumSceneState state );
    EnumSceneState GetCurrState() const { return state_curr; }

    void OnHandle( SDL_Event& event );
    void OnUpdate( float delta );
    void OnRender( const Camera& cam );

private:
    explicit SceneManager() = default;

private:
    Scene* scene_curr = nullptr;
    EnumSceneState state_curr = EnumSceneState::MENU;
    Scene* scene_menu = nullptr;
    Scene* scene_gaming = nullptr;
    Scene* scene_end = nullptr;
};

#endif