#pragma once
#include"Vector3.h"
#include "Vector4.h"
#include"Matrix4.h"
#include "WinApp.h"
#include<d3d12.h>
#include<wrl.h>
#include "Input.h"

class ViewProjection
{
public:// �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataViewProjection {
		Matrix4 view;       // ���[���h �� �r���[�ϊ��s��
		Matrix4 projection; // �r���[ �� �v���W�F�N�V�����ϊ��s��
		Vector3 cameraPos;  // �J�������W�i���[���h���W�j
	};
public:// �����o�֐�
	// �ÓI������
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �s����X�V����
	/// </summary>
	void UpdateMatrix();

	// �o�b�t�@�̃Q�b�^�[
	ID3D12Resource* GetBuff() { return constBuff.Get(); }

	// �f�o�b�O�J�����̏�����
	void DebugCameraInitialze(Input* input);

	// �f�o�b�O�J�����̍X�V����
	void DebugCameraUpdate();

	// �J���������_�܂ł̋����̃Z�b�g
	void SetDistance(float distance) {
		this->distance = distance;
	}

private:// �v���C�x�[�g�֐�
	// �~����
	const float PI = 3.141592f;

	/// <summary>
	/// �萔�o�b�t�@����
	/// </summary>
	void CreateConstBuffer();

	/// <summary>
	/// �}�b�s���O����
	/// </summary>
	void Map();

	// �x�����烉�W�A���ɕϊ�
	float ToRadian(float angle) { return angle * (PI / 180); }

	// �x�N�g���ɂ�鎋�_�ړ�
	void MoveEyeVector(const Vector3& move);

	// �x�N�g���ɂ��ړ�
	void MoveVector(const Vector3& move);



public:// �p�u���b�N�ϐ�
#pragma region �r���[�s��̐ݒ�
	// ���_���W
	Vector3 eye = { 0, 0, -20.0f };
	// �����_���W
	Vector3 target = { 0, 0, 0 };
	// ������x�N�g��
	Vector3 up = { 0, 1, 0 };
#pragma endregion

#pragma region �ˉe�s��̐ݒ�
	// ������������p
	float fovAngleY = ToRadian(45.0f);
	// �r���[�|�[�g�̃A�X�y�N�g��
	float aspectRatio = (float)WinApp::window_width / WinApp::window_height;
	// �[�x���E�i��O���j
	float nearZ = 0.1f;
	// �[�x���E�i�����j
	float farZ = 1000.0f;
#pragma endregion

	// �r���[�s��
	Matrix4 matView;
	// �ˉe�s��
	Matrix4 matProjection;

private:// �����o�ϐ�

	// �f�o�C�X�i�؂�Ă���j
	static Microsoft::WRL::ComPtr<ID3D12Device> device_;

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	// �}�b�s���O�ς݃A�h���X
	ConstBufferDataViewProjection* constMap = nullptr;

	// �f�o�b�O�J�����ɕK�v�ȕϐ�
#pragma region �f�o�b�O�J����
	// ���̓N���X�̃|�C���^
	Input* input;
	// �J���������_�܂ł̋���
	float distance = 3;
	// �X�P�[�����O
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	// ��]�s��
	Matrix4 matRot;
#pragma endregion

};

