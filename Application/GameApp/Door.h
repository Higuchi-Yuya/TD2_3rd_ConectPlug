#pragma once
#include "Collider.h"
#include "Object3d.h"

class Plug;

class Door
{
public:
	Door();
	~Door();
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw(ViewProjection* viewProjection);
	//colliderのgetter
	Collider* GetCollider() { return collider_; };
	Collider* GetClearCollider() { return clearCollider_; };
	Vector3 GetPos() { return clearPos.position_; }
	Vector3 GetRadius() { return C_Radius_; }
	//plugのsetter
	void SetPlug(Plug*plug);
	//colliderのsetter
	void SetCollider(Collider* collider);

	// 開いてるかどうか
	bool GetOpen() { return isOpen_; }

	// リセット
	void Reset();

private:
	Model* doorModel = nullptr;
	//ゲームオブジェクト
	Object3d* gameObject_ = nullptr;
	//当たり判定
	Collider* collider_ = nullptr;

	WorldTransform clearPos;
	Collider* clearCollider_ = nullptr;
	//開いているか
	bool isOpen_ = false;

	//半径
	float radius_ = 1.0f;
	Vector3 C_Radius_ = { 0.5f,0.5f,0.5f };

	//プラグ
	std::vector<Plug*> plug_;
	int plugNum_ = 0;
};

