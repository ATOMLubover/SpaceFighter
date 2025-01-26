#include "EntityManager.h"

#include "Game.h"
#include "CollisionManager.h"
#include "AudioManager.h"

const float EntityManager::offset_check_inside = 0;

EntityManager& EntityManager::GetInstance()
{
    static EntityManager manager;
    return manager;
}

void EntityManager::Init()
{
    // player init
    {
        player = new Player;

        int width_win = Game::GetInstance().GetWidth();
        int height_win = Game::GetInstance().GetHeight();
        int width_player = player->GetWidth();
        int height_player = player->GetHeight();
        PlaneVector pos(
            static_cast< double >( width_win / 2 - width_player / 2 ),
            static_cast< double >( height_win - height_player - 100 ) );
        player->SetPosition( pos );
    }

    // random generator init
    {
        // generate a real even distribution on [0.0, 1.0)
        std::random_device rd;
        rand_gen = std::move( std::mt19937( rd() ) );
        hypodispersion = std::move( std::uniform_real_distribution<float>( 0.0f, 1.0f ) );
    }
}

void EntityManager::Deinit()
{
    for ( Enemy* enemy : enemy_list )
        delete enemy;

    for ( BulletPlayer* bullet : bullets_player )
        delete bullet;

    delete player;
}

void EntityManager::OnHandle( SDL_Event& event )
{
    player->OnHandle( event );
}

void EntityManager::OnUpdate( float delta )
{
    player->OnUpdate( delta );
    if ( !have_player_exploded )
        have_player_exploded = player->IsExploding();

    if ( !have_player_exploded )
    {
        if ( player->IsShooting() )
            GenerateBulletPlayer();

        TryGenerateEnemy();

        for ( Enemy* enemy : enemy_list )
        {
            enemy->OnUpdate( delta );
            if ( enemy->IsShooting() )
                GenerateBulletEnemy( enemy );
        }

        for ( BulletPlayer* bullet : bullets_player )
            bullet->OnUpdate( delta );
        for ( BulletEnemy* bullet : bullets_enemy )
            bullet->OnUpdate( delta );

        for ( DropItem* drop : drop_list )
            drop->OnUpdate( delta );

        CollisionManager::GetInstance().ProcessCollision();

        DestroyInvalidEntity();
    }
}

void EntityManager::OnRender( const Camera& cam )
{
    for ( BulletPlayer* bullet : bullets_player )
        bullet->OnRender( cam );
    for ( BulletEnemy* bullet : bullets_enemy )
        bullet->OnRender( cam );

    for ( DropItem* drop : drop_list )
        drop->OnRender( cam );

    for ( Enemy* enemy : enemy_list )
        enemy->OnRender( cam );

    if ( !IsPlayerDead() )
        player->OnRender( cam );
}

bool EntityManager::CheckInsideWindow( const Entity& entity )
{
    int width_win = Game::GetInstance().GetWidth();
    int height_win = Game::GetInstance().GetHeight();
    const PlaneVector& pos_player = entity.GetPosition();
    int width_entity = entity.GetWidth();
    int height_entity = entity.GetHeight();

    bool is_inside =
        pos_player.x + width_entity >= -offset_check_inside
        && pos_player.x <= width_win + offset_check_inside
        && pos_player.y + height_entity >= -offset_check_inside
        && pos_player.y <= height_win + offset_check_inside;
    return is_inside;
}

float EntityManager::GetRandomFloat()
{
    return hypodispersion( rand_gen );
}

void EntityManager::TryGenerateEnemy()
{
    if ( GetRandomFloat() < 1.0 / 60 )
    {
        Enemy* enemy = new Enemy;

        int width_win = Game::GetInstance().GetWidth();
        PlaneVector pos_enemy( GetRandomFloat() * ( width_win - enemy->GetWidth() ), -enemy->GetHeight() );
        enemy->SetPosition( std::move( pos_enemy ) );
        std::function<void( Enemy* )> on_aftermath
            = [ & ] ( Enemy* owner ) { TryGenerateDropItem( owner ); };
        enemy->SetOnAftermath( on_aftermath );

        enemy_list.push_back( enemy );
    }
}

void EntityManager::GenerateBulletPlayer()
{
    BulletPlayer* bullet = new BulletPlayer;

    const PlaneVector& pos_player = player->GetPosition();
    int width_player = player->GetWidth();
    int width_bullet = bullet->GetWidth();
    PlaneVector pos_bullet = { pos_player.x + width_player / 2 - width_bullet / 2, pos_player.y };
    bullet->SetPosition( std::move( pos_bullet ) );

    bullets_player.push_back( bullet );

    AudioManager::GetInstance().PlayAudio( "shoot_player" );
}

void EntityManager::GenerateBulletEnemy( Enemy* enemy_curr )
{
    BulletEnemy* bullet = new BulletEnemy;

    const PlaneVector& pos_player = player->GetPosition();
    const PlaneVector& pos_enemy = enemy_curr->GetPosition();
    PlaneVector center_player( pos_player.x + player->GetWidth() / 2, pos_player.y + player->GetHeight() / 2 );
    PlaneVector center_enemy( pos_enemy.x + enemy_curr->GetWidth() / 2, pos_enemy.y + enemy_curr->GetHeight() / 2 );
    PlaneVector dir_bullet = std::move( center_player - center_enemy );
    bullet->SetPosition( std::move(
        PlaneVector(
            pos_enemy.x + enemy_curr->GetWidth() / 2 - bullet->GetWidth() / 2,
            pos_enemy.y - bullet->GetHeight() ) ) );

    float radians = std::atan2( dir_bullet.y, dir_bullet.x );
    bullet->SetRadians( radians );

    bullets_enemy.push_back( bullet );

    AudioManager::GetInstance().PlayAudio( "shoot_enemy" );
}

void EntityManager::TryGenerateDropItem( Enemy* owner )
{
    if ( GetRandomFloat() < 0.3f )
    {
        // TODO: possibly generate different drops depending on owner.GetType()
        DropItem* item = new DropItem( DropItem::EnumType::HEAL );

        double radians = 2 * 3.1415926 * GetRandomFloat();
        item->SetDirection( radians );
        item->SetLogicCenter( owner->GetTexCenter() );

        drop_list.push_back( item );
    }
}

void EntityManager::DestroyInvalidEntity()
{
    for ( auto iter = bullets_player.begin(); iter != bullets_player.end(); )
    {
        if ( ( *iter )->CheckValid() )
        {
            ++iter;
        }
        else
        {
            delete* iter;
            iter = bullets_player.erase( iter );
        }
    }
    for ( auto iter = bullets_enemy.begin(); iter != bullets_enemy.end();)
    {
        if ( ( *iter )->CheckValid() )
        {
            ++iter;
        }
        else
        {
            delete* iter;
            iter = bullets_enemy.erase( iter );
        }
    }

    for ( auto iter = enemy_list.begin(); iter != enemy_list.end(); )
    {
        if ( ( *iter )->CheckValid() )
        {
            ++iter;
        }
        else
        {
            delete* iter;
            iter = enemy_list.erase( iter );
        }
    }

    for ( auto iter = drop_list.begin(); iter != drop_list.end(); )
    {
        if ( ( *iter )->CheckValid() )
        {
            iter++;
        }
        else
        {
            delete* iter;
            iter = drop_list.erase( iter );
        }
    }
}
