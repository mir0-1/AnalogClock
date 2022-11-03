#include <tchar.h>
#include <Windows.h>
#include "CAnalogClock.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX cWnd;
	{
		cWnd.cbClsExtra = 0;
		cWnd.cbWndExtra = 0;
		cWnd.cbSize = sizeof(WNDCLASSEX);
		cWnd.hbrBackground = (HBRUSH)COLOR_WINDOW;
		cWnd.hCursor = LoadCursor(NULL, IDC_ARROW);
		cWnd.hIcon = NULL;
		cWnd.hIconSm = NULL;
		cWnd.hInstance = hInstance;
		cWnd.lpfnWndProc = WndProc;
		cWnd.lpszClassName = TEXT("analogclock");
		cWnd.lpszMenuName = NULL;
		cWnd.style = CS_HREDRAW | CS_VREDRAW;
	};

	if (!RegisterClassEx(&cWnd))
	{
		MessageBox(0, TEXT("Reg class error"), NULL, MB_ICONERROR);
		return -1;
	}

	HWND hWnd = CreateWindow(
							TEXT("analogclock"), 
							TEXT("Примерен аналогов часовник"), 
							WS_TILEDWINDOW, 
							CW_USEDEFAULT, 
							CW_USEDEFAULT, 
							800, 
							600, 
							NULL, 
							NULL, 
							hInstance, 
							NULL
							);

	ShowWindow(hWnd, nCmdShow);

	MSG sMsg;
	while (GetMessage(&sMsg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&sMsg);
		DispatchMessage(&sMsg);
	}

	return (int)sMsg.wParam;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT sPs;
	static HDC hMemDC;
	static HBITMAP hMemBitmap;
	static UINT uiScreenWidth;
	static UINT uiScreenHeight;
	static CAnalogClock cClk;
	static RECT sClientRect;
	static POINT sCenter;
	static SYSTEMTIME sTime;
	static INT uiRadius;
	static const UINT_PTR cpUiTimerID = 10001;

	switch (uiMsg)
	{
		case WM_ERASEBKGND:
			return 1;
		case WM_TIMER:
			if (wParam == cpUiTimerID)
				InvalidateRect(hWnd, 0, true);
			return 0;
		case WM_CREATE:
		{
			HDC hDC = GetDC(hWnd);

			hMemDC = CreateCompatibleDC(hDC);

			uiScreenWidth = GetSystemMetrics(SM_CXSCREEN);
			uiScreenHeight = GetSystemMetrics(SM_CYSCREEN);

			hMemBitmap = CreateCompatibleBitmap(hDC, uiScreenWidth, uiScreenHeight);
			
			ReleaseDC(hWnd, hDC);
			SetTimer(hWnd, cpUiTimerID, 1000, NULL);

			cClk.setBackgroundColor(RGB(146, 222, 170));
			return 0;
		}
		case WM_DESTROY:
			KillTimer(hWnd, cpUiTimerID);
			DeleteObject(hMemBitmap);
			DeleteDC(hMemDC);
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
			BeginPaint(hWnd, &sPs);
			GetClientRect(hWnd, &sClientRect);
			GetLocalTime(&sTime);
			SelectObject(hMemDC, hMemBitmap);

			sCenter.x = sClientRect.right / 2;
			sCenter.y = sClientRect.bottom / 2;

			if (sClientRect.bottom < sClientRect.right)
				uiRadius = sClientRect.bottom / 2 * 0.8;
			else
				uiRadius = sClientRect.right / 2 * 0.8;

			cClk.setEssentials(0b1111110, sCenter, uiRadius, TEXT("Times New Roman"), sTime.wHour, sTime.wMinute, sTime.wSecond, NULL);
			cClk.setHourAndMinuteArrow(RGB(46, 120, 240), uiRadius * 0.03);
			cClk.setSecondsArrow(RGB(200, 0, 0), uiRadius * 0.01);

			FillRect(hMemDC, &sClientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
			cClk.draw(hMemDC);

			BitBlt(sPs.hdc, 0, 0, sClientRect.right, sClientRect.bottom, hMemDC, 0, 0, SRCCOPY);
			EndPaint(hWnd, &sPs);
			break;
		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}