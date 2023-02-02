#pragma once
#include "Object3d.h"
#include "Input.h"
#include "MathFunc.h"
#include "Collider.h"
#include "Door.h"

class Stage;
class Plug;
class Door;
class Enemy;

class Player
{
public:
	enum Step
	{
		Grond,
		First,
		Second,
		Third,
	};
	Player();
	~Player();
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw(ViewProjection* viewProjection);
	//移動
	void Move();

	// 待機のモーション
	void WaitMotion();

	//当たり判定
	void OnVerticalCollision();
	void OnHorizonCollision();
	void OnCollision();
	//ワールド座標を取得
	Vector3 GetWorldPosition();
	//半径を返す関数
	float GetRadius();
	//移動を返す関数
	Vector3 GetMove();
	//座標setter
	void SetPlayer(Vector3 pos);
	void SetStage(Stage* stage);
	void SetPlug(Plug* plug);
	void SetDoor(Door* door);
	void SetEnemy(Enemy* enemy);

	// 生存フラグの取得
	bool GetIsAlive() { return isPlayerAlive_; }

	bool GetIsClear() { return isClear_; }

	//移動キーフラグの取得
	bool GetIsUp() { return isUp_; }
	bool GetIsDown() { return isDown_; }
	bool GetIsRight() { return isRight_; }
	bool GetIsLeft() { return isLeft_; }

	//死亡
	void Dead();

	// リセット
	void Reset();

private:
	//上昇
	void Rise();
	//下降
	void Descent();
	//プラグを操作する
	void ActionPlug();

	// モデル
	Model* playerModel = nullptr;
	Model* playerHand1Model = nullptr;


	//オブジェクト
	Object3d* gameObject_ = nullptr;

	Object3d* playerHandObj1 = nullptr;
	Object3d* playerHandObj2 = nullptr;

	//当たり判定
	Collider* collider_ = nullptr;

	//ステージ
	Stage* stage_ = nullptr;

	//ドア
	Door* door_ = nullptr;

	//敵
	Enemy* enemy_ = nullptr;

	//キー入力
	Input* input = nullptr;

	//当たり判定のフラグ
	//縦フラグ
	bool isHitV_ = false;

	//横フラグ
	bool isHitH_ = false;

	//操作受付
	bool operate_ = true;

	//ブロックに乗っているか
	bool onBlock_ = false;

	//浮いているか
	bool onGround_ = true;

	//生存フラグ
	bool isPlayerAlive_ = true;

	// クリアしてるかどうか
	bool isClear_ = false;

	//半径
	float radius_ = 1.0f;
	Vector3 Pradius = { 1,1,1 };
	//上昇量
	float amountRise_ = 0.0f;

	//移動量
	float kCharacterSpeed_ = 0.0f;
	//移動キーのフラグ
	bool isUp_ = false;
	bool isDown_ = false;
	bool isRight_ = false;
	bool isLeft_ = false;

	//移動
	Vector3 move_ = { 0,0,0 };

	//プラグ
	std::vector<Plug*> plug_;
	int plugNum_ = 0;


	// 上る時のモーション
	Vector3 orignalHandPos1 = { 0.5f,0,0 };
	Vector3 orignalHandPos2 = { -0.5f,0,0 };
	float upHandPosX = 0.5f, upHandPosZ = 0.8f;
	float upHandPosY = 0.3f;
	float handSpeed = 0.08f;

	bool isUpHand = false;
	bool upFlag = false;

	// 待機時モーション
	float angle = 0.0f;
	float angleMove = 0.5f;
	Step step = Step::Grond;
	bool waitFlag = true;
	bool waitUp = true;
	bool waitDown = false;
};
