#include "Background.h"

#include "Game.h"

void Background::OnUpdate( float delta )
{
    offset += speed * delta;
    if ( offset > 0 )
        offset -= height;
}

void Background::OnRender( const Camera& cam )
{
    int width_win = Game::GetInstance().GetWidth();
    int height_win = Game::GetInstance().GetHeight();
    int num_h = static_cast< int >( std::ceil( width_win / ( float ) width ) );
    int num_v = static_cast< int >( std::ceil( ( height_win - offset ) / ( float ) height ) );
    for ( int i = 0; i < num_h; i++ )
    {
        for ( int j = 0; j < num_v; j++ )
        {
            SDL_FRect rect_dst = {
                ( float ) i * width, ( float ) offset + j * height,
                ( float ) width, ( float ) height };
            cam.RenderTexture( raw_tex, nullptr, &rect_dst, 0, nullptr );
        }
    }
}

void Background::SetTexture( SDL_Texture* texture )
{
    raw_tex = texture;
    SDL_QueryTexture( texture, nullptr, nullptr, &width, &height );
    width /= 2;
    height /= 2;
}
