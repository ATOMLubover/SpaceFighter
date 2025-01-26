#ifndef _COLLISION_BOX_H_
#define _COLLISION_BOX_H_

#include "EnumCollisionLayer.h"
#include "PlaneVector.h"

#include <functional>
#include <variant>

class CollisionManager;
class Bullet;
class DropItem;

class CollisionBox
{
    friend class CollisionManager;

public:
    enum class EnumReflectTag
    {
        NONE,
        PLAYER,
        DROP,
        BULLET
    };

public:
    CollisionBox( const CollisionBox& rhs ) = delete;
    CollisionBox( CollisionBox&& rhs ) = delete;
    CollisionBox& operator=( const CollisionBox& rhs ) = delete;

    void SetLayerSrc( EnumCollisionLayer layer ) { layer_src = layer; }
    void SetLayerDst( EnumCollisionLayer layer ) { layer_dst = layer; }

    void SetOnCollideSrc( const std::function<void()>& src ) { on_collide_src = src; }
    void SetOnCollideDst( const std::function<void( int )>& dst ) { on_collide_dst = dst; }

   void SetEnabled( bool flag ) { is_enabled = flag; }

    void SetSize( const PlaneVector& size ) { this->size = size; }
    const PlaneVector& GetSize() const { return size; }

    void SetLogicCenter( const PlaneVector& center );

    void SetReflectTag( EnumReflectTag tag ) { this->tag = tag; }

    void SetBulletOwner( Bullet* owner );
    Bullet* GetBulletOwner() const;

    void SetDropOwner( DropItem* owner );
    DropItem* GetDropOwner() const;

    void SetOnCollideDstGetdrop( const std::function<void( DropItem* )>& dst_getdrop );

private:
    CollisionBox() = default;
    ~CollisionBox();

private:
    PlaneVector size;
    PlaneVector position;

    bool is_enabled = true;
    std::function<void()> on_collide_src;
    std::function<void( int )> on_collide_dst;

    EnumCollisionLayer layer_src = EnumCollisionLayer::NONE;
    EnumCollisionLayer layer_dst = EnumCollisionLayer::NONE;

    EnumReflectTag tag = EnumReflectTag::NONE;
    union
    {
        Bullet* owner_bullet;
        DropItem* owner_drop;
        std::function<void( DropItem* )>* on_collide_dst_getdrop;
    };
};

#endif