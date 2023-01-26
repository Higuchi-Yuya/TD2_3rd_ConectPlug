#include "Input.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include <cassert>

WinApp* Input::winApp_ = nullptr;

void Input::StaticInitialize(WinApp* winApp)
{
	// �؂�Ă���WinApp�̃C���X�^���X���L�^
	Input::winApp_ = winApp;
}

void Input::Initialize()
{
	HRESULT result;

	//DirectInput�̏�����
	result = DirectInput8Create(winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//�L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	// �}�E�X�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	assert(SUCCEEDED(result));

	// �L�[�{�[�h���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result));

	// �L�[�{�[�h�r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	// �}�E�X���̓f�[�^�`���̃Z�b�g
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // �W���`��
	assert(SUCCEEDED(result));

	// �}�E�X�r�����䃌�x���̃Z�b�g
	result = devMouse->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	// �O��̃L�[���͂�ۑ�
	memcpy(keyPre, key, sizeof(key));

	// �L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();

	// �S�L�[�̓��͏�Ԃ��擾����
	keyboard->GetDeviceState(sizeof(key), key);

	// �}�E�X����J�n
	devMouse->Acquire(); 

	// �O��̓��͂�ۑ�
	mouseStatePre = mouseState;

	// �}�E�X�̓���
	devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
}

bool Input::PushKey(BYTE keyNumber)
{
	// �w�肵���L�[�������Ă����true��Ԃ�
	if (key[keyNumber]) {
		return true;
	}

	// �����łȂ����false��Ԃ�
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// �w��L�[��O�t���[���ŉ����Ă��Ȃ��A���̃t���[���ŉ����Ă����true��Ԃ�
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}

	// �����łȂ����false��Ԃ�
	return false;
}

bool Input::ReleasedKey(BYTE keyNumber)
{
	// �w��L�[��O�t���[���ŉ����Ă��āA���̃t���[���ŉ����Ă��Ȃ����true��Ԃ�
	if (keyPre[keyNumber] && !key[keyNumber]) {
		return true;
	}

	// �����łȂ����false��Ԃ�
	return false;
}

bool Input::PushMouseLeft()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[0]) {
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::PushMouseMiddle()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[2]) {
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerMouseLeft()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::TriggerMouseMiddle()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[2] && mouseState.rgbButtons[2]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

Input::MouseMove Input::GetMouseMove()
{
	MouseMove tmp;
	tmp.lX = mouseState.lX;
	tmp.lY = mouseState.lY;
	tmp.lZ = mouseState.lZ;
	return tmp;
}
