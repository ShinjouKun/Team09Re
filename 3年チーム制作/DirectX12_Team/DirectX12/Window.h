#pragma once
#include<Windows.h>

class Window
{
public:
	static const int Window_Width = 1280;
	static const int Window_Height = 720;
	static const wchar_t WindowClassName[];
public:
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);//ウィンドウプロシージャ
	void CreateGameWindow();//Window作成
	void DeleateGameWindow();//Window破棄
	bool ProcesssMessage();//メッセージ（ｍｓｇ）の処理
	HWND GetHWND() { return hwnd; }//ウィンドウハンドルの獲得
	HINSTANCE GetInstance() { return wndClass.hInstance; }//インスタンス獲得
private:
	HWND hwnd = nullptr;//ウィンドウハンドル
	WNDCLASSEX wndClass{};
};
