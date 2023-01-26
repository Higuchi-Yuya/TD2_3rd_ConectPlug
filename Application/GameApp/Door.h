#pragma once
#include "Collider.h"
#include "Object3d.h"

class Plug;

class Door
{
public:
	Door();
	~Door();
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw(ViewProjection* viewProjection);
	//collider��getter
	Collider* GetCollider() { return collider_; };
	//plug��setter
	void SetPlug(Plug*plug);
	//collider��setter
	void SetCollider(Collider* collider);

	// ���Z�b�g
	void Reset();

private:
	Model* doorModel = nullptr;
	//�Q�[���I�u�W�F�N�g
	Object3d* gameObject_ = nullptr;
	//�����蔻��
	Collider* collider_ = nullptr;
	//�J���Ă��邩
	bool isOpen_ = false;

	//���a
	float radius_ = 1.0f;

	//�v���O
	std::vector<Plug*> plug_;
	int plugNum_ = 0;
};

