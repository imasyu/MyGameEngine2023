#include "Input.h"
#include <string>

namespace Input
{

	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr; //デバイスオブジェクト
	BYTE keyState[256];                        //現在の各キーの状態
	BYTE prevkeyState[256];                    //前フレームでの各キーの状態
	//マウス
	LPDIRECTINPUTDEVICE8    pMouseDevice_; //デバイスオブジェクト
	DIMOUSESTATE mouseState_;       //マウスの状態
	DIMOUSESTATE prevMouseState_;   //前フレームのマウスの状態
	XMFLOAT3 mousePosition_;        //マウスカーソルの位置

	void Initialize(HWND hWnd)
	{
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		//マウス
		pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice_, nullptr);
		pMouseDevice_->SetDataFormat(&c_dfDIMouse);
		pMouseDevice_->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE);
	}

	void Update()
	{
		memcpy(prevkeyState, keyState, sizeof(keyState));

		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);

		//マウス
		pMouseDevice_->Acquire();
		memcpy(&prevMouseState_, &mouseState_, sizeof(mouseState_));
		pMouseDevice_->GetDeviceState(sizeof(mouseState_), &mouseState_);
	}

	bool IsKey(int keyCode)
	{
		if (keyState[keyCode] & 0x80)  //2進数:& 128  シフト演算:&(1<<7)
		{
			return true;
		}
		return false;
	}

	//押した瞬間
	bool IsKeyDown(int keyCode)
	{
		// 今は押してて、前回は押してない
		if ((keyState[keyCode] & 0x80) && !(prevkeyState[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsMouseButton(int buttonCode)
	{
		//押してる
		if (mouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonDown(int buttonCode)
	{
		//今は押してて、前回は押してない
		if (IsMouseButton(buttonCode) && !(prevMouseState_.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonUp(int buttonCode)
	{
		//今押してなくて、前回は押してる
		if (!IsMouseButton(buttonCode) && (prevMouseState_.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	XMFLOAT3 GetMousePosition()
	{
		return mousePosition_;
	}

	XMFLOAT3 GetMouseMove()
	{
		XMFLOAT3 result = XMFLOAT3((float)mouseState_.lX,
			(float)mouseState_.lY,
			(float)mouseState_.lZ);
		return result;
	}

	//マウスカーソルの位置をセット
	void SetMousePosition(int x, int y)
	{
		mousePosition_.x = x;
		mousePosition_.y = y;
		std::string resStr = std::to_string(x) + ", " + std::to_string(y);
		OutputDebugString(resStr.c_str());;
	}

	//離した瞬間
	bool IsKeyUp(int keyCode)
	{
		// さっきは押してて、今は押してない状態
		if (!(keyState[keyCode] & 0x80) && (prevkeyState[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	void Release()
	{
		SAFE_RELEASE(pDInput);
	}
	
}

