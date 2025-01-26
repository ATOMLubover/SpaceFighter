#include "Player.h"

#include "ResourceManager.h"
#include "CollisionManager.h"
#include "Game.h"
#include "DropItem.h"
#include "AudioManager.h"

Player::Player()
{
	speed = 400.0f;

	hp = MAX_HP;

	texture = ResourceManager::GetInstance().FindTexture( "player" );
	SDL_QueryTexture( texture, nullptr, nullptr, &width, &height );
	width /= 4;
	height /= 4;

	collision_box = CollisionManager::GetInstance().CreateCollisionBox();
	collision_box->SetLayerSrc( EnumCollisionLayer::PLAYER );
	collision_box->SetLayerDst( EnumCollisionLayer::NONE );
	collision_box->SetSize( { static_cast<double>( 3.0 * width / 5 ), static_cast<double>( height ) } );
	collision_box->SetOnCollideDst(
		[&] ( int damage )
		{
			DecreaseHp( damage );
			SDL_Log( "now player hp: %d", hp );

			collision_box->SetEnabled( false );
			timer_invalnerable.Restart();

			timer_blink.Restart();
			timer_blink.Resume();

			is_shaking = true;
			timer_shake.Restart();
		} );
	collision_box->SetReflectTag( CollisionBox::EnumReflectTag::PLAYER );
	collision_box->SetOnCollideDstGetdrop(
		[&] ( DropItem* drop )
		{
			switch ( drop->GetType() )
			{
				case DropItem::EnumType::HEAL:
					IncreaseHp( 1 );
					SDL_Log( "now player hp: %d", hp );
					break;

				default:
					break;
			}
		} );

	timer_cool.SetOneshot( true );
	timer_cool.SetWaitTime( 0.25f );
	timer_cool.SetOnTimeout(
		[&] ()
		{
			is_cooled_down = true;
		} );

	timer_invalnerable.SetOneshot( true );
	timer_invalnerable.SetWaitTime( 0.5f );
	timer_invalnerable.SetOnTimeout(
		[&] ()
		{
			collision_box->SetEnabled( true );

			is_visible = true;
			timer_blink.Pause();
		} );

	timer_blink.SetOneshot( false );
	timer_blink.SetWaitTime( 3.0f / 60 );
	timer_blink.SetOnTimeout(
		[&] ()
		{
			is_visible = !is_visible;
		} );

	timer_shake.SetOneshot( true );
	timer_shake.SetWaitTime( 0.15f );
	timer_shake.SetOnTimeout(
		[&] ()
		{
			is_shaking = false;
		} );

	Atlas& atlas_explode_player = *ResourceManager::GetInstance().FindAtlas( "atlas_explosion_player" );
	anim_explode.LoadAtlas( atlas_explode_player );
	anim_explode.SetRenderScale( 0.4f );
	anim_explode.SetLoop( false );
	anim_explode.SetInterval( 0.4f );
	anim_explode.SetOnStarted(
		[&] ()
		{
			AudioManager::GetInstance().PlayAudio( "explosion_player" );
		} );
	anim_explode.SetOnFinished(
		[&] ()
		{
			MakeInvalid();
		} );
}

Player::~Player()
{
	CollisionManager::GetInstance().DestroyCollisionBox( collision_box );
}

void Player::OnHandle( SDL_Event& event )
{
	auto keyboard_state = SDL_GetKeyboardState( nullptr );

	is_up_key_down = keyboard_state[ SDL_SCANCODE_W ] ? true : false;
	is_down_key_down = keyboard_state[ SDL_SCANCODE_S ] ? true : false;
	is_left_key_down = keyboard_state[ SDL_SCANCODE_A ] ? true : false;
	is_right_key_down = keyboard_state[ SDL_SCANCODE_D ] ? true : false;

	is_fire_key_down = keyboard_state[ SDL_SCANCODE_SPACE ] ? true : false;
}

void Player::OnUpdate( float delta )
{
	if ( hp > 0 )
	{
		timer_cool.OnUpdate( delta );
		timer_invalnerable.OnUpdate( delta );
		timer_blink.OnUpdate( delta );
		timer_shake.OnUpdate( delta );

		// move logic
		{
			int width_win = Game::GetInstance().GetWidth();
			int height_win = Game::GetInstance().GetHeight();

			PlaneVector vec_dir( 0, 0 );
			if ( is_up_key_down && position.y > 0 )  vec_dir.y -= 1;
			if ( is_down_key_down && position.y + height < height_win ) vec_dir.y += 1;
			if ( is_left_key_down && position.x > 0 ) vec_dir.x -= 1;
			if ( is_right_key_down && position.x + width < width_win ) vec_dir.x += 1;

			PlaneVector velocity = vec_dir.ToNormalized() * speed;
			position += velocity * delta;
		}

		UpdateCollisionBox(); // update position of collisionbox

		// shoot logic
		if ( is_cooled_down && is_fire_key_down )
		{
			is_cooled_down = false;
			is_try_shoot = true;
			timer_cool.Restart(); // start counting
		}

		// shake logic
		if ( is_shaking )
			Game::GetInstance().GetMainCam().Shake( 3.0f, 0.04f );
	}
	else
	{
		anim_explode.SetLogicCenter( GetTexCenter() );
		anim_explode.OnUpdate( delta );
	}
}

void Player::OnRender( const Camera& cam )
{
	if ( hp > 0 )
	{
		if ( is_visible )
		{
			SDL_FRect rect_src = {
				(float) position.x, (float) position.y,
				width, height };
			cam.RenderTexture( texture, nullptr, &rect_src, 0, nullptr );
		}
	}
	else
	{
		anim_explode.OnRender( cam );
	}
}

void Player::Refresh()
{
	is_valid = true;
	collision_box->SetEnabled( true );

	is_cooled_down = true;
	is_try_shoot = false;
	timer_cool.Restart();

	is_visible = true;
	timer_blink.Restart();
	timer_invalnerable.Restart();

	bool is_shaking = false;
	timer_shake.Restart();

	hp = MAX_HP;

	anim_explode.Reset();

	score = 0;
}

bool Player::IsShooting()
{
	if ( is_try_shoot )
		return !( is_try_shoot = false );
	return false;
}

void Player::IncreaseHp( int heal )
{
	hp = std::min( hp + heal, MAX_HP );
}

void Player::DecreaseHp( int damage )
{
	hp -= damage;
}

void Player::UpdateCollisionBox()
{
	collision_box->SetLogicCenter( GetTexCenter() );
}
