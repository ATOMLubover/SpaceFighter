#include "DropItem.h"

#include "ResourceManager.h"
#include "CollisionManager.h"
#include "EntityManager.h"
#include "Game.h"
#include "AudioManager.h"

DropItem::DropItem( EnumType type )
{
    switch ( type )
    {
        case EnumType::HEAL:
        {
            speed = 250.0f;

            total_rebound_time = 3;

            texture = ResourceManager::GetInstance().FindTexture( "drop_heal" );
            SDL_QueryTexture( texture, nullptr, nullptr, &width, &height );
            width /= 4;
            height /= 4;

            collision_box = CollisionManager::GetInstance().CreateCollisionBox();
            collision_box->SetLayerSrc( EnumCollisionLayer::DROP );
            collision_box->SetLayerDst( EnumCollisionLayer::PLAYER );
            collision_box->SetSize( { ( double ) width, ( double ) height } );
            collision_box->SetOnCollideSrc( [ & ] ()
                {
                    EntityManager::GetInstance().IncrementScore( 5 );

                    MakeInvalid();
                    AudioManager::GetInstance().PlayAudio( "pick_drop" );
                } );
            collision_box->SetReflectTag( CollisionBox::EnumReflectTag::DROP );
            collision_box->SetDropOwner( this );
        }
    }
}

void DropItem::OnUpdate( float delta )
{
    PlaneVector pos_ori = position;
    position += velocity * delta;
    if ( ( times_rebounded < total_rebound_time ) && CheckReachEdge() )
        Rebound( pos_ori );

    UpdateCollisionBox();

    if ( !EntityManager::CheckInsideWindow( *this ) )
        is_valid = false;
}

void DropItem::OnRender( const Camera& cam )
{
    SDL_FRect rect_dst = {
        ( float ) position.x, ( float ) position.y,
        width, height };
    cam.RenderTexture( texture, nullptr, &rect_dst, 0, nullptr );
}

void DropItem::SetDirection( double radians )
{
    velocity.x = speed * std::cos( radians );
    velocity.y = speed * std::sin( radians );
}

void DropItem::UpdateCollisionBox()
{
    collision_box->SetLogicCenter( GetTexCenter() );
}

bool DropItem::CheckReachEdge() const
{
    int width_win = Game::GetInstance().GetWidth();
    int height_win = Game::GetInstance().GetHeight();
    bool is_reaching_edge =
        position.x + width > ( double ) width_win
        || position.x < 0
        || position.y + height >( double ) height_win
        || position.y < 0;
    return is_reaching_edge;
}

void DropItem::Rebound( const PlaneVector& pos_ori )
{
    int width_win = Game::GetInstance().GetWidth();
    int height_win = Game::GetInstance().GetHeight();
    //float offset_check_inside = EntityManager::GetOffsetCheckInside();
    if ( position.x + width > ( double ) width_win
        || position.x < 0 )
        velocity.x = -velocity.x;
    if ( position.y + height > ( double ) height_win
        || position.y < 0 )
        velocity.y = -velocity.y;

    position = pos_ori;
    times_rebounded++;
}
