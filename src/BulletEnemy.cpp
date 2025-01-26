#include "BulletEnemy.h"

#include "ResourceManager.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "AudioManager.h"

BulletEnemy::BulletEnemy()
{
    speed = 350.0f;

    damage = 1;

    texture = ResourceManager::GetInstance().FindTexture( "bullet_enemy" );
    SDL_QueryTexture( texture, nullptr, nullptr, &width, &height );
    width /= 2;
    height /= 2;

    collision_box = CollisionManager::GetInstance().CreateCollisionBox();
    collision_box->SetLayerSrc( EnumCollisionLayer::BULLET );
    collision_box->SetLayerDst( EnumCollisionLayer::PLAYER );
    collision_box->SetSize( { width / 2.0f, height / 2.0f } );
    collision_box->SetOnCollideSrc( [ & ] ()
        {
            MakeInvalid();
            AudioManager::GetInstance().PlayAudio( "hit" );
        } );
    collision_box->SetReflectTag( CollisionBox::EnumReflectTag::BULLET );
    collision_box->SetBulletOwner( this );
}

BulletEnemy::~BulletEnemy()
{
    CollisionManager::GetInstance().DestroyCollisionBox( collision_box );
}

void BulletEnemy::OnUpdate( float delta )
{
    position += PlaneVector( std::cos( radians ), std::sin( radians ) ) * ( speed * delta );

    UpdateCollisionBox();

    if ( !EntityManager::CheckInsideWindow( *this ) )
        MakeInvalid();
}

void BulletEnemy::OnRender( const Camera& cam )
{
    float angle = radians * 180 / 3.1415926;
    SDL_FRect rect_src = { position.x, position.y, width, height };
    cam.RenderTexture( texture, nullptr, &rect_src, angle - 90, nullptr );
}

void BulletEnemy::UpdateCollisionBox()
{
    collision_box->SetLogicCenter( GetTexCenter() );
}
