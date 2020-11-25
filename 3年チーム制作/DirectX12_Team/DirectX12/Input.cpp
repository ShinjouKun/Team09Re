#include "Input.h"


Input::Input()
{
}

Input::~Input()
{
}

void Input::Init(HWND hwnd)
{
	HRESULT result;
	ComPtr<IDirectInput8> dInput ;//インプット
	
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	
	//インプット初期化
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, nullptr);
	result = dInput->CreateDevice(GUID_SysKeyboard, &devKeyboard, NULL);

	//入力データ形式のセット
	result = devKeyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	//排他制御レベルのセット
	result = devKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::Update()
{
	HRESULT result;
	result = devKeyboard->Acquire();//キーボード情報のかくとく開始
	

	result = devKeyboard->Acquire();//キーボード情報のかくとく開始
	//1フレーム前のキーを更新
		for (int i = 0; i < 256; i++)
		{
			oldkey[i] = key[i];
		}

		//全キーの入力状態を獲得
		result = devKeyboard->GetDeviceState(sizeof(key), key);
}

bool Input::KeyDown(int keyNum)
{
	if (keyNum < 0x00)return false;//エラー対策
	if (keyNum > 0xff)return false;//エラー対策
	if (!oldkey[keyNum] && key[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::KeyState(int keyNum)
{
	if (keyNum < 0x00)return false;//エラー対策
	if (keyNum > 0xff)return false;//エラー対策
	if (key[keyNum])
	{
		return true;
	}
	return false;
}

ComPtr<IDirectInputDevice8> Input::devKeyboard = nullptr;
BYTE Input::key[256] = { 0 };
BYTE Input::oldkey[256] = { 0 };//前の状態
