#pragma once
#include "Amimation.h"
#include "Bullet.h"
#include "player.h"
#include<ctime>
#include<cstdlib>
#include "const_val.h"

class Enemy
{
public:
	Enemy(Atlas* atlas_left,Atlas* atlas_right)
	{
		anima_left = new Animation(atlas_left, 45);
		anima_right = new Animation(atlas_right, 45);
		loadimage(&img_shadow, _T("img/shadow_enemy.png"));
		enum class SpawnEdge
		{
			Up = 0,
			Down,
			Left,
			Right
		};

		srand(time(NULL));
		SpawnEdge edge = (SpawnEdge)(rand() % 4);
		switch (edge)
		{
		case SpawnEdge::Up:
			position.x = rand() % WINDOW_WIDTH;
			position.y = -FRAME_HEIGHT;
			break;
		case SpawnEdge::Down:
			position.x = rand() % WINDOW_WIDTH;
			position.y = WINDOW_HEIGHT;
			break;
		case SpawnEdge::Left:
			position.x = -FRAME_WIDTH;
			position.y = rand() % WINDOW_HEIGHT;
			break;
		case SpawnEdge::Right:
			position.x = WINDOW_WIDTH;
			position.y = rand() % WINDOW_HEIGHT;
			break;
		default:
			break;
		}
	}
	~Enemy()
	{
		delete anima_left;
		delete anima_right;
	}

	bool CheckBulletCollision(const Bullet& bullet);

	bool CheckPlayerCollision(const Player& player);

	void Move(const Player& player);

	void Hurt();

	bool CheckAlive();

	void Draw(int delta);
	
	void increase_speed(int score);

private:
	int SPEED = 4;
	const int SHADOW_WIDTH = 48;
	const int FRAME_HEIGHT = 80;
	const int FRAME_WIDTH = 80;

private:
	IMAGE img_shadow;
	POINT position = { 0, 0 };
	Animation* anima_left;
	Animation* anima_right;
	bool facing_left = false;
	bool alive = true;
};

