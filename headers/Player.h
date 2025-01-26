#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Charactor.h"
#include "PlaneVector.h"
#include "Timer.h"
#include "Animation.h"

#include <SDL.h>

class ResourceManager;

class Player : public Charactor
{
public:
    Player();
    ~Player();

    void OnHandle( SDL_Event& event );
    void OnUpdate( float delta ) override;
    void OnRender( const Camera& cam ) override;

    void Refresh();

    bool IsShooting();

    bool IsExploding() const { return is_valid && ( hp <= 0 ); }

    void IncreaseHp( int heal );
    void DecreaseHp( int damage );
    int GetHp() const { return hp; }
    int GetMaxHp() const { return MAX_HP; }

    void IncrementScore( int delta ) { score += delta; }
    int GetScore() const { return score; }

protected:
    void UpdateCollisionBox() override;

private:
    bool is_up_key_down = false;
    bool is_down_key_down = false;
    bool is_left_key_down = false;
    bool is_right_key_down = false;

    bool is_fire_key_down = false;
    bool is_cooled_down = true;
    Timer timer_cool;
    bool is_try_shoot = false;

    Timer timer_invalnerable; // make invalnerable thru unable collisionbox
    bool is_visible = true;
    Timer timer_blink;

    bool is_shaking = false;
    Timer timer_shake;

    const int MAX_HP = 5;

    Animation anim_explode;

    int score = 0;
};

#endif // !_PLAYER_H_