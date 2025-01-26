#include "Timer.h"

void Timer::OnUpdate( float delta )
{
    if ( is_paused ) return;

    passed_time += delta;
    if ( passed_time >= wait_time )
    {
        bool can_shot = ( !oneshot_mode || ( oneshot_mode && !is_shotted ) );
        is_shotted = true;
        if ( can_shot && on_timeout ) on_timeout();

        passed_time -= wait_time;
    }
}

void Timer::Restart()
{
    passed_time = 0;
    is_shotted = false;
}