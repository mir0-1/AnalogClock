#pragma once
#include <tchar.h>
#include <Windows.h>

enum Bits_uiInitFlags
{
	CENTERCOORDS = 0b1000000,
	RADIUS = 0b0100000,
	FONT = 0b0010000,
	WHOURS = 0b0001000,
	WMINUTES = 0b0000100,
	WSECONDS = 0b0000010,
	WMILLISECONDS = 0b0000001,
	ALL = 0b1111111,
	NONE = 0b0000000,
	SUBSET_REQUIRED = 0b1111110,
	SUBSET_TIME = 0b0001111
};

typedef struct
{
	WORD wHours;
	WORD wMinutes;
	WORD wSeconds;
	WORD wMilliseconds;
} SCLOCKTIME;

class CAnalogClock
{
	private:
		static LPCTSTR arrSzNumbers[12];
		unsigned int uiRadius;
		POINT sCenterCoords;
		COLORREF uiBackgroundColor;
		HPEN hPenHourAndMinuteArrows;
		HPEN hPenSecondArrow;
		HFONT hFontOfHours;
		ABC sArrCharWidthInfo[10];
		HDC hLastDC;
		unsigned int uiInitFlag;
		SCLOCKTIME sTime;

		void DrawNumbers();
		void DrawArrows();
		void DefaultInitPensAndBrushes();

	public:
		void draw(HDC hDC);

		unsigned int getRadius();
		SCLOCKTIME getTime();
		POINT getCenterCoords();

		void setRadius(unsigned int uiNewRadius);
		void setTime(unsigned int uiParamMask, WORD wHour, WORD wMin, WORD wSeconds, WORD wMilliseconds);
		void setBackgroundColor(COLORREF uiNewColorValue);
		void setHourAndMinuteArrow(COLORREF uiNewColorValue, int iWidth);
		void setSecondsArrow(COLORREF uiNewColorValue, int iWidth);
		void setCenterCoords(POINT sNewCenterCoords);
		void setFont(LPCTSTR szFontName);

		void setEssentials(unsigned int uiParamMask, 
						POINT sCenterCoords, 
						unsigned int uiRadius, 
						LPCTSTR szFontName,
						WORD wHour, WORD wMin, WORD wSeconds, WORD wMilliseconds);
		bool areAllEssentialsSet();

		CAnalogClock();
		CAnalogClock(unsigned int uiParamMask, 
					POINT sCenterCoords, 
					unsigned int uiRadius, 
					LPCTSTR szFontName, 
					WORD wHour, WORD wMin, WORD wSeconds, WORD wMilliseconds);
		~CAnalogClock();
};

