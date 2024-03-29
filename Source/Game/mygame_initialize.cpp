#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Core/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{
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
	//loading background image
	background.LoadBitmap("Bitmaps\\InitBackground.bmp");

	int playBtnBmp[] = { IDB_PLAYBUTTON_1, IDB_PLAYBUTTON_2, IDB_PLAYBUTTON_3, IDB_PLAYBUTTON_4,
							 IDB_PLAYBUTTON_5, IDB_PLAYBUTTON_6, IDB_PLAYBUTTON_7, IDB_PLAYBUTTON_8,
							 IDB_PLAYBUTTON_9, IDB_PLAYBUTTON_10, IDB_PLAYBUTTON_11, IDB_PLAYBUTTON_12
	};
	for (int i = 0; i < 12; i++)
	{
		playButton.SetAnimation(playBtnBmp[i], RGB(0, 0, 0));
	}
	//playButton.SetDelayCount(4);
	//clickedPlayButton.LoadBitmap("Bitmaps\\PlayButtonClicked.bmp", RGB(0, 0, 0));

}

void CGameStateInit::OnBeginState()
{
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
}
