#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include <SDL_mixer.h>
#include <SDL.h>

#include <vector>
#include <string>
#include <unordered_map>

class AudioManager
{
    enum class EnumLoadType
    {
        WAV,
        MUSIC,
        MP3
    };


    struct AudioResInfo
    {
        std::string id;
        std::string relative_path;
        EnumLoadType type = EnumLoadType::WAV;
    };

public:
    static AudioManager& GetInstance();

public:
    AudioManager( const AudioManager& rhs ) = delete;
    AudioManager& operator=( const AudioManager& rhs ) = delete;
    ~AudioManager() = default;

    void Init( int channel_num = 32 );
    void Deinit();

    void PlayAudio( const std::string& id, int loops = 0 ) const;
    void StopMusic() const;

    void SetVolumeMusic( int volume ) { Mix_VolumeMusic( volume ); }
    void SetVolumeChunk( int volume, int channel = -1 ) { Mix_Volume( channel, volume ); }

private:
    AudioManager() = default;

private:
    static const std::vector<AudioResInfo> audio_info_list;

    std::unordered_map<std::string, Mix_Music*> music_list;
    std::unordered_map<std::string, Mix_Chunk*> chunk_list;
};

#endif