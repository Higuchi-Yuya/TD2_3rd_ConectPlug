#pragma once
#include "Object3d.h"


class Lamp
{
public:
	Lamp();
	~Lamp();
	// 初期化処理
	void Initialize(Vector3 pos);

	// 更新処理
	void Update();

	// 描画処理
	void Draw(ViewProjection* viewProjection);

	// 光るフラグ
	bool GetisShining() { return isShining; }

	void SetisShining(bool isShining) { this->isShining = isShining; }

	// 位置
	Vector3 GetPos() { return lampObj->worldTransform_.position_; }

	void SetPosition(Vector3& position) { lampObj->worldTransform_.position_ = position; }

	// リセット
	void Reset(Vector3 pos);

private:
	Model* lampModel = nullptr;
	// オブジェクト
	Object3d* lampObj = nullptr;
	// 光るフラグ
	bool isShining = false;

	// 徐々に光らせるに必要なステータス
	// R,G,Bそれぞれのいろ（１から０）
	float r = 0.2f, g = 0.2f, b = 0.2f;

	// 徐々に増える色の定数
	const float colorConst = 0.025f;
};


