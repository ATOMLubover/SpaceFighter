#include "ResourceManager.h"

#include "Game.h"
#include "AudioManager.h"

const std::vector<ResourceManager::TexResInfo> ResourceManager::tex_info_list =
{
    { "player",             "resources/image/SpaceShip.png" },
    { "bullet_player",      "resources/image/laser-1.png" },
    { "enemy",              "resources/image/insect-1.png" },
    { "bullet_enemy",       "resources/image/bullet-1.png" },

    { "explosion_enemy",    "resources/effect/explosion.png" },

    { "drop_heal",          "resources/image/bonus_life.png" },

    { "stars_near",         "resources/image/Stars-A.png" },
    { "stars_far",          "resources/image/Stars-B.png" },

    { "health_heart",       "resources/image/Health_UI_Black.png" }
};

const std::vector<ResourceManager::AtlasResInfo> ResourceManager::atlas_info_list =
{
    { "atlas_explosion_player", "resources/image/sun_ex_explode_%d.png", 5 }
};

ResourceManager& ResourceManager::GetInstance()
{
    static ResourceManager manager;
    return manager;
}

void ResourceManager::LoadResources( SDL_Renderer* renderer )
{
    for ( const auto& info : tex_info_list )
    {
        SDL_Texture* tex = IMG_LoadTexture( renderer, info.relative_path.c_str() );
        if ( !tex )
        {
            SDL_LogError( SDL_LOG_CATEGORY_ERROR,
                "Resource load failed on path: %s\n", info.relative_path.c_str() );
            throw std::exception();
        }
        tex_pool[ info.id ] = tex;
    }

    for ( const auto& info : atlas_info_list )
    {
        Atlas* atlas = new Atlas;
        atlas->Load( Game::GetInstance().GetRenderer(), info.relative_path, info.num );
        atlas_pool[ info.id ] = atlas;
    }

    AudioManager::GetInstance().Init();
}

void ResourceManager::UnloadResources()
{
    AudioManager::GetInstance().Deinit();

    for ( const auto& pair : tex_pool )
        SDL_DestroyTexture( pair.second );

    for ( const auto& pair : atlas_pool )
        pair.second->Clear();
}

SDL_Texture* ResourceManager::FindTexture( const std::string& id ) const
{
    auto iter = tex_pool.find( id );
    if ( iter != tex_pool.end() ) return iter->second;
    else return nullptr;
}

Atlas* ResourceManager::FindAtlas( const std::string& id ) const
{
    auto iter = atlas_pool.find( id );
    if ( iter != atlas_pool.end() ) return iter->second;
    else return nullptr;
}
