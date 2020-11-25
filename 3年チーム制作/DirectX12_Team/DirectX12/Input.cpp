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
	ComPtr<IDirectInput8> dInput ;//�C���v�b�g
	
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	
	//�C���v�b�g������
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, nullptr);
	result = dInput->CreateDevice(GUID_SysKeyboard, &devKeyboard, NULL);

	//���̓f�[�^�`���̃Z�b�g
	result = devKeyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	//�r�����䃌�x���̃Z�b�g
	result = devKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::Update()
{
	HRESULT result;
	result = devKeyboard->Acquire();//�L�[�{�[�h���̂����Ƃ��J�n
	

	result = devKeyboard->Acquire();//�L�[�{�[�h���̂����Ƃ��J�n
	//1�t���[���O�̃L�[���X�V
		for (int i = 0; i < 256; i++)
		{
			oldkey[i] = key[i];
		}

		//�S�L�[�̓��͏�Ԃ��l��
		result = devKeyboard->GetDeviceState(sizeof(key), key);
}

bool Input::KeyDown(int keyNum)
{
	if (keyNum < 0x00)return false;//�G���[�΍�
	if (keyNum > 0xff)return false;//�G���[�΍�
	if (!oldkey[keyNum] && key[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::KeyState(int keyNum)
{
	if (keyNum < 0x00)return false;//�G���[�΍�
	if (keyNum > 0xff)return false;//�G���[�΍�
	if (key[keyNum])
	{
		return true;
	}
	return false;
}

ComPtr<IDirectInputDevice8> Input::devKeyboard = nullptr;
BYTE Input::key[256] = { 0 };
BYTE Input::oldkey[256] = { 0 };//�O�̏��
