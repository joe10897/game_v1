#pragma once
#include "gameutil.h"
#include <ddraw.h>

/////////////////////////////////////////////////////////////////////////////
// �w�qCGame��CGameState�ҨϥΪ��T�Ӫ��A�`��
/////////////////////////////////////////////////////////////////////////////

enum GAME_STATES {
	GAME_STATE_INIT,
	GAME_STATE_RUN,
	GAME_STATE_OVER
};

/////////////////////////////////////////////////////////////////////////////
// Header for STL (Standard Template Library)
/////////////////////////////////////////////////////////////////////////////

#include <list>
#include <vector>
#include <map>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// �^���{�����~�Ϊ�macro
// �Ƶ��G�o�̨ϥ�macro�H�K�O�d�ɮצW�٤Φ渹�A�Q��debug�C
/////////////////////////////////////////////////////////////////////////////

#define GAME_ASSERT(boolexp,str)											\
		if (!(boolexp)) {													\
			int id;															\
			char s[300]="";													\
			sprintf(s,"Game fatal error:\n\n%s\n\nFile: %s\n\nLine: %d"		\
				"\n\n(Press Retry to debug the application, "				\
				"if it is executed in debug mode.)"							\
				"\n(Press Cancel otherwise.)",								\
				 str , __FILE__,__LINE__);									\
			id = AfxMessageBox(s, MB_RETRYCANCEL);							\
																			\
			if (id == IDCANCEL)												\
				exit(1);													\
			AfxDebugBreak();												\
		}

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���Ѯɶ��B���~������
	// �@�몺�C���ä��ݪ����ާ@�o�Ӫ���A�]���i�H���޳o��class���ϥΤ�k
	/////////////////////////////////////////////////////////////////////////////

	class CSpecialEffect {
	public:
		static void  SetCurrentTime();					// �x�s�ثe���ɶ���ctime
		static DWORD GetEllipseTime();					// Ū���ثe���ɶ� - ctime
		static int   GetCurrentTimeCount();				// Ū���x�sctime������
		static void  Delay(DWORD ms);					// ���� x ms
		static void  DelayFromSetCurrentTime(DWORD ms);	// ��ctime�_��A���� x ms
	private:
		static DWORD ctime;
		static int	 ctimeCount;
	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class�|�إ�DirectDraw����A�H���Ѩ�Lclass�ϥ�
	// �@�몺�C���ä��ݪ����ާ@�o�Ӫ���A�]���i�H���޳o��class���ϥΤ�k
	/////////////////////////////////////////////////////////////////////////////

	class CDDraw {
		friend class CMovingBitmap;
	public:
		~CDDraw();
		static void  BltBackColor(DWORD);		// �NBack plain�����ۤW���w���C��
		static void  BltBackToPrimary();		// �NBack plain�K��Primary plain
		static CDC*  GetBackCDC();				// ���oBack Plain��DC (device context)
		static void  GetClientRect(CRect &r);	// ���o�]�w���ѪR��
		static void  Init(int, int);			// Initialize direct draw
		static void  ReleaseBackCDC();			// ��Back Plain��DC (device context)
		static bool  SetFullScreen(bool);		// �]�w�����ù��Ҧ�/�����Ҧ�
		static bool  IsFullScreen();			// �^���O�_�����ù��Ҧ�/�����Ҧ�
	private:
		CDDraw();								// private constructor
		static void  BltBitmapToBack(unsigned SurfaceID, int x, int y);
		static void  BltBitmapToBack(unsigned SurfaceID, int x, int y, double factor);
		static void  BltBitmapToBitmap(unsigned SourceID, unsigned TargetID, int x, int y);
		static void	 CheckDDFail(char *s);
		static bool  CreateSurface();
		static bool  CreateSurfaceFullScreen();
		static bool  CreateSurfaceWindowed();
		static void  LoadBitmap(int i, int IDB_BITMAP);
		static void  LoadBitmap(int i, char *filename);
		static void  LoadBitmapFromExistHBITMAP(int i, HBITMAP bitmap);
		static DWORD MatchColorKey(LPDIRECTDRAWSURFACE lpDDSurface, COLORREF color);
		static int   RegisterBitmap(int IDB_BITMAP, COLORREF ColorKey);
		static int   RegisterBitmap(char *filename, COLORREF ColorKey);
		static int   RegisterBitmapWithHBITMAP(HBITMAP hbitmap);
		static void  ReleaseSurface();
		static void  RestoreSurface();
		static void  SetColorKey(unsigned SurfaceID, COLORREF color);
		static HDC					hdc;
		static CDC					cdc;
		static CView				*pCView;
		static LPDIRECTDRAW2		lpDD;
		static LPDIRECTDRAWCLIPPER	lpClipperPrimary;
		static LPDIRECTDRAWCLIPPER	lpClipperBack;
		static LPDIRECTDRAWSURFACE	lpDDSPrimary;
		static LPDIRECTDRAWSURFACE	lpDDSBack;
		static vector<LPDIRECTDRAWSURFACE>	lpDDS;
		static HRESULT				ddrval;
		static vector<int>			BitmapID;
		static vector<string>		BitmapName;
		static vector<CRect>		BitmapRect;
		static vector<COLORREF>		BitmapColorKey;
		static bool					fullscreen;
		static CDDraw				ddraw;
		static int					size_x, size_y;
	};

	/////////////////////////////////////////////////////////////////////////////
	// �ŧi�|���w�q��class
	/////////////////////////////////////////////////////////////////////////////

	class CGame;
	class CGameStateInit;
	class CGameStateRun;
	class CGameStateOver;

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����U�ت��A��Base class(�O�@��abstract class)
	// �C��Public Interface���Ϊk���n���AImplementation�i�H����
	/////////////////////////////////////////////////////////////////////////////

	class CGameState {
	public:
		CGameState(CGame *g);
		void OnDraw();			// Template Method
		void OnCycle();			// Template Method
		//
		// virtual functions, ���~�Ӫ̴���implementation
		//
		virtual ~CGameState() {}								// virtual destructor
		virtual void OnBeginState() {}							// �]�w�C���i�J�o�Ӫ��A�ɩһݪ����
		virtual void OnInit() {}								// ���A����Ȥιϧγ]�w
		virtual void OnKeyDown(UINT, UINT, UINT) {}				// �B�z��LDown���ʧ@
		virtual void OnKeyUp(UINT, UINT, UINT) {}				// �B�z��LUp���ʧ@
		virtual void OnLButtonDown(UINT nFlags, CPoint point) {}// �B�z�ƹ����ʧ@
		virtual void OnLButtonUp(UINT nFlags, CPoint point) {}	// �B�z�ƹ����ʧ@
		virtual void OnMouseMove(UINT nFlags, CPoint point) {}  // �B�z�ƹ����ʧ@ 
		virtual void OnRButtonDown(UINT nFlags, CPoint point) {}// �B�z�ƹ����ʧ@
		virtual void OnRButtonUp(UINT nFlags, CPoint point) {}	// �B�z�ƹ����ʧ@
		static bool sound, music;								//Switches of sound and music //Add by Limbo 

	protected:
		void GotoGameState(int state);							// ���D�ܫ��w��state
		void ShowInitProgress(int percent, string message);						// ��ܪ�l�ƪ��i��
		void ShowLoading();
																				//
		// virtual functions, ���~�Ӫ̴���implementation
		//
		virtual void OnMove() {}								// ���ʳo�Ӫ��A���C������
		virtual void OnShow() = 0;								// ��ܳo�Ӫ��A���C���e��
		CGame *game;
		CMovingBitmap loadingBitmap;
		static CMovingBitmap settingButton, settingButtonClicked;
	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class�O�C�����֤ߡA����C�����i��
	// �@�몺�C���ä��ݪ����ާ@�o�Ӫ���A�]���i�H���޳o��class���ϥΤ�k
	/////////////////////////////////////////////////////////////////////////////

	class CGame {
	public:
		CGame();										// Constructor
		~CGame();										// Destructor
		bool IsRunning();								// Ū���C���O�_���b�i�椤
		void OnDraw();									// ����CGameView��OnDraw()
		void OnFilePause();								// �C���Ȱ�
		void OnInit();									// �C��ø�Ϥέ��Ī���l��
		void OnInitStates();							// �C���U���A����Ȥιϧγ]�w
		bool OnIdle();									// �C�����D�j��
		void OnKeyDown(UINT, UINT, UINT);				// �B�z��LDown���ʧ@
		void OnKeyUp(UINT, UINT, UINT);					// �B�z��LUp���ʧ@
		void OnKillFocus();								// �C���Q���Ȱ�
		void OnLButtonDown(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		void OnMouseMove(UINT nFlags, CPoint point);    // �B�z�ƹ����ʧ@ 
		void OnRButtonDown(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		void OnResume();								// �B�z�ۡu�ݩR�v�٭쪺�ʧ@
		void OnSetFocus();								// �B�zFocus
		void OnSuspend();								// �B�z�u�ݩR�v���ʧ@
		void SetGameState(int);
		static CGame *Instance();
	private:
		bool			running;			// �C���O�_���b�i�椤(���QPause)
		bool            suspended;			// �C���O�_�Qsuspended
		const int		NUM_GAME_STATES;	// �C�������A��(3�Ӫ��A)
		CGameState		*gameState;			// pointer���V�ثe���C�����A
		CGameState		*gameStateTable[3];	// �C�����A����pointer
		static CGame	instance;			// �C���ߤ@��instance
	};
}
