#include "Entity.h"

void Entity::MakeInvalid()
{
    is_valid = false;
    collision_box->SetEnabled( false );
}

PlaneVector Entity::GetTexCenter() const
{
    return PlaneVector( position.x + width / 2.0, position.y + height / 2.0 );
}

void Entity::SetLogicCenter( const PlaneVector& l_center )
{
    position = { l_center.x - width / 2.0, l_center.y - height / 2.0 };
}
