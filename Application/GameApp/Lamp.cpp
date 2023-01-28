#include "Lamp.h"

Lamp::Lamp()
{
	lampObj = Object3d::Create();
	lampModel = new Model();
	lampModel = Model::LoadFromOBJ("sphere",true);
}

Lamp::~Lamp()
{
	delete lampModel;
	delete lampObj;
}

void Lamp::Initialize()
{
	// �I�u�W�F�N�g�̏�����
	lampObj->SetModel(lampModel);
	lampObj->worldTransform_.position_ = { 4,2,2 };
	lampObj->worldTransform_.color_ = { 0.2f,0.2,0.2f,0.8f };

	isShining = false;

}

void Lamp::Update()
{
	// �����Ă�����J���[�𖾂邭����
	if (isShining == true) {
		// ���X�ɖ��邭����
		r += colorConst;
		g += colorConst;
		b += colorConst;
		// ���
		lampObj->worldTransform_.color_ = { r,g,b,0.8f };
		// ���̐F���𒴂��Ȃ��悤��
		if (r >= 1.0f) {
			r = 1.0f;
			g = 1.0f;
			b = 1.0f;
		}
	}
	// ��������Ȃ���ΈÂ�����
	else if (isShining == false) {
		// ���X�ɈÂ�����
		r -= colorConst;
		g -= colorConst;
		b -= colorConst;
		// ���
		lampObj->worldTransform_.color_ = { r,g,b,0.8f };
		// �K��l�̐F���𒴂��Ȃ��悤��
		if (r <= 0.2f) {
			r = 0.2f;
			g = 0.2f;
			b = 0.2f;
		}
	}
	lampObj->Update();
}

void Lamp::Draw(ViewProjection* viewProjection)
{
	lampObj->Draw(viewProjection);
}

void Lamp::Reset()
{
	lampObj->worldTransform_.position_ = { 4,2,2 };
	lampObj->worldTransform_.color_ = { 0.2f,0.2,0.2f,0.8f };
	r = 0.2f, g = 0.2f, b = 0.2f;
	lampObj->worldTransform_.UpdateMatrix();
	isShining = false;
}
