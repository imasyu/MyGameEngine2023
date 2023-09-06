#include "Input.h"
#include <string>

namespace Input
{

	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr; //�f�o�C�X�I�u�W�F�N�g
	BYTE keyState[256];                        //���݂̊e�L�[�̏��
	BYTE prevkeyState[256];                    //�O�t���[���ł̊e�L�[�̏��
	//�}�E�X
	LPDIRECTINPUTDEVICE8    pMouseDevice_; //�f�o�C�X�I�u�W�F�N�g
	DIMOUSESTATE mouseState_;       //�}�E�X�̏��
	DIMOUSESTATE prevMouseState_;   //�O�t���[���̃}�E�X�̏��
	XMFLOAT3 mousePosition_;        //�}�E�X�J�[�\���̈ʒu

	void Initialize(HWND hWnd)
	{
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		//�}�E�X
		pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice_, nullptr);
		pMouseDevice_->SetDataFormat(&c_dfDIMouse);
		pMouseDevice_->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE);
	}

	void Update()
	{
		memcpy(prevkeyState, keyState, sizeof(keyState));

		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);

		//�}�E�X
		pMouseDevice_->Acquire();
		memcpy(&prevMouseState_, &mouseState_, sizeof(mouseState_));
		pMouseDevice_->GetDeviceState(sizeof(mouseState_), &mouseState_);
	}

	bool IsKey(int keyCode)
	{
		if (keyState[keyCode] & 0x80)  //2�i��:& 128  �V�t�g���Z:&(1<<7)
		{
			return true;
		}
		return false;
	}

	//�������u��
	bool IsKeyDown(int keyCode)
	{
		// ���͉����ĂāA�O��͉����ĂȂ�
		if ((keyState[keyCode] & 0x80) && !(prevkeyState[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsMouseButton(int buttonCode)
	{
		//�����Ă�
		if (mouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonDown(int buttonCode)
	{
		//���͉����ĂāA�O��͉����ĂȂ�
		if (IsMouseButton(buttonCode) && !(prevMouseState_.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonUp(int buttonCode)
	{
		//�������ĂȂ��āA�O��͉����Ă�
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

	//�}�E�X�J�[�\���̈ʒu���Z�b�g
	void SetMousePosition(int x, int y)
	{
		mousePosition_.x = x;
		mousePosition_.y = y;
		std::string resStr = std::to_string(x) + ", " + std::to_string(y);
		OutputDebugString(resStr.c_str());;
	}

	//�������u��
	bool IsKeyUp(int keyCode)
	{
		// �������͉����ĂāA���͉����ĂȂ����
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

