#pragma once
#include "Object3d.h"
#include "Collider.h"
#include "Input.h"
#include "MathFunc.h"
class Stage;

class Enemy
{
public:
	//�G�̌���
	enum face
	{
		NORTH, //��
		EAST,  //�E
		SOUTH,  //��
		WEST  //��
	};
public:
	Enemy();
	~Enemy();
	//������
	void Initialize(int face,int plusFace);
	//�X�V
	void Update();
	//�`��
	void Draw(ViewProjection* viewProjection);
	//�ړ�
	void Move();
	//���S
	void Dead();
	//stage��setter
	void SetStage(Stage* stage);
	//��]
	void Turn();
	// ���Z�b�g
	void Reset(int face, int plusFace);

private:
	Model* enemyModel = nullptr;

	//�I�u�W�F�N�g
	Object3d* gameObject_ = nullptr;

	//�����蔻��
	Collider* collider_ = nullptr;

	//�X�e�[�W
	Stage* stage_ = nullptr;

	//�L�[����
	Input* input = nullptr;

	//�ړ�
	Vector3 move_ = { 0,0,0 };

	//����
	int face_ = NORTH;

	//��]��
	float amountRotation_ = 0;

	//���a
	float radius_ = 1.0f;
	float radiusVec = 1.0f;

	//���aVector3
	Vector3 vecPlusRadius_ = { radiusVec,radiusVec,radiusVec };
	Vector3 vecMinusRadius_ = { radiusVec,radiusVec,radiusVec };

	//�����t���O
	bool isAlive_ = true;

	//��]�t���O
	bool isTurn_ = false;

	//�ړ��t���O
	bool isMove_ = true;

	//�ǂɐG�ꂽ����face��ύX����ϐ�
	int plusFace_ = 0;

	//���x
	float kCharacterSpeed_ = 0.05f;
};

