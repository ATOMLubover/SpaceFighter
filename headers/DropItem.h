#ifndef _DROP_ITEM_H_
#define _DROP_ITEM_H_

#include "Entity.h"

#include <cmath>

class DropItem : public Entity
{
public:
    enum class EnumType
    {
        HEAL,
        SHIELD,
        TIME
    };

public:
    DropItem( EnumType type );
    ~DropItem() = default;

    void OnUpdate( float delta );
    void OnRender( const Camera& cam ) override;

    void SetDirection( double radians );

    EnumType GetType() const { return type; }

protected:
    void UpdateCollisionBox() override;

    bool CheckReachEdge() const;

private:
    void Rebound( const PlaneVector& pos_ori );

private:
    EnumType type = EnumType::HEAL;

    float speed;
    PlaneVector velocity;

    int total_rebound_time;
    int times_rebounded = 0;
};

#endif