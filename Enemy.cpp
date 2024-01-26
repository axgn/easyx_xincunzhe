#include "Enemy.h"
bool Enemy::CheckBulletCollision(const Bullet& bullet)
{
	bool is_overlap_x = bullet.position.x >= position.x && bullet.position.x <= position.x + FRAME_WIDTH;
	bool is_overlap_y = bullet.position.y >= position.y && bullet.position.y <= position.y + FRAME_HEIGHT;
	return is_overlap_x && is_overlap_y;
}

bool Enemy::CheckPlayerCollision(const Player& player)
{
	POINT check_position;
	check_position.x = position.x + FRAME_WIDTH / 2;
	check_position.y = position.y + FRAME_HEIGHT / 2;
	POINT player_pos = player.GetPosition();
	bool is_overlap_x = check_position.x >= player_pos.x && check_position.x <= player_pos.x + player.Get_width();
	bool is_overlap_y = check_position.y >= player_pos.y && check_position.y <= player_pos.y + player.Get_height();
	return is_overlap_x && is_overlap_y;
}

void Enemy::Move(const Player& player)
{
	const POINT& player_position = player.GetPosition();
	int dir_x = player_position.x - position.x;
	int dir_y = player_position.y - position.y;
	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
	if (len_dir != 0)
	{
		double normalized_x = dir_x / len_dir;
		double normalized_y = dir_y / len_dir;
		position.x += (int)(SPEED * normalized_x);
		position.y += (int)(SPEED * normalized_y);
	}
	if (dir_x > 0)
		facing_left = false;
	else if (dir_x < 0)
		facing_left = true;
}

void Enemy::Draw(int delta)
{
	int shadow_pos_x = position.x + (FRAME_WIDTH / 2) - (SHADOW_WIDTH / 2);
	int shadow_pos_y = position.y + FRAME_HEIGHT - 35;
	put_image_alpha(shadow_pos_x, shadow_pos_y, &img_shadow);

	if (facing_left)
		anima_left->Play(position.x, position.y, delta);
	else
		anima_right->Play(position.x, position.y, delta);
}

void Enemy::Hurt()
{
	alive = false;
}

bool Enemy::CheckAlive()
{
	return alive;
}