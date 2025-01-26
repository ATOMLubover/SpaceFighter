#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

#include "CollisionBox.h"

#include <SDL.h>

class CollisionManager
{
public:
    static CollisionManager& GetInstance();

public:
    CollisionBox* CreateCollisionBox();
    void DestroyCollisionBox( CollisionBox* box );

    void ProcessCollision();

private:
    CollisionManager() = default;
    ~CollisionManager() = default;

    bool CheckCollsion( CollisionBox* src, CollisionBox* dst );

    void RunOnCollision( CollisionBox* src, CollisionBox* dst );

private:
    std::vector<CollisionBox*> collision_box_list;
};

#endif