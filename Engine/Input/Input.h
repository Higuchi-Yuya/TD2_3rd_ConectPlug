#pragma once
#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "WinApp.h"

// ����
class Input
{
private:
	// namespace�̏ȗ�
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	struct MouseMove {
		LONG lX;
		LONG lY;
		LONG lZ;
	};
public: // �����o�֐�
	// �ÓI������
	static void StaticInitialize(WinApp* winApp);

	// ������
	void Initialize();

	// �X�V
	void Update();

	/// <summary>
	/// �L�[�̉����������`�F�b�N(������)
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�(DIK_0 ��)</param>
	/// <returns>������Ă��邩</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// �L�[�̃g���K�[���`�F�b�N(�������u��)
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�(DIK_0 ��)</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// �L�[�̃g���K�[���`�F�b�N(�������u��)
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�(DIK_0 ��)</param>
	/// <returns>�g���K�[��</returns>
	bool ReleasedKey(BYTE keyNumber);

	/// <summary>
	/// �L�[�̍��{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool PushMouseLeft();

	/// <summary>
	/// �L�[�̒��{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool PushMouseMiddle();

	/// <summary>
	/// �L�[�̍��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseLeft();

	/// <summary>
	/// �L�[�̒��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseMiddle();

	/// <summary>
	/// �}�E�X�ړ��ʂ��擾
	/// </summary>
	/// <returns>�}�E�X�ړ���</returns>
	MouseMove GetMouseMove();

private:// �ÓI�����o�ϐ�

	// DirectInput�̃C���X�^���X
	ComPtr<IDirectInput8> directInput = nullptr;

	// �L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> keyboard = nullptr;

	// �S�L�[�̏��
	BYTE key[256] = {};

	// �O��̑S�L�[�̏��
	BYTE keyPre[256] = {};

	// �}�E�X�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devMouse;

	// �}�E�X�̏��
	DIMOUSESTATE2 mouseState = {};

	// �}�E�X�̑O�̏��
	DIMOUSESTATE2 mouseStatePre = {};

	// WindowsAPI
	static WinApp* winApp_;
};

