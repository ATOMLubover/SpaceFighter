#include "AudioManager.h"

const std::vector<AudioManager::AudioResInfo> AudioManager::audio_info_list =
{
    { "bgm_battle",         "resources/music/03_Racing_Through_Asteroids_Loop.ogg",     EnumLoadType::MUSIC },
    { "bgm_menu",           "resources/music/06_Battle_in_Space_Intro.ogg",             EnumLoadType::MUSIC},
    { "bgm_end",            "resources/music/bgm.mp3",                                  EnumLoadType::MUSIC },

    { "shoot_player",       "resources/sound/laser_shoot4.wav",                         EnumLoadType::WAV },
    { "shoot_enemy",        "resources/sound/xs_laser.wav",                             EnumLoadType::WAV },
    { "explosion_player",   "resources/sound/explosion1.wav",                           EnumLoadType::WAV },
    { "explosion_enemy",    "resources/sound/explosion.wav",                            EnumLoadType::WAV },
    { "hit",                "resources/sound/eff11.wav",                                EnumLoadType::WAV },
    { "pick_drop",          "resources/sound/eff5.wav",                                 EnumLoadType::WAV },

    { "ui_comfirm",         "resources/sound/ui_confirm.wav",                           EnumLoadType::WAV }
};

AudioManager& AudioManager::GetInstance()
{
    static AudioManager manager;
    return manager;
}

void AudioManager::Init( int channel_num )
{
    Mix_AllocateChannels( channel_num );

    for ( const auto& info : audio_info_list )
    {
        switch ( info.type )
        {
            case EnumLoadType::MUSIC:
            {
                Mix_Music* music = Mix_LoadMUS( info.relative_path.c_str() );
                music_list[ info.id ] = music;
                break;
            }

            case EnumLoadType::WAV:
            {
                Mix_Chunk* chunk = Mix_LoadWAV( info.relative_path.c_str() );
                chunk_list[ info.id ] = chunk;
                break;
            }

            default:
                break;
        }
    }
}

void AudioManager::Deinit()
{
    Mix_HaltMusic();

    for ( const auto& pair : chunk_list )
        Mix_FreeChunk( pair.second );
    for ( const auto& pair : music_list )
        Mix_FreeMusic( pair.second );
}

void AudioManager::PlayAudio( const std::string& id, int loops ) const
{
    {
        auto iter = chunk_list.find( id );
        if ( iter != chunk_list.end() )
        {
            int channel = -1;
            if ( iter->first == "shoot_player" ) channel = 0;
            Mix_PlayChannel( channel, iter->second, loops );
            return;
        }
    }
    {
        auto iter = music_list.find( id );
        if ( iter != music_list.end() )
        {
            Mix_PlayMusic( iter->second, loops );
            return;
        }
    }
}

void AudioManager::StopMusic() const
{
    Mix_HaltMusic();
}
