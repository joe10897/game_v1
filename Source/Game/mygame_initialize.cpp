#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Core/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "config.h"

using namespace game_framework;
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{
	playBtnClicked = false;
}

void CGameStateInit::OnInit()
{
	//
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(0, "Start Initialize...");	// �@�}�l��loading�i�׬�0%
	//ShowLoading();
	//
	// �}�l���J���
	//
	//Sleep(1000);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
	//
	//Load background image
	background.LoadBitmap("Bitmaps\\InitBackground.bmp");

	//Load play button
	playButton.LoadBitmap({ "Bitmaps\\PlayButton1.bmp","Bitmaps\\PlayButton2.bmp","Bitmaps\\PlayButton3.bmp",
		"Bitmaps\\PlayButton4.bmp" ,"Bitmaps\\PlayButton5.bmp" ,"Bitmaps\\PlayButton6.bmp" ,"Bitmaps\\PlayButton7.bmp" ,
		"Bitmaps\\PlayButton8.bmp" ,"Bitmaps\\PlayButton9.bmp" ,"Bitmaps\\PlayButton10.bmp" ,"Bitmaps\\PlayButton11.bmp","Bitmaps\\PlayButton12.bmp" }, RGB(0, 0, 0));
	playButton.SetTopLeft(520, 550);
	playButton.SetAnimation(24, false);

	clickedPlayButton.LoadBitmap("Bitmaps\\PlayButtonClicked.bmp", RGB(0, 0, 0));

	//Load tiffy
	tiffy.LoadBitmap({"Bitmaps\\Tiffy_0.bmp","Bitmaps\\Tiffy_1.bmp","Bitmaps\\Tiffy_2.bmp","Bitmaps\\Tiffy_3.bmp"
		,"Bitmaps\\Tiffy_4.bmp" ,"Bitmaps\\Tiffy_5.bmp" ,"Bitmaps\\Tiffy_6.bmp" ,"Bitmaps\\Tiffy_7.bmp","Bitmaps\\Tiffy_8.bmp","Bitmaps\\Tiffy_9.bmp"},RGB(255,255,255));
	tiffy.SetTopLeft(90, 400);
	tiffy.SetAnimation(48, false);

	//Load Toffee
	toffee.LoadBitmap({ "Bitmaps\\Toffee_1.bmp","Bitmaps\\Toffee_2.bmp","Bitmaps\\Toffee_3.bmp","Bitmaps\\Toffee_4.bmp","Bitmaps\\Toffee_5.bmp","Bitmaps\\Toffee_6.bmp" },RGB(255,255,255));
	toffee.SetTopLeft(900, 60);
	toffee.SetAnimation(48, false);

	//Load Candy Crush Logo
	candycrush.LoadBitmap({ "Bitmaps\\candy_crush (1).bmp","Bitmaps\\candy_crush (2).bmp","Bitmaps\\candy_crush (3).bmp","Bitmaps\\candy_crush (4).bmp","Bitmaps\\candy_crush (5).bmp","Bitmaps\\candy_crush (6).bmp",
		"Bitmaps\\candy_crush (7).bmp" ,"Bitmaps\\candy_crush (8).bmp" ,"Bitmaps\\candy_crush (9).bmp" ,"Bitmaps\\candy_crush (10).bmp" ,"Bitmaps\\candy_crush (11).bmp" ,"Bitmaps\\candy_crush (12).bmp" ,"Bitmaps\\candy_crush (13).bmp" ,
		"Bitmaps\\candy_crush (14).bmp" ,"Bitmaps\\candy_crush (15).bmp" ,"Bitmaps\\candy_crush (16).bmp" ,"Bitmaps\\candy_crush (17).bmp" ,"Bitmaps\\candy_crush (18).bmp" ,"Bitmaps\\candy_crush (19).bmp",
		"Bitmaps\\candy_crush (20).bmp","Bitmaps\\candy_crush (21).bmp","Bitmaps\\candy_crush (22).bmp","Bitmaps\\candy_crush (23).bmp" }, RGB(255, 255, 255));
	candycrush.SetTopLeft(350, -50);
	candycrush.SetAnimation(60, true);
	candycrush.ToggleAnimation();

	//load audio
	CAudio::Instance()->Load(AUDIO_STAGE, "sounds\\Overworld_Level_Select.mp3");
	CAudio::Instance()->Load(AUDIO_BTN_CLICK, "sounds\\button_press.wav");
	CAudio::Instance()->Load(AUDIO_BTN_RELEASE, "sounds\\button_release.wav");

	//load settinf button
	//settingButton.LoadBitmap({"Bitmaps/SettingButton-0.bmp"}, RGB(255, 255, 255));
	//settingButton.LoadBitmap({"Bitmaps/SettingButton-1.bmp" }, RGB(255, 255, 255));
	//settingButton.LoadBitmap({"Bitmaps/SettingButton-2.bmp"}, RGB(255, 255, 255));
	//settingButton.LoadBitmap({ "Bitmaps/SettingButton-1.bmp" }, RGB(255, 255, 255));
	//settingButtonClicked.LoadBitmap("Bitmaps\\settingButtonClicked.bmp", RGB(0, 0, 0));

}

void CGameStateInit::OnBeginState()
{
	if (finishLoaded)
	{
		playBtnClicked = false; //rest playbutton state
	}
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
}


void CGameStateInit::OnShow()
{
	background.ShowBitmap();
	playButton.ShowBitmap();
	tiffy.ShowBitmap();
	toffee.ShowBitmap();
	candycrush.ShowBitmap();
	//�K�WPlay Button
	//if (playBtnClicked)
	//{
	//	clickedPlayButton.SetTopLeft(SIZE_X / 2 , SIZE_Y / 2);
	//	clickedPlayButton.ShowBitmap();
	//}
	//else
	//{
	//	playButton.SetTopLeft(SIZE_X / 2, SIZE_Y / 2);

}
