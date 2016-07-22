// BrickGame.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "BrickGame.h"

#define MAX_LOADSTRING	100
#define TIMER_ID		1

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
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

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BRICKGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BRICKGAME));

    MSG msg;

    // ����Ϣѭ��: 
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

//	����:SetClientSize()
//
//	Ŀ��:���ÿͻ�����С
//
void SetClientSize(HWND hWnd, int width, int height)
{
	if (!hWnd)
	{
		//MessageBox(NULL, "�������ô���", NULL, 1);
		exit(1);
	}
	RECT  rectProgram, rectClient;
	GetWindowRect(hWnd, &rectProgram);   //��ó��򴰿�λ����Ļ����
	GetClientRect(hWnd, &rectClient);    //��ÿͻ�������
										 //�ǿͻ�����,��
	int nWidth = rectProgram.right - rectProgram.left - (rectClient.right - rectClient.left);
	int nHeight = rectProgram.bottom - rectProgram.top - (rectClient.bottom - rectClient.top);
	nWidth += width;
	nHeight += height;

	int showToScreenx = GetSystemMetrics(SM_CXSCREEN) / 2 - nWidth / 2;    //���д���
	int showToScreeny = GetSystemMetrics(SM_CYSCREEN) / 2 - nHeight / 2;
	MoveWindow(hWnd, showToScreenx, showToScreeny, nWidth, nHeight, FALSE);
}

//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
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
		gameCtrl.DrawGameFrame();//���ƿ��
		gameCtrl.DrawGameArea();//������Ϸ��
		gameCtrl.DrawCurBricks();//����curBricks
		//����������
		gameCtrl.DrawNextArea();//����NextArea����
		gameCtrl.DrawNextBricks();//����nextBricks
		//gameCtrl.graphicsMgr.DrawTwoDigits(hdc, 38, false);
		//��Ӵ�����Ʒ���������ʱ�䡢״̬��ť�ȡ�
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
		case VK_RETURN://�س���
			if (gameCtrl.curStatus == GS_Running) {//��ͣ
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

// �����ڡ������Ϣ�������
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
