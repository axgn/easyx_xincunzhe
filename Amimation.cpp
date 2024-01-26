#include "Amimation.h"

Animation::Animation(Atlas* atlas,int interval)
{
	anima_atlas = atlas;
	interval_ms = interval;
}

Animation::~Animation() = default;

void Animation::Play(int x, int y, int delta)
{
	timer += delta;
	if (timer >= interval_ms)
	{
		idx_frame = (idx_frame + 1) % anima_atlas->frame_list.size();
		timer = 0;
	}
	put_image_alpha(x, y, anima_atlas->frame_list[idx_frame]);
}