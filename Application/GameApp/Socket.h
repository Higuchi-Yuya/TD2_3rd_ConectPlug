#pragma once
#include "Object3d.h"
#include "Collider.h"

class Socket {
public:
	//�R���X�g���N�^
	Socket();

	//�f�X�g���N�^
	~Socket();

	//�����o�֐�
	void Initialize(Vector3 pos , int face);

	void Update();

	void Draw(ViewProjection* viewProjection);

	//�A�N�Z�b�T
	int GetFace();

	Collider GetCollider();
	WorldTransform GetWorldTransform();

	//enum
	enum FACE {
		NORTH ,
		EAST ,
		SOUTH ,
		WEST ,
	};

	//�����o�ϐ�
private:
	Model* soketModel = nullptr;

	Object3d* gameObject_ = nullptr;

	Collider* collider_ = nullptr;

	int face_ = NORTH;

};

