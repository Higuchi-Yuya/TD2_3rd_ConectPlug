#include "Lamp.h"

Lamp::Lamp()
{
	lampObj = Object3d::Create();
	lampModel = new Model();
	lampModel = Model::LoadFromOBJ("Lamp",true);
}

Lamp::~Lamp()
{
	delete lampModel;
	delete lampObj;
}

void Lamp::Initialize(Vector3 pos)
{
	// オブジェクトの初期化
	lampObj->SetModel(lampModel);
	lampObj->worldTransform_.position_ = pos;
	lampObj->worldTransform_.color_ = { 0.2f,0.2,0.2f,1.0f };

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
		lampObj->worldTransform_.color_ = { r,g,b,1.0f };
		// 元の色情報を超えないように
		if (r >= 0.8f) {
			r = 0.8f;
			g = 0.8f;
			b = 0.8f;
		}
	}
	// そうじゃなければ暗くする
	else if (isShining == false) {
		// 徐々に暗くする
		r -= colorConst;
		g -= colorConst;
		b -= colorConst;
		// 代入
		lampObj->worldTransform_.color_ = { r,g,b,1.0f };
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

void Lamp::Reset(Vector3 pos)
{
	lampObj->worldTransform_.position_ = pos;
	lampObj->worldTransform_.color_ = { 0.2f,0.2,0.2f,0.8f };
	r = 0.2f, g = 0.2f, b = 0.2f;
	lampObj->worldTransform_.UpdateMatrix();
	isShining = false;
}
