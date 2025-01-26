#ifndef _BULLET_H_
#define _BULLET_H_

#include "Entity.h"
#include "PlaneVector.h"

#include <SDL.h>

class Bullet : public Entity
{
public:
    Bullet() = default;
    virtual ~Bullet() = default;

    virtual void OnUpdate( float delta ) = 0;
    virtual void OnRender( const Camera& cam ) override = 0;

    virtual void MakeInvalid();

    int GetDamage() const { return damage; }

protected:
    float speed;
    int damage;
};

#endif