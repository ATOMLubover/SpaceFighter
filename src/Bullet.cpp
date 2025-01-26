#include "Bullet.h"

void Bullet::MakeInvalid()
{
    is_valid = false;
    collision_box->SetEnabled( false );
}
