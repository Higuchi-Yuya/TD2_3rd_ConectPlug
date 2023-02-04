#pragma once
#include "Object3d.h"


class Lamp
{
public:
	Lamp();
	~Lamp();
	// ����������
	void Initialize(Vector3 pos);

	// �X�V����
	void Update();

	// �`�揈��
	void Draw(ViewProjection* viewProjection);

	// ����t���O
	bool GetisShining() { return isShining; }

	void SetisShining(bool isShining) { this->isShining = isShining; }

	// �ʒu
	Vector3 GetPos() { return lampObj->worldTransform_.position_; }

	void SetPosition(Vector3& position) { lampObj->worldTransform_.position_ = position; }

	// ���Z�b�g
	void Reset(Vector3 pos);

private:
	Model* lampModel = nullptr;
	// �I�u�W�F�N�g
	Object3d* lampObj = nullptr;
	// ����t���O
	bool isShining = false;

	// ���X�Ɍ��点��ɕK�v�ȃX�e�[�^�X
	// R,G,B���ꂼ��̂���i�P����O�j
	float r = 0.2f, g = 0.2f, b = 0.2f;

	// ���X�ɑ�����F�̒萔
	const float colorConst = 0.025f;
};


