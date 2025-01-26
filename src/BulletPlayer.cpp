#include "BulletPlayer.h"

#include "EntityManager.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "AudioManager.h"

BulletPlayer::BulletPlayer()
{
    speed = 450.0f;

    damage = 1;

    texture = ResourceManager::GetInstance().FindTexture( "bullet_player" );
    SDL_QueryTexture( texture, nullptr, nullptr, &width, &height );
    width /= 3;
    height /= 3;

    collision_box = CollisionManager::GetInstance().CreateCollisionBox();
    collision_box->SetLayerSrc( EnumCollisionLayer::BULLET );
    collision_box->SetLayerDst( EnumCollisionLayer::ENEMY );
    collision_box->SetSize( { static_cast< double >( width ), static_cast< double >( height ) } );
    collision_box->SetOnCollideSrc( [ & ] ()
        {
            MakeInvalid();
            AudioManager::GetInstance().PlayAudio( "hit" );
        } );
    collision_box->SetReflectTag( CollisionBox::EnumReflectTag::BULLET );
    collision_box->SetBulletOwner( this );
}

BulletPlayer::~BulletPlayer()
{
    CollisionManager::GetInstance().DestroyCollisionBox( collision_box );
}

void BulletPlayer::OnUpdate( float delta )
{
    position += PlaneVector( 0, -1 ) * ( speed * delta );

    UpdateCollisionBox();

    if ( !EntityManager::CheckInsideWindow( *this ) )
        MakeInvalid();
}

void BulletPlayer::OnRender( const Camera& cam )
{
    SDL_FRect rect_dst = {
        ( float ) position.x, ( float ) position.y,
        width, height };
    cam.RenderTexture( texture, nullptr, &rect_dst, 0, nullptr );
}

void BulletPlayer::UpdateCollisionBox()
{
    collision_box->SetLogicCenter( GetTexCenter() );
}
