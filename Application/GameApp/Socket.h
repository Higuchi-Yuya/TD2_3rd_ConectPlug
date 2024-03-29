#pragma once
#include "Object3d.h"
#include "Collider.h"

class Socket {
public:
	//コンストラクタ
	Socket();

	//デストラクタ
	~Socket();

	//メンバ関数
	void Initialize(Vector3 pos , int face);

	void Update();

	void Draw(ViewProjection* viewProjection);

	// リセット
	void Reset(Vector3 pos, int face);

	//アクセッサ
	int GetFace();

	Collider GetCollider();
	WorldTransform GetWorldTransform();
	void SetParent(WorldTransform& parent) { gameObject_->worldTransform_.parent_ = &parent; }
	bool GetIsEnemy() { return isEnemy; }
	void SetisEnemy(bool isEnemy) { this->isEnemy = isEnemy; }
	//enum
	enum FACE {
		NORTH ,
		EAST ,
		SOUTH ,
		WEST ,
	};

	//メンバ変数
private:
	Model* soketModel = nullptr;

	Object3d* gameObject_ = nullptr;

	Collider* collider_ = nullptr;

	int face_ = NORTH;

	bool isEnemy = false;
};

