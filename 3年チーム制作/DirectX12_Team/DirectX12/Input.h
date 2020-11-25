#pragma once
#include"Windows.h"
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��
#include"dinput.h"//�C���v�b�g�n
#include"wrl.h"
using namespace Microsoft::WRL;
class Input
{
public:
	Input();
	~Input();
	static void Init(HWND hwnd);//������
	static void Update();
	static bool KeyDown(int keyNum);//�����������H
	static bool KeyState(int keyNum);//�������ςȂ����H
private:
	static ComPtr<IDirectInputDevice8> devKeyboard;
	static BYTE key[256];
	static BYTE oldkey[256];//�O�̏��
};
