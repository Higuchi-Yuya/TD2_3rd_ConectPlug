#include "Lamp.h"

Lamp::Lamp()
{
	lampObj = Object3d::Create();
	lampModel = new Model();
	lampModel = Model::LoadFromOBJ("sphere",true);
}

Lamp::~Lamp()
{
	delete lampModel;
	delete lampObj;
}

void Lamp::Initialize()
{
	// オブジェクトの初期化
	lampObj->SetModel(lampModel);
	lampObj->worldTransform_.position_ = { 4,2,2 };
	lampObj->worldTransform_.color_ = { 0.2f,0.2,0.2f,0.8f };

	isShining = false;

}

void Lamp::Update()
{
	// 光っていたらカラーを明るくする
	if (isShining == true) {
		// 徐々に明るくする
		r += colorConst;
		g += colorConst;
		b += colorConst;
		// 代入
		lampObj->worldTransform_.color_ = { r,g,b,0.8f };
		// 元の色情報を超えないように
		if (r >= 1.0f) {
			r = 1.0f;
			g = 1.0f;
			b = 1.0f;
		}
	}
	// そうじゃなければ暗くする
	else if (isShining == false) {
		// 徐々に暗くする
		r -= colorConst;
		g -= colorConst;
		b -= colorConst;
		// 代入
		lampObj->worldTransform_.color_ = { r,g,b,0.8f };
		// 規定値の色情報を超えないように
		if (r <= 0.2f) {
			r = 0.2f;
			g = 0.2f;
			b = 0.2f;
		}
	}
	lampObj->Update();
}

void Lamp::Draw(ViewProjection* viewProjection)
{
	lampObj->Draw(viewProjection);
}

void Lamp::Reset()
{
	lampObj->worldTransform_.position_ = { 4,2,2 };
	lampObj->worldTransform_.color_ = { 0.2f,0.2,0.2f,0.8f };
	r = 0.2f, g = 0.2f, b = 0.2f;
	lampObj->worldTransform_.UpdateMatrix();
	isShining = false;
}
