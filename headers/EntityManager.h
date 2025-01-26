#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#include "Player.h"
#include "BulletPlayer.h"
#include "Enemy.h"
#include "BulletEnemy.h"
#include "DropItem.h"

#include <list>
#include <algorithm>
#include <random>
#include <cmath>

class EntityManager
{
public:
    static EntityManager& GetInstance();

    static bool CheckInsideWindow( const Entity& entity );
    static bool GetOffsetCheckInside() { return offset_check_inside; }

public:
    EntityManager( const EntityManager& rhs ) = delete;
    EntityManager& operator=( const EntityManager& rhs ) = delete;
    ~EntityManager() = default;

    void Init();
    void Deinit();

    void OnHandle( SDL_Event& event );
    void OnUpdate( float delta );
    void OnRender( const Camera& cam );

    bool IsPlayerDead() const { return !player->CheckValid(); }
    bool PlayerHasExploded() const { return have_player_exploded; }

    Player* const GetPlayer() const { return player; }
    void IncrementScore( int delta ) { player->IncrementScore( delta ); }
    int GetScore() const { return player->GetScore(); }

private:
    EntityManager() = default;

    float GetRandomFloat();

    void TryGenerateEnemy();

    void GenerateBulletPlayer();
    void GenerateBulletEnemy( Enemy* enemy_curr );

    void TryGenerateDropItem( Enemy* owner );

    void DestroyInvalidEntity();

private:
    static const float offset_check_inside;

private:
    Player* player = nullptr;
    bool have_player_exploded = false;

    std::list<BulletPlayer*> bullets_player;
    std::list<BulletEnemy*> bullets_enemy;

    std::list <Enemy*> enemy_list;

    std::list<DropItem*> drop_list;

    // random generator
    std::mt19937 rand_gen;
    std::uniform_real_distribution<float> hypodispersion;
};

#endif // !_ENTITY_MANAGER_H_