#include "Enemy.h"

#include "ResourceManager.h"
#include "CollisionManager.h"
#include "AudioManager.h"
#include "Game.h"
#include "EntityManager.h"

Enemy::Enemy()
{
    speed = 180.0f;
    speed_explode = 160.0f;

    hp = 3;

    texture = ResourceManager::GetInstance().FindTexture( "enemy" );
    SDL_QueryTexture( texture, nullptr, nullptr, &width, &height );
    width /= 3;
    height /= 3;

    collision_box = CollisionManager::GetInstance().CreateCollisionBox();
    collision_box->SetLayerSrc( EnumCollisionLayer::ENEMY );
    collision_box->SetLayerDst( EnumCollisionLayer::PLAYER );
    collision_box->SetSize( { static_cast< double >( width ), static_cast< double >( height ) } );
    collision_box->SetOnCollideSrc(
        [ & ] ()
        {
            OnDie();
        } );
    collision_box->SetOnCollideDst(
        [ & ] ( int damage )
        {
            hp -= damage;
            if ( hp <= 0 )
                OnDie();
        } );

    timer_cool.SetOneshot( true );
    timer_cool.SetWaitTime( 1.2f );
    timer_cool.SetOnTimeout(
        [ & ] ()
        {
            is_cooled_down = true;
        } );
    timer_cool.Restart(); // start first time when generated

    SDL_Texture* tex_atlas_explode = ResourceManager::GetInstance().FindTexture( "explosion_enemy" );
    int width_tex, height_tex;
    SDL_QueryTexture( tex_atlas_explode, nullptr, nullptr, &width_tex, &height_tex );
    int num_h = width_tex / height_tex;
    anim_explode.LoadAtlas( tex_atlas_explode, num_h );
    anim_explode.SetLoop( false );
    anim_explode.SetInterval( 0.08f );
    anim_explode.SetRenderScale( 1.5f );
    anim_explode.SetOnStarted(
        [ & ] ()
        {
            EntityManager::GetInstance().IncrementScore( 20 );

            AudioManager::GetInstance().PlayAudio( "explosion_enemy" );
        } );
    anim_explode.SetOnFinished(
        [ & ] ()
        {
            on_aftermath( this );
            MakeInvalid();
        } );
}

Enemy::~Enemy()
{
    CollisionManager::GetInstance().DestroyCollisionBox( collision_box );
}

void Enemy::OnUpdate( float delta )
{
    if ( hp > 0 )
    {
        timer_cool.OnUpdate( delta );

        position += PlaneVector( 0, 1 ) * ( speed * delta );

        UpdateCollisionBox();

        if ( is_cooled_down )
        {
            is_cooled_down = false;
            is_try_shoot = true;
            timer_cool.Restart();
        }
    }
    else
    {
        position += PlaneVector( 0, 1 ) * ( speed_explode * delta );
        speed_explode -= 300 * delta; // decrease 300pix per second

        anim_explode.SetLogicCenter( PlaneVector(
            position.x + ( width / 2.0 ), position.y + ( height / 2.0 ) ) );
        anim_explode.OnUpdate( delta );
    }

    if ( position.y > Game::GetInstance().GetHeight() + 2 )
        MakeInvalid();
}

void Enemy::OnRender( const Camera& cam )
{
    if ( hp > 0 )
    {
        SDL_FRect rect_dst = {
            ( float ) position.x, ( float ) position.y,
            ( float ) width, ( float ) height };
        cam.RenderTexture( texture, nullptr, &rect_dst, 0, nullptr );
    }
    else
    {
        anim_explode.OnRender( cam );
    }
}

bool Enemy::IsShooting()
{
    if ( is_try_shoot )
        return !( is_try_shoot = false );
    return false;
}

void Enemy::OnDie()
{
    hp = -1;
    collision_box->SetEnabled( false );
}

void Enemy::UpdateCollisionBox()
{
    collision_box->SetLogicCenter( GetTexCenter() );
}
