#pragma once

//方块的大小
#define BRICK_PIXELS	20
//方块之间的间隙宽度
#define CAP_PIXELS		2
//单个方块内部边界与实体时间的间隙
#define INNER_CAP		2
//游戏区方块的行数和列数
#define MAX_ROWS			20
#define MAX_COLS			10
//框架宽度
#define FRAME_LINE_WIDTH	2
//游戏区距离客户区原点的偏移量
#define OFFSET			10
//控制区的宽度
#define CTLAREA_WIDTH	8*BRICK_PIXELS
//与游戏有关的全局变量

//用于同步线程的临界区变量
static CRITICAL_SECTION cs;


class GraphicsManager
{
private:

	HDC hdc;
	HDC hdcMeta;
	HBRUSH hBrush;
	HPEN hPen;
	HMETAFILE hmfInactive, hmfActive;
	HWND hwnd;

	COLORREF crInactive = RGB(220, 220, 220);
	COLORREF crActive = RGB(75, 75, 75);
private:
	// 函数:在内存里绘制激活的和未激活的方块单元
	//
	//
	//
	void PrepareBricks() {
		//绘制未激活的方块，保存至hmfInactive中
		hdcMeta = CreateMetaFile(NULL);
		hBrush = CreateSolidBrush(crInactive);
		hPen = CreatePen(PS_SOLID, 1, crInactive);
		SelectObject(hdcMeta, hPen);
		Rectangle(hdcMeta, 0, 0, BRICK_PIXELS, BRICK_PIXELS);
		SelectObject(hdcMeta, hBrush);
		Rectangle(hdcMeta, INNER_CAP, INNER_CAP, BRICK_PIXELS - INNER_CAP, BRICK_PIXELS - INNER_CAP);
		hmfInactive = CloseMetaFile(hdcMeta);
		DeleteObject(hPen);
		DeleteObject(hBrush);
		//绘制激活的方块，保存至hmfActive中
		hdcMeta = CreateMetaFile(NULL);
		hPen = CreatePen(PS_SOLID, 1, crActive);
		SelectObject(hdcMeta, hPen);
		hBrush = CreateSolidBrush(crActive);
		Rectangle(hdcMeta, 0, 0, BRICK_PIXELS, BRICK_PIXELS);
		SelectObject(hdcMeta, hBrush);
		Rectangle(hdcMeta, INNER_CAP, INNER_CAP, BRICK_PIXELS - INNER_CAP, BRICK_PIXELS - INNER_CAP);
		hmfActive = CloseMetaFile(hdcMeta);
		DeleteObject(hPen);
		DeleteObject(hBrush);
	}
	// 根据方块的状态在制定的设备坐标处绘制方块
	//
	//
	//
	void DrawBrick(POINT p, BOOL isactive) {
		EnterCriticalSection(&cs);
		hdc = GetDC(hwnd);
		SetViewportOrgEx(hdc, p.x, p.y, NULL);
		if (isactive)
		{
			PlayMetaFile(hdc, hmfActive);
		}
		else {
			PlayMetaFile(hdc, hmfInactive);
		}
		SetViewportOrgEx(hdc, 0, 0, NULL);
		ReleaseDC(hwnd, hdc);
		LeaveCriticalSection(&cs);
	}
public:
	COLORREF crBackground = RGB(255, 255, 255);
	GraphicsManager() {
		InitializeCriticalSection(&cs);
		PrepareBricks();
	}
	GraphicsManager(HWND hwnd) {
		InitializeCriticalSection(&cs);
		this->hwnd = hwnd;
		PrepareBricks();
	}
	~GraphicsManager() {
		ReleaseDC(hwnd, hdc);
		DeleteCriticalSection(&cs);
	}

	void GetHWND(HWND hwnd) {
		this->hwnd = hwnd;
	}

	
	//根据方块的行列索引绘制方块
	void DrawBrick(int column_index, int row_index, BOOL isactive) {
		POINT p;
		p.x = OFFSET + FRAME_LINE_WIDTH + CAP_PIXELS + column_index*(BRICK_PIXELS + CAP_PIXELS);
		p.y = OFFSET + FRAME_LINE_WIDTH + CAP_PIXELS + row_index*(BRICK_PIXELS + CAP_PIXELS);
		DrawBrick(p, isactive);
	}
	//在NextArea显示一个方块
	void DrawBrickAtNextArea( int column_index, int row_index, BOOL isactive) {
		if (column_index > 3 || row_index > 3)//行列索引均不超过3
			return;
		POINT p;
		p.x = OFFSET + FRAME_LINE_WIDTH + CAP_PIXELS + (MAX_COLS+2 + column_index)*(BRICK_PIXELS + CAP_PIXELS);
		p.y = OFFSET + FRAME_LINE_WIDTH + CAP_PIXELS + (MAX_ROWS-10 + row_index)*(BRICK_PIXELS + CAP_PIXELS);
		DrawBrick(p, isactive);
	}
	//绘制游戏区
	void DrawGameFrame() {
		hdc = GetDC(hwnd);
		SetViewportOrgEx(hdc, OFFSET, OFFSET, NULL);
		hPen = CreatePen(PS_SOLID, FRAME_LINE_WIDTH, crActive);
		SelectObject(hdc, hPen);
		Rectangle(hdc, 0, 0, MAX_COLS*(BRICK_PIXELS + CAP_PIXELS) + CAP_PIXELS + 2 * FRAME_LINE_WIDTH,
			MAX_ROWS*(BRICK_PIXELS + CAP_PIXELS) + CAP_PIXELS + 2 * FRAME_LINE_WIDTH);
		DeleteObject(hPen);
		ReleaseDC(hwnd, hdc);
	}

	//void DrawGameArea() {
	//	for (int x = 0; x < 10; x++)
	//	{
	//		for (int y = 0; y < 20; y++)
	//		{
	//			BOOL status = false;// (rand() % 2) * (rand() % 2);
	//			DrawBrick(x, y, status);
	//		}
	//	}
	//}


	// 准备显示Next方块组合的区域4*4大小
	void DrawNextArea() {
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				DrawBrickAtNextArea(x, y, FALSE);
			}
		}
	}
	// 绘制数字
	void DrawDigit(HDC hdc, int iNumber)
	{
		static BOOL  fSevenSegment[10][7] = {
			1, 1, 1, 0, 1, 1, 1,     // 0
			0, 0, 1, 0, 0, 1, 0,     // 1
			1, 0, 1, 1, 1, 0, 1,     // 2
			1, 0, 1, 1, 0, 1, 1,     // 3
			0, 1, 1, 1, 0, 1, 0,     // 4
			1, 1, 0, 1, 0, 1, 1,     // 5
			1, 1, 0, 1, 1, 1, 1,     // 6
			1, 0, 1, 0, 0, 1, 0,     // 7
			1, 1, 1, 1, 1, 1, 1,     // 8
			1, 1, 1, 1, 0, 1, 1 };  // 9
		//static POINT ptSegment[7][6] = {
		//	7,  6,  11,  2,  31,  2,  35,  6,  31, 10,  11, 10,
		//	6,  7,  10, 11,  10, 31,   6, 35,   2, 31,   2, 11,
		//	36,  7,  40, 11,  40, 31,  36, 35,  32, 31,  32, 11,
		//	7, 36,  11, 32,  31, 32,  35, 36,  31, 40,  11, 40,
		//	6, 37,  10, 41,  10, 61,   6, 65,   2, 61,   2, 41,
		//	36, 37,  40, 41,  40, 61,  36, 65,  32, 61,  32, 41,
		//	7, 66,  11, 62,  31, 62,  35, 66,  31, 70,  11, 70 };
		static POINT ptSegment[7][6] = {
			3,  2,  5,  1,  15,  1,  17,  2,  15, 4,  5, 4,
			2,  3,  4, 5,  4, 15,   2, 17,   0, 15,   0, 5,
			17,  3,  19, 5,  19, 15,  17, 17,  15, 15,  15, 5,
			3, 17,  5, 15,  15, 15,  17, 17,  15, 19,  5, 19,
			2, 18,  4, 20,  4, 30,   2, 32,   0, 30,   0, 20,
			17, 18,  19, 19,  19, 30,  17, 32,  15, 30,  15, 20,
			2, 32,  5, 30,  15, 30,  17, 32,  15, 34,  5, 34 };
		int          iSeg;

		for (iSeg = 0; iSeg < 7; iSeg++)
			if (fSevenSegment[iNumber][iSeg])
				Polygon(hdc, ptSegment[iSeg], 6);
	}

	void DrawTwoDigits(HDC hdc, int iNumber, BOOL fSuppress)
	{
		if (!fSuppress || (iNumber / 10 != 0))
			DrawDigit(hdc, iNumber / 10);

		OffsetWindowOrgEx(hdc, -20, 0, NULL);
		DrawDigit(hdc, iNumber % 10);
		OffsetWindowOrgEx(hdc, -20, 0, NULL);
	}

	void DrawColon(HDC hdc)
	{
		POINT ptColon[2][4] = { 2,  21,  6,  17,  10, 21,  6, 25,
			2,  51,  6,  47,  10, 51,  6, 55 };

		Polygon(hdc, ptColon[0], 4);
		Polygon(hdc, ptColon[1], 4);

		OffsetWindowOrgEx(hdc, -12, 0, NULL);
	}

	void DrawTime(HDC hdc, BOOL f24Hour, BOOL fSuppress)
	{
		SYSTEMTIME st;

		GetLocalTime(&st);

		if (f24Hour)
			DrawTwoDigits(hdc, st.wHour, fSuppress);
		else
			DrawTwoDigits(hdc, (st.wHour %= 12) ? st.wHour : 12, fSuppress);

		DrawColon(hdc);
		DrawTwoDigits(hdc, st.wMinute, FALSE);
		DrawColon(hdc);
		DrawTwoDigits(hdc, st.wSecond, FALSE);
	}

};

