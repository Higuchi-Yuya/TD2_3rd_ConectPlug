#pragma once
#include "WorldTransform.h"


class Collider {
public:

	//コンストラクタ
	Collider();

	//デストラクタ
	~Collider();

	//メンバ関数
	void Initialize(WorldTransform* parent);

	void Update();

	//当たり判定をチェック(全面)
	bool CheckCollision(Collider collider);

	//可変四角形の当たり判定
	bool boxCollision(Vector3 a, Vector3 b, Vector3 aRadius, Vector3 bPlusRadius,Vector3 bMinusRadius);

	//可変四角形の当たり判定
	bool boxCollision(Vector3 a, Vector3 b, Vector3 aRadius, Vector3 bRadius);

	//アクセッサ
	void SetRadius(float radius);
	Vector3 GetWorldPos();

	//メンバ変数
private:
	WorldTransform worldTransform_;
	float radius_ = 1.0f;
};