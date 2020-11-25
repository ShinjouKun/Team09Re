#pragma once
#include"Windows.h"
#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定
#include"dinput.h"//インプット系
#include"wrl.h"
using namespace Microsoft::WRL;
class Input
{
public:
	Input();
	~Input();
	static void Init(HWND hwnd);//初期化
	static void Update();
	static bool KeyDown(int keyNum);//今押したか？
	static bool KeyState(int keyNum);//押しっぱなしか？
private:
	static ComPtr<IDirectInputDevice8> devKeyboard;
	static BYTE key[256];
	static BYTE oldkey[256];//前の状態
};
