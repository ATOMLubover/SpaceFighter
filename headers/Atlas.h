#ifndef _ATLAS_H_
#define _ATLAS_H_

#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <string>

// preprocess scattered pics
class Atlas
{
public:
    Atlas() = default;
    ~Atlas() { Clear(); }

    void Load( SDL_Renderer* renderer, const std::string& path_format, int num );
    void Clear();

    SDL_Texture* GetTextureAt( int index ) const;

    int GetSize() const { return ( int ) tex_list.size(); }

    //void AddTexture( SDL_Texture* tex ) { tex_list.push_back( tex ); }

private:
    std::vector<SDL_Texture*> tex_list;
};

#endif