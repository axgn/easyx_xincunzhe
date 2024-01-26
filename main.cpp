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

#pragma comment(lib,"MSIMG32.LIB")
#pragma comment(lib,"WINMM.LIB")

Atlas* atlas_player_left;
Atlas* atlas_player_right;
Atlas* atlas_enemy_left;
Atlas* atlas_enemy_right;

void put_image_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

void TryGenerateEnemy(std::vector<Enemy*>& enemy_list)
{
	const int INTERVAL = 100;
	static int counter = 0;
	if ((++counter) % INTERVAL == 0)
		enemy_list.push_back(new Enemy(atlas_enemy_left,atlas_enemy_right));
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

	mciSendString(_T("play bgm  repeat from 0"), NULL, 0, NULL);

	bool running = true;

	atlas_player_left = new Atlas(_T("img/player_left_%d.png"),6);
	atlas_player_right = new Atlas(_T("img/player_right_%d.png"), 6);
	atlas_enemy_left = new Atlas(_T("img/enemy_left_%d.png"), 6);
	atlas_enemy_right = new Atlas(_T("img/enemy_right_%d.png"), 6);
	int score = 0;
	Player player(atlas_player_left, atlas_player_right);
	ExMessage msg;
	IMAGE img_background;
	std::vector<Enemy*> enemy_list;
	std::vector<Bullet> bullet_list(3);
	std::ifstream ifile("score.txt", std::ios::in);
	int max_score = 0;
	ifile >> max_score;
	ifile.close();

	loadimage(&img_background, _T("img/background.png"));

	BeginBatchDraw();

	while (running)
	{
		DWORD start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			player.Process_event(msg);
		}

		player.Move();

		TryGenerateEnemy(enemy_list);
		for (Enemy*& enemy : enemy_list)
			enemy->Move(player);

		for (Enemy*& enemy : enemy_list)
		{
			if (enemy->CheckPlayerCollision(player))
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

		UpdateBullets(bullet_list, player);

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

		cleardevice();

		putimage(0, 0, &img_background);
		player.Draw(1000 / 60);
		for (Enemy*& enemy : enemy_list)
			enemy->Draw(1000 / 60);
		for (Bullet& bullet : bullet_list)
			bullet.Draw();
		DrawPlayerScore(score,max_score);

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

	EndBatchDraw();
	return 0;
}