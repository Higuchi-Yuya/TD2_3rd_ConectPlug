#include "Door.h"
#include "Plug.h"
#include "Sound.h"

Door::Door()
{
	gameObject_ = Object3d::Create();
	doorModel = new Model();
	doorModel = Model::LoadFromOBJ("door");
	collider_ = new Collider();
	clearCollider_ = new Collider;
}

Door::~Door()
{
	delete doorModel;
	delete gameObject_;
	delete collider_;
	delete clearCollider_;

	delete openSE;
}

void Door::Initialize()
{
	gameObject_->SetModel(doorModel);
	gameObject_->worldTransform_.scale_ = { 2,1.8f,1 };
	gameObject_->worldTransform_.position_ = { 1,0.8f,4 };
	gameObject_->worldTransform_.color_ = { 0.5f,0.5f, 0.5f, 1 };

	clearPos = gameObject_->worldTransform_;
	clearPos.position_ = { 1,0,4 };

	collider_->Initialize(&gameObject_->worldTransform_);
	collider_->SetRadius(radius_);

	clearCollider_->Initialize(&clearPos);
	clearCollider_->SetRadius(0.01f);

	isOpen_ = false;

	openSE = new Sound;
	openSE->SoundLoadWave("Resources/Sound/DoorOpenSE1.wav");
}

void Door::Update()
{
	gameObject_->Update();
	collider_->Update();
	clearCollider_->Update();
	//ƒvƒ‰ƒO‚ª‚Â‚È‚ª‚Á‚Ä‚¢‚½‚çŠJ‚­
	for (int i = 0; i < plugNum_; i++) {
		if (connectMax == 1) {
			if (plug_[0]->GetIsConnect() == true)
			{
				connectOpen = true;
			}
			else
			{
				connectOpen = false;
			}
		}
		else if(connectMax==2){
			if (plug_[0]->GetIsConnect() == true&& plug_[1]->GetIsConnect() == true)
			{
				connectOpen = true;
			}
			else
			{
				connectOpen = false;
			}
		}
		else if (connectMax ==3) {
			if (plug_[0]->GetIsConnect() == true && 
				plug_[1]->GetIsConnect() == true&&
				plug_[3]->GetIsConnect() == true)
			{
				connectOpen = true;
			}
			else
			{
				connectOpen = false;
			}
		}
	}

	if (connectOpen == true)
	{
		isOpen_ = true;
		if (isOpenSE == true) {

			openSE->SoundPlayWave(false, 1.0f);
			isOpenSE = false;
		}

		gameObject_->worldTransform_.position_.y += 0.25f;
		if (gameObject_->worldTransform_.position_.y >= 4)
		{
			gameObject_->worldTransform_.position_.y = 4;
		}
	}
	else
	{
		isOpen_ = false;
		isOpenSE = true;
		gameObject_->worldTransform_.position_.y -= 0.25f;
		if (gameObject_->worldTransform_.position_.y <= 0.8)
		{
			gameObject_->worldTransform_.position_.y = 0.8;
		}
	}
}

void Door::Draw(ViewProjection* viewProjection)
{
	
	gameObject_->Draw(viewProjection);

}

void Door::SetPlug(Plug* plug) {

	plug_.resize(plugNum_ + 1);

	plug_[plugNum_] = plug;

	plugNum_++;
}


void Door::SetCollider(Collider* collider)
{
	collider_ = collider;
}

void Door::Reset(int connectMax)
{
	gameObject_->worldTransform_.scale_ = { 2,1.8f,1 };
	gameObject_->worldTransform_.position_ = { 1,0.8f,4 };
	gameObject_->worldTransform_.color_ = { 0.5f,0.5f, 0.5f, 1 };
	collider_->Initialize(&gameObject_->worldTransform_);
	collider_->SetRadius(radius_);

	clearCollider_->Initialize(&clearPos);
	clearCollider_->SetRadius(0.01f);

	this->connectMax = connectMax;
	this->connectOpen = false;
	isOpen_ = false;
}
