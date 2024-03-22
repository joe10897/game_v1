//#define	 INITGUID
#include "stdafx.h"
#include "../Core/game.h"
#include "../Core/MainFrm.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <direct.h>
#include <string.h>
#include "audio.h"
#include "gameutil.h"
#include "gamecore.h"
#include "Shlwapi.h"
#include "../Game/config.h"
#include "../Game/mygame.h"
#include <filesystem>
#include <experimental/filesystem> // Header file for pre-standard implementation
#include <comdef.h>

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// CMovingBitmap: Moving Bitmap class
	// �o�� class ���ѥi�H���ʪ��ϧ�
	// �n���o���I�s(�B��)��U�د�O�A���O�i�H�����U�C���{���O����N��
	/////////////////////////////////////////////////////////////////////////////

	//! CMovingBitmap �غc�l
	/*! 
		�Ω�Хߤ@�ө|��Ū���Ϥ�������C
	*/
	CMovingBitmap::CMovingBitmap()
	{
		isBitmapLoaded = false;
	}

	//! ���o CMovingBitmap ���󪺹Ϥ����סC
	/*!
		�ݭn�����J�Ϥ��C
		\return �Ϥ����סA�H���������C
	*/
	int CMovingBitmap::GetHeight()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Height() is called !!!");
		return locations[frameIndex].bottom - locations[frameIndex].top;
	}

	//! ���o CMovingBitmap ���󪺥��W���� x �b�y�ЭȡC
	/*!
		�ݭn�����J�Ϥ��C
		\return �Ϥ����W���� x �b�y�ЭȡC
	*/
	int CMovingBitmap::GetLeft()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Left() is called !!!");
		return locations[frameIndex].left;
	}

	//! Ū���Ϥ��귽�C
	/*!
		�z�L�귽�s�� `IDB_BITMAP` ��Ū���������Ϥ��A�åB�L�o�S�w�C�� `color`�C
		\param IDB_BITMAP �Ϥ��귽�s��
		\param color ���L�o���C��]�w�]�� `CLR_INVALID`�A�i�Q�� `RGB(<R>, <G>, <B>`) �ӳ]�m�C��^
	*/
	void CMovingBitmap::LoadBitmap(int IDB_BITMAP, COLORREF color)
	{
		CBitmap bitmap;
		BOOL rval = bitmap.LoadBitmap(IDB_BITMAP);
		GAME_ASSERT(rval, "Load bitmap failed !!! Please check bitmap ID (IDB_XXX).");
		BITMAP bitmapSize;
		bitmap.GetBitmap(&bitmapSize);
		
		InitializeRectByBITMAP(bitmapSize);

		surfaceID.push_back(CDDraw::RegisterBitmap(IDB_BITMAP, color));
		filterColor = color;
		isBitmapLoaded = true;
	}

	//! Ū���Ϥ��귽�C
	/*!
		�z�L�Ϥ��۹��| `filepath` ��Ū���������Ϥ��A�åB�L�o�S�w�C�� `color`�C
		\param filepath �Ϥ��۹��|
		\param color ���L�o���C��]�w�]�� `CLR_INVALID`�A�i�Q�� `RGB(<R>, <G>, <B>`) �ӳ]�m�L�o�C��^
	*/
	void CMovingBitmap::LoadBitmap(char *filepath, COLORREF color)
	{
		HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, filepath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (hbitmap == NULL) {
			char error_msg[300];
			sprintf(error_msg, "Loading bitmap	from file \"%s\" failed !!!", filepath);
			GAME_ASSERT(false, error_msg);
		}

		CBitmap *bmp = CBitmap::FromHandle(hbitmap); // memory will be deleted automatically
		BITMAP bitmapSize;
		bmp->GetBitmap(&bitmapSize);

		InitializeRectByBITMAP(bitmapSize);

		surfaceID.push_back(CDDraw::RegisterBitmap(filepath, color));
		imageFileName = string(filepath);
		filterColor = color;
		isBitmapLoaded = true;

		bmp->DeleteObject();
	}

	//! Ū���Ϥ��귽�C
	/*!
		�z�L�Ϥ��۹��|�� `filepaths` ��Ū���h�ӹϤ��A�åB�L�o�S�w�C�� `color`�C
		\param filepaths �Ϥ��۹��|��
		\param color ���L�o���C��]�w�]�� `CLR_INVALID`�A�i�Q�� `RGB(<R>, <G>, <B>`) �ӳ]�m�L�o�C��^
		\sa https://en.cppreference.com/w/cpp/container/vector
	*/
	void CMovingBitmap::LoadBitmap(vector<char*> filepaths, COLORREF color)
	{
		for (int i = 0; i < (int)filepaths.size(); i++) {
			LoadBitmap(filepaths[i], color);
		}
	}

	//! Ū���Ϥ��귽�C
	/*!
		�z�L�Ϥ��۹��|�� `filepaths` ��Ū���h�ӹϤ��A�åB�L�o�S�w�C�� `color`�C
		\param filepaths �Ϥ��۹��|��
		\param color ���L�o���C��]�w�]�� `CLR_INVALID`�A�i�Q�� `RGB(<R>, <G>, <B>`) �ӳ]�m�L�o�C��^
		\sa https://en.cppreference.com/w/cpp/container/vector
	*/
	void CMovingBitmap::LoadBitmapByString(vector<string> filepaths, COLORREF color)
	{

		for (int i = 0; i < (int)filepaths.size(); i++) {
			LoadBitmap((char*)filepaths[i].c_str(), color);
		}
	}
	
	//! Ū���ťչϤ��귽�C
	/*!
		Ū���@�ӯS�w�j�p���զ��I�}�ϡC
		\param height �Ϥ�����
		\param width �Ϥ��e��
	*/
	void CMovingBitmap::LoadEmptyBitmap(int height, int width) {
		HBITMAP hbitmap = CreateBitmap(width, height, 1, 32, NULL);
		CBitmap *bmp = CBitmap::FromHandle(hbitmap); // memory will be deleted automatically

		/* Fill white color to bitmap */
		HDC hdc = CreateCompatibleDC(NULL);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdc, hbitmap);
		PatBlt(hdc, 0, 0, width, height, WHITENESS);
		SelectObject(hdc, hOldBitmap);
		DeleteDC(hdc);

		BITMAP bitmapSize;
		bmp->GetBitmap(&bitmapSize);

		InitializeRectByBITMAP(bitmapSize);

		surfaceID.push_back(CDDraw::RegisterBitmapWithHBITMAP(hbitmap));
		isBitmapLoaded = true;

		bmp->DeleteObject();
	}
	
	//! ������ܹϤ��C
	/*!
		@deprecated �q v1.0.0 �������ΡA������ܹϤ��Цb `OnShow()` �ɤ��I�s `ShowBitmap()` �Y�i
		\sa ShowBitmap()
	*/
	void CMovingBitmap::UnshowBitmap()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before SetTopLeft() is called !!!");
		isAnimation = false;
		this->ShowBitmap(0);
	}

	//! �]�m�Ϥ��ܵe�����w�y�ФW�C
	/*!
		�N�|��Ϥ����W���]�m�ܫ��w�y�ФW�C
		\param x ���W�� x �y��
		\param y ���W�� y �y��
	*/
	void CMovingBitmap::SetTopLeft(int x, int y)
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before SetTopLeft() is called !!!");

		for (int i = 0; i < int(locations.size()); i++) {
			int dx = locations[i].left - x;
			int dy = locations[i].top - y;
			locations[i].left = x;
			locations[i].top = y;
			locations[i].right -= dx;
			locations[i].bottom -= dy;
		}
	}

	//! �]�m�Ϥ��O�_���ʵe�C
	/*!
		�Y CMovingBitmap Ū�J�h�ӹϤ��A�h�i�H�ϥΦ���ƨӳ]�w���󬰰ʵe�C
		\param delay �ʵe��������]�H�@������^
		\param once �ʵe�O�_���@���ʰʵe�A�Y�O�h�ݭn�H `ToggleAnimation()` �өI�s�ʵe�ҰʡC
		\sa ToggleAnimation()
	*/
	void CMovingBitmap::SetAnimation(int delay, bool once) {
		if(!once) isAnimation = true;
		isOnce = once;
		delayCount = delay;
	}
	
	//! ��ܹϤ��C
	/*!
		�ȯ�b `onShow()` �ɩI�s�A�B�Ϥ��ݭn�QŪ���C
	*/
	void CMovingBitmap::ShowBitmap()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before ShowBitmap() is called !!!");
		CDDraw::BltBitmapToBack(surfaceID[frameIndex], locations[frameIndex].left, locations[frameIndex].top);
		ShowBitmapBySetting();
	}

	//! ��ܹϤ��C
	/*!
		�ȯ�b `onShow()` �ɩI�s�A�B�Ϥ��ݭn�QŪ���C
		\param factor ��j���v�A�ݭn VGA ��d���䴩�A�_�h�|�ܱo���`�C�C
	*/
	void CMovingBitmap::ShowBitmap(double factor)
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before ShowBitmap() is called !!!");
		CDDraw::BltBitmapToBack(surfaceID[frameIndex], locations[frameIndex].left, locations[frameIndex].top, factor);
		ShowBitmapBySetting();
	}

	//! �]�m��e�Ϥ���ܴV�����ޭȡC
	/*!
		�Ϥ���ܴV�����ޭȥH 0 �}�l�C
		\param frameIndex �Ϥ���ܴV�����ޭȡC
	*/
	void CMovingBitmap::SetFrameIndexOfBitmap(int frameIndex) {
		GAME_ASSERT(frameIndex < (int) surfaceID.size(), "選擇圖片時索引出界");
		this->frameIndex = frameIndex;
	}

	//! ���o��e�Ϥ���ܴV�����ޭȡC
	/*!
		\return �Ϥ���ܴV�����ޭȡC
	*/
	int CMovingBitmap::GetFrameIndexOfBitmap() {
		return frameIndex;
	}

	//! ���o��e�Ϥ����W�� y �b���y�ЭȡC
	/*!
		\return �Ϥ����W�� y �b���y�ЭȡC
	*/
	int CMovingBitmap::GetTop()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Top() is called !!!");
		return locations[frameIndex].top;
	}

	//! ���o��e�Ϥ��e�סC
	/*!
		\return ���o��e�Ϥ��e�סC
	*/
	int CMovingBitmap::GetWidth()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Width() is called !!!");
		return locations[frameIndex].right - locations[frameIndex].left;
	}
	
	//! �Ұʳ榸�ʵe�C
	/*!
		�N�ʵe�]����l�V�A�åB��l�Ƴ榸�ʵe���ѼƭȡC
	*/
	void CMovingBitmap::ToggleAnimation() {
		frameIndex = 0;
		isAnimation = true;
		isAnimationDone = false;
	}

	//! ����O�_���ʵe����C
	/*!
		\return ���L�ȡA��ܪ���O�_���ʵe����C
	*/
	bool CMovingBitmap::IsAnimation() {
		return isAnimation;
	}

	//! �ʵe����O�_�w���槹�ʵe�C
	/*!
		\return ���L�ȡA��ܰʵe����O�_�w���槹�ʵe�C
	*/
	bool CMovingBitmap::IsAnimationDone() {
		return isAnimationDone;
	}

	//! �ʵe����O�_���榸�ʵe����C
	/*!
		\return ���L�ȡA��ܰʵe����O�_���榸�ʵe����C
	*/
	bool CMovingBitmap::IsOnceAnimation() {
		return isOnce;
	}

	//! ����O�_�wŪ���I�}�ϡC
	/*!
		\return ���L�ȡA��ܪ���O�_�wŪ���I�}�ϡC
	*/
	bool CMovingBitmap::IsBitmapLoaded() {
		return isBitmapLoaded;
	}

	//! �^�Ǫ��󪺴V�ơC
	/*!
		\return �^�Ǫ��󪺴V�ơC
	*/
	int CMovingBitmap::GetFrameSizeOfBitmap() {
		return (int) surfaceID.size();
	}

	//! �ھ� BITMAP �Ӫ�l�� CMovingBitmap ���� location ����C
	/*!
		\param bitmapSize �w��l�ƹL�� BITMAP ����A�a���I�}�Ϫ����P�e
	*/
	void CMovingBitmap::InitializeRectByBITMAP(BITMAP bitmapSize) {
		const unsigned NX = 0;
		const unsigned NY = 0;
		CRect newCrect;
		newCrect.left = NX;
		newCrect.top = NY;
		newCrect.right = NX + bitmapSize.bmWidth;
		newCrect.bottom = NY + bitmapSize.bmHeight;
		locations.push_back(newCrect);
	}

	//! �ھڨϥΪ̳]�w���Ѽƨ���ܹϤ��C
	void CMovingBitmap::ShowBitmapBySetting() {
		if (isAnimation == true && clock() - last_time >= delayCount) {
			frameIndex += 1;
			last_time = clock();
			if (frameIndex == surfaceID.size() && animationCount > 0) {
				animationCount -= 1;
			}
			if (frameIndex == surfaceID.size() && (isOnce || animationCount == 0)) {
				isAnimation = false;
				isAnimationDone = true;
				frameIndex = surfaceID.size() - 1;
				return;
			}
			frameIndex = frameIndex % surfaceID.size();
		}
	}

	//! ���o������J�Ϥ��W�١C
	/*!
		\return �^�ǹϤ��W�١A�Y�Ϥ��|�����J�A�h�^�Ǥ@�Ŧr��C
	*/
	string CMovingBitmap::GetImageFileName() {
		return imageFileName;
	}

	//! ���o����L�o�C��C
	/*!
		\return �^�ǹL�o�C��A�Y���󥼳]�w�L�o�C��A�^�� `CLR_INVALID`�C
	*/
	COLORREF CMovingBitmap::GetFilterColor() {
		return filterColor;
	}

	//! �⪫��O�_���|�C
	/*!
		\param bmp1 �Ĥ@�� CMovingBitmap ����
		\param bmp2 �ĤG�� CMovingBitmap ����
		\return �^�ǥ��L�ȡA�N��⪫��O�_���|�C
	*/
	bool CMovingBitmap::IsOverlap(CMovingBitmap bmp1, CMovingBitmap bmp2) {
		CRect rect;
		BOOL isOverlap = rect.IntersectRect(bmp1.locations[bmp1.GetFrameIndexOfBitmap()], bmp2.locations[bmp2.GetFrameIndexOfBitmap()]);
		return isOverlap;
	}

	/////////////////////////////////////////////////////////////////////////////
	// CTextDraw: The class provide the ability to draw the text.
	// �o�� class ���Ѥ�r���e�{
	// �n���o���I�s(�B��)��U�د�O�A���O�i�H�����U�C���{���O����N��
	/////////////////////////////////////////////////////////////////////////////

	//! �b�e���W�L�X��r�C
	/*!
		�ڭ̱N pDC ���o���d��浹 caller�A�A�����n���o��e�� pDC ���СA�M��O�o����C
		\param pDC�A�@�� CDC ���СA�i�ϥ� `CDDraw::GetBackCDC()` ���o�C
		\param x ��r��ܪ����W�� x �y��
		\param y ��r��ܪ����W�� y �y��
		\param str ����ܪ���r
	*/
	void CTextDraw::Print(CDC *pDC, int x, int y, string str) {
		x = CDDraw::IsFullScreen() ? x + (RESOLUTION_X - SIZE_X) / 2 : x;
		y = CDDraw::IsFullScreen() ? y + (RESOLUTION_Y - SIZE_Y) / 2 : y;
		pDC->TextOut(x, y, str.c_str());
	}

	//! �]�w��e��r���ݩʡC
	/*!
		�ڭ̱N pDC ���o���d��浹 caller�A�A�����n���o��e�� pDC ���СA�M��O�o����C
		���]�w�|�b�]�w�� pDC ���гQ����ɥ��h�]�w�ȡC
		\param pDC�A�@�� CDC ���СA�i�ϥ� `CDDraw::GetBackCDC()` ���o�C
		\param size ��r���j�p�]�H pt �����A���]�޳N�����A�ä��@�w�|�����ŦX pt ����쪺�j�p�^
		\param fontName �r��W��
		\param fontColor �r���C��
		\param weight �r��ʫס]�w�]�� 500�^
	*/
	void CTextDraw::ChangeFontLog(CDC *pDC, int size, string fontName, COLORREF fontColor, int weight) {
		CFont* fp;

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(fontColor);
		LOGFONT lf;
		CFont f;
		memset(&lf, 0, sizeof(lf));
		lf.lfHeight = -MulDiv(size, GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY), 96);
		lf.lfWeight = weight;
		strcpy(lf.lfFaceName, fontName.c_str());
		f.CreateFontIndirect(&lf);
		fp = pDC->SelectObject(&f);
	}

}         
