#include "SceneGaming.h"

#include "EntityManager.h"
#include "AudioManager.h"
#include "ResourceManager.h"
#include "Game.h"
#include "SceneManager.h"

SceneGaming::SceneGaming()
{
    LoadResources();
}

SceneGaming::~SceneGaming()
{
    UnloadResources();
}

void SceneGaming::OnEnter()
{
    Refresh();

    AudioManager::GetInstance().PlayAudio( "bgm_battle", -1 );
}

void SceneGaming::OnExit()
{
    AudioManager::GetInstance().StopMusic();
}

void SceneGaming::OnHandle( SDL_Event& event )
{
    switch ( event.type )
    {
        case SDL_KEYDOWN:
        {
            if ( event.key.keysym.scancode == SDL_SCANCODE_ESCAPE )
                SceneManager::GetInstance().SwitchScene( SceneManager::EnumSceneState::MENU );
            break;
        }

        default:
            break;
    }

    EntityManager::GetInstance().OnHandle( event );
}

void SceneGaming::OnUpdate( float delta )
{
    EntityManager::GetInstance().OnUpdate( delta );

    if ( EntityManager::GetInstance().IsPlayerDead() )
        SceneManager::GetInstance().SwitchScene( SceneManager::EnumSceneState::END );
}

void SceneGaming::OnRender( const Camera& cam )
{
    EntityManager::GetInstance().OnRender( cam );

    RenderUi( cam );
}

void SceneGaming::Refresh()
{ }

void SceneGaming::LoadResources()
{
    tex_ui_heart = ResourceManager::GetInstance().FindTexture( "health_heart" );

    font_score = TTF_OpenFont( "resources/font/VonwaonBitmap-12px.ttf", 24 );
}

void SceneGaming::UnloadResources()
{
    TTF_CloseFont( font_score );
}

void SceneGaming::RenderUi( const Camera& cam )
{
    RenderHeartHp( cam );

    RenderScore( cam );
}

void SceneGaming::RenderHeartHp( const Camera& cam )
{
    int width_dst = 32;
    int height_dst = 32;
    float space_offset = width_dst / 3.5f;

    int x_start = 10;
    int y_start = 10;
    int hp_player = EntityManager::GetInstance().GetPlayer()->GetHp();
    int max_hp_player = EntityManager::GetInstance().GetPlayer()->GetMaxHp();

    SDL_SetTextureColorMod( tex_ui_heart, 100, 100, 100 );
    for ( int i = 0; i < max_hp_player; i++ )
    {
        SDL_FRect rect_dst = {
            ( float ) x_start + i * ( width_dst + space_offset ), ( float ) y_start,
            ( float ) width_dst, ( float ) height_dst };
        cam.RenderTexture( tex_ui_heart, nullptr, &rect_dst, 0, nullptr );
    }

    SDL_SetTextureColorMod( tex_ui_heart, 255, 255, 255 );
    for ( int i = 0; i < hp_player; i++ )
    {
        SDL_FRect rect_dst = {
            ( float ) x_start + i * ( width_dst + space_offset ), ( float ) y_start,
            ( float ) width_dst, ( float ) height_dst };
        cam.RenderTexture( tex_ui_heart, nullptr, &rect_dst, 0, nullptr );
    }
}

void SceneGaming::RenderScore( const Camera& cam )
{
    int score = EntityManager::GetInstance().GetScore();
    std::string str_score = "SCORE: " + std::to_string( score );
    SDL_Color color_score = { 255, 255, 255, 255 };
    SDL_Surface* suf_score = TTF_RenderUTF8_Solid( font_score, str_score.c_str(), color_score );
    SDL_Texture* tex_score = SDL_CreateTextureFromSurface( Game::GetInstance().GetRenderer(), suf_score );

    int x_end = Game::GetInstance().GetWidth() - 10;
    int y_start = 10;

    SDL_FRect rect_dst = {
        ( float ) x_end - suf_score->w, ( float ) y_start,
        ( float ) suf_score->w, ( float ) suf_score->h };
    cam.RenderTexture( tex_score, nullptr, &rect_dst, 0, nullptr );

    // clean up
    SDL_DestroyTexture( tex_score );
    SDL_FreeSurface( suf_score );
}
