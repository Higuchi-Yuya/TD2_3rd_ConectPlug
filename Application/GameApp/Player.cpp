#include "Player.h"
#include "Stage.h"
#include "Plug.h"
#include "Door.h"
#include "Enemy.h"
#include "Sound.h"

Player::Player() {
	// プレイヤーの本体の初期化
	gameObject_ = Object3d::Create();
	playerModel = new Model();
	playerModel = Model::LoadFromOBJ("playerHead", true);

	// プレイヤーのおてての初期化
	playerHandObj1 = Object3d::Create();
	playerHand1Model = new Model();
	playerHand1Model = Model::LoadFromOBJ("playerHand", true);


	playerHandObj2 = Object3d::Create();


	// コライダーの初期化
	collider_ = new Collider();

	input = new Input();
	input->Initialize();
}

Player::~Player() {
	delete playerModel;
	delete playerHand1Model;
	delete playerHandObj1;
	delete playerHandObj2;
	delete gameObject_;
	delete collider_;
	delete input;
	delete deathSE;
}

void Player::Initialize() {
	// オブジェクトの初期化
	gameObject_->SetModel(playerModel);
	gameObject_->worldTransform_.position_ = { 7,0,-5 };


	// プレイヤーのおてての初期化
	playerHandObj1->SetModel(playerHand1Model);
	playerHandObj1->worldTransform_.position_ = { 1.0f,0,0 };
	playerHandObj1->worldTransform_.scale_ = { 0.2f,0.2f, 0.2f };
	playerHandObj1->worldTransform_.parent_ = &gameObject_->worldTransform_;

	playerHandObj2->SetModel(playerHand1Model);
	playerHandObj2->worldTransform_.position_ = { -1.0f,0,0 };
	playerHandObj2->worldTransform_.scale_ = { 0.2f,0.2f, 0.2f };
	playerHandObj2->worldTransform_.parent_ = &gameObject_->worldTransform_;


	collider_->Initialize(&gameObject_->worldTransform_);
	collider_->SetRadius(radius_);

	// フラグ系の初期化
	isHitV_ = false;
	isHitH_ = false;
	operate_ = true;
	onBlock_ = false;
	onGround_ = true;
	isUpHand = false;
	upFlag = false;

	// 使っているメンバ変数の初期化
	radius_ = 1.0f;
	amountRise_ = 0.0f;
	kCharacterSpeed_ = 0.0f;

	deathSE = new Sound;
	deathSE->SoundLoadWave("Resources/Sound/DeathSE.wav");
};

void Player::Update() {
	input->Update();
	//プラグ
	ActionPlug();
	//移動
	Move();
	//WaitMotion();
	//当たり判定
	collider_->Update();
	//自動でブロックを登る
	Rise();
	//飛び降りる
	Descent();

	//更新
	playerHandObj1->Update();
	playerHandObj2->Update();
	gameObject_->Update();



	//死亡
	Dead();


};

void Player::Draw(ViewProjection* viewProjection) {
	gameObject_->Draw(viewProjection);
	playerHandObj1->Draw(viewProjection);
	playerHandObj2->Draw(viewProjection);
};

void Player::Move() {
	move_ = { 0 , 0 , 0 };
	isUp_ = false;
	isDown_ = false;
	isRight_ = false;
	isLeft_ = false;

	waitFlag = true;
	Vector3 pos = gameObject_->worldTransform_.position_;
	// プレイヤーが今いる高さを検索
	if (pos.y <= 0.5f && pos.y >= -0.5f) {
		// ０段目
		step = Step::Grond;
	}
	else if (pos.y <= 2.5f && pos.y >= 1.5f) {
		// 一段目
		step = Step::First;
	}
	else if (pos.y <= 4.5f && pos.y >= 3.5f) {
		// 二段目
		step = Step::Second;
	}
	else if (pos.y <= 6.5f && pos.y >= 5.5f) {
		// 三段目
		step = Step::Third;
	}

	if (operate_ == true) {
		//押した方向で移動量を変化
		if (input->PushKey(DIK_RIGHT)) {
			isRight_ = true;
			kCharacterSpeed_ = 0.1f;
			move_.x = kCharacterSpeed_;
			gameObject_->worldTransform_.rotation_.y = MathFunc::Utility::Deg2Rad(90.0f);
			orignalHandPos1 = playerHandObj1->worldTransform_.position_;
			orignalHandPos2 = playerHandObj2->worldTransform_.position_;
			waitFlag = false;

		}
		else if (input->PushKey(DIK_LEFT)) {
			isLeft_ = true;
			kCharacterSpeed_ = 0.1f;
			move_.x = -kCharacterSpeed_;
			gameObject_->worldTransform_.rotation_.y = MathFunc::Utility::Deg2Rad(270.0f);
			orignalHandPos1 = playerHandObj1->worldTransform_.position_;
			orignalHandPos2 = playerHandObj2->worldTransform_.position_;
			waitFlag = false;
		}

		if (input->PushKey(DIK_UP)) {
			isUp_ = true;
			kCharacterSpeed_ = 0.1f;
			move_.z = kCharacterSpeed_;
			gameObject_->worldTransform_.rotation_.y = MathFunc::Utility::Deg2Rad(0.0f);
			orignalHandPos1 = playerHandObj1->worldTransform_.position_;
			orignalHandPos2 = playerHandObj2->worldTransform_.position_;
			waitFlag = false;
		}
		else if (input->PushKey(DIK_DOWN)) {
			isDown_ = true;
			kCharacterSpeed_ = 0.1f;
			move_.z = -kCharacterSpeed_;
			gameObject_->worldTransform_.rotation_.y = MathFunc::Utility::Deg2Rad(180.0f);
			orignalHandPos1 = playerHandObj1->worldTransform_.position_;
			orignalHandPos2 = playerHandObj2->worldTransform_.position_;
			waitFlag = false;
		}
	}

	gameObject_->worldTransform_.position_ += move_;
	gameObject_->Update();

	collider_->Update();

	//ステージとの当たり判定
	for (int i = 0; i < stage_->GetObjectcount(); i++)
	{
		if (collider_->CheckCollision(stage_->GetCollider(i)))
		{

			if (stage_->GetCollider(i).GetWorldPos().y < 4)
			{
				if (mapNum == 0) {
					if (stage_->GetMapNum
					(
						stage_->GetCollider(i).GetWorldPos().x / 2.0f + 2.0,
						stage_->GetCollider(i).GetWorldPos().y + 2.0,
						-stage_->GetCollider(i).GetWorldPos().z / 2.0f + 2.0
					) == 1)
					{

						isHitH_ = false;
						operate_ = true;
						gameObject_->worldTransform_.position_ -= move_;
						break;
					}

					if (stage_->GetMapNum
					(
						stage_->GetCollider(i).GetWorldPos().x / 2.0f + 2.0,
						stage_->GetCollider(i).GetWorldPos().y + 2.0,
						-stage_->GetCollider(i).GetWorldPos().z / 2.0f + 2.0
					) == 0)
					{
						OnHorizonCollision();
					}
				}
				else if (mapNum == 3) {
					if (stage_->GetMap3Num
					(
						stage_->GetCollider(i).GetWorldPos().x / 2.0f + 2.0,
						stage_->GetCollider(i).GetWorldPos().y + 2.0,
						-stage_->GetCollider(i).GetWorldPos().z / 2.0f + 2.0
					) == 1)
					{

						isHitH_ = false;
						operate_ = true;
						gameObject_->worldTransform_.position_ -= move_;
						break;
					}

					if (stage_->GetMap3Num
					(
						stage_->GetCollider(i).GetWorldPos().x / 2.0f + 2.0,
						stage_->GetCollider(i).GetWorldPos().y + 2.0,
						-stage_->GetCollider(i).GetWorldPos().z / 2.0f + 2.0
					) == 0)
					{
						OnHorizonCollision();
					}
				}
			}
			else {
				OnHorizonCollision();
			}
		}
	}
	//ドアとの当たり判定
	if (door_->GetOpen() == true &&
		collider_->boxCollision(gameObject_->worldTransform_.position_, door_->GetPos(), Pradius, door_->GetRadius())) {
		isClear_ = true;
	}
	else if (collider_->CheckCollision(*door_->GetCollider()))
	{
		gameObject_->worldTransform_.position_ -= move_;
	}

	//敵との当たり判定
	for (int i = 0; i < enemyNum_; i++)
	{
		if (collider_->CheckCollision(*enemy_[i]->GetCollider()))
		{
			gameObject_->worldTransform_.position_ -= move_;
			isPlayerAlive_ = false;
		}
	}

}

void Player::WaitMotion()
{
	if (waitFlag == true) {
		switch (step)
		{
		case Player::Grond:
			// 0段目
			if (gameObject_->worldTransform_.position_.y >= 0.3f)
			{
				waitDown = true;
				waitUp = false;
			}
			else if (gameObject_->worldTransform_.position_.y <= -0.0f)
			{
				waitUp = true;
				waitDown = false;
			}
			break;
		case Player::First:
			// 1段目
			if (gameObject_->worldTransform_.position_.y >= 2.3f)
			{
				waitDown = true;
				waitUp = false;
			}
			else if (gameObject_->worldTransform_.position_.y <= 2.0f)
			{
				waitUp = true;
				waitDown = false;
			}
			break;
		case Player::Second:
			// 2段目
			if (gameObject_->worldTransform_.position_.y >= 4.3f)
			{
				waitDown = true;
				waitUp = false;
			}
			else if (gameObject_->worldTransform_.position_.y <= 4.0f)
			{
				waitUp = true;
				waitDown = false;
			}
			break;
		case Player::Third:
			// 3段目
			if (gameObject_->worldTransform_.position_.y >= 6.3f)
			{
				waitDown = true;
				waitUp = false;
			}
			else if (gameObject_->worldTransform_.position_.y <= 6.0f)
			{
				waitUp = true;
				waitDown = false;
			}
			break;
		default:
			break;
		}
		//if (angle>=30) {
		//	angleMove = -0.5f;
		//}
		//else if (angle <= 0.0f) {
		//	angleMove = +0.5f;
		//}
		//angle += angleMove;
		//gameObject_->worldTransform_.position_.y = sinf(MathFunc::Utility::Deg2Rad(angle));
		if (waitUp) {
			gameObject_->worldTransform_.position_.y += 0.01f;
		}
		if (waitDown) {
			gameObject_->worldTransform_.position_.y -= 0.01f;
		}

	}
}

//縦の当たり判定フラグ関数
void Player::OnVerticalCollision()
{
	isHitV_ = true;
	operate_ = false;
}

//横の当たり判定フラグ関数
void Player::OnHorizonCollision() {
	isHitH_ = true;
	operate_ = false;
}

//当たり判定関数
void Player::OnCollision() {
	isHitH_ = true;
}
//ワールド座標を取得
Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れるための変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = gameObject_->worldTransform_.matWorld_.m[3][0];
	worldPos.y = gameObject_->worldTransform_.matWorld_.m[3][1];
	worldPos.z = gameObject_->worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

//半径を返す関数
float Player::GetRadius() {
	return radius_;
}

//移動を返す関数
Vector3 Player::GetMove() {
	return move_;
}

//ブロックに当たった時に一定量上昇する関数
void Player::Rise() {

	if (isHitH_ == true && operate_ == false) {

		onGround_ = false;


		if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT) || input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN)) {
			isUpHand = true;
		}

		if (isUpHand == true) {
			Vector3 desPos1 = orignalHandPos1 + Vector3{ 0, upHandPosY, upHandPosZ };
			Vector3 desPos2 = orignalHandPos2 + Vector3{ 0, upHandPosY, upHandPosZ };
			Vector3 vec1 = desPos1 - orignalHandPos1;
			Vector3 vec2 = desPos2 - orignalHandPos2;
			vec1.normalize();
			vec2.normalize();
			vec1 *= handSpeed;
			vec2 *= handSpeed;

			if (playerHandObj1->worldTransform_.position_.y < desPos1.y) {
				playerHandObj1->worldTransform_.position_ += vec1;
				playerHandObj2->worldTransform_.position_ += vec2;
			}
			else if (playerHandObj1->worldTransform_.position_.y >= desPos1.y) {
				upFlag = true;
			}
		}


		if (upFlag == true) {
			amountRise_ += 0.025f;
			gameObject_->worldTransform_.position_.y += amountRise_;
			if (amountRise_ >= 0.40f) {
				playerHandObj1->worldTransform_.position_ = { 1.0f,0,0 };
				playerHandObj2->worldTransform_.position_ = { -1.0f,0,0 };
				amountRise_ = 0.0f;
				upFlag = false;
				isUpHand = false;
				isHitH_ = false;
				operate_ = true;
			}

		}


	}
}

//足場を離れた時に下降する関数
void Player::Descent() {
	if (onGround_ == false && isHitH_ == false && isHitV_ == false) {
		gameObject_->worldTransform_.position_.y -= 0.2f;
		gameObject_->Update();

		collider_->Update();

		for (int i = 0; i < stage_->GetObjectcount(); i++)
		{
			if (collider_->CheckCollision(stage_->GetCollider(i)))
			{
				if (gameObject_->worldTransform_.position_.y <= 0.1f)
				{
					gameObject_->worldTransform_.position_.y = 0.0f;

				}
				else if (gameObject_->worldTransform_.position_.y <= 2.1f)
				{
					gameObject_->worldTransform_.position_.y = 2.0f;

				}
				else if (gameObject_->worldTransform_.position_.y <= 4.1f)
				{
					gameObject_->worldTransform_.position_.y = 4.0f;

				}
				else if (gameObject_->worldTransform_.position_.y <= 6.1f)
				{
					gameObject_->worldTransform_.position_.y = 6.0f;

				}
			}
		}

		if (gameObject_->worldTransform_.position_.y <= 0) {
			gameObject_->worldTransform_.position_.y = 0.0f;
			onGround_ = true;
		}
	}
}

//プラグ
void Player::ActionPlug() {

	if (0 < plugNum_) {

		for (int i = 0; i < plugNum_; i++) {
			if (input->TriggerKey(DIK_SPACE)) {
				if (collider_->CheckCollision(*plug_[i]->GetPlugCollider())) {

					if (plug_[i]->GetIsGrabbed() == false) {


						plug_[i]->SetIsGrabbed(true);
						plug_[i]->SetIsConnect(false);

					}
					else {
						plug_[i]->SetIsGrabbed(false);
					}

				}
			}

			if (plug_[i]->GetIsGrabbed() == true) {

				if (plug_[i]->GetIsLimit() == true) {
					gameObject_->worldTransform_.position_ = plug_[i]->GetPlugWorldTransform().position_;

				}
				else {
					plug_[i]->SetWorldPos(gameObject_->worldTransform_.position_);
				}
			}
			//プラグとの当たり判定(当たったら止まる)
			for (int j = 0; j < 20; j++) {
				if (j != 0 && j != 1) {
					if (plug_[i]->GetIsConnect() == true) {
						if (collider_->CheckCollision(*plug_[i]->GetCordCollider(j, 0)) == true) {
							gameObject_->worldTransform_.position_ -= move_;

							break;
						}
					}
				}

			}
		}
	}

}

//Playerの座標setter
void Player::SetPlayer(Vector3 pos) {
	gameObject_->worldTransform_.position_ = pos;
}

//ステージのsetter
void Player::SetStage(Stage* stage) {
	stage_ = stage;
}

//プラグのsetter
void Player::SetPlug(Plug* plug) {

	plug_.resize(plugNum_ + 1);

	plug_[plugNum_] = plug;

	plugNum_++;

}

//ドアのsetter
void Player::SetDoor(Door* door)
{
	door_ = door;
}

void Player::SetEnemy(Enemy* enemy)
{
	enemy_.resize(enemyNum_ + 1);

	enemy_[enemyNum_] = enemy;

	enemyNum_++;
}

void Player::Dead()
{
	if (isPlayerAlive_ == false)
	{
		operate_ = false;
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

void Player::Reset(Vector3 pos, int mapNum)
{
	gameObject_->worldTransform_.position_ = pos;
	gameObject_->worldTransform_.rotation_.y = MathFunc::Utility::Deg2Rad(0.0f);
	gameObject_->worldTransform_.rotation_.x = MathFunc::Utility::Deg2Rad(0.0f);
	playerHandObj1->worldTransform_.position_ = { 1.0f,0,0 };
	playerHandObj1->worldTransform_.scale_ = { 0.2f,0.2f, 0.2f };
	playerHandObj1->worldTransform_.parent_ = &gameObject_->worldTransform_;
	playerHandObj2->worldTransform_.position_ = { -1.0f,0,0 };
	playerHandObj2->worldTransform_.scale_ = { 0.2f,0.2f, 0.2f };
	playerHandObj2->worldTransform_.parent_ = &gameObject_->worldTransform_;
	collider_->Initialize(&gameObject_->worldTransform_);
	collider_->SetRadius(radius_);

	// マップの番号をセット
	this->mapNum = mapNum;

	// フラグ系の初期化
	isHitV_ = false;
	isHitH_ = false;
	operate_ = true;
	onBlock_ = false;
	onGround_ = true;
	isUpHand = false;
	upFlag = false;
	isPlayerAlive_ = true;
	isClear_ = false;
	isUp_ = false;
	isDown_ = false;
	isRight_ = false;
	isLeft_ = false;

	// 使っているメンバ変数の初期化
	radius_ = 1.0f;
	amountRise_ = 0.0f;
	kCharacterSpeed_ = 0.0f;
}
