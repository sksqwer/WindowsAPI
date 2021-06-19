// WindowsAPI.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#define _USE_MATH_DEFINES

#include "framework.h"
#include "WindowsAPI.h"
#include <tchar.h>
#include <cmath>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

/// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//Custom Function
void DrawGrid(HDC, const RECT &, int);
void DrawCircle(HDC, const FLOAT, const FLOAT, const FLOAT);
void DrawSunflower(HDC, const FLOAT, const FLOAT, const FLOAT, const int);
void DrawRectangle(HDC, const RECT&);
void DrawInputText(HDC, RECT, TCHAR[]);
void DrawStar(HDC, const POINT &, const int );
void Reaction_Key(HDC, const int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
	

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSAPI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSAPI));

    MSG msg;

    // 기본 메시지 루프입니다:
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



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex; // 윈도우 클래스 타입 구조체 WNDCLASSEXW

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSAPI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const int str_size = 1000;
	RECT rect;
	static bool bKeyDown = false;
	static TCHAR str[str_size];
	static int count = 0, pre;
	static int pos_x = 0, pos_y = 0;
	static SIZE size;
	static int direction_Key = 0;

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_CREATE:
		{
			int breakPoint = 999;

//			CreateCaret(hWnd, NULL, 15, 15);
//			ShowCaret(hWnd);
		}
		break;
	case WM_KEYDOWN:
		{

		if (wParam == VK_UP)
			direction_Key = 1;
		else if (wParam == VK_LEFT)
			direction_Key = 2;
		else if (wParam == VK_RIGHT)
			direction_Key = 3;
		else if (wParam == VK_DOWN)
			direction_Key = 4;
		else
			direction_Key = 0;

		InvalidateRect(hWnd, NULL, true);

		}
		break;
	case WM_CHAR:
		{
			/*pre = count;
			bKeyDown = true;
			if (wParam == VK_BACK && count > 0)
				count--;
			else if (wParam != VK_BACK && count < str_size)
				str[count++] = wParam;
			else if (wParam == VK_RETURN)
			{
				if (count == str_size - 1) break;
				str[count++] = '\n';
				str[count] = NULL;
			}
			else
			{
				if (count == str_size - 1) break;
				str[count++] = wParam;
				str[count] = NULL;
			}
			str[count] = NULL;*/


			
		}
		break;
	case WM_KEYUP:
	{
		/*for(int i = pre; i < count; i++)
			str[i] = ' ';*/
		direction_Key = 0;
		InvalidateRect(hWnd, NULL, true);
	}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
/*GetDC 사용
			HDC hdc2 = GetDC(hWnd);

			ReleaseDC(hWnd, hdc2);
*/
			//Text 출력
			/*TextOut(hdc, 100, 100, _T("Hello World!"), _tcslen(_T("Hello World")));

			rect.left = 50;
			rect.top = 50;
			rect.right = 200;
			rect.bottom = 120;
			SetTextColor(hdc, RGB(255, 0, 0));
			DrawText(hdc, _T("Hello Wolrd"), _tcslen(_T("Hello World")), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
*/
//			SetTextColor(hdc, RGB(0, 0, 0));
			//키 입력 출력
/*			if (bKeyDown)
			{
				RECT rt = { 0,0,400,400 };
				DrawText(hdc, str, _tcslen(str), &rt, DT_TOP | DT_LEFT);
			}*/
//				TextOut(hdc, pos_x, pos_y, str, _tcslen(str));
			
			//Caret
			/*GetTextExtentPoint(hdc, str, _tcslen(str), &size);
			TextOut(hdc, 0, 0, str, _tcslen(str));
			SetCaretPos(size.cx, 0);*/

			//격자출력
/*			RECT rt = { 50,50,250,250 };
			DrawGrid(hdc, rt, 10);*/

			////원출력
			//DrawCircle(hdc, 600, 600, 100);

			//원-원 출력
//			DrawSunflower(hdc, 400, 400, 100, 36);

			//Rectangle
//			DrawRectangle(hdc, rt);

			//TextInputText
//			TCHAR str[100] = _T("Hello World!");
//			DrawInputText(hdc, rt, str);

			//STAR
//			POINT p = { 600, 600 };
//			DrawStar(hdc, p, 300);

			//PEN
			/*HPEN pen = CreatePen(PS_DOT, 1, RGB(255, 0, 0));
			HPEN oldpen = (HPEN)SelectObject(hdc, pen);
			Ellipse(hdc, 200, 200, 150, 150);
			SelectObject(hdc, oldpen);
			DeleteObject(pen);*/

			//Brush
			//HBRUSH brush, oldbrush;
			////brush = CreateSolidBrush(RGB(0, 255, 255));
			//brush = (HBRUSH)GetStockObject(NULL_BRUSH);
			//oldbrush = (HBRUSH)SelectObject(hdc, brush);
			//Ellipse(hdc, 10, 10, 150, 150);
			//SelectObject(hdc, oldbrush);
			//DeleteObject(brush);


			//DirectionKey
			Reaction_Key(hdc, direction_Key);


            EndPaint(hWnd, &ps);
			

        }
        break;
    case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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



void DrawGrid(HDC hdc, const RECT & rt, int rc)
{
	LONG dif = (rt.right - rt.left) / rc;
	RECT r = rt;

	for (int i = 0; i < rc + 1 ; i++)
	{
		MoveToEx(hdc, r.left, r.top, NULL);
		LineTo(hdc, r.left, r.bottom+1);
		r.left += dif;
	}

	r = rt;
	for (int i = 0; i < rc + 1; i++)
	{
		MoveToEx(hdc, r.left, r.top, NULL);
		LineTo(hdc, r.right, r.top);
		r.top += dif;
	}

}

void DrawCircle(HDC hdc, const FLOAT x, const FLOAT y, const FLOAT r)
{

	Ellipse(hdc, x - r, y - r, x + r, y + r);
}

void DrawSunflower(HDC hdc, const FLOAT x, const FLOAT y, const FLOAT R, const int n)
{
	if (n < 3) return;
	DrawCircle(hdc, x, y, R);

	FLOAT radian = 2 * M_PI / n;
	FLOAT r = FLOAT((R * sin(radian / 2.0))) / FLOAT((1.0 - sin(radian / 2.0)));

	FLOAT sx, sy;

	for (int i = 0; i < n; i++)
	{
		sx = sin(radian * FLOAT(i)) * (FLOAT(R) + r);
		sy = cos(radian * FLOAT(i)) * (FLOAT(R) + r);

		DrawCircle(hdc, (FLOAT)x + sx, (FLOAT)y - sy, r);
	}

}
void DrawRectangle(HDC hdc, const RECT &rt)
{
	POINT point[4] = { {rt.left, rt.top}, {rt.right, rt.top},{rt.right, rt.bottom}, {rt.left, rt.bottom} };
	Polygon(hdc, point, 4);
}
void DrawInputText(HDC hdc, RECT rt, TCHAR str[])
{
	DrawRectangle(hdc, rt);
	rt.top += 10;
	rt.bottom -= 10;
	rt.left += 10;
	rt.right -= 10;


	DrawText(hdc, str, _tcslen(str), &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	SIZE size;

	GetTextExtentPoint(hdc, str, _tcslen(str), &size);
	SetCaretPos(size.cx + rt.left, rt.top);

}
void DrawStar(HDC hdc, const POINT &p, const int R)
{
	if (R == 0) return;
	FLOAT radian = 2 * M_PI / 5;
	POINT point[10];
	FLOAT x = p.x, y = p.y, r = R * cos(radian) / cos(radian / 2);
	for (int i = 0; i < 5; i++)
	{
		point[i * 2].x = p.x + R * sin(radian * i);
		point[i * 2].y = p.y - R * cos(radian * i);
		point[i * 2 + 1].x = p.x + r * sin((radian / 2) + radian * i);
		point[i * 2 + 1].y = p.y - r * cos((radian / 2) + radian * i);
	}

	Polygon(hdc, point, 10);
	DrawStar(hdc, p, R - 10);
}

void Reaction_Key(HDC hdc, const int key)
{
	const RECT rt = { 400, 0, 700, 600 };
	long difx = (rt.right - rt.left) / 3, dify = (rt.bottom - rt.top)/3;
	TCHAR dir[4][10] = { _T("위쪽"), _T("왼쪽") , _T("오른쪽") , _T("아래쪽") };
	DrawInputText(hdc, RECT{ rt.left + difx, rt.top, rt.right - difx, rt.bottom - 2 * dify,}, dir[0]);
	DrawInputText(hdc, RECT{ rt.left - 1, rt.top + dify+1, rt.right - 2 * difx - 1, rt.bottom - dify + 1 }, dir[1]);
	DrawInputText(hdc, RECT{ rt.left + 2 * difx + 1, rt.top + dify + 1, rt.right + 2, rt.bottom - dify + 1}, dir[2]);
	DrawInputText(hdc, RECT{ rt.left + difx, rt.top + 2 * dify + 2, rt.right - difx, rt.bottom + 2 }, dir[3]);
	if (key != 0)
	{
		HBRUSH brush, oldbrush;
		brush = CreateSolidBrush(RGB(255, 0, 0));
		oldbrush = (HBRUSH)SelectObject(hdc, brush);
		switch (key)
		{
		case 1:
			DrawRectangle(hdc, RECT{ rt.left + difx, rt.top, rt.right - difx, rt.bottom - 2 * dify, });
			break;
		case 2:
			DrawRectangle(hdc, RECT{ rt.left - 1, rt.top + dify + 1, rt.right - 2 * difx - 1, rt.bottom - dify + 1 });
			break;
		case 3:
			DrawRectangle(hdc, RECT{ rt.left + 2 * difx + 1, rt.top + dify + 1, rt.right + 2, rt.bottom - dify + 1 });
			break;
		case 4:
			DrawRectangle(hdc, RECT{ rt.left + difx, rt.top + 2 * dify + 2, rt.right - difx, rt.bottom + 2 });
			break;
		}
		SelectObject(hdc, oldbrush);
		DeleteObject(brush);
	}

}