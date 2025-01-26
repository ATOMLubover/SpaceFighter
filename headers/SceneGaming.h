#ifndef _SCENE_GAMING_H_
#define _SCENE_GAMING_H_

#include "Scene.h"
#include "Background.h"

#include <SDL_ttf.h>

class SceneGaming : public Scene
{
public:
    SceneGaming();
    ~SceneGaming();

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
    // void UpdateUi( float delta );
    void RenderUi( const Camera& cam );

    void RenderHeartHp( const Camera& cam );
    void RenderScore( const Camera& cam );

private:
    SDL_Texture* tex_ui_heart = nullptr;

    TTF_Font* font_score = nullptr;
};

#endif // !_SCENE_GAMING_H_