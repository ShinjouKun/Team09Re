#include "Window.h"
const wchar_t Window::WindowClassName[] = L"ShinjouKunEngine";
LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY://ウィンドウが破壊された
		PostQuitMessage(0);//OSに対してアプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//標準の処理を行う
}

void Window::CreateGameWindow()
{
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = (WNDPROC)WindowProc;//ウィンドウプロシージャ作成
	wndClass.lpszClassName =WindowClassName;//ウィンドウクラス名
	wndClass.hInstance = GetModuleHandle(nullptr);//ウィンドウハンドル
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定

	//ウィンドウクラスをOSに登録
	RegisterClassEx(&wndClass);
	//ウィンドウサイズ[X座標,Y座標,横幅,立幅]
	RECT wrc = { 0,0,Window_Width,Window_Height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//自動でサイズ補正

	//ウィンドウオブジェクトの作成
	hwnd = CreateWindow(wndClass.lpszClassName,//クラス名
		WindowClassName,//タイトルバー文字
		WS_OVERLAPPEDWINDOW,//標準的なウィンドウスタイル
		CW_USEDEFAULT,//X座標（OS任せ）
		CW_USEDEFAULT,//Y座標（OS任せ）
		wrc.right - wrc.left,//ウィンドウ横幅
		wrc.bottom - wrc.top,//ウィンドウ縦幅
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		wndClass.hInstance,//呼び出しアプリケーションハンドル
		nullptr//オプション
	);
	//ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

void Window::DeleateGameWindow()
{
	//ウィンドウクラスの登録解除
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
}

bool Window::ProcesssMessage()
{
	MSG msg{};//メッセージ
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);//キー入力メッセージの処理
		DispatchMessage(&msg);//プロシージャにメッセージを送る
	}
	if (msg.message == WM_QUIT)//終了メッセージが来たらループを抜ける
	{
		return true;
	}

	return false;
}
