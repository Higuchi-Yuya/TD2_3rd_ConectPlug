#include "Enemy.h"
#include "Stage.h"
#include "Plug.h"
#include "Sound.h"
Enemy::Enemy()
{
	gameObject_ = Object3d::Create();
	input = new Input();
	input->Initialize();
	enemyModel = new Model();
	enemyModel = Model::LoadFromOBJ("enemy");
	collider_ = new Collider();
	enemySocket = new Socket;
}

Enemy::~Enemy()
{
	delete input;
	delete enemyModel;
	delete gameObject_;
	delete collider_;
	delete enemySocket;
	delete deathSE;
}

//初期化
void Enemy::Initialize(Vector3 pos,int face, int plusFace)
{
	face_ = face;
	plusFace_ = plusFace;

	gameObject_->SetModel(enemyModel);

	gameObject_->worldTransform_.position_ = pos;


	enemySocket->Initialize({0,0,-1}, Socket::SOUTH);
	enemySocket->SetParent(gameObject_->worldTransform_);

	collider_->Initialize(&gameObject_->worldTransform_);
	collider_->SetRadius(0.5f);

	deathSE = new Sound;
	deathSE->SoundLoadWave("Resources/Sound/DeathSE.wav");
}

//更新
void Enemy::Update()
{
	input->Update();
	//移動
	if (isEnemyAlive_ == true && isMove_ == true)
	{
		Move();
	}
	//反転
	Turn();

	//当たり判定
	collider_->Update();
	//更新
	gameObject_->Update();
	enemySocket->Update();

	//プラグがつながったら死亡フラグを切り替える
	if (0 < plugNum_) {

		for (int i = 0; i < plugNum_; i++) {
			if (plug_[i]->GetIsEnemyConnect(enemyNum)==true) {
				isEnemyAlive_ = false;

			}
		}
	}
	

	//死亡
	Dead();
}

//描画
void Enemy::Draw(ViewProjection* viewProjection)
{
	gameObject_->Draw(viewProjection);
}

//移動
void Enemy::Move()
{
	move_ = { 0,0,0 };

	if (face_ == NORTH)
	{
		move_.z = kCharacterSpeed_;
		vecMinusRadius_.z = 0;
		vecMinusRadius_.x = radius_;
		vecPlusRadius_.z = radius_;
		vecPlusRadius_.x = radius_;
		gameObject_->worldTransform_.rotation_.y = MathFunc::Utility::Deg2Rad(0);
	}
	else if (face_ == EAST)
	{
		move_.x = kCharacterSpeed_;
		vecMinusRadius_.z = radius_;
		vecMinusRadius_.x = 0;
		vecPlusRadius_.z = radius_;
		vecPlusRadius_.x = radius_;
		gameObject_->worldTransform_.rotation_.y = MathFunc::Utility::Deg2Rad(90);

	}
	else if (face_ == SOUTH)
	{
		move_.z = -kCharacterSpeed_;
		vecMinusRadius_.z = radius_;
		vecMinusRadius_.x = radius_;
		vecPlusRadius_.z = 0;
		vecPlusRadius_.x = radius_;
		gameObject_->worldTransform_.rotation_.y = MathFunc::Utility::Deg2Rad(180);
	}
	else if (face_ == WEST)
	{
		move_.x = -kCharacterSpeed_;
		vecMinusRadius_.z = radius_;
		vecMinusRadius_.x = radius_;
		vecPlusRadius_.z = radius_;
		vecPlusRadius_.x = 0;
		gameObject_->worldTransform_.rotation_.y = MathFunc::Utility::Deg2Rad(-90);
	}
	gameObject_->worldTransform_.position_ += move_;
	gameObject_->Update();
}
//反転
void Enemy::Turn()
{
	//当たり判定をとる
	collider_->Update();

	//壁に当たった時
	for (int i = 0; i < stage_->GetObjectcount(); i++)
	{
		if (collider_->boxCollision(stage_->GetWorldTransform(i).position_, gameObject_->worldTransform_.position_,
			stage_->GetRadius(), vecPlusRadius_, vecMinusRadius_))
		{
			gameObject_->worldTransform_.position_ -= move_;
			//フラグを切り替える
			isTurn_ = true;
			isMove_ = false;

			break;
		}

	}

			//コードに当たった時
	if (0 < plugNum_)
	{
		for (int i = 0; i < plugNum_; i++)
		{
			for (int j = 0; j < 50; j++)
			{
				if (plug_[i]->GetIsConnect() == true)
				{
					if (collider_->CheckCollision(*plug_[i]->GetCordCollider(j, 0)) == true)
					{
						gameObject_->worldTransform_.position_ -= move_;
						//フラグを切り替える
						isTurn_ = true;
						isMove_ = false;
						break;
					}
				}

			}
		}
	}
	if (isEnemyAlive_ == true && isMove_ == false && isTurn_ == true)
	{
		////回転させる
		//amountRotation_ = 0.05f;
		//gameObject_->worldTransform.rotation_.y = amountRotation_;
		//if (amountRotation_ >= MathFunc::Utility::Deg2Rad(90 * plusFace_))
		//{
		//	amountRotation_ = 0.0f;
			//フラグを切り替える
		isTurn_ = false;
		isMove_ = true;
		//moveを逆にする
		move_ = -move_;
		//向きを変える
		face_ += plusFace_;
		if (face_ >= 4)
		{
			face_ %= 4;
		}
		/*}*/
	}
}

void Enemy::Reset(Vector3 pos,int face, int plusFace,int enemyNum)
{
	this->enemyNum = enemyNum;
	face_ = face;
	plusFace_ = plusFace;
	gameObject_->worldTransform_.position_ = pos;
	gameObject_->worldTransform_.rotation_ = { 0,0,0 };
	collider_->Initialize(&gameObject_->worldTransform_);
	collider_->SetRadius(0.5f);

	isEnemyAlive_ = true;
	isTurn_ = false;
	isMove_ = true;
}

//死亡
void Enemy::Dead()
{
	if (isEnemyAlive_ == false)
	{
		gameObject_->worldTransform_.position_.y += 0.5f;
		gameObject_->worldTransform_.rotation_.y++;
		gameObject_->worldTransform_.rotation_.x++;

		if (isSE == true) {
			deathSE->SoundPlayWave(false, 1.0f);
			isSE = false;
		}
	}
	else {
		isSE = true;
	}
}

//stageのsetter
void Enemy::SetStage(Stage* stage)
{
	stage_ = stage;
}