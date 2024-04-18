#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Core/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <string>
#include <string.h>
#include <set>

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// ï¿½oï¿½ï¿½classï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½æª«ï¿½ï¿½Aï¿½Dï¿½nï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½{ï¿½ï¿½ï¿½ï¿½ï¿½bï¿½oï¿½ï¿½
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{}

CGameStateRun::~CGameStateRun()
{}

void CGameStateRun::OnBeginState()
{
	background.SetTopLeft(0, 0);						// ³]©w­I´ºªº°_©l®y¼Ð
}

void CGameStateRun::OnMove()							// ï¿½ï¿½ï¿½Ê¹Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
{
	
}

void CGameStateRun::OnInit()  								// ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È¤Î¹Ï§Î³]ï¿½w
{
	background.LoadBitmap("Bitmaps\\inGameBG1.bmp");// ¸ü¤J­I´ºªº¹Ï§Î
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // ï¿½Bï¿½zï¿½Æ¹ï¿½ï¿½ï¿½ï¿½Ê§@
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// ï¿½Bï¿½zï¿½Æ¹ï¿½ï¿½ï¿½ï¿½Ê§@
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// ï¿½Bï¿½zï¿½Æ¹ï¿½ï¿½ï¿½ï¿½Ê§@
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // ï¿½Bï¿½zï¿½Æ¹ï¿½ï¿½ï¿½ï¿½Ê§@
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// ï¿½Bï¿½zï¿½Æ¹ï¿½ï¿½ï¿½ï¿½Ê§@
{
}

void CGameStateRun::OnShow()
{
	background.ShowBitmap();
}
