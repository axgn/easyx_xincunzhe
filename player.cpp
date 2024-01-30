#include "player.h"

void Player::Draw(const int delta)
{
	int shadow_pos_x = position.x + (FRAME_WIDTH / 2) - (SHADOW_WIDTH / 2);
	int shadow_pos_y = position.y + FRAME_HEIGHT - 8;
	put_image_alpha(shadow_pos_x, shadow_pos_y, &img_shadow);

	static bool facing_left = false;
	if (is_move_right - is_move_left < 0)
		facing_left = true;
	else if (is_move_right - is_move_left > 0)
		facing_left = false;
	if (facing_left)
		anima_left->Play(position.x, position.y, delta);
	else
		anima_right->Play(position.x, position.y, delta);
}

void Player::Move()
{
	int dir_x = is_move_right - is_move_left;
	int dir_y = is_move_down - is_move_up;
	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
	if (len_dir != 0)
	{
		double normalized_x = dir_x / len_dir;
		double normalized_y = dir_y / len_dir;
		position.x += (int)(SPEED * normalized_x);
		position.y += (int)(SPEED * normalized_y);
	}

	if (position.x < 0) position.x = 0;
	if (position.y < 0) position.y = 0;
	if (position.x + FRAME_WIDTH > WINDOW_WIDTH)
		position.x = WINDOW_WIDTH - FRAME_WIDTH;
	if (position.y + FRAME_HEIGHT > WINDOW_HEIGHT)
		position.y = WINDOW_HEIGHT - FRAME_HEIGHT;
}

void Player::Process_event(const ExMessage& msg)
{
	if (msg.message == WM_KEYDOWN)
	{
		switch (msg.vkcode)
		{
		case VK_UP:
			is_move_up = true; break;
		case VK_DOWN:
			is_move_down = true; break;
		case VK_LEFT:
			is_move_left = true; break;
		case VK_RIGHT:
			is_move_right = true; break;
		default:
			break;
		}
	}
	else if (msg.message == WM_KEYUP)
	{
		switch (msg.vkcode)
		{
		case VK_UP:
			is_move_up = false; break;
		case VK_DOWN:
			is_move_down = false; break;
		case VK_LEFT:
			is_move_left = false; break;
		case VK_RIGHT:
			is_move_right = false; break;
		default:
			break;
		}
	}
}

const POINT& Player::GetPosition() const
{
	return position;
}

const int Player::Get_width() const
{
	return  FRAME_WIDTH;
}

const int Player::Get_height() const
{
	return FRAME_HEIGHT;
}