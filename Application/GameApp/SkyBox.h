#pragma once
#include "Object3d.h"

class SkyBox
{
public:
	// namespace�̏ȗ�
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public:// �����o�֐�
	SkyBox();
	~SkyBox();

	// ������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw(ViewProjection* viewProjection);

private:// �����o�ϐ�
	Model* skyModel = nullptr;

	// �I�u�W�F�N�g
	Object3d* skyBox_ = nullptr;
	
};

