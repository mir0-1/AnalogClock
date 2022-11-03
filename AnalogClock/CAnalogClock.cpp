#include "CAnalogClock.h"
#include <math.h>
#define PI 3.14159265359

LPCTSTR CAnalogClock::arrSzNumbers[12] =
{
	TEXT("1"),
	TEXT("2"),
	TEXT("3"),
	TEXT("4"),
	TEXT("5"),
	TEXT("6"),
	TEXT("7"),
	TEXT("8"),
	TEXT("9"),
	TEXT("10"),
	TEXT("11"),
	TEXT("12"),
};

void CAnalogClock::DrawNumbers()
{
	int iPrevBkMode = SetBkMode(this->hLastDC, TRANSPARENT);

	int i, iCurrentCharWidth_FirstDigit, iCurrentCharWidth_SecondDigit;
	for (i = 0; i < 12; i++)
	{
		iCurrentCharWidth_FirstDigit =  this->sArrCharWidthInfo[(i + 1) % 10].abcA + 
										this->sArrCharWidthInfo[(i + 1) % 10].abcB + 
										this->sArrCharWidthInfo[(i + 1) % 10].abcC;
		if (i >= 9)
		{
			iCurrentCharWidth_SecondDigit = this->sArrCharWidthInfo[(i + 1) / 10].abcA +
											this->sArrCharWidthInfo[(i + 1) / 10].abcB +
											this->sArrCharWidthInfo[(i + 1) / 10].abcC;
		}
		else
			iCurrentCharWidth_SecondDigit = 0;

		TextOut(
				this->hLastDC,
				this->sCenterCoords.x + this->uiRadius * 0.8 * cos(2 * PI / 12 * (i-2)) - (iCurrentCharWidth_FirstDigit + iCurrentCharWidth_SecondDigit) / 2,
				this->sCenterCoords.y + this->uiRadius * 0.8 * sin(2 * PI / 12 * (i-2)) - uiRadius*0.2/2,
				this->arrSzNumbers[i],
				2
			);
	}

	SetBkMode(this->hLastDC, iPrevBkMode);
}

void CAnalogClock::DrawArrows()
{
	HPEN hOldPen = (HPEN)SelectObject(this->hLastDC, this->hPenHourAndMinuteArrows);
	MoveToEx(this->hLastDC, this->sCenterCoords.x, this->sCenterCoords.y, NULL);
	// ��������� �� �������� ��� ��������� �� �������� ������� ��������� ������� ����������,
	// ��� ���� ��������� ����������� �� �� 12, � �� 12*60 (�� ����� ������)
	// ���� �������� ������� ������ ���� �� ����� �� ����� ������.
	// ��� �� �� ������������� �� ����� ������ ��� ������ ����, �������� �� ��������� ���� � ��������, � �� ������
	// ��������, �������� ������� �� ���� ����� �� 11 ������ ����� � 11:00. �� �� �� ���� � �� ��������� ������ ������� ��� 11:59.
	// � ������ ����� 12:00, ��������� �������� �� �� �������� �� 12. ������ � ���������� ���� ������� - ��������� ����� �� �� ���������
	// ��� ��������� ��� (�� ����� ������ �� ���������), �� �� �� ��������� ������ �� ���������� ���������.
	// 2*PI / (60 ��� * 12 ����) * (60 ��� * �� + ��)

	// ������ �� �� �������� �����, �� � �������� �� ��������� ����������� ������� (0 �������) ������ 15:00 (��� 03:00)
	// ��� �� ������� � ������� ������� ��:�� � 00:00, ���������� ������� �� �� 0 �������, ����� �� ���� ��� 3�.
	// ��� �������� ��������� � 90 ������� � ����������� ������ (� ������ ��� � ������� �� �������������), �� �� �������
	// �� 12� � �� �� �������� ������������. ������ � �������� ������� �� ������ -PI/2 ������ ��������

	LineTo(
		this->hLastDC,
		this->sCenterCoords.x + this->uiRadius * 0.5 * cos(2 * PI / (60 * 12) * (60 * this->sTime.wHours + this->sTime.wMinutes) - PI / 2),
		this->sCenterCoords.y + this->uiRadius * 0.5 * sin(2 * PI / (60 * 12) * (60 * this->sTime.wHours + this->sTime.wMinutes) - PI / 2)
	);

	// ������ ������ �� ������� � �� ��������� �������, �� ����������� ���, ������ ������ �������-������� �������, ���� ��� �
	// ������� �������-�������� �������
	MoveToEx(this->hLastDC, this->sCenterCoords.x, this->sCenterCoords.y, NULL);

	LineTo(
		this->hLastDC,
		this->sCenterCoords.x + this->uiRadius * 0.8 * cos(2 * PI / (60 * 60) * (60 * this->sTime.wMinutes + this->sTime.wSeconds) - PI / 2),
		this->sCenterCoords.y + this->uiRadius * 0.8 * sin(2 * PI / (60 * 60) * (60 * this->sTime.wMinutes + this->sTime.wSeconds) - PI / 2)
	);

	SelectObject(this->hLastDC, this->hPenSecondArrow);

	// ���� ������ �� �� ������� �� ����������� �������, ��� ���� ������� ��������� � ���������� �����.
	MoveToEx(this->hLastDC, this->sCenterCoords.x, this->sCenterCoords.y, NULL);

	LineTo(
		this->hLastDC,
		this->sCenterCoords.x + this->uiRadius * 0.8 * cos(2 * PI / (60) * (this->sTime.wSeconds) - PI / 2),
		this->sCenterCoords.y + this->uiRadius * 0.8 * sin(2 * PI / (60) * (this->sTime.wSeconds) - PI / 2)
	);

	SelectObject(this->hLastDC, hOldPen);
}

void CAnalogClock::DefaultInitPensAndBrushes()
{
	this->hPenHourAndMinuteArrows = (HPEN)GetStockObject(BLACK_PEN);
	this->hPenSecondArrow = (HPEN)GetStockObject(BLACK_PEN);
	this->uiBackgroundColor = RGB(255, 255, 255);
}

void CAnalogClock::draw(HDC hDC)
{
	if (this->areAllEssentialsSet())
	{
		HFONT hOldFont = (HFONT)SelectObject(hDC, this->hFontOfHours);

		GetCharABCWidths(hDC, TEXT('0'), TEXT('9'), this->sArrCharWidthInfo);
		this->hLastDC = hDC;

		HBRUSH hOldBrush = (HBRUSH)SelectObject(this->hLastDC, GetStockObject(DC_BRUSH));
		SetDCBrushColor(this->hLastDC, this->uiBackgroundColor);
		Ellipse(
			hDC,
			(sCenterCoords.x) - (this->uiRadius),
			(sCenterCoords.y) - (this->uiRadius),
			(sCenterCoords.x) + (this->uiRadius),
			(sCenterCoords.y) + (this->uiRadius)
		);
		SelectObject(this->hLastDC, hOldBrush);

		this->DrawNumbers();
		this->DrawArrows();

		SelectObject(hDC, hOldFont);
	}
}

unsigned int CAnalogClock::getRadius()
{
	return this->uiRadius;
}

SCLOCKTIME CAnalogClock::getTime()
{
	return this->sTime;
}

POINT CAnalogClock::getCenterCoords()
{
	return this->sCenterCoords;
}

void CAnalogClock::setCenterCoords(POINT sNewCenterCoords)
{
	this->sCenterCoords = sNewCenterCoords;
	this->uiInitFlag |= Bits_uiInitFlags::CENTERCOORDS;
}

void CAnalogClock::setFont(LPCTSTR szFontName)
{
	if (this->uiInitFlag & Bits_uiInitFlags::FONT)
		DeleteObject(this->hFontOfHours);

	this->hFontOfHours = CreateFont(
		this->uiRadius * 0.2,
		0, 0, 0, 0, 0, 0, 0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		DEFAULT_PITCH,
		szFontName
	);

	this->hLastDC = (HDC)INVALID_HANDLE_VALUE;
	this->uiInitFlag |= Bits_uiInitFlags::FONT;
}


void CAnalogClock::setEssentials(unsigned int uiParamMask, 
								POINT sCenterCoords, 
								unsigned int uiRadius, 
								LPCTSTR szFontName,
								WORD wHour, WORD wMin, WORD wSeconds, WORD wMilliseconds)
{
	if (uiParamMask & Bits_uiInitFlags::CENTERCOORDS)
		this->sCenterCoords = sCenterCoords;

	if (uiParamMask & Bits_uiInitFlags::RADIUS)
		this->uiRadius = uiRadius;

	if (uiParamMask & Bits_uiInitFlags::FONT)
		this->setFont(szFontName);

	this->uiInitFlag = uiParamMask;
	this->setTime(this->uiInitFlag & Bits_uiInitFlags::SUBSET_TIME, wHour, wMin, wSeconds, wMilliseconds);
}

bool CAnalogClock::areAllEssentialsSet()
{
	return ((this->uiInitFlag & Bits_uiInitFlags::SUBSET_REQUIRED) == Bits_uiInitFlags::SUBSET_REQUIRED);
}

CAnalogClock::CAnalogClock() 
{ 
	this->uiInitFlag = Bits_uiInitFlags::NONE; 
	this->DefaultInitPensAndBrushes();
}

CAnalogClock::CAnalogClock(unsigned int uiParamMask, 
							POINT sCenterCoords, 
							unsigned int uiRadius, 
							LPCTSTR szFontName,
							WORD wHour, WORD wMin, WORD wSeconds, WORD wMilliseconds)
{
	this->setEssentials(uiParamMask, sCenterCoords, uiRadius, szFontName, wHour, wMin, wSeconds, wMilliseconds);
	this->DefaultInitPensAndBrushes();
}

CAnalogClock::~CAnalogClock()
{
	if (this->uiInitFlag & Bits_uiInitFlags::FONT)
		DeleteObject(this->hFontOfHours);
}

void CAnalogClock::setRadius(unsigned int uiNewRadius)
{
	this->uiRadius = uiNewRadius;
	this->uiInitFlag |= Bits_uiInitFlags::RADIUS;
}

void CAnalogClock::setTime(unsigned int uiParamMask, WORD wHour, WORD wMin, WORD wSeconds, WORD wMilliseconds)
{
	if (uiParamMask)
	{
		if (uiParamMask & Bits_uiInitFlags::WHOURS)
			this->sTime.wHours = wHour;

		if (uiParamMask & Bits_uiInitFlags::WMINUTES)
			this->sTime.wMinutes = wMin;

		if (uiParamMask & Bits_uiInitFlags::WSECONDS)
			this->sTime.wSeconds = wSeconds;

		if (uiParamMask & Bits_uiInitFlags::WMILLISECONDS)
			this->sTime.wMilliseconds = wMilliseconds;
	}

	this->uiInitFlag |= uiParamMask;
}

void CAnalogClock::setBackgroundColor(COLORREF uiNewColorValue)
{
	this->uiBackgroundColor = uiNewColorValue;
}

void CAnalogClock::setHourAndMinuteArrow(COLORREF uiNewColorValue, int iWidth)
{
	if (this->hPenHourAndMinuteArrows != NULL)
		DeleteObject(this->hPenHourAndMinuteArrows);

	this->hPenHourAndMinuteArrows = CreatePen(PS_SOLID, iWidth, uiNewColorValue);
}

void CAnalogClock::setSecondsArrow(COLORREF uiNewColorValue, int iWidth)
{
	if (this->hPenSecondArrow != NULL)
		DeleteObject(this->hPenSecondArrow);

	this->hPenSecondArrow = CreatePen(PS_SOLID, iWidth, uiNewColorValue);
}
