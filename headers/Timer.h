#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

class Timer
{
public:
    explicit Timer() = default;
    ~Timer() = default;

    void OnUpdate( float delta );

    void Restart();

    void Pause() { is_paused = true; }
    void Resume() { is_paused = false; }

    void SetWaitTime( float second ) { wait_time = second; }
    void SetOneshot( bool flag ) { oneshot_mode = flag; }
    void SetOnTimeout( std::function<void()> callback ) { on_timeout = callback; }

    bool IsPaused() const { return is_paused; }
    bool IsShotted() const { return is_shotted; }

    bool IsReady() const { return !is_shotted && passed_time == 0.0f && !is_paused; }

    float GetPassedTime() const { return passed_time; }

private:
    float passed_time = 0;
    float wait_time = 0;

    bool is_paused = false;
    bool is_shotted = false;
    bool oneshot_mode = false;

    std::function<void()> on_timeout;
};

#endif