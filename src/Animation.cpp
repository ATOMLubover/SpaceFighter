#include "Animation.h"

Animation::Animation()
{
    timer.SetOneshot( false );
    timer.SetOnTimeout(
        [ & ] ()
        {
            idx_frame++;
            if ( idx_frame >= frame_list.size() )
            {
                idx_frame = is_loop ? 0 : frame_list.size() - 1;
                if ( !is_loop && on_finished ) on_finished();
            }
        }
    );
}

void Animation::Reset()
{
    timer.Restart();

    idx_frame = 0;
}

void Animation::LoadAtlas( SDL_Texture* texture_atlas, int num_h ) // num_h = num_horizontal 
{
    int width, height;
    SDL_QueryTexture( texture_atlas, nullptr, nullptr, &width, &height );

    int width_per_frame = width / num_h;

    for ( int i = 0; i < num_h; i++ )
    {
        SDL_Rect rect_src = {
            i * width_per_frame, 0,
            width_per_frame, height };
        frame_list.emplace_back( texture_atlas, rect_src );
    }
}

void Animation::LoadAtlas( Atlas& atlas )
{
    for ( int i = 0; i < atlas.GetSize(); i++ )
    {
        SDL_Texture* texture = atlas.GetTextureAt( i );

        int width, height;
        SDL_QueryTexture( texture, nullptr, nullptr, &width, &height );

        SDL_Rect rect_src = { 0, 0, width, height };
        frame_list.emplace_back( texture, rect_src );
    }
}

void Animation::OnUpdate( float delta )
{
    if ( timer.IsReady() && on_started )
        on_started();
    timer.OnUpdate( delta );
}

void Animation::OnRender( const Camera& camera )
{
    const Frame& frame = frame_list[ idx_frame ];

    float width_render = frame.rect_src.w * scale_ratio;
    float height_render = frame.rect_src.h * scale_ratio;

    SDL_FRect rect_dst = {
        pos_center.x - width_render / 2, pos_center.y - height_render / 2,
        width_render,  height_render };
    camera.RenderTexture( frame.texture, &frame.rect_src, &rect_dst, angle, &centre );
}
