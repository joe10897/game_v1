#ifndef CINTEGER_H
#define CINTEGER_H

class CInteger {
		friend int operator+(const CInteger& lhs, const CInteger& rhs);
		friend int operator-(const CInteger& lhs, const CInteger& rhs);
		friend int operator*(const CInteger& lhs, const CInteger& rhs);
		friend int operator/(const CInteger& lhs, const CInteger& rhs);
		friend bool operator==(const CInteger& lhs, const CInteger& rhs);
		friend bool operator<(const CInteger& lhs, const CInteger& rhs);
		friend bool operator<=(const CInteger& lhs, const CInteger& rhs);
		friend bool operator>(const CInteger& lhs, const CInteger& rhs);
		friend bool operator>=(const CInteger& lhs, const CInteger& rhs);
	public:
		CInteger();
		CInteger(int);
		CInteger(double);
		int  GetInteger();			// 回傳整數值
		void LoadBitmap();			// 載入0..9及負號之圖形
		void operator+=(const int rhs);
		void operator++(int);
		void operator++();
		void operator-=(const int rhs);
		void operator--(int);
		void operator--();
		void operator*=(const int rhs);
		void operator/=(const int rhs);
		void operator=(const int rhs);
		void SetInteger(int);		// 設定整數值
		void SetTopLeft(int, int);	// 將動畫的左上角座標移至 (x,y)
		void ShowBitmap();			// 將動畫貼到螢幕	
		void SetDigit(int digit);   // set the size of the number
		void SetType(int Type);
	private:
		int NUMDIGITS;			// 共顯示NUMDIGITS個位數
		static CMovingBitmap digit[44]; // 儲存0..9及負號之圖形(bitmap)
		int x, y;						// 顯示的座標
		int n;							// 整數值
		bool isBmpLoaded;				// 是否已經載入圖形
		int type;
	};
#endif