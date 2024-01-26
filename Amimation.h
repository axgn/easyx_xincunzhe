#pragma once
#include<graphics.h>
#include<vector>
#include "Atlas.h"

void put_image_alpha(int x, int y, IMAGE* img);

class Animation
{
public:
	Animation(Atlas* atlas ,int interval);
	~Animation();
	void Play(int x, int y, int delta);
private:
	int interval_ms = 0;
	int timer = 0; //动画计数器
	int idx_frame = 0; // 动画帧索引

private:
	Atlas* anima_atlas;
};

