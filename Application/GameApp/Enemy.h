#pragma once
#include "Object3d.h"
#include "Collider.h"
#include "Input.h"
#include "MathFunc.h"
#include "Socket.h"
class Stage;
class Plug;

class Enemy
{
public:
	//敵の向き
	enum face
	{
		NORTH, //上
		EAST,  //右
		SOUTH,  //下
		WEST  //左
	};
public:
	Enemy();
	~Enemy();
	//初期化
	void Initialize(Vector3 pos,int face,int plusFace);
	//更新
	void Update();
	//描画
	void Draw(ViewProjection* viewProjection);
	//移動
	void Move();
	//死亡
	void Dead();
	//colliderのgetter
	Collider* GetCollider() { return collider_; };
	Socket* GetSocket() { return enemySocket; }
	void SetPlug(Plug* plug){
		plug_.resize(plugNum_ + 1);

		plug_[plugNum_] = plug;

		plugNum_++;
	};
	//stageのsetter
	void SetStage(Stage* stage);
	//回転
	void Turn();
	// リセット
	void Reset(Vector3 pos,int face, int plusFace);

private:
	Model* enemyModel = nullptr;

	//オブジェクト
	Object3d* gameObject_ = nullptr;

	//当たり判定
	Collider* collider_ = nullptr;

	//ステージ
	Stage* stage_ = nullptr;

	//キー入力
	Input* input = nullptr;

	//移動
	Vector3 move_ = { 0,0,0 };

	//向き
	int face_ = NORTH;

	//回転量
	float amountRotation_ = 0;

	//半径
	float radius_ = 1.0f;
	float radiusVec = 1.0f;

	//半径Vector3
	Vector3 vecPlusRadius_ = { radiusVec,radiusVec,radiusVec };
	Vector3 vecMinusRadius_ = { radiusVec,radiusVec,radiusVec };

	//生存フラグ
	bool isEnemyAlive_ = true;

	//回転フラグ
	bool isTurn_ = false;

	//移動フラグ
	bool isMove_ = true;

	//壁に触れた時にfaceを変更する変数
	int plusFace_ = 0;

	//速度
	float kCharacterSpeed_ = 0.05f;

	// ソケット
	Socket* enemySocket = nullptr;

	//プラグ
	std::vector<Plug*> plug_;
	int plugNum_ = 0;
};

