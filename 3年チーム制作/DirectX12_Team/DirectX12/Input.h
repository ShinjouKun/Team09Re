#pragma once
#include"Windows.h"
#include"dinput.h"//インプット系
#include"wrl.h"

#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定

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


	static bool InitGamepad(HWND hwnd);

	static void UpdateGamepad();


	/// <summary>
/// ボタン押下チェック
/// </summary>
/// <param name="buttonNumber">スティックボタン番号</param>
/// <returns>押されているか</returns>
	static bool PushButton(BYTE buttonNumber);

	/// <summary>
	/// ボタン押下チェック
	/// </summary>
	/// <param name="buttonNumber">スティックボタン番号</param>
	/// <returns>押されているか</returns>
	static bool TriggerButton(BYTE buttonNumber);

	/// <summary>
	/// 左スティックの傾きをチェック
	/// </summary>
	/// <param name="stickNumber">左スティック向き情報</param>
	/// <returns>傾いているか</returns>
	static bool LeftStickTilt(LONG stickNumber);


	/// <summary>
	/// 十字ボタンをチェック
	/// </summary>
	/// <param name="buttonData">十字ボタン情報</param>
	/// <returns>押されているか</returns>
	static bool CrossButton(LONG buttonData);


private:

	static ComPtr<IDirectInputDevice8> devKeyboard;
	//static ComPtr<IDirectInputDevice8> devgamepad;

	static BYTE key[256];
	static BYTE oldkey[256];//前の状態


	static BYTE buttonPre[32];

	static DIPROPDWORD diprop; //軸モード設定
	static DIPROPRANGE diprg; //軸の値範囲設定


public:

	static DIJOYSTATE pad_data;

};


/*　ゲームパッドコード　*/
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
