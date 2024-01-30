#include "StartGameButton.h"
void StartGameButton::OnClick()
{
	mciSendString(_T("play bgm  repeat from 0"), NULL, 0, NULL);

	is_start_game = true;
}

void QuitGameButton::OnClick()
{
	running = false;
}