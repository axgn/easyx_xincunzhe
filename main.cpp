#include<graphics.h>
#include<string>
#include "Amimation.h"
#include "player.h"
#include "Enemy.h"
#include<numbers>
#include<cmath>
#include "mmsystem.h"
#include<fstream>
#include "Atlas.h"
#include "const_val.h"
#include "StartGameButton.h"
#include "eff.h"

bool running = true;
bool is_game_started = false;

#pragma comment(lib,"MSIMG32.LIB")
#pragma comment(lib,"WINMM.LIB")

Atlas* atlas_player_left;
Atlas* atlas_player_right;
Atlas* atlas_enemy_left;
Atlas* atlas_enemy_right;
Atlas* atlas_white_player;

void put_image_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

void TryGenerateEnemy(std::vector<Enemy*>& enemy_list,int score)
{
	const int INTERVAL = 100;
	static int counter = 0;
	if ((++counter) % INTERVAL == 0)
	{
		enemy_list.push_back(new Enemy(atlas_enemy_left, atlas_enemy_right));
		enemy_list.back()->increase_speed(score);
	}
}

void UpdateBullets(std::vector<Bullet>& bullet_list, const Player& player)
{
	const POINT& player_pos = player.GetPosition();
	const double PADIAL_SPEED = 0.0045;
	const double TANGENT_SPEED = 0.0055;
	double radian_interval = 2 * std::numbers::pi / bullet_list.size();
	double radius = 100 + 25 * sin(GetTickCount() * PADIAL_SPEED);
	for (size_t i = 0; i < bullet_list.size(); i++)
	{
		double radian = GetTickCount() * TANGENT_SPEED + radian_interval * i;
		bullet_list[i].position.x = player_pos.x + player.Get_width() / 2 + (int)(radius * sin(radian));
		bullet_list[i].position.y = player_pos.y + player.Get_height() / 2 + (int)(radius * cos(radian));
	}
}

void DrawPlayerScore(int score,int max_score)
{
	static TCHAR text[256];
	_stprintf_s(text, _T("当前玩家得分：%d      最高得分为：%d"), score,max_score);

	setbkmode(TRANSPARENT);
	settextcolor(RGB(255, 85, 185));
	outtextxy(10, 10, text);
}

int main()
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);

	mciSendString(_T("open mus/bgm.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("open mus/hit.wav alias hit"), NULL, 0, NULL);

	atlas_player_left = new Atlas(_T("img/player_left_%d.png"),6);
	atlas_player_right = new Atlas(_T("img/player_right_%d.png"), 6);
	atlas_enemy_left = new Atlas(_T("img/enemy_left_%d.png"), 6);
	atlas_enemy_right = new Atlas(_T("img/enemy_right_%d.png"), 6);
	int score = 0;
	Player player(atlas_player_left, atlas_player_right);
	ExMessage msg;
	IMAGE img_background;
	IMAGE img_menu;
	std::vector<Enemy*> enemy_list;
	std::vector<Bullet> bullet_list(3);
	std::vector<IMAGE*> white_player;
	for (int i = 0; i < 6; i++)
	{
		IMAGE* temp = new IMAGE(atlas_player_left->frame_list[i]->getwidth(), atlas_player_left->frame_list[i]->getheight());
		turn_white(atlas_player_left->frame_list[i],temp);
		white_player.push_back(temp);
	}
	atlas_white_player = new Atlas(white_player);
	Player player_w(atlas_white_player,atlas_player_right);
	
	RECT region_btn_start_game, region_btn_quit_game;

	region_btn_start_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_start_game.right = region_btn_start_game.left + BUTTON_WIDTH;
	region_btn_start_game.top = 430;
	region_btn_start_game.bottom = region_btn_start_game.top + BUTTON_HEIGHT;
	
	region_btn_quit_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_quit_game.right = region_btn_quit_game.left + BUTTON_WIDTH;
	region_btn_quit_game.top = 550;
	region_btn_quit_game.bottom = region_btn_quit_game.top + BUTTON_HEIGHT;

	StartGameButton btn_start_game(region_btn_start_game,
		_T("img/ui_start_idle.png"), _T("img/ui_start_hovered.png"), _T("img/ui_start_pushed.png"),is_game_started);
	QuitGameButton btn_quit_game(region_btn_quit_game,
		_T("img/ui_quit_idle.png"), _T("img/ui_quit_hovered.png"), _T("img/ui_quit_pushed.png"), running);


	std::ifstream ifile("score.txt", std::ios::in);
	int max_score = 0;
	ifile >> max_score;
	ifile.close();

	loadimage(&img_background, _T("img/background.png"));
	loadimage(&img_menu, _T("img/menu.png"));

	IMAGE img_background2;
	overturn(GetImageBuffer(&img_background), img_background.getwidth(), img_background.getheight(), &img_background2);

	BeginBatchDraw();

	while (running)
	{
		DWORD start_time = GetTickCount();

		while (peekmessage(&msg,EX_MOUSE | EX_KEY))
		{
			if(is_game_started)
				player_w.Process_event(msg);
			else
			{
				btn_quit_game.ProcessEvent(msg);
				btn_start_game.ProcessEvent(msg);
			}
		}
		if (is_game_started)
		{
			player_w.Move();

			TryGenerateEnemy(enemy_list,score);
			for (Enemy*& enemy : enemy_list)
				enemy->Move(player_w);

			for (Enemy*& enemy : enemy_list)
			{
				if (enemy->CheckPlayerCollision(player_w))
				{
					static TCHAR text[128];
					_stprintf_s(text, _T("最终得分为：%d !"), score);
					MessageBox(GetHWnd(), text, _T("游戏结束"), MB_OK);
					std::ofstream ofile;
					ofile.open("score.txt", std::ios::out);
					if (max_score < score)
					{
						ofile << score;
					}
					ofile.close();
					running = false;
					break;
				}
			}

			UpdateBullets(bullet_list, player_w);

			for (Enemy*& enemy : enemy_list)
			{
				for (Bullet& bullet : bullet_list)
				{
					if (enemy->CheckBulletCollision(bullet))
					{
						enemy->Hurt();
						mciSendString(_T("play hit  from 0"), NULL, 0, NULL);
						score++;
					}
				}
			}
			for (size_t i = 0; i < enemy_list.size(); i++)
			{
				Enemy* enemy = enemy_list[i];
				if (!enemy->CheckAlive())
				{
					std::swap(enemy_list[i], enemy_list.back());
					enemy_list.pop_back();
					i--;
					delete enemy;
				}
			}
		}
		
		cleardevice();

		if (is_game_started)
		{
			putimage(0, 0, &img_background);
			player_w.Draw(1000 / 60);
			for (Enemy*& enemy : enemy_list)
				enemy->Draw(1000 / 60);
			for (Bullet& bullet : bullet_list)
				bullet.Draw();
			DrawPlayerScore(score, max_score);
		}
		else
		{
			putimage(0, 0, &img_menu);
			btn_start_game.Draw();
			btn_quit_game.Draw();
		}
		FlushBatchDraw();

		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;

		if (delta_time < 1000 / 60)
		{
			Sleep(1000 / 60 - delta_time);
		}
	}

	delete atlas_player_left;
	delete atlas_player_right;
	delete atlas_enemy_left;
	delete atlas_enemy_right;
	delete atlas_white_player;

	EndBatchDraw();
	return 0;
}