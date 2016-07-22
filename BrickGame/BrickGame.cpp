// BrickGame.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "BrickGame.h"

#define MAX_LOADSTRING	100
#define TIMER_ID		1

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void				SetClientSize(HWND, int, int);


GameControl gameCtrl;



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BRICKGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BRICKGAME));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//	函数:SetClientSize()
//
//	目的:设置客户区大小
//
void SetClientSize(HWND hWnd, int width, int height)
{
	if (!hWnd)
	{
		//MessageBox(NULL, "窗口设置错误", NULL, 1);
		exit(1);
	}
	RECT  rectProgram, rectClient;
	GetWindowRect(hWnd, &rectProgram);   //获得程序窗口位于屏幕坐标
	GetClientRect(hWnd, &rectClient);    //获得客户区坐标
										 //非客户区宽,高
	int nWidth = rectProgram.right - rectProgram.left - (rectClient.right - rectClient.left);
	int nHeight = rectProgram.bottom - rectProgram.top - (rectClient.bottom - rectClient.top);
	nWidth += width;
	nHeight += height;

	int showToScreenx = GetSystemMetrics(SM_CXSCREEN) / 2 - nWidth / 2;    //居中处理
	int showToScreeny = GetSystemMetrics(SM_CYSCREEN) / 2 - nHeight / 2;
	MoveWindow(hWnd, showToScreenx, showToScreeny, nWidth, nHeight, FALSE);
}

//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BRICKGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BRICKGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_SYSMENU|WS_CAPTION|WS_MINIMIZEBOX|WS_OVERLAPPED,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   SetClientSize(hWnd,
	   2*OFFSET + 2 * FRAME_LINE_WIDTH + CAP_PIXELS + MAX_COLS*(BRICK_PIXELS + CAP_PIXELS) + CTLAREA_WIDTH,
	   2*OFFSET + 2 * FRAME_LINE_WIDTH + CAP_PIXELS + MAX_ROWS*(BRICK_PIXELS + CAP_PIXELS));
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
VOID ClearGameArea(PVOID pvoid) {
	gameCtrl.ClearGameAreaRowByRow(NULL);
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HMETAFILE hmf;
	static int       cxClient, cyClient;
	HDC              hdc;
	PAINTSTRUCT      ps;
	double			 speed;

	switch (message)
	{
	case WM_CREATE:
		gameCtrl.GetHWND(hWnd);
		gameCtrl.PrepareStart();
		return 0;
	case WM_SIZE:

		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_TIMER:
		if (gameCtrl.curStatus == GS_Running)
		{
			gameCtrl.Running();
		}
		else if (gameCtrl.curStatus == GS_GameOver) {
			KillTimer(hWnd, TIMER_ID);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetBkColor(hdc, gameCtrl.graphicsMgr.crBackground);
		SetBkMode(hdc, OPAQUE);
		SetMapMode(hdc, MM_TEXT);
		gameCtrl.DrawGameFrame();//绘制框架
		gameCtrl.DrawGameArea();//绘制游戏区
		gameCtrl.DrawCurBricks();//绘制curBricks
		//控制区绘制
		gameCtrl.DrawNextArea();//绘制NextArea背景
		gameCtrl.DrawNextBricks();//绘制nextBricks
		//gameCtrl.graphicsMgr.DrawTwoDigits(hdc, 38, false);
		//添加代码绘制分数、级别、时间、状态按钮等。
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		KillTimer(hWnd, TIMER_ID);
		DeleteMetaFile(hmf);
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		//gameOver
		case VK_F2:
			if (gameCtrl.curStatus == GS_GameOver)
			{
				_beginthread(ClearGameArea, 0, NULL);
				gameCtrl.PrepareStart();
			}
			break;
		case VK_UP:
			if (gameCtrl.curStatus == GS_Running)
			{
				gameCtrl.TryToRoateCurBricks();
			}
			else{
				gameCtrl.SpeedUp();
			}
			break;
		case VK_LEFT:
			if (gameCtrl.curStatus == GS_Running)
			{
				gameCtrl.TryToMoveLeftCurBricks();
			}
			else {
				gameCtrl.SpeedDown();
			}
			break;
		case VK_RIGHT:
			if (gameCtrl.curStatus == GS_Running)
			{
				gameCtrl.TryToMoveRightCurBricks();
			}
			else {
				gameCtrl.SpeedUp();
			}
			break;
		case VK_DOWN:
			if (gameCtrl.curStatus == GS_Running)
			{
				gameCtrl.Running();
			}else {
				gameCtrl.SpeedDown();
			}
			break;
		case VK_RETURN://回车键
			if (gameCtrl.curStatus == GS_Running) {//暂停
				gameCtrl.Pause();
				KillTimer(hWnd, TIMER_ID);
			}
			else if (gameCtrl.curStatus == GS_Paused ) {
				gameCtrl.Resume();
				SetTimer(hWnd, TIMER_ID, (10 - gameCtrl.level) * 100, NULL);
			}
			else if (gameCtrl.curStatus == GS_PrepareForStart)
			{
				gameCtrl.Start();
				SetTimer(hWnd, TIMER_ID, (10 - gameCtrl.level) * 100, NULL);
			}
			break;
		case VK_SPACE:
			if (gameCtrl.curStatus == GS_Running) {
				bool temp = true;
				do {
					temp = gameCtrl.TryToMoveDownCurBricks();
				} while (temp == true);
				gameCtrl.DoAfterCanNotMoveDown();
			}
			//else if (gameCtrl.curStatus == GS_PrepareForStart) {
			//	gameCtrl.Start();
			//	SetTimer(hWnd, TIMER_ID, (10-gameCtrl.level)*100, NULL);
			//}

			break;
		}
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
