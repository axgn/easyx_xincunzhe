#pragma once
#include "Button.h"
class StartGameButton : public Button
{
public:
	StartGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed,bool& is_game_start)
		:Button(rect, path_img_idle, path_img_hovered, path_img_pushed),is_start_game(is_game_start) {}
	~StartGameButton() = default;

private:
	void OnClick();

private:
	bool& is_start_game;
};

class QuitGameButton : public Button
{
public:
	QuitGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed,bool& running)
		:Button(rect, path_img_idle, path_img_hovered, path_img_pushed) ,running(running){}
	~QuitGameButton() = default;

private:
	void OnClick();

private:
	bool& running;
};