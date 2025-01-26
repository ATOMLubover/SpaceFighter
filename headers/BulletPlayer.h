#ifndef _BULLET_PLAYER_H_
#define _BULLET_PLAYER_H_

#include "Bullet.h"

class BulletPlayer : public Bullet
{
public:
    BulletPlayer();
    ~BulletPlayer();

    void OnUpdate( float delta ) override;
    void OnRender( const Camera& cam ) override;

protected:
    void UpdateCollisionBox() override;
};

#endif