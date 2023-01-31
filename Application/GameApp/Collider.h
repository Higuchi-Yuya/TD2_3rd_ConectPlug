#pragma once
#include "WorldTransform.h"


class Collider {
public:

	//�R���X�g���N�^
	Collider();

	//�f�X�g���N�^
	~Collider();

	//�����o�֐�
	void Initialize(WorldTransform* parent);

	void Update();

	//�����蔻����`�F�b�N(�S��)
	bool CheckCollision(Collider collider);

	//�ώl�p�`�̓����蔻��
	bool boxCollision(Vector3 a, Vector3 b, Vector3 aRadius, Vector3 bPlusRadius,Vector3 bMinusRadius);

	//�ώl�p�`�̓����蔻��
	bool boxCollision(Vector3 a, Vector3 b, Vector3 aRadius, Vector3 bRadius);

	//�A�N�Z�b�T
	void SetRadius(float radius);
	Vector3 GetWorldPos();

	//�����o�ϐ�
private:
	WorldTransform worldTransform_;
	float radius_ = 1.0f;
};