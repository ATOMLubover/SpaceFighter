#include "Game.h"

#include "SceneManager.h"
#include "EntityManager.h"
#include "ResourceManager.h"
#include "AudioManager.h"

Game& Game::GetInstance()
{
	static Game game;
	return game;
}

Game::Game()
{ }

Game::~Game()
{
	Deinit();
}

void Game::Init()
{
	if ( is_initialized ) return;
	is_initialized = true;

	// SDL init
	{
		if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
		{
			SDL_LogError( SDL_LOG_CATEGORY_ERROR, "SDL init failed, error: %s\n", SDL_GetError() );
			is_quit = true;
		}

		window = SDL_CreateWindow( "Space Fighter",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WIDTH_WIN, HEIGHT_WIN, SDL_WINDOW_SHOWN );
		if ( window == nullptr )
		{
			SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Window creation failed, error: %s\n", SDL_GetError() );
			is_quit = true;
		}

		renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
		if ( renderer == nullptr )
		{
			SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Renderer creation failed, error: %s\n", SDL_GetError() );
			is_quit = true;
		}

		if ( IMG_Init( IMG_INIT_PNG ) != IMG_INIT_PNG )
		{
			SDL_LogError( SDL_LOG_CATEGORY_ERROR, "SDL_image init failed, error: %s\n", IMG_GetError() );
			is_quit = true;
		}

		if ( Mix_Init( MIX_INIT_MP3 | MIX_INIT_OGG ) != ( MIX_INIT_MP3 | MIX_INIT_OGG ) )
		{
			SDL_LogError( SDL_LOG_CATEGORY_ERROR, "SDL_mixer init failed, error: %s\n", Mix_GetError() );
			is_quit = true;
		}
		if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
		{
			SDL_LogError( SDL_LOG_CATEGORY_ERROR, "SDL_OpenAudio init failed, error: %s\n", Mix_GetError() );
			is_quit = true;
		}

		if ( TTF_Init() == -1 )
		{
			SDL_LogError( SDL_LOG_CATEGORY_ERROR, "SDL_ttf init failed, error: %s\n", TTF_GetError() );
			is_quit = true;
		}
	}

	// managers init
	ResourceManager::GetInstance().LoadResources( renderer ); // be aware that resources should be loaded most firstly
	EntityManager::GetInstance().Init();
	SceneManager::GetInstance().Init();

	// data members init
	camera.SetRenderer( renderer );

	// background init
	stars_near.SetTexture( ResourceManager::GetInstance().FindTexture( "stars_near" ) );
	stars_near.SetSpeed( 35 );
	stars_far.SetTexture( ResourceManager::GetInstance().FindTexture( "stars_far" ) );
	stars_far.SetSpeed( 20 );
}

void Game::Deinit()
{
	if ( is_cleaned_up ) return;
	is_cleaned_up = true;

	EntityManager::GetInstance().Deinit();
	SceneManager::GetInstance().Deinit();
	ResourceManager::GetInstance().UnloadResources();

	TTF_Quit();

	Mix_CloseAudio();
	Mix_Quit();

	IMG_Quit();

	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();
}

void Game::Run()
{
	if ( !is_initialized || is_cleaned_up )
		std::cout << "Game is not initialized or cleaned up, failed to run" << std::endl;

	SDL_Event event;

	using namespace std::chrono;

	const nanoseconds FRAME_DURATION( ( long long ) 1e9 / FPS );
	steady_clock::time_point last_tick = steady_clock::now();

	while ( !is_quit )
	{
		HandleEvent( event );

		steady_clock::time_point start_tick = steady_clock::now();
		duration<float> delta( start_tick - last_tick );
		last_tick = start_tick;
		Update( delta.count() );

		Render();

		nanoseconds sleep_duration( FRAME_DURATION - ( steady_clock::now() - start_tick ) );
		if ( sleep_duration > nanoseconds( 0 ) )
			std::this_thread::sleep_for( sleep_duration );
	}
}

void Game::HandleEvent( SDL_Event& event )
{
	while ( SDL_PollEvent( &event ) )
	{
		switch ( event.type )
		{
			case SDL_QUIT:
				is_quit = true;
				break;

			default:
				break;
		}

		SceneManager::GetInstance().OnHandle( event );
	}
}

void Game::Update( float delta )
{
	if ( SceneManager::GetInstance().GetCurrState() != SceneManager::EnumSceneState::END
		 && !EntityManager::GetInstance().PlayerHasExploded() )
		UpdateBackground( delta );

	SceneManager::GetInstance().OnUpdate( delta );

	camera.OnUpdate( delta );
}

void Game::Render()
{
	SDL_RenderClear( renderer );

	if ( SceneManager::GetInstance().GetCurrState() != SceneManager::EnumSceneState::END )
		RenderBackground();
	SceneManager::GetInstance().OnRender( camera );

	SDL_RenderPresent( renderer );
}

void Game::UpdateBackground( float delta )
{
	if ( SceneManager::GetInstance().GetCurrState() != SceneManager::EnumSceneState::END )
	{
		stars_far.OnUpdate( delta );
		stars_near.OnUpdate( delta );
	}
}

void Game::RenderBackground()
{
	if ( SceneManager::GetInstance().GetCurrState() != SceneManager::EnumSceneState::END )
	{
		stars_far.OnRender( camera );
		stars_near.OnRender( camera );
	}
}
