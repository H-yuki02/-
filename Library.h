#pragma once
#include "DxLib.h"
#include "Functions.h"


//Library from here
//Functions for rhythm games
static int rithm_lib(int music, double BPM) {
	int beat;

	//Play music if music is stopped. It's just a format, so I'll change the way I play music later
	if (CheckSoundMem(music)) {

	}
	else {
		ChangeVolumeSoundMem(255 * 50 / 100, music);
		PlaySoundMem(music, DX_PLAYTYPE_BACK);
	}

	//Outputting the music playback time as it is is too large, so it is a part to reduce it a little
	beat = (int)(GetSoundCurrentTime(music) * BPM * 64 / 60000);
	return beat;
}

//The part of the function below that actually stops and plays the music
static void music_onoff_lib(int music, int beat, int stop_now, double BPM) {
	int elapsed_time = (int)(beat / BPM / 64 * 60000);
	if (stop_now == 1) {
		StopSoundMem(music);
	}
	else {
		SetSoundCurrentTime(elapsed_time, music);
		PlaySoundMem(music, DX_PLAYTYPE_BACK, 0);
	}
}

//Function for switching between stopping and playing music
static bool stop_lib(int music, int beat, double BPM) {
	static bool stop_now = 0;
	static bool ttt = 1;
	/*if (CheckHitKey(KEY_INPUT_T)) {
		if (ttt) {
			onoff(&stop_now);
			music_onoff_lib(music, beat, stop_now, BPM);
			ttt = 0;
		}
	}
	else {
		ttt = 1;
	}
*/
	return stop_now;
}

//Functions for manipulating the elapsed time of music while music is stopped
static int movetime_lib() {
	int x = 0;
	if (CheckHitKey(KEY_INPUT_UP)) {
		++x;
	}
	if (CheckHitKey(KEY_INPUT_DOWN)) {
		--x;
	}
	if (CheckHitKey(KEY_INPUT_LSHIFT)) {
		x *= 10;
	}
	return x;
}

//Functions to manage the drawing of monotonous or immobile objects and backgrounds
static void image_back_ground_lib(bool stop_now, int beat) {
	int i, j;
	static int hondana = LoadGraph("img/hondana.jpg");
	struct position {
		int x;
		int y;
	};
	static position hondanaposi = {
		0,
		60
	};
	static position hondanaposi2 = {
		320,
		60
	};
	static position hondanaposi3 = {
		640,
		60
	};


	//This is a square that keeps flowing to the left after a certain period of time. This part is the bookshelf
	DrawGraph(hondanaposi.x, hondanaposi.y, hondana, TRUE);
	DrawGraph(hondanaposi2.x, hondanaposi2.y, hondana, TRUE);
	DrawGraph(hondanaposi3.x, hondanaposi3.y, hondana, TRUE);
	if (beat >= 17) {
		if (!stop_now) {
			hondanaposi.x -= 4;
			hondanaposi2.x -= 4;
			hondanaposi3.x -= 4;
		}
	}
	else {
		hondanaposi.x = 0;
		hondanaposi.y = 60;
		hondanaposi2.x = 320;
		hondanaposi2.y = 60;
		hondanaposi3.x = 640;
		hondanaposi3.y = 60;

	}
	if (hondanaposi.x <= -320) {
		hondanaposi.x = 640;
	}
	if (hondanaposi2.x <= -320) {
		hondanaposi2.x = 640;
	}
	if (hondanaposi3.x <= -320) {
		hondanaposi3.x = 640;
	}
}

//A function to manage the drawing of the character operated by the player
static void image_hero_lib(bool stop_now, int beat, bool hantei) {
	static int player1 = LoadGraph("img/zikuride1.png");
	static int player2 = LoadGraph("img/zikuride2.png");
	static int player3 = LoadGraph("img/zikuride3.png");
	struct position {
		int x;
		int y;
	};
	position hero = {
		250 + 50,
		240
	};

	if (hantei) {
		//Blinks if you fail to kill a pile of books or magic bullets
		static bool n = 0;
		if (n == 0) {
			DrawGraph(hero.x, hero.y, player1, TRUE);
		}
		onoff(&n);
	}
	else {
		//Draw a blue square box (Brave)
	}
	//Put out a blue square box (sword) in the direction in which the direction key was entered
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		DrawGraph(hero.x, hero.y, player2, TRUE);
	}
	else if (CheckHitKey(KEY_INPUT_LEFT)) {
		DrawGraph(hero.x, hero.y, player3, TRUE);
	}
	else {
		DrawGraph(hero.x, hero.y, player1, TRUE);
	}

}
//Functions for managing drawing of moving objects
static void gimmick_image_lib(bool stop_now, int beat, bool hantei, bool book_kill, bool magic_kill, bool hentai) {
	int i, j;
	static bool start = 1;
	static int uraGraph = LoadGraph("img/ura.png");
	static int urasound = LoadSoundMem("se/magic.wav");

	struct position {
		int x;
		int y;
	};
	//Green square (oura) and red circle (magic bullet)
	{
		static int magic_beat = beat;
		static position ura = {
			-150,
			0
		};
		static position magic = {
			-100,
			100
		};
		static int r = 0;
		DrawGraph(ura.x, ura.y, uraGraph, 1);
		DrawCircle(magic.x, magic.y, 25, YELLOW, 1, 1);
		if (hentai && magic_beat != beat) {
			magic_beat = beat;
			magic.x = 80 + 65;
			magic.y = 80 + 15;
			PlaySoundMem(urasound, DX_PLAYTYPE_BACK, TRUE);
		}
		if (beat >= 110) {
			if (ura.x < 0) {
				ura.x += 2;
				magic.x = ura.x - 100;
				magic.y = ura.y - 100;
				r = 0;
			}
			else {
				r = 25;
				//if (magic.x < 945) {
				magic.x += 5;
				magic.y += 7;
			}
		}
		else {
			ura.x = -150;
		}
		if (magic_kill) {
			magic.x = 1000;
			magic.y = 1000;
		}
	}
	//Brown square (a pile of books)
	{
		static position books[256];
		static int num = 0;
		static int books_beat = beat;
		static int book = LoadGraph("img/book.png");
		if (start) {
			for (i = 0; i < 256; ++i) {
				books[i].x = -100;
				books[i].y = -100;
			}
			start = 0;
		}
		if (hantei && books_beat != beat) {
			books_beat = beat;
			books[num].x = 100 + 25 + 600;
			books[num].y = 350;

			++num;
			if (num > 200) {
				num = 0;
			}
		}
		if (book_kill) {
			int s = 2;
			books[num - s].x = -100;
			books[num - s].y = -100;
		}

		for (i = 0; i < 256; ++i) {
			DrawGraph(books[i].x, books[i].y, book, 1);
			if (!stop_now) {
				books[i].x -= 7.01;
			}
		}
	}
}

//Music score management
static int humen_lib(char name[], int beat, bool stop_now) {
	static bool hantei[6][4096][2];
	static bool start = 1;
	int tokuten = 0;
	static bool rensya_return = 1;
	static bool rensya_right = 1;
	static bool rensya_left = 1;
	static bool rensya_up = 1;
	static bool rensya_down = 1;
	static bool rensya[6] = { 1,1,1,1,1,1 };
	bool book_kill = 0;
	bool magic_kill = 0;
	static int killsound = LoadSoundMem("se/kill.wav");


	int i, j;
	//Loading score data
	if (start) {

		FILE* fo;
		fopen_s(&fo, name, "rb");
		if (fo != NULL) {
			for (i = 0; i < 6; ++i) {
				for (j = 0; j < 4096; ++j) {
					fread(&hantei[i][j][1], sizeof(hantei[i][j][1]), 1, fo);
				}
			}
			fclose(fo);
		}
		else {
			for (i = 0; i < 6; ++i) {
				for (j = 0; j < 4096; ++j) {
					hantei[i][j][1] = 0;
				}
			}
		}
		for (i = 0; i < 6; ++i) {
			for (j = 0; j < 4096; ++j) {
				hantei[i][j][0] = hantei[i][j][1];
			}
		}
		start = 0;
	}

	//Save score
	if (CheckHitKey(KEY_INPUT_S)) {
		FILE* fs;
		fopen_s(&fs, name, "wb");
		for (i = 0; i < 6; ++i) {
			for (j = 0; j < 4096; ++j) {
				fwrite(&hantei[i][j][1], sizeof(hantei[i][j][1]), 1, fs);
			}
		}
		fclose(fs);
	}

	//Return the score to the saved one
	if (CheckHitKey(KEY_INPUT_R)) {
		for (i = 0; i < 6; ++i) {
			for (j = 0; j < 4096; ++j) {
				hantei[i][j][0] = hantei[i][j][1];
			}
		}
	}
	//Judgment
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		DrawBox(235, 160, 255, 180, GREEN, 1);

		if (rensya_right) {
			for (i = -2; i <= 2; ++i) {
				if (hantei[2][beat + 16 + i][0]) {
					tokuten = 100 - 10 * abs(i);				//Score addition (deviation from just * 10 minus)
					hantei[2][beat + 16 + i][0] = 0;
					book_kill = 1;
					//ChangeVolumeSoundMem(255 * VOL / 100, killsound);
					PlaySoundMem(killsound, DX_PLAYTYPE_BACK, TRUE);
				}
			}
			for (i = -6; i <= 6; ++i) {
				if (hantei[2][beat + 16 + i][0]) {
					hantei[2][beat + 16 + i][0] = 0;
				}
			}
			rensya_right = 0;
		}
	}
	else {
		rensya_right = 1;
	}

	if (CheckHitKey(KEY_INPUT_LEFT)) {
		DrawBox(235, 190, 255, 210, GREEN, 1);

		if (rensya_left) {
			for (i = -2; i <= 2; ++i) {
				if (hantei[3][beat + 16 + i][0]) {
					tokuten = 100 - 10 * abs(i);				//Score addition (deviation from just * 10 minus)
					hantei[3][beat + 16 + i][0] = 0;
					magic_kill = 1;
					PlaySoundMem(killsound, DX_PLAYTYPE_BACK, TRUE);
				}
			}
			for (i = -6; i <= 6; ++i) {
				if (hantei[3][beat + 16 + i][0]) {
					hantei[3][beat + 16 + i][0] = 0;
				}
			}
			rensya_left = 0;
		}
	}
	else {
		rensya_left = 1;
	}

	//Judgment creation
	{
		if (CheckHitKey(KEY_INPUT_3)) {
			if (rensya[2]) {
				onoff(&hantei[2][beat + 16][1]);
				hantei[2][beat + 16][0] = hantei[2][beat + 16][1];
				rensya[2] = 0;
			}
		}
		else {
			rensya[2] = 1;
		}

		if (CheckHitKey(KEY_INPUT_4)) {
			if (rensya[3]) {
				onoff(&hantei[3][beat + 16][1]);
				hantei[3][beat + 16][0] = hantei[3][beat + 16][1];
				rensya[3] = 0;
			}
		}
		else {
			rensya[3] = 1;
		}

	}

	//drawing
	image_back_ground_lib(stop_now, beat / 16);
	gimmick_image_lib(stop_now, beat / 16, hantei[2][beat + 40][0], book_kill, magic_kill, hantei[3][beat + 40][0]);
	j = 0;
	for (i = 0; i < 3; ++i) {
		if (hantei[2][beat + 13 - i][0]) {
			j = 1;
		}
	}
	image_hero_lib(stop_now, beat / 16, j);

	return tokuten;
}

static void ending() {
	SetFontSize(50);
	DrawString(15, 240, " THANK YOU for PLAYING!!", WHITE);
	SetFontSize(30);
	DrawString(15, 300, " PUSH SPACE", WHITE);
}

void lib_execute() {

	int rithm_lib(int music, double BPM);
	void image_hero_lib(bool stop_now, int beat);
	void gimmick_image_lib(bool stop_now, int beat, bool hantei, bool book_kill);
	int humen_lib(char name[], int beat, bool stop_now);
	void music_onoff_lib(int, int, int, double);
	bool stop_lib(int music, int elapsed_time, double BPM);
	int movetime_lib();

	//BGM data
	struct BGM {
		char dat[64];//The storage location of musical score data. Enter .dat as the extension at the end of the string as in the "test" example just below
		int music;   //BGM file
		double BPM;  //BGM BPM
	};
	BGM test = {
		"hu/library.dat",
		LoadSoundMem("music/library.mp3"),
		128.0
	};
	int score = 0;
	int elapsed_time = 0; //elapsed time
	int beat = 0;         //Number of beats
	bool stop_now = 0;    //Whether the rhythm game is stopped

	PlaySoundMem(test.music, DX_PLAYTYPE_BACK);

	while (ProcessMessage() == 0)
	{

		if (stop_lib(test.music, elapsed_time, test.BPM) == 0) {
			//If the rhythm game is not stopped, substitute the elapsed time of BGM for elapsed_time.
			elapsed_time = rithm_lib(test.music, test.BPM);
			stop_now = 0;
		}
		else {
			//Shift elapsed_time by the manually entered amount
			elapsed_time += movetime_lib();
			stop_now = 1;
		}
		beat = (elapsed_time) / 4;

		DrawBox(0, 350, 640, 480, RED, 1);
		score += humen_lib(test.dat, beat, stop_now);
		DrawFormatString(0, 0, WHITE, "SCORE : %d", score);

		DxLibDefault();
		if (CheckHitKey(KEY_INPUT_ESCAPE) || !CheckSoundMem(test.music)) {
			break;
		}
	}

	while (CheckHitKey(KEY_INPUT_ESCAPE));
	int jjj;

	if (!CheckSoundMem(test.music)) {
		while (ProcessMessage() == 0)
		{

			DrawFormatString(270, 200, WHITE, "SCORE:%d", score);
			if (user_data.HS_library < score) {
				DrawFormatString(270, 215, WHITE, "HIGH SCORE!!!");
			}
			jjj = score * pow(1.1, user_data.obtain) / 100;
			DrawFormatString(270, 230, WHITE, "%dGOLD GET!!!", jjj);
			DxLibDefault();
			if (CheckHitKey(KEY_INPUT_RETURN)) {
				break;
			}
		}

		while (CheckHitKey(KEY_INPUT_RETURN));
		if (user_data.HS_library < score) {
			user_data.HS_library = score;
		}

		if (user_data.gold + jjj >= INT_MAX) {
			user_data.gold = INT_MAX;
		}
		else {
			user_data.gold += jjj;
		}

	}
	else {
		StopSoundMem(test.music);
	}
	seen = game_select;


}

//Up to here library


