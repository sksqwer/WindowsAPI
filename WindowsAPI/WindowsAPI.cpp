// WindowsAPI.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#define _USE_MATH_DEFINES

#include "framework.h"
#include "WindowsAPI.h"
#include <tchar.h>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <list>
#include <vector>
#include <string>
#include <stdio.h>
#include <commdlg.h>

// Custom header
#include "CShape.h"


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
void OuFromFile(TCHAR filename[], HWND hWnd)
{
	FILE *fPtr;
	HDC hdc;
	int line;
	TCHAR buffer[500];
	line = 0;
	hdc = GetDC(hWnd);
#ifdef _UNICODE
	_tfopen_s(&fPtr, filename, _T("r, ccs = UNICODE"));
#else
	_tfopen_s(&fPtr, filename, _T("r"));
#endif
	while (_fgetts(buffer, 100, fPtr) != NULL)
	{
		if (buffer[_tcslen(buffer) - 1] == _T('\n'))
			buffer[_tcslen(buffer) - 1] = NULL;
		TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));
		line++;
	}
	fclose(fPtr);
	ReleaseDC(hWnd, hdc);
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//대화상자 색깔
	CHOOSECOLOR COLOR;
	static COLORREF tmp[16], color;
	HBRUSH hBrush, OldBrush;
	int i;

	//열기 대화상자
	OPENFILENAME OFN;
	TCHAR str[100], lpstrFile[100] = _T("");
	TCHAR filter[] = _T("Every File(*,*) \0*.*\0Text File\0*.txt;*.doc\0");

	const int str_size = 1000;
	static RECT rect;
	static RECT rectview;
	static bool bKeyDown = false;
//	static TCHAR str[str_size];
	static int count = 0, pre;
	static int pos_x = 0, pos_y = 0;
	static SIZE size;
	static int direction_Key = 0;
	static int Velocity = 1;
	static int R, G, B;
	static int Click = 0;
	static POINT p;
	static std::vector<CShape *> Shape_List;
	static CShape *c;
	static int flag = 1;

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
			case ID_FILEOPEN:
			{
				memset(&OFN, 0, sizeof(OPENFILENAME));
				OFN.lStructSize = sizeof(OPENFILENAME);
				OFN.hwndOwner = hWnd;
				OFN.lpstrFilter = filter;
				OFN.lpstrFile = lpstrFile;
				OFN.nMaxFile = 256;
				OFN.lpstrInitialDir = _T(".");
				if (GetOpenFileName(&OFN) != 0)
				{
					_stprintf_s(str, _T("%s 파일을 열겠습니까?"), OFN.lpstrFile);
					int n = MessageBox(hWnd, str, _T("열기 선택"), MB_YESNO);
					if(n == IDYES)
						OuFromFile(OFN.lpstrFile, hWnd);
				}
			}
				break;
			case ID_COLORDLG:
			{
				for (i = 0; i < 16; i++)
					tmp[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
				memset(&COLOR, 0, sizeof(CHOOSECOLOR));
				COLOR.lStructSize = sizeof(CHOOSECOLOR);
				COLOR.hwndOwner = hWnd;
				COLOR.lpCustColors = tmp;
				COLOR.Flags = CC_FULLOPEN;
				if (ChooseColor(&COLOR) != 0)
				{
					color = COLOR.rgbResult;
					InvalidateRect(hWnd, NULL, true);

				}
			}
			break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case Func_1:
			{
				int ans = MessageBox(hWnd, _T("기본 작동 기능1 선택했습니다."), _T("기능 선택"), MB_YESNOCANCEL);
			switch (ans)
			{
			case IDYES:
				MessageBox(hWnd, _T("YES버튼 선택"), _T("확인"), MB_OK);
				break;
			case IDNO:
				MessageBox(hWnd, _T("NO버튼 선택"), _T("확인"), MB_OK);
				break;
			case IDCANCEL:
				MessageBox(hWnd, _T("CANCEL버튼 선택"), _T("확인"), MB_OK);
				break;
			}
			}
				break;
			case Func_2:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case Func_3:
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
			GetClientRect(hWnd, &rectview);
			pos_x = 1200;
			pos_y = 600;
			int breakPoint = 999;
			SetTimer(hWnd, 1, 1, NULL);

//			CreateCaret(hWnd, NULL, 15, 15);
//			ShowCaret(hWnd);
		}
		break;
	case WM_TIMER:
	{
		int size = Shape_List.size();
		for (int i = 0; i < Shape_List.size(); i++)
		{
			for (int j = i + 1; j < Shape_List.size(); j++)
			{
				Shape_List[i]->Collision(Shape_List, i, j);
			}
		}
		for (int i = 0; i < Shape_List.size(); i++)
		{
			if (Shape_List[i]->_delete)
			{
				delete Shape_List[i];
				Shape_List.erase(Shape_List.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < size; i++)
		{
			Shape_List[i]->Update(rectview, flag);
			Shape_List[i]->Radian += M_PI / 180;
			if (Shape_List[i]->Radian >= 2 * M_PI)
				Shape_List[i]->Radian = 0;
		}
		

		InvalidateRect(hWnd, NULL, true);
	}
	break;
	case WM_SIZE:
	{
		int breakPoint = 999;
	}
	break;
	case WM_KEYDOWN:
		{

		if (wParam == '1')
			flag = 1;
		else if (wParam == '2')
			flag = 2;
		else if (wParam == '3')
			flag = 3;

		/*if (wParam == VK_UP)
		{
			if(rectview.top < pos_y - 100 - Velocity)
				pos_y -= Velocity;
			bKeyDown = true;
		}
		else if (wParam == VK_LEFT)
		{
			if (rectview.left < pos_x - 100 - Velocity)
				pos_x -= Velocity;
			bKeyDown = true;

		}
		else if (wParam == VK_RIGHT)
		{
			if (rectview.right > pos_x + 100 + Velocity)
				pos_x += Velocity;
			bKeyDown = true;

		}
		else if (wParam == VK_DOWN)
		{
			if (rectview.bottom > pos_y + 100 + Velocity)
				pos_y += Velocity;
			bKeyDown = true;

		}*/

	

//		if (wParam == VK_RIGHT) SetTimer(hWnd, 1, 70, NULL);


		//InvalidateRect(hWnd, NULL, true);

		}
		break;
	case WM_CHAR:
		{
			


			
		}
		break;
	case WM_KEYUP:
	{
		bKeyDown = false;
		Velocity = 1;
		/*for(int i = pre; i < count; i++)
			str[i] = ' ';*/
		direction_Key = 0;
		InvalidateRect(hWnd, NULL, true);
	}
		break;
	case WM_LBUTTONDOWN:
	{
		Click = 1;
		Point p;
		p.x = LOWORD(lParam);
		p.y = HIWORD(lParam);
//		c = new CCircle(p, 100, 3, M_PI / 180.0 * (FLOAT(rand() % 360)), CShape::Circle);
		int n = rand() % 2 + 1;
//		static int n = 2;
//		static int i = 11;
		int _size = 100;
//		n %= 2;
//		n++;
		switch (n)
		{
		case 1:
			c = new CCircle(p, rand() % 2 * +1, rand() % 2 + 1, _size / 2, CShape::Circle);
//			c = new CCircle(p, 0,i,50 , CShape::Circle);
//			i -= 10;
			break;
		case 2:
			c = new CRectangle(p, rand() % 2 + 1, rand() % 2 + 1, _size, _size, CShape::Rectangle);
			break;
		case 3:
			c = new CStar(p, rand() % 2 + 1, rand() % 2 + 1, _size / 2, CShape::Star);
			break;
		default:
			break;
		}
		Shape_List.push_back(c);
		InvalidateRect(hWnd, NULL, true);
	}
	break;
	case WM_RBUTTONDOWN:
	{


		InvalidateRect(hWnd, NULL, true);
	}
	break;
	case WM_RBUTTONUP:
		Click = 0;
		break;
	case WM_LBUTTONUP:
		Click = 0;
		break;
	case WM_MOUSEMOVE:
	{
		//if (Click > 0)
		//{
		//	pos_x = LOWORD(lParam);
		//	pos_y = HIWORD(lParam);
		//	InvalidateRect(hWnd, NULL, true);
		//}
	}
	break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
			//대화상자 샊깔
			hBrush = CreateSolidBrush(color);
			OldBrush = (HBRUSH)SelectObject(hdc, hBrush);
//			Ellipse(hdc, 10, 10, 200, 200);
			SelectObject(hdc, OldBrush);
			DeleteObject(hBrush);




//			HBRUSH brush, oldbrush;
			int i = 0;
			//for (auto itr = Shape_List.begin(); itr != Shape_List.end(); itr++)
			//	(*itr)->Show(hdc);
			
			TCHAR t[10] = { 0 };
			TCHAR(flag) + '0';
			_itot_s(flag, t, 10);

			for (int i = 0; i < Shape_List.size(); i++)
				Shape_List[i]->Show(hdc);
			TextOut(hdc, 100, 100, t, _tcslen(t));
			/*if(bKeyDown)
			{
				R = (rand() + rand()) % 256; G = (rand() + rand()) % 256; B = (rand() + rand()) % 256;

				brush = CreateSolidBrush(RGB(R + G, G, B));
				oldbrush = (HBRUSH)SelectObject(hdc, brush);
				DrawCircle(hdc, pos_x, pos_y, 100);
				SelectObject(hdc, oldbrush);
				DeleteObject(brush);
			}
			else
			{
				brush = (HBRUSH)GetStockObject(NULL_BRUSH);
				oldbrush = (HBRUSH)SelectObject(hdc, brush);
				DrawCircle(hdc, pos_x, pos_y, 100);
				SelectObject(hdc, oldbrush);
				DeleteObject(brush);
			}
*/
			//brush = (HBRUSH)GetStockObject(NULL_BRUSH);

			EndPaint(hWnd, &ps);
			

        }
        break;
    case WM_DESTROY:
		KillTimer(hWnd, 1);
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