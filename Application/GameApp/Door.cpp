#include "Door.h"
#include "Plug.h"


Door::Door()
{
	gameObject_ = Object3d::Create();
	doorModel = new Model();
	doorModel = Model::LoadFromOBJ("cube");
	collider_ = new Collider();
	clearCollider_ = new Collider;
}

Door::~Door()
{
	delete doorModel;
	delete gameObject_;
	delete collider_;
	delete clearCollider_;
}

void Door::Initialize()
{
	gameObject_->SetModel(doorModel);
	gameObject_->worldTransform_.scale_ = { 2,2,1 };
	gameObject_->worldTransform_.position_ = { 1,0,5 };

	clearPos = gameObject_->worldTransform_;

	collider_->Initialize(&gameObject_->worldTransform_);
	collider_->SetRadius(radius_);

	clearCollider_->Initialize(&clearPos);
	clearCollider_->SetRadius(0.01f);

	isOpen_ = false;
}

void Door::Update()
{
	gameObject_->Update();
	collider_->Update();
	clearCollider_->Update();
	//プラグがつながっていたら開く
	if (plug_[0]->GetIsConnect() == true)
	{
		isOpen_ = true;

		gameObject_->worldTransform_.position_.y += 0.25f;
		if (gameObject_->worldTransform_.position_.y >= 4)
		{
			gameObject_->worldTransform_.position_.y = 4;
		}
	}
	else
	{
		isOpen_ = false;
		gameObject_->worldTransform_.position_.y -= 0.25f;
		if (gameObject_->worldTransform_.position_.y <= 0)
		{
			gameObject_->worldTransform_.position_.y = 0;
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

void Door::Reset()
{
	gameObject_->worldTransform_.scale_ = { 2,2,1 };
	gameObject_->worldTransform_.position_ = { 1,0,5 };

	collider_->Initialize(&gameObject_->worldTransform_);
	collider_->SetRadius(radius_);

	clearCollider_->Initialize(&clearPos);
	clearCollider_->SetRadius(0.01f);

	isOpen_ = false;
}
