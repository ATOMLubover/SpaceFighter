#ifndef _BULLET_ENEMY_H_
#define _BULLET_ENEMY_H_

#include "Bullet.h"

#include <cmath>

class BulletEnemy : public Bullet
{
public:
    BulletEnemy();
    ~BulletEnemy();

    void OnUpdate( float delta ) override;
    void OnRender( const Camera& cam ) override;

    void SetRadians( float radians ) { this->radians = radians; }

protected:
    void UpdateCollisionBox() override;

private:
    float radians;
};

#endif