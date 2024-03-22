	/////////////////////////////////////////////////////////////////////////////
	// CInteger: 這個class提供顯示整數圖形的能力
	// 1. 要懂得怎麼呼叫(運用)其各種能力，但是可以不懂下列的程式是什麼意思
	// 2. 自己寫到運用CMovingBitmap的程式時，可以參考下列程式的寫法
	/////////////////////////////////////////////////////////////////////////////
#include "../Library/gamecore.h"
#include "../Library/gameutil.h"
#include "stdafx.h"
#include "CInteger.h"
#include "../OurLibrary/Resource.h"
#include "stdafx.h"
#include "game.h"
#include "MainFrm.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <direct.h>
#include <string.h>
#include <set>
#include "audio.h"
#include "../Game/mygame.h"

	CMovingBitmap CInteger::digit[44];

	int GetDigit(int n)
	{
		n = abs(n);
		int digit = 0;
		while (n > 0)
		{
			digit++;
			n /= 10;
		}
		return digit == 0 ? 1 : digit;
	}

	CInteger::CInteger()
		: NUMDIGITS(1), n(0), type(0)
	{
		isBmpLoaded = false;
	}

	CInteger::CInteger(int n)
		: n(n)
	{
		NUMDIGITS = GetDigit(this->n);
		isBmpLoaded = false;
	}

	CInteger::CInteger(double n)
		: n((int)n)
	{
		NUMDIGITS = GetDigit(this->n);
		isBmpLoaded = false;
	}

	int CInteger::GetInteger()
	{
		return n;
	}
	
	void CInteger::SetDigit(int digit)
	{
		NUMDIGITS = digit;
	}
	void CInteger::SetType(int Type)
	{
		type = Type;
	}
	void CInteger::LoadBitmap()
	{
		//
		// digit[i]為class varibale，所以必須避免重複LoadBitmap
		//
		if (!isBmpLoaded) {
			int d[44] = { IDB_0, IDB_1, IDB_2, IDB_3, IDB_4, IDB_5, IDB_6, IDB_7, IDB_8, IDB_9, IDB_MINUS,
						  IDB1_0, IDB1_1, IDB1_2, IDB1_3, IDB1_4, IDB1_5, IDB1_6, IDB1_7, IDB1_8, IDB1_9, IDB_MINUS,
						  IDB2_0, IDB2_1, IDB2_2, IDB2_3, IDB2_4, IDB2_5, IDB2_6, IDB2_7, IDB2_8, IDB2_9, IDB_MINUS,
						  IDB3_0, IDB3_1, IDB3_2, IDB3_3, IDB3_4, IDB3_5, IDB3_6, IDB3_7, IDB3_8, IDB3_9, IDB_MINUS };
			for (int i = 0; i < 44; i++)
			{
				if (i < 11) digit[i].LoadBitmap(d[i], RGB(253, 191, 200));
				else if (i < 22) digit[i].LoadBitmap(d[i], RGB(0, 0, 0));
				else if (i < 33) digit[i].LoadBitmap(d[i], RGB(251, 230, 239));
				else if (i < 44) digit[i].LoadBitmap(d[i], RGB(255, 255, 255));
			}
				
			isBmpLoaded = true;
		}
	}

	void CInteger::operator+=(int rhs)
	{
		this->n += rhs;
		this->SetDigit(GetDigit(this->n));
	}

	void CInteger::operator++(int)
	{
		this->n++;
		this->SetDigit(GetDigit(this->n));
	}

	void CInteger::operator++()
	{
		++(this->n);
		this->SetDigit(GetDigit(this->n));
	}

	void CInteger::operator-=(int rhs)
	{
		this->n -= rhs;
		this->SetDigit(GetDigit(this->n));
	}

	void CInteger::operator--(int)
	{
		this->n--;
		this->SetDigit(GetDigit(this->n));
	}

	void CInteger::operator--()
	{
		--(this->n);
		this->SetDigit(GetDigit(this->n));
	}

	void CInteger::operator*=(int rhs)
	{
		this->n *= rhs;
		this->SetDigit(GetDigit(this->n));
	}

	void CInteger::operator/=(int rhs)
	{
		this->n /= rhs;
		this->SetDigit(GetDigit(this->n));
	}

	void CInteger::operator=(int rhs)
	{
		this->n = rhs;
		this->SetDigit(GetDigit(this->n));
	}

	void CInteger::SetInteger(int i)
	{
		n = i;
		SetDigit(GetDigit(n));
	}

	void CInteger::SetTopLeft(int nx, int ny)		// 將動畫的左上角座標移至 (x,y)
	{
		x = nx; y = ny;
	}

	void CInteger::ShowBitmap()
	{
		int Type = type * 11;
		GAME_ASSERT(NUMDIGITS, "CInteger: 請先執行SetDigit，然後才能ShowBitmap");
		int nx;		// 待顯示位數的 x 座標
		int MSB;	// 最左邊(含符號)的位數的數值
		if (n >= 0) {
			MSB = n;
			nx = x + digit[0 + Type].Width()*(NUMDIGITS - 1);
		}
		else {
			MSB = -n;
			nx = x + digit[0 + Type].Width()*NUMDIGITS;
		}
		for (int i = 0; i < NUMDIGITS; i++) {
			int d = MSB % 10;
			MSB /= 10;
			digit[d + Type].SetTopLeft(nx, y);
			digit[d + Type].ShowBitmap();
			nx -= digit[d + Type].Width();
		}
		if (n < 0) { // 如果小於0，則顯示負號
			digit[10 + Type].SetTopLeft(nx, y);
			digit[10 + Type].ShowBitmap();
		}
	}

	int operator+(const CInteger& lhs, const CInteger& rhs)
	{
		return lhs.n + rhs.n;
	}

	int operator-(const CInteger& lhs, const CInteger& rhs)
	{
		return lhs.n - rhs.n;
	}

	int operator*(const CInteger& lhs, const CInteger& rhs)
	{
		return lhs.n * rhs.n;
	}

	int operator/(const CInteger& lhs, const CInteger& rhs)
	{
		return lhs.n / rhs.n;
	}

	bool operator==(const CInteger& lhs, const CInteger& rhs)
	{
		return lhs.n == rhs.n;
	}

	bool operator<(const CInteger& lhs, const CInteger& rhs)
	{
		return lhs.n < rhs.n;
	}

	bool operator<=(const CInteger& lhs, const CInteger& rhs)
	{
		return lhs.n <= rhs.n;
	}

	bool operator>(const CInteger& lhs, const CInteger& rhs)
	{
		return lhs.n > rhs.n;
	}

	bool operator>=(const CInteger& lhs, const CInteger& rhs)
	{
		return lhs.n >= rhs.n;
	}