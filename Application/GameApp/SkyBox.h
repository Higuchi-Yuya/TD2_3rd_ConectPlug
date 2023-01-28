#pragma once
#include "Object3d.h"

class SkyBox
{
public:
	// namespaceの省略
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public:// メンバ関数
	SkyBox();
	~SkyBox();

	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw(ViewProjection* viewProjection);

private:// メンバ変数
	Model* skyModel = nullptr;

	// オブジェクト
	Object3d* skyBox_ = nullptr;
	
};

