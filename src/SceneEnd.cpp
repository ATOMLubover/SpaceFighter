#include "SceneEnd.h"

#include "SceneManager.h"
#include "EntityManager.h"
#include "Game.h"
#include "AudioManager.h"

using JSON = nlohmann::json;

const std::string SceneEnd::path_rank_save = "save/ranking_list.json";

SceneEnd::SceneEnd()
{
	LoadResources();

	InitTimers();
}

SceneEnd::~SceneEnd()
{
	UnloadResources();

	AudioManager::GetInstance().StopMusic();
}

void SceneEnd::OnEnter()
{
	SDL_Log( "enter scene_end\n" );

	Refresh();

	if ( !SDL_IsTextInputActive() )
		SDL_StartTextInput();
	if ( !SDL_IsTextInputActive() )
		SDL_LogError( SDL_LOG_CATEGORY_ERROR, "text input start failed, error: %s\n", SDL_GetError() );

	AudioManager::GetInstance().PlayAudio( "bgm_end", -1 );
}

void SceneEnd::OnExit()
{
	SDL_Log( "exit scene_end\n" );

	SceneManager::GetInstance().Refresh();
	//EntityManager::GetInstance().Refresh();

	AudioManager::GetInstance().StopMusic();
}

void SceneEnd::OnUpdate( float delta )
{
	timer_blink.OnUpdate( delta );
}

void SceneEnd::OnRender( const Camera& cam )
{
	if ( is_typing )
		RenderTypingPage( cam );
	else
		RenderRankPage( cam );
}

void SceneEnd::Refresh()
{
	is_typing = true;

	is_underline_visible = true;
}

void SceneEnd::LoadResources()
{
	font_title = TTF_OpenFont( "resources/font/VonwaonBitmap-16px.ttf", 64 );
	font_text = TTF_OpenFont( "resources/font/VonwaonBitmap-16px.ttf", 32 );

	LoadRankingList();
}

void SceneEnd::UnloadResources()
{
	TTF_CloseFont( font_text );
	TTF_CloseFont( font_title );

	SaveRankingList();
}

void SceneEnd::InitTimers()
{
	timer_blink.SetOneshot( false );
	timer_blink.SetWaitTime( 0.5f );
	timer_blink.SetOnTimeout(
		[&] ()
		{
			is_underline_visible = !is_underline_visible;
		} );
}

void SceneEnd::RenderTypingPage( const Camera& cam )
{
	float height_win = Game::GetInstance().GetHeight();

	std::string str_score = "Your final score:" + std::to_string( EntityManager::GetInstance().GetScore() );
	float y_score = height_win * 1.0f / 5;
	SceneManager::RenderText( cam, str_score, font_text, nullptr, &y_score );

	std::string str_gameover( "GAME OVER!" );
	float y_gameover = height_win * 2.0f / 5;
	SceneManager::RenderText( cam, str_gameover, font_title, nullptr, &y_gameover );

	std::string str_instr( "Enter your ID to save your record:" );
	float y_instr = height_win * 2.7f / 5;
	SceneManager::RenderText( cam, str_instr, font_text, nullptr, &y_instr );

	if ( !id_player.empty() )
	{
		float y_id = height_win * 3.1f / 5;
		PlaneVector size = SceneManager::RenderText( cam, id_player, font_text, nullptr, &y_id );

		if ( is_underline_visible )
		{
			SDL_Surface* suf_underline = TTF_RenderUTF8_Solid( font_text, "_", { 255, 255, 255, 255 } );
			SDL_Texture* tex_underline = SDL_CreateTextureFromSurface( Game::GetInstance().GetRenderer(), suf_underline );

			float x_underline = Game::GetInstance().GetWidth() / 2.0f + (float) size.x / 2.0f;
			SDL_FRect rect_dst = {
				x_underline, y_id,
				(float) suf_underline->w, (float) suf_underline->h };
			cam.RenderTexture( tex_underline, nullptr, &rect_dst, 0, nullptr );

			SDL_DestroyTexture( tex_underline );
			SDL_FreeSurface( suf_underline );
		}
	}
	else
	{
		if ( is_underline_visible )
		{
			float y_empty = height_win * 3.1f / 5;
			SceneManager::RenderText( cam, "_", font_text, nullptr, &y_empty );
		}
	}
}

void SceneEnd::RenderRankPage( const Camera& cam )
{
	float height_win = Game::GetInstance().GetHeight();

	std::string str_title( "Leader Board" );
	float y_title = height_win * 0.12f;
	SceneManager::RenderText( cam, str_title, font_title, nullptr, &y_title );

	int i = 1;
	for ( const auto& record : ranking_list )
	{
		const std::string& id = std::to_string( i ) + "." + record.second;
		const std::string& score = std::to_string( record.first );

		float y_record = height_win * ( 0.18f + i * 0.05f );
		float x_record_offset = 100;
		SceneManager::RenderText( cam, id, font_text, &x_record_offset, &y_record );
		SceneManager::RenderText( cam, score, font_text, &x_record_offset, &y_record, nullptr, false );

		i++;
	}

	if ( is_underline_visible )
	{
		float y_restart = height_win * 0.8f;
		SceneManager::RenderText( cam, "Press ENTER to return to menu", font_text, nullptr, &y_restart );
	}
}

void SceneEnd::PopBackUtf8( std::string& str )
{
	if ( str.empty() )
		return;

	// en char starts with 01xx xxxx
	// while cn chars starts with 11xx xxxx, followed by 10xx xxxx bytes
	if ( ( str.back() & 0x80 ) == 0x80 )
	{
		str.pop_back();
		while ( ( str.back() & 0xc0 ) != 0xc0 ) str.pop_back();
	}
	str.pop_back();
}

void SceneEnd::LoadRankingList()
{
	std::ifstream ifs( path_rank_save );
	if ( ifs.is_open() )
	{
		std::stringstream ss_rank;
		ss_rank << ifs.rdbuf();
		std::string str_rank( ss_rank.str() );
		JSON json_rank = JSON::parse( str_rank );

		std::map<int, std::string> reserved_rank = json_rank.get<std::map<int, std::string>>();
		for ( const auto& pair : reserved_rank )
			ranking_list.insert( std::move( pair ) );
	}
	else
	{
		SDL_Log( "Unable to open save file with ifs\n" );
	}
	ifs.close();
}

void SceneEnd::SaveRankingList()
{
	std::ofstream ofs( "save/ranking_list.json" );
	if ( ofs.is_open() )
	{
		JSON json_rank = ranking_list;
		std::string str_rank = json_rank.dump( 4 );
		ofs << str_rank.c_str();
	}
	else
	{
		SDL_Log( "Unable to open save file with ofs\n" );
	}
	ofs.close();
}

void SceneEnd::InsertRankingList( const std::string& id, int score )
{
	for ( auto iter = ranking_list.begin(); iter != ranking_list.end(); ++iter )
	{
		if ( iter->second == id )
		{
			if ( score > iter->first )
				ranking_list.erase( iter );
			break;
		}
	}
	ranking_list.emplace( score, id );
	if ( ranking_list.size() > 8 )
		ranking_list.erase( --ranking_list.end() );
}

void SceneEnd::OnHandle( SDL_Event& event )
{
	if ( is_typing )
	{
		switch ( event.type )
		{
			case SDL_TEXTINPUT:
				if ( id_player.size() <= 15 )
					id_player += event.text.text; // text contains only one char(en/cn)
				break;

			case SDL_KEYDOWN:
				switch ( event.key.keysym.scancode )
				{
					case SDL_SCANCODE_RETURN:
					{
						is_typing = false;
						SDL_StopTextInput();

						if ( !id_player.empty() )
							InsertRankingList( id_player, EntityManager::GetInstance().GetScore() );

						timer_blink.Restart();

						break;
					}

					case SDL_SCANCODE_BACKSPACE:
						PopBackUtf8( id_player );
						break;

					default:
						break;
				}
				break;

			default:
				break;
		}
	}
	else
	{
		switch ( event.type )
		{
			case SDL_KEYDOWN:
				if ( event.key.keysym.scancode == SDL_SCANCODE_RETURN )
				{
					SceneManager::GetInstance().SwitchScene( SceneManager::EnumSceneState::MENU );
				}
				AudioManager::GetInstance().PlayAudio( "ui_comfirm" );
				break;

			default:
				break;
		}
	}
}
