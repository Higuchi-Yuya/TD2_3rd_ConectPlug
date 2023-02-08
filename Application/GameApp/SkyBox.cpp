#include "SkyBox.h"

SkyBox::SkyBox()
{
	skyBox_ = Object3d::Create();
	skyBox2_ = Object3d::Create();
	skyModel = new Model;
	skyModel = Model::LoadFromOBJ("skyBox");

}

SkyBox::~SkyBox()
{
	delete skyModel;
	delete skyBox2_;
	delete skyBox_;
}

void SkyBox::Initialize()
{
	skyBox_->SetModel(skyModel);
	skyBox_->worldTransform_.scale_ = { 20.0f,40.0f,20.0f };
	skyBox_->worldTransform_.position_ = { 5,10,-7 };

	skyBox2_->SetModel(skyModel);
	skyBox2_->worldTransform_.scale_ = { 22,1,22 };
	skyBox2_->worldTransform_.position_ = { 5,-15,-7 };
	skyBox2_->worldTransform_.color_ = { 0.5f,0.5f,0.5f,1.0f };
	skyBox2_->Update();
}

void SkyBox::Update()
{
	skyBox_->Update();

}

void SkyBox::Draw(ViewProjection* viewProjection)
{
	skyBox_->Draw(viewProjection);
	skyBox2_->Draw(viewProjection);
}
