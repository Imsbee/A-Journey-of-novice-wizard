// A_novice_wizard's_journey.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "A_novice_wizard's_journey.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HDC memdc;      // 메모리 DC 값
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
POINT user = { 230, 260 };
POINT monster_pos = { 1000, 130  };
POINT userAttack = { -10000, -10000 };
RECT monster = { monster_pos.x, monster_pos.y, monster_pos.x + 200, monster_pos.y + 320 };
BOOL is_jump = false;

int hp_monster = 100;
int user_jump = user.y;
float jumpTime = 0.f;
float jumpHeight = 0;
float jumpPower = 50.f;

// 윈도우 크기
const int windows_size_width = 1280;
const int windows_size_height = 720;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
	LoadStringW(hInstance, IDC_ANOVICEWIZARDSJOURNEY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ANOVICEWIZARDSJOURNEY));

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

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ANOVICEWIZARDSJOURNEY));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ANOVICEWIZARDSJOURNEY);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

	HWND hWnd = CreateWindowW(szWindowClass, L"A novise wizard journey", WS_OVERLAPPED | WS_SYSMENU,
		0, 0, windows_size_width, windows_size_height, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void jump()
{
	if (!is_jump) return;

	jumpHeight = (jumpTime * jumpTime - jumpPower * jumpTime) / 7.f;
	jumpTime += 2.f;

	if (jumpTime > jumpPower)
	{
		jumpTime = 0;
		jumpHeight = 0;
		is_jump = false;
	}
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
	HDC hdc;
	RECT rect;

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
		SetTimer(hWnd, 10, 10, NULL);

	}
	break;
	// 방향키 입력 처리
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
			// 왼쪽 키를 눌렀을 시 캐릭터를 왼쪽 방향으로 15px씩 이동.
		case VK_LEFT:
		{
			user.x -= 10;

			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		// 오른쪽 키를 눌렀을 시 캐릭터를 왼쪽 방향으로 15px씩 이동.
		case VK_RIGHT:
		{
			user.x += 10;

			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		}
	}
	break;
	// 행동키 입력 처리
	case WM_CHAR:
	{
		if (wParam == 88 || wParam == 120)	// x  키 입력 시 처리
		{
			userAttack.x = user.x + 170;
			userAttack.y = user.y + 70;
			SetTimer(hWnd, 0, 100, NULL);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 67 || wParam == 99)		// c 키 입력 시 처리
		{
			is_jump = TRUE;
			SetTimer(hWnd, 1, 50, NULL);
		}
		else if (wParam == 81 || wParam == 113)		// q 키 입력 시 처리
		{

		}
		else if (wParam == 87 || wParam == 119)		// w 키 입력 시 처리
		{

		}
	}
	break;
	case WM_TIMER:
	{
		if (wParam == 0)
		{
			RECT rect_userAttack = { userAttack.x, userAttack.y, userAttack.x + 50, userAttack.y + 50 };
			if (IntersectRect(&rect, &monster, &rect_userAttack))
			{
				hp_monster -= 1;
			}
			else
			{
				userAttack.x += 30;
			}
	
			InvalidateRect(hWnd, NULL, TRUE);
		}
		if (wParam == 1)
		{
			jump();
			InvalidateRect(hWnd, NULL, TRUE);
		}
		if (wParam == 2)
		{

		}
		if (wParam == 10)
		{
			
		}
		
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HBRUSH brush;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT rect;
		std::string str_monster_hp = std::to_string(hp_monster);
		std::wstring stemp = std::wstring(str_monster_hp.begin(), str_monster_hp.end());
		LPCWSTR monster_hp = stemp.c_str();
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

		// HP 바
		RECT hp = { 35, 0, 400, 30 };
		TextOut(hdc, 5, 5, L"HP : ", 4);
		brush = CreateSolidBrush(RGB(255, 0, 0));
		Rectangle(hdc, 35, 0, 400, 30);
		FillRect(hdc, &hp, brush);

		//  MP 바
		RECT mp = { 35, 45, 300, 75 };
		TextOut(hdc, 5, 50, L"MP : ", 4);
		brush = CreateSolidBrush(RGB(0, 0, 255));
		Rectangle(hdc, 35, 45, 300, 75);
		FillRect(hdc, &mp, brush);

		// 하단바
		Rectangle(hdc, 0, 500, 1264, 630);
		// 상점
		Rectangle(hdc, 40, 530, 130, 590);
		TextOut(hdc, 67, 553, L"상점", 2);
		// 스킬
		Rectangle(hdc, 150, 530, 240, 590);
		TextOut(hdc, 177, 553, L"스킬", 2);
		// 스탯
		Rectangle(hdc, 260, 530, 350, 590);
		TextOut(hdc, 287, 553, L"스탯", 2);
		// 구분선
		MoveToEx(hdc, 400, 500, NULL);
		LineTo(hdc, 400, 630);
		// 창
		Rectangle(hdc, 410, 510, 1254, 620);

		// 경험치바
		RECT exp = { 35, 635, 1255, 660 };
		brush = CreateSolidBrush(RGB(0, 255, 100));
		Rectangle(hdc, 35, 635, 1255, 660);
		FillRect(hdc, &exp, brush);
		TextOut(hdc, 5, 640, L"exp", 3);

		// 유저
		Rectangle(hdc, user.x, user.y + jumpHeight, user.x + 150, user.y + 200 + jumpHeight);
		TextOut(hdc, user.x + 60, user.y + 95, L"유저", 2);

		// 유저 공격
		Rectangle(hdc, userAttack.x, userAttack.y, userAttack.x + 50, userAttack.y + 50);

		// 몬스터
		Rectangle(hdc, monster_pos.x, monster_pos.y, monster_pos.x + 200, monster_pos.y + 320);
		TextOut(hdc, 1075, 270, L"몬스터", 3);

		// 몬스터 체력바
		TextOut(hdc, 1090, 100, monster_hp, wcslen(monster_hp));

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
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
