#include "SkyBox.h"

SkyBox::SkyBox()
{
	skyBox_ = Object3d::Create();
	skyModel = new Model;
	skyModel = Model::LoadFromOBJ("skyBox");
}

SkyBox::~SkyBox()
{
	delete skyModel;
	delete skyBox_;
}

void SkyBox::Initialize()
{
	skyBox_->SetModel(skyModel);
	skyBox_->worldTransform_.scale_ = { 10.0f,40.0f,12.0f };
	skyBox_->worldTransform_.position_ = { 5,5,-7 };
}

void SkyBox::Update()
{
	skyBox_->Update();
}

void SkyBox::Draw(ViewProjection* viewProjection)
{
	skyBox_->Draw(viewProjection);
}
