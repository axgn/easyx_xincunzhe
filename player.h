#pragma once
#include "Amimation.h"
#include "const_val.h"

class Player
{
public:
	Player(Atlas* atlas_left, Atlas* atlas_right)
	{
		anima_left = new Animation(atlas_left, 45);
		anima_right = new Animation(atlas_right, 45);
		loadimage(&img_shadow, _T("img/shadow_player.png"));
	}
	~Player()
	{
		delete anima_left;
		delete anima_right;
	}
	void Process_event(const ExMessage& msg);

	void Move();

	const POINT& GetPosition() const;

	void Draw(const int delta);

	const int Get_width() const;
	
	const int Get_height() const;

private:
	const int SPEED = 3;
	const int SHADOW_WIDTH = 32;
	const int FRAME_HEIGHT = 80;
	const int FRAME_WIDTH = 80;

private:
	IMAGE img_shadow;
	POINT position = { 500, 500 };
	Animation* anima_left;
	Animation* anima_right;
	bool is_move_up = false;
	bool is_move_down = false;
	bool is_move_left = false;
	bool is_move_right = false;
};

