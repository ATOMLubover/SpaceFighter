#include "SceneManager.h"

#include "EntityManager.h"
#include "Game.h"

SceneManager& SceneManager::GetInstance()
{
    static SceneManager manager;
    return manager;
}

PlaneVector SceneManager::RenderText(
    const Camera& cam,
    const std::string& text, TTF_Font* font,
    float* x, float* y, SDL_Color* color,
    bool based_on_left )
{
    SDL_Color color_white = { 255, 255, 255, 255 };
    if ( color == nullptr )
        color = &color_white;

    SDL_Surface* suf_text = TTF_RenderUTF8_Solid( font, text.c_str(), *color );
    SDL_Texture* tex_text = SDL_CreateTextureFromSurface( Game::GetInstance().GetRenderer(), suf_text );
    PlaneVector size( suf_text->w, suf_text->h );

    SDL_FRect rect_dst = {};
    float x_mid = Game::GetInstance().GetWidth() / 2.0f - suf_text->w / 2.0f;
    float y_mid = Game::GetInstance().GetHeight() / 2.0f - suf_text->h / 2.0f;

    float x_dst, y_dst;
    if ( x ) x_dst = based_on_left ? *x : ( Game::GetInstance().GetWidth() - *x - ( float ) suf_text->w );
    else x_dst = x_mid;
    if ( y )  y_dst = *y;
    else y_dst = y_mid;
    rect_dst = { x_dst, y_dst, ( float ) suf_text->w, ( float ) suf_text->h };
    SDL_FRect rect_dst_shade = { rect_dst.x + 2, rect_dst.y + 2, rect_dst.w, rect_dst.h };

    SDL_SetTextureColorMod( tex_text, 128, 128, 128 );
    cam.RenderTexture( tex_text, nullptr, &rect_dst_shade, 0, nullptr ); // render shade
    SDL_SetTextureColorMod( tex_text, 255, 255, 255 );
    cam.RenderTexture( tex_text, nullptr, &rect_dst, 0, nullptr );

    // clean up
    SDL_DestroyTexture( tex_text );
    SDL_FreeSurface( suf_text );

    return size;
}

void SceneManager::Init()
{
    scene_menu = new SceneMenu;
    scene_gaming = new SceneGaming;
    scene_end = new SceneEnd;

    SwitchScene( EnumSceneState::MENU );
}
void SceneManager::Deinit()
{
    scene_curr->OnExit();
    delete scene_end;
    delete scene_gaming;
    delete scene_menu;
}

void SceneManager::Refresh()
{
    //state_curr = EnumSceneState::MENU;
}

void SceneManager::SwitchScene( EnumSceneState state )
{
    if ( scene_curr ) 
        scene_curr->OnExit();

    state_curr = state;
    switch ( state )
    {
        case EnumSceneState::MENU:
            scene_curr = scene_menu;
            break;

        case EnumSceneState::GAMING:
            scene_curr = scene_gaming;
            break;

        case EnumSceneState::END:
            scene_curr = scene_end;
            break;

        default:
            break;
    }

    scene_curr->OnEnter();
}

void SceneManager::OnHandle( SDL_Event& event )
{
    scene_curr->OnHandle( event );
}

void SceneManager::OnUpdate( float delta )
{
    scene_curr->OnUpdate( delta );
}

void SceneManager::OnRender( const Camera& cam )
{
    scene_curr->OnRender( cam );
}
