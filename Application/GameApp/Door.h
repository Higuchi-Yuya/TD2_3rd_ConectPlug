#pragma once
#include "Collider.h"
#include "Object3d.h"

class Plug;
class Sound;

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
	Collider* GetClearCollider() { return clearCollider_; };
	Vector3 GetPos() { return clearPos.position_; }
	Vector3 GetRadius() { return C_Radius_; }
	//plug��setter
	void SetPlug(Plug*plug);
	//collider��setter
	void SetCollider(Collider* collider);

	// �J���Ă邩�ǂ���
	bool GetOpen() { return isOpen_; }

	// ���Z�b�g
	void Reset(int connectMax);

private:
	Model* doorModel = nullptr;
	//�Q�[���I�u�W�F�N�g
	Object3d* gameObject_ = nullptr;
	//�����蔻��
	Collider* collider_ = nullptr;

	WorldTransform clearPos;
	Collider* clearCollider_ = nullptr;
	//�J���Ă��邩
	bool connectOpen = 0;
	int connectMax = 1;
	bool isOpen_ = false;

	//���a
	float radius_ = 1.0f;
	Vector3 C_Radius_ = { 0.5f,0.5f,0.5f };

	//�v���O
	std::vector<Plug*> plug_;
	int plugNum_ = 0;

	// �h�A���J�����̌��ʉ�
	Sound* openSE = nullptr;
	bool isOpenSE = false;
};

