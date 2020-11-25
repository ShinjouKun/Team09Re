#pragma once
#include"Windows.h"
#include"dinput.h"//�C���v�b�g�n
#include"wrl.h"

#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��

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


	static bool InitGamepad(HWND hwnd);

	static void UpdateGamepad();


	/// <summary>
/// �{�^�������`�F�b�N
/// </summary>
/// <param name="buttonNumber">�X�e�B�b�N�{�^���ԍ�</param>
/// <returns>������Ă��邩</returns>
	static bool PushButton(BYTE buttonNumber);

	/// <summary>
	/// �{�^�������`�F�b�N
	/// </summary>
	/// <param name="buttonNumber">�X�e�B�b�N�{�^���ԍ�</param>
	/// <returns>������Ă��邩</returns>
	static bool TriggerButton(BYTE buttonNumber);

	/// <summary>
	/// ���X�e�B�b�N�̌X�����`�F�b�N
	/// </summary>
	/// <param name="stickNumber">���X�e�B�b�N�������</param>
	/// <returns>�X���Ă��邩</returns>
	static bool LeftStickTilt(LONG stickNumber);


	/// <summary>
	/// �\���{�^�����`�F�b�N
	/// </summary>
	/// <param name="buttonData">�\���{�^�����</param>
	/// <returns>������Ă��邩</returns>
	static bool CrossButton(LONG buttonData);


private:

	static ComPtr<IDirectInputDevice8> devKeyboard;
	//static ComPtr<IDirectInputDevice8> devgamepad;

	static BYTE key[256];
	static BYTE oldkey[256];//�O�̏��


	static BYTE buttonPre[32];

	static DIPROPDWORD diprop; //�����[�h�ݒ�
	static DIPROPRANGE diprg; //���̒l�͈͐ݒ�


public:

	static DIJOYSTATE pad_data;

};


/*�@�Q�[���p�b�h�R�[�h�@*/
#define LSTICK_UP     0x00000001l //(pad_data.IY < 0)
#define LSTICK_DOWN   0x00000002l //(pad_data.IY > 0)
#define LSTICK_LEFT   0x00000004l //(pad_data.IX < 0)
#define LSTICK_RIGHT  0x00000008l //(pad_data.IX > 0)

#define RSTICK_UP     0x00000010l //(pad_data.IZ < 0)
#define RSTICK_DOWN   0x00000020l //(pad_data.IZ > 0)
#define RSTICK_LEFT   0x00000040l //(pad_data.IRz < 0)
#define RSTICK_RIGHT  0x00000080l //(pad_data.IRz > 0)

#define BUTTON_UP     0x00000001l //(pad_data.rgdwPOV.IY < 0)
#define BUTTON_DOWN   0x00000002l //(pad_data.rgdwPOV.IY > 0)
#define BUTTON_LEFT   0x00000004l //(pad_data.rgdwPOV.IX < 0)
#define BUTTON_RIGHT  0x00000008l //(pad_data.rgdwPOV.IX > 0)

#define BUTTON_X     (0) //(pad_data.rgbButtons[0])
#define BUTTON_Y     (1) //(pad_data.rgbButtons[1])
#define BUTTON_A     (2) //(pad_data.rgbButtons[2])
#define BUTTON_B     (3) //(pad_data.rgbButtons[3])
#define BUTTON_L1    (4) //(pad_data.rgbButtons[4])
#define BUTTON_R1    (5) //(pad_data.rgbButtons[5])
#define BUTTON_L2    (6) //(pad_data.rgbButtons[6])
#define BUTTON_R2    (7) //(pad_data.rgbButtons[7])
#define BUTTON_L3    (8) //(pad_data.rgbButtons[8])
#define BUTTON_R3    (9) //(pad_data.rgbButtons[9])
#define BUTTON_BACK  (10) //(pad_data.rgbButtons[10])
#define BUTTON_START (11) //(pad_data.rgbButtons[11])
