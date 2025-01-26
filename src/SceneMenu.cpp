#include "SceneMenu.h"

#include "ResourceManager.h"
#include "AudioManager.h"
#include "SceneManager.h"
#include "Game.h"
#include "EntityManager.h"

SceneMenu::SceneMenu()
{
    LoadResources();

    InitTimers();
}

SceneMenu::~SceneMenu()
{
    UnloadResources();
}

void SceneMenu::OnEnter()
{
    SDL_Log( "enter scene_menu\n" );

    Refresh();

    AudioManager::GetInstance().PlayAudio( "bgm_menu", -1 );
}

void SceneMenu::OnExit()
{
    SDL_Log( "exit scene_menu\n" );

    AudioManager::GetInstance().StopMusic();
}

void SceneMenu::OnHandle( SDL_Event& event )
{
    switch ( event.type )
    {
        case SDL_KEYDOWN:
        {
            if ( event.key.keysym.scancode == SDL_SCANCODE_J )
            {
                SceneManager::GetInstance().SwitchScene( SceneManager::EnumSceneState::GAMING );
                AudioManager::GetInstance().PlayAudio( "ui_comfirm" );
            }

            break;
        }

        default:
            break;
    }
}

void SceneMenu::OnUpdate( float delta )
{
    timer_text_blink.OnUpdate( delta );
}

void SceneMenu::OnRender( const Camera& cam )
{
    float y_title = Game::GetInstance().GetHeight() * 2.0f / 5;
    SceneManager::RenderText( cam, "Space Fighter", font_title, nullptr, &y_title );

    if ( is_text_visible )
    {
        float y_tip = Game::GetInstance().GetHeight() * 3.8f / 5;
        SceneManager::RenderText( cam, "WASD to move, SPACE to shoot", font_tip, nullptr, &y_tip );

        float y_start = Game::GetInstance().GetHeight() * 3.5f / 5;
        SceneManager::RenderText( cam, "Press J to start", font_text, nullptr, &y_start );
    }
}

void SceneMenu::InitTimers()
{
    timer_text_blink.SetOneshot( false );
    timer_text_blink.SetWaitTime( 0.5f );
    timer_text_blink.SetOnTimeout( [ & ] () { is_text_visible = !is_text_visible; } );
}

void SceneMenu::Refresh()
{
    timer_text_blink.Restart();
    timer_text_blink.Resume();

    is_text_visible = true;

    EntityManager::GetInstance().Refresh();
}

void SceneMenu::LoadResources()
{
    font_title = TTF_OpenFont( "resources/font/VonwaonBitmap-16px.ttf", 64 );
    font_text = TTF_OpenFont( "resources/font/VonwaonBitmap-16px.ttf", 32 );
    font_tip = TTF_OpenFont( "resources/font/VonwaonBitmap-16px.ttf", 16 );
}

void SceneMenu::UnloadResources()
{
    TTF_CloseFont( font_tip );
    TTF_CloseFont( font_text );
    TTF_CloseFont( font_title );
}
