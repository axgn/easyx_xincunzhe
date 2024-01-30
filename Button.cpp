#include "Button.h"

void Button::ProcessEvent(const ExMessage& msg)
{
	switch (msg.message)
	{
	case WM_MOUSEMOVE:
		if (status == Status::Idle && CheckCursorHit(msg.x, msg.y))
			status = Status::Hovered;
		else if (status == Status::Hovered && !CheckCursorHit(msg.x, msg.y))
			status = Status::Idle;
		break;
	case WM_LBUTTONDOWN:
		if (CheckCursorHit(msg.x, msg.y))
			status = Status::Pushed;
		break;
	case WM_LBUTTONUP:
		if (status == Status::Pushed)
			OnClick();
		break;
	}
}

void Button::Draw()
{
	switch (status)
	{
	case Button::Status::Idle:
		putimage(region.left, region.top, &img_idle);
		break;
	case Button::Status::Hovered:
		putimage(region.left, region.top, &img_hovered);
		break;
	case Button::Status::Pushed:
		putimage(region.left, region.top, &img_pushed);
		break;
	}
}