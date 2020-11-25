#include "Window.h"
const wchar_t Window::WindowClassName[] = L"ShinjouKunEngine";
LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//���b�Z�[�W�ŕ���
	switch (msg)
	{
	case WM_DESTROY://�E�B���h�E���j�󂳂ꂽ
		PostQuitMessage(0);//OS�ɑ΂��ăA�v���̏I����`����
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//�W���̏������s��
}

void Window::CreateGameWindow()
{
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = (WNDPROC)WindowProc;//�E�B���h�E�v���V�[�W���쐬
	wndClass.lpszClassName =WindowClassName;//�E�B���h�E�N���X��
	wndClass.hInstance = GetModuleHandle(nullptr);//�E�B���h�E�n���h��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);//�J�[�\���w��

	//�E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&wndClass);
	//�E�B���h�E�T�C�Y[X���W,Y���W,����,����]
	RECT wrc = { 0,0,Window_Width,Window_Height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//�����ŃT�C�Y�␳

	//�E�B���h�E�I�u�W�F�N�g�̍쐬
	hwnd = CreateWindow(wndClass.lpszClassName,//�N���X��
		WindowClassName,//�^�C�g���o�[����
		WS_OVERLAPPEDWINDOW,//�W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,//X���W�iOS�C���j
		CW_USEDEFAULT,//Y���W�iOS�C���j
		wrc.right - wrc.left,//�E�B���h�E����
		wrc.bottom - wrc.top,//�E�B���h�E�c��
		nullptr,//�e�E�B���h�E�n���h��
		nullptr,//���j���[�n���h��
		wndClass.hInstance,//�Ăяo���A�v���P�[�V�����n���h��
		nullptr//�I�v�V����
	);
	//�E�B���h�E�\��
	ShowWindow(hwnd, SW_SHOW);
}

void Window::DeleateGameWindow()
{
	//�E�B���h�E�N���X�̓o�^����
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
}

bool Window::ProcesssMessage()
{
	MSG msg{};//���b�Z�[�W
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);//�L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg);//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}
	if (msg.message == WM_QUIT)//�I�����b�Z�[�W�������烋�[�v�𔲂���
	{
		return true;
	}

	return false;
}
