#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include "Atlas.h"

#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <exception>

class ResourceManager
{
    struct TexResInfo
    {
        std::string id;
        std::string relative_path;
    };

    struct AtlasResInfo
    {
        std::string id;
        std::string relative_path;
        int num;
    };


public:
    static ResourceManager& GetInstance();

public:
    ResourceManager( const ResourceManager& rhs ) = delete;
    ResourceManager& operator=( const ResourceManager& rhs ) = delete;
    ~ResourceManager() = default;

    void LoadResources( SDL_Renderer* renderer );
    void UnloadResources();

    SDL_Texture* FindTexture( const std::string& id ) const;
    Atlas* FindAtlas( const std::string& id ) const;

private:
    explicit ResourceManager() = default;

private:
    static const std::vector<TexResInfo> tex_info_list;
    static const std::vector<AtlasResInfo> atlas_info_list;

private:
    std::unordered_map<std::string, SDL_Texture*> tex_pool;
    std::unordered_map<std::string, Atlas*> atlas_pool;
};

#endif // !_RESOURCE_MANAGER_H_