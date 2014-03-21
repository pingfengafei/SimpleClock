#include <windows.h>
#include <stdio.h>
#include <math.h>

#pragma warning (disable:4244)

#define PI ((double)(3.141592653589793238))

#define Dim(a) (sizeof(a)/sizeof(a[0]))

//在这里声明处理消息的函数
#define MY_DECLARED_MESSAGE											\
static LRESULT	OnCreate(HWND , UINT , WPARAM , LPARAM);			\
static LRESULT	OnTimer(HWND , UINT , WPARAM , LPARAM);				\
static LRESULT	OnMouseMove(HWND , UINT , WPARAM , LPARAM);			\
static LRESULT	OnLButtonDown(HWND , UINT , WPARAM , LPARAM);		\
static LRESULT	OnPaint(HWND , UINT , WPARAM , LPARAM);				\
static LRESULT	OnClose(HWND , UINT , WPARAM , LPARAM);				\
static LRESULT	OnDestroy(HWND , UINT , WPARAM , LPARAM);			\

//在这里写函数和消息的对应
#define MY_MESSAGE_MAP										\
	const FuncList LiteWnd::ProcMap[]={						\
	WM_CREATE , OnCreate,									\
	WM_TIMER , OnTimer,										\
	WM_MOUSEMOVE , OnMouseMove,								\
	WM_LBUTTONDOWN , OnLButtonDown,							\
	WM_PAINT , OnPaint,										\
	WM_CLOSE , OnClose,										\
	WM_DESTROY , OnDestroy,									\
};															\

#include "clock.h"


LiteApp myliteapp;


//在这里写出函数体
LRESULT	LiteWnd::OnCreate(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
{
	SetTimer(hWnd , 127 , 1000 , NULL);
	return NULL;
}

LRESULT	LiteWnd::OnTimer(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
{
	RECT SysRect;
	GetClientRect(hWnd , &SysRect);
	InvalidateRect(hWnd , &SysRect , TRUE);
	return NULL;
}

LRESULT	LiteWnd::OnMouseMove(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
{
	RECT SysRect;
	GetClientRect(hWnd , &SysRect);
	int ArrCenter = (SysRect.right>SysRect.bottom)?(SysRect.bottom/20):(SysRect.right/20);
	int xPos = LOWORD(lParam); 
	int yPos = HIWORD(lParam); 
	if((xPos>0)&&(xPos<2*ArrCenter)&&(yPos>0)&&(yPos<2*ArrCenter))
	{
		SetClassLong(hWnd , GCL_HCURSOR , (long)LoadCursor(NULL , IDC_HAND));
	}
	else
	{
		SetClassLong(hWnd , GCL_HCURSOR , (long)LoadCursor(NULL , IDC_ARROW));
	}
	return NULL;
}

LRESULT	LiteWnd::OnLButtonDown(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
{
	RECT SysRect;
	GetClientRect(hWnd , &SysRect);
	int ArrCenter = (SysRect.right>SysRect.bottom)?(SysRect.bottom/20):(SysRect.right/20);
	int xPos = LOWORD(lParam); 
	int yPos = HIWORD(lParam); 
	if((xPos>0)&&(xPos<2*ArrCenter)&&(yPos>0)&&(yPos<2*ArrCenter))
	{
		KillTimer(hWnd , 127);
		DestroyWindow(hWnd);
	}
	else
	{
		SendMessage(hWnd , WM_NCLBUTTONDOWN , HTCAPTION , 0);;
	}
	return NULL;
}

LRESULT	LiteWnd::OnPaint(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd , &ps);
	RECT SysRect;
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);
	GetClientRect(hWnd , &SysRect);
	SelectObject(hdc , GetStockObject(BLACK_BRUSH));

	int xCentre = ((SysRect.right - SysRect.left)>>1);
	int yCentre = ((SysRect.bottom-SysRect.top)>>1);
	int r = (xCentre>yCentre)?(SysRect.bottom/3):(SysRect.right/3);


	HFONT hf = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "微软雅黑");
	HFONT hOldFont = (HFONT)SelectObject(hdc,hf);

	char str[32];
	sprintf(str , "现在时间：%.2d-%.2d-%.2d" , SysTime.wHour , SysTime.wMinute , SysTime.wSecond);
	SetBkMode(hdc , TRANSPARENT);
	SetTextColor(hdc , RGB(255 , 0 , 128));
	DrawText(hdc , str , -1 , &SysRect , DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
	DeleteObject(hf);

	HPEN hPen = CreatePen(PS_DASH , 2 , RGB(255 , 0 , 128));
	HPEN hOldPen = (HPEN)SelectObject(hdc , hPen);

	MoveToEx(hdc , SysRect.left , SysRect.top , NULL);
	LineTo(hdc , SysRect.right-1 , SysRect.top);
	LineTo(hdc , SysRect.right-1 , SysRect.bottom-2);
	LineTo(hdc , SysRect.left , SysRect.bottom-2);
	LineTo(hdc , SysRect.left , SysRect.top);


	int ArrCenter = r/7;
	int bias = r/10;
	DeleteObject(hPen);
	hPen = CreatePen(PS_DASH , 5 , RGB(255 , 0 , 128));
	SelectObject(hdc , hPen);
	MoveToEx(hdc , ArrCenter , ArrCenter+bias , NULL);
	LineTo(hdc ,  3*ArrCenter , ArrCenter+bias);
	MoveToEx(hdc , ArrCenter , ArrCenter+bias , NULL);
	LineTo(hdc ,  2*ArrCenter , ArrCenter/3+bias);
	MoveToEx(hdc , ArrCenter , ArrCenter+bias , NULL);
	LineTo(hdc ,  2*ArrCenter , 5*ArrCenter/3+bias);



	DeleteObject(hPen);
	hPen = CreatePen(PS_DASH , 3 , RGB(255 , 0 , 128));
	SelectObject(hdc , hPen);
	Ellipse(hdc , xCentre-r , yCentre-r , xCentre+r , yCentre+r);
	int xDest , yDest;

	xDest = xCentre + r*sin((SysTime.wSecond*PI)/30)*(0.9);
	yDest = yCentre - r*cos((SysTime.wSecond*PI)/30)*(0.9);

	DeleteObject(hPen);
	hPen = CreatePen(PS_DASH , 3 , RGB(128 , 255 , 255));
	SelectObject(hdc , hPen);
	MoveToEx(hdc , xCentre , yCentre , NULL);
	LineTo(hdc , xDest , yDest);

	xDest = xCentre + r*sin(((SysTime.wMinute+float(SysTime.wSecond)/60)*PI)/30)*(0.7);
	yDest = yCentre - r*cos(((SysTime.wMinute+float(SysTime.wSecond)/60)*PI)/30)*(0.7);

	DeleteObject(hPen);
	hPen = CreatePen(PS_DASH , 4 , RGB(255 , 0 , 128));
	SelectObject(hdc , hPen);
	MoveToEx(hdc , xCentre , yCentre , NULL);
	LineTo(hdc , xDest , yDest);

	xDest = xCentre + r*sin(((SysTime.wHour+float(SysTime.wMinute)/60)*PI)/6)*(0.4);
	yDest = yCentre - r*cos(((SysTime.wHour+float(SysTime.wMinute)/60)*PI)/6)*(0.5);
	DeleteObject(hPen);
	hPen = CreatePen(PS_DASH , 7 , RGB(255 , 0 , 128));
	SelectObject(hdc , hPen);
	MoveToEx(hdc , xCentre , yCentre , NULL);
	LineTo(hdc , xDest , yDest);

	DeleteObject(hPen);
	hPen = CreatePen(PS_DASH , 9 , RGB(128 , 128 , 255));
	SelectObject(hdc , hPen);
	Ellipse(hdc , xCentre-3 , yCentre-3 , xCentre+3 , yCentre+3);

	DeleteObject(hPen);
	hPen = CreatePen(PS_DASH , 3 , RGB(255 , 0 , 128));
	SelectObject(hdc , hPen);
	RECT numRect;
	for(int sim = 5 ; sim<=60 ; sim+=5)
	{
		int xstart = xCentre + r*sin((sim*PI)/30);
		int ystart = yCentre - r*cos((sim*PI)/30);
		xDest = xCentre + r*sin((sim*PI)/30)*(0.97);
		yDest = yCentre - r*cos((sim*PI)/30)*(0.97);
		MoveToEx(hdc , xstart , ystart , NULL);
		LineTo(hdc , xDest , yDest);

		hf = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "微软雅黑");
		SelectObject(hdc,hf);
		sprintf(str , "%d" , sim/5);
		xDest = xCentre + r*sin((sim*PI)/30)*(0.9);
		yDest = yCentre - r*cos((sim*PI)/30)*(0.9);
		numRect.left = xDest - 7;
		numRect.right = xDest + 7;
		numRect.top = yDest - 7;
		numRect.bottom = yDest + 7;
		DrawText(hdc , str , -1 , &numRect , DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		DeleteObject(hf);
	}
	DeleteObject(hPen);
	SelectObject(hdc , hOldPen);
	SelectObject(hdc , hOldFont);
	EndPaint(hWnd , &ps);
	return NULL;
}

LRESULT	LiteWnd::OnClose(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
{
	KillTimer(hWnd , 127);
	DestroyWindow(hWnd);
	return NULL;
}

LRESULT	LiteWnd::OnDestroy(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
{
	PostQuitMessage(0);
	return NULL;
}
