#pragma once
#include<Windows.h>

class Window
{
public:
	static const int Window_Width = 1280;
	static const int Window_Height = 720;
	static const wchar_t WindowClassName[];
public:
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);//�E�B���h�E�v���V�[�W��
	void CreateGameWindow();//Window�쐬
	void DeleateGameWindow();//Window�j��
	bool ProcesssMessage();//���b�Z�[�W�i�������j�̏���
	HWND GetHWND() { return hwnd; }//�E�B���h�E�n���h���̊l��
	HINSTANCE GetInstance() { return wndClass.hInstance; }//�C���X�^���X�l��
private:
	HWND hwnd = nullptr;//�E�B���h�E�n���h��
	WNDCLASSEX wndClass{};
};
