#include "CollisionManager.h"

#include "Bullet.h"
#include "DropItem.h"

CollisionManager& CollisionManager::GetInstance()
{
    static CollisionManager manager;
    return manager;
}

CollisionBox* CollisionManager::CreateCollisionBox()
{
    CollisionBox* box = new CollisionBox;
    collision_box_list.push_back( box );
    return box;
}

void CollisionManager::DestroyCollisionBox( CollisionBox* box )
{
    collision_box_list.erase(
        std::remove( collision_box_list.begin(), collision_box_list.end(), box ),
        collision_box_list.end() );
    delete box;
}

void CollisionManager::ProcessCollision()
{
    for ( CollisionBox* box_src : collision_box_list )
    {
        if ( !box_src->is_enabled || box_src->layer_dst == EnumCollisionLayer::NONE )
            continue;

        for ( CollisionBox* box_dst : collision_box_list )
        {
            if ( !box_dst->is_enabled
                || box_src == box_dst || box_src->layer_dst != box_dst->layer_src )
                continue;

            // only when dst is charactor it can reach here
            if ( CheckCollsion( box_src, box_dst ) )
                RunOnCollision( box_src, box_dst );
        }
    }
}

bool CollisionManager::CheckCollsion( CollisionBox* src, CollisionBox* dst )
{
    // bool is_collided_x = (
    //     std::max( src->position.x + src->size.x, dst->position.x + dst->size.x )
    //     - std::min( src->position.x, dst->position.x )
    //     <= src->size.x + dst->size.x );
    // bool is_collided_y = (
    //     std::max( src->position.y + src->size.y, dst->position.y + dst->size.y )
    //     - std::min( src->position.y, dst->position.y )
    //     <= src->size.y + dst->size.y );

    // return is_collided_x && is_collided_y;

    SDL_Rect rect_src = {
        static_cast< int >( src->position.x ),static_cast< int >( src->position.y ),
        static_cast< int >( src->size.x ), static_cast< int >( src->size.y ) };
    SDL_Rect rect_dst = {
        static_cast< int >( dst->position.x ),static_cast< int >( dst->position.y ),
        static_cast< int >( dst->size.x ),static_cast< int >( dst->size.y ) };
    return SDL_HasIntersection( &rect_src, &rect_dst );
}

void CollisionManager::RunOnCollision( CollisionBox* src, CollisionBox* dst )
{
    if ( src->on_collide_src )
        src->on_collide_src();

    switch ( src->layer_src )
    {
        case EnumCollisionLayer::BULLET:
            if ( src->owner_bullet && dst->on_collide_dst )
                dst->on_collide_dst( src->owner_bullet->GetDamage() );
            break;

        case EnumCollisionLayer::DROP:
            if ( dst->layer_src != EnumCollisionLayer::PLAYER )
                break; // only effects on player

            if ( dst->on_collide_dst_getdrop
                && *dst->on_collide_dst_getdrop && src->owner_drop )
                ( *dst->on_collide_dst_getdrop )( src->owner_drop );
            break;

        default: // if src is charactor
            if ( dst->on_collide_dst )
                dst->on_collide_dst( 1 );
            break;
    }
}
