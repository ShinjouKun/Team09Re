#include "Input.h"
#include <cassert>

ComPtr<IDirectInputDevice8> Input::devKeyboard = nullptr;
BYTE Input::key[256] = { 0 };
BYTE Input::oldkey[256] = { 0 };//�O�̏��

ComPtr<IDirectInputDevice8>devgamepad = nullptr;
ComPtr<IDirectInput8> dInput = nullptr;//�C���v�b�g
DIJOYSTATE Input::pad_data;
BYTE Input::buttonPre[32] = { 0 };
DIPROPDWORD Input::diprop;
DIPROPRANGE Input::diprg;

Input::Input()
{
}

Input::~Input()
{
}

void Input::Init(HWND hwnd)
{
	HRESULT result;

	HINSTANCE hInstance = GetModuleHandle(nullptr);

	//�C���v�b�g������
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&dInput, nullptr);
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


					/*�@�Q�[���p�b�h�@*/

BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	HRESULT result;

	result = dInput->CreateDevice(ipddi->guidInstance,
		&devgamepad, NULL);
	if (FAILED(result)) {

		return DIENUM_CONTINUE;
	}


	return DIENUM_STOP;
}


bool Input::InitGamepad(HWND hwnd)
{
	HRESULT result;


	/* �Q�[���p�b�h */
	dInput->EnumDevices(DI8DEVCLASS_GAMECTRL,
		DeviceFindCallBack, NULL, DIEDFL_ATTACHEDONLY);
	if (!devgamepad) {
		return false;
	}

	result = devgamepad->SetDataFormat(&c_dfDIJoystick); // �W���`��
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//�@�r������
	result = devgamepad->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//�@�����[�h�ݒ�
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;

	result = devgamepad->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}



	//�@���l�͈͐ݒ�
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = -1000;
	diprg.lMax = 1000;

	//�@X���̒l�͈͐ݒ�
	diprg.diph.dwObj = DIJOFS_X;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}
	//�@Y���̒l�͈͐ݒ�
	diprg.diph.dwObj = DIJOFS_Y;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}
	//�@Z���̒l�͈͐ݒ�
	diprg.diph.dwObj = DIJOFS_Z;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//�@Z���̒l�͈͐ݒ�
	diprg.diph.dwObj = DIJOFS_RZ;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	result = devgamepad->Acquire();

	return true;
}

void Input::UpdateGamepad()
{
	HRESULT result;
	memcpy(buttonPre, pad_data.rgbButtons, sizeof(pad_data.rgbButtons));

	if (devgamepad) {
		devgamepad->Poll();
		result = devgamepad->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);
		if (result == DIERR_INPUTLOST) {
			result = devgamepad->Acquire();

		}
	}

}

bool Input::PushButton(BYTE buttonNumber)
{
	// �ُ�Ȉ��������o
	//assert(0 <= buttonNumber && buttonNumber <= 32);

	// 0�łȂ���Ή����Ă���
	if (pad_data.rgbButtons[buttonNumber]) {
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerButton(BYTE buttonNumber)
{
	// �ُ�Ȉ��������o
	//assert(0 <= buttonNumber && buttonNumber <= 32);

	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!buttonPre[buttonNumber] && pad_data.rgbButtons[buttonNumber]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::LeftStickTilt(LONG stickNumber)
{
	//�@������

	//�@�������͈�
	DWORD unresponsive_range = 0;

	switch (stickNumber)
	{
	case LSTICK_UP:
		
		return pad_data.lY < unresponsive_range;
	case LSTICK_DOWN:

		return pad_data.lY < unresponsive_range;
	case LSTICK_RIGHT:

		return pad_data.lX > unresponsive_range;
	case LSTICK_LEFT:

		return pad_data.lX < unresponsive_range;
		
	}



	return false;
}

bool Input::CrossButton(LONG buttonData)
{
	//�@������

	if (pad_data.rgdwPOV[0] != buttonData)
	{
		/*switch (pad_data.rgdwPOV[0])
		{
		case 0:
			break;
		}*/
	}


	return false;
}



