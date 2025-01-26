#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "Charactor.h"
#include "Timer.h"
#include "Animation.h"

#include <functional>

class Enemy : public Charactor
{
public:
    Enemy();
    ~Enemy();

    void OnUpdate( float delta ) override;
    void OnRender( const Camera& cam ) override;

    bool IsShooting();

    void OnDie(); // death after hit

    void SetOnAftermath( const std::function<void( Enemy* )>& on_gen ) { on_aftermath = on_gen; }

protected:
    void UpdateCollisionBox() override;

private:
    bool is_cooled_down = false; // dont shoot when just generated
    Timer timer_cool;
    bool is_try_shoot = false;

    Animation anim_explode;
    float speed_explode; // used to create a slowdowm in explosion fire

    std::function<void( Enemy* )> on_aftermath;
};

#endif