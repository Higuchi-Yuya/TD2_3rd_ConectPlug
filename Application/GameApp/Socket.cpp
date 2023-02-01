#include "Socket.h"
#include "MathFunc.h"
Socket::Socket() {
	gameObject_ = Object3d::Create();

	soketModel = new Model();
	soketModel = Model::LoadFromOBJ("socket");
	collider_ = new Collider;
}

Socket::~Socket() {
	delete soketModel;
	delete gameObject_;
	delete collider_;

}

void Socket::Initialize(Vector3 pos , int face) {

	face_ = face;

	gameObject_->SetModel(soketModel);

	gameObject_->worldTransform_.position_ = pos;

	gameObject_->worldTransform_.rotation_ = {
		0 ,
		MathFunc::Utility::Deg2Rad(90) * face_ ,
		0
	};

	gameObject_->Update();

	
	collider_->Initialize(&gameObject_->worldTransform_);
	collider_->SetRadius(2.0f);
}

void Socket::Update() {
	gameObject_->Update();
	collider_->Update();
}

void Socket::Draw(ViewProjection* viewProjection) {
	gameObject_->Draw(viewProjection);
}

// ���Z�b�g�֐�
void Socket::Reset(Vector3 pos, int face)
{
	face_ = face;
	// �\�P�b�g�̃|�W�V�����̃Z�b�g
	gameObject_->worldTransform_.position_ = pos;

	// �\�P�b�g�̌����̃Z�b�g
	gameObject_->worldTransform_.rotation_ = {
		0 ,
		MathFunc::Utility::Deg2Rad(90) * face_ ,
		0
	};

	gameObject_->Update();

	// �R���C�_�[�Ƀ\�P�b�g�̏����Z�b�g
	collider_->Initialize(&gameObject_->worldTransform_);
	collider_->SetRadius(2.0f);
}

int Socket::GetFace() {
	return face_;
}

Collider Socket::GetCollider() {
	return *collider_;
}

WorldTransform Socket::GetWorldTransform() {
	return gameObject_->worldTransform_;
}
