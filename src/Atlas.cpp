#include "Atlas.h"

void Atlas::Load( SDL_Renderer* renderer, const std::string& path_format, int num )
{
    Clear();

    tex_list.resize( num );
    const char* path_template = path_format.c_str();
    for ( int i = 0; i < num; i++ )
    {
        char path[ 256 ];
        sprintf_s( path, path_template, i + 1 );
        SDL_Texture* texture = IMG_LoadTexture( renderer, path );
        tex_list[ i ] = texture;
    }
}

void Atlas::Clear()
{
    for ( SDL_Texture* texture : tex_list )
        SDL_DestroyTexture( texture );
    tex_list.clear();
}

SDL_Texture* Atlas::GetTextureAt( int index ) const
{
    if ( index < 0 || index >= ( int ) tex_list.size() )
        return nullptr;

    return tex_list[ index ];
}
