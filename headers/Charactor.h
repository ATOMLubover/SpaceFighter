#ifndef _CHARACTOR_H_
#define _CHARACTOR_H_

#include "Entity.h"

#include <SDL.h>

class Charactor : public Entity
{
public:
    Charactor() = default;
    virtual ~Charactor() = default;

    virtual void OnUpdate( float delta ) = 0;
    virtual void OnRender( const Camera& cam ) override = 0;

protected:
    float speed;

    int hp;
};

#endif