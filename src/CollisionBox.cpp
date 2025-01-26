#include "CollisionBox.h"

void CollisionBox::SetLogicCenter( const PlaneVector& center )
{
    position = PlaneVector( center.x - size.x / 2.0, center.y - size.y / 2.0 );
}

void CollisionBox::SetBulletOwner( Bullet* owner )
{
    if ( tag != EnumReflectTag::BULLET ) return;

    owner_bullet = owner;
}

Bullet* CollisionBox::GetBulletOwner() const
{
    if ( tag != EnumReflectTag::BULLET ) return nullptr;

    return owner_bullet;
}

void CollisionBox::SetDropOwner( DropItem* owner )
{
    if ( tag != EnumReflectTag::DROP ) return;

    owner_drop = owner;
}

DropItem* CollisionBox::GetDropOwner() const
{
    if ( tag != EnumReflectTag::DROP ) return nullptr;

    return owner_drop;
}

void CollisionBox::SetOnCollideDstGetdrop( const std::function<void( DropItem* )>& dst_getdrop )
{
    if ( tag != EnumReflectTag::PLAYER ) return;

    on_collide_dst_getdrop = new std::function<void( DropItem* )>( dst_getdrop );
}

CollisionBox::~CollisionBox()
{
    if ( tag == EnumReflectTag::PLAYER )
        delete on_collide_dst_getdrop;
}

