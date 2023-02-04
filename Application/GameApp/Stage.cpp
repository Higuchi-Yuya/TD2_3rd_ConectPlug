#include "Stage.h"

Stage::Stage()
{
	blockModel = new Model;
	blockModel = Model::LoadFromOBJ("block");
}

Stage::~Stage()
{
	delete blockModel;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++)
		{
			delete stage0W[j][i];
			delete stage1W[j][i];
			delete stage2W[j][i];
			delete stage3W[j][i];
		}
	}
	for (int i = 0; i < 500; i++) {
		delete stageBlock[i];
		delete collider_[i];
	}

}

void Stage::Initialize()
{
	
	TutorialPosCreate();
	//// 必要なブロック分をVector型のサイズに合わせる
	//stageBlock.resize((size_t)500 + 1);
	//collider_.resize((size_t)500 + 1);
	for (int i = 0; i < 500; i++) {
		stageBlock[i] = Object3d::Create();
		stageBlock[i]->SetModel(blockModel);

		collider_[i] = new Collider;
	}
	TutorialPosSet();
}

void Stage::Update()
{

	for (int i = 0; i < objectCount; i++) {
		stageBlock[i]->Update();
		collider_[i]->Update();
	}
}

void Stage::Draw(ViewProjection* viewProjection)
{
	for (int i = 0; i < objectCount; i++) {
		stageBlock[i]->Draw(viewProjection);
	}
}

void Stage::TutorialReset()
{
	int num = 0;
	// 必要なブロック分だけ初期化
	for (int z = 0; z < 10; z++) {
		for (int x = 0; x < 10; x++)
		{
			if (zeroStage[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage0W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (firstStage[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage1W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (secondStage[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage2W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (thirdStage[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage3W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
		}
	}

}

void Stage::TutorialPosCreate()
{
	// ブロックの初期化&オブジェクトの必要な数をカウント
	for (int z = 0; z < 10; z++) {
		for (int x = 0; x < 10; x++)
		{
			// 零段目
			stage0W[z][x] = new WorldTransform;
			stage0W[z][x]->Initialize();
			stage0W[z][x]->position_ = { 0.0f + ((x - 2.0f) * 2.0f),-2.0f,0.0f - ((z - 2.0f) * 2.0f) };
			// 一段目
			stage1W[z][x] = new WorldTransform;
			stage1W[z][x]->Initialize();
			stage1W[z][x]->position_ = { 0.0f + ((x - 2.0f) * 2.0f),0.0f,0.0f - ((z - 2.0f) * 2.0f) };
			// 二段目
			stage2W[z][x] = new WorldTransform;
			stage2W[z][x]->Initialize();
			stage2W[z][x]->position_ = { 0.0f + ((x - 2.0f) * 2.0f),2.0f,0.0f - ((z - 2.0f) * 2.0f) };
			// 三段目
			stage3W[z][x] = new WorldTransform;
			stage3W[z][x]->Initialize();
			stage3W[z][x]->position_ = { 0.0f + ((x - 2.0f) * 2.0f),4.0f,0.0f - ((z - 2.0f) * 2.0f) };

			if (zeroStage[z][x] == 1) {
				objectCount++;
			}
			if (firstStage[z][x] == 1) {
				objectCount++;
			}
			if (secondStage[z][x] == 1) {
				objectCount++;
			}
			if (thirdStage[z][x] == 1) {
				objectCount++;
			}
		}
	}
}

void Stage::TutorialPosSet()
{
	int num = 0;
	// 必要なブロック分だけ初期化
	for (int z = 0; z < 10; z++) {
		for (int x = 0; x < 10; x++)
		{
			if (zeroStage[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage0W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (firstStage[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage1W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (secondStage[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage2W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (thirdStage[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage3W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
		}
	}
}

#pragma region ステージ1のセット
void Stage::Stage1Reset()
{
	objectCount = 0;
	int num = 0;
	// 必要なブロック分だけ初期化
	for (int z = 0; z < 10; z++) {
		for (int x = 0; x < 10; x++)
		{
			if (zeroStage1[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage0W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (firstStage1[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage1W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (secondStage1[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage2W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (thirdStage1[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage3W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}

			if (zeroStage1[z][x] == 1) {
				objectCount++;
			}
			if (firstStage1[z][x] == 1) {
				objectCount++;
			}
			if (secondStage1[z][x] == 1) {
				objectCount++;
			}
			if (thirdStage1[z][x] == 1) {
				objectCount++;
			}
		}
	}
}
#pragma endregion

#pragma region ステージ2のセット
void Stage::Stage2Reset()
{
	objectCount = 0;
	int num = 0;
	// 必要なブロック分だけ初期化
	for (int z = 0; z < 10; z++) {
		for (int x = 0; x < 10; x++)
		{
			if (zeroStage2[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage0W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (firstStage2[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage1W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (secondStage2[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage2W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (thirdStage2[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage3W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}

			if (zeroStage2[z][x] == 1) {
				objectCount++;
			}
			if (firstStage2[z][x] == 1) {
				objectCount++;
			}
			if (secondStage2[z][x] == 1) {
				objectCount++;
			}
			if (thirdStage2[z][x] == 1) {
				objectCount++;
			}
		}
	}
}
#pragma endregion

#pragma region ステージ3のセット
void Stage::Stage3Reset()
{
	objectCount = 0;
	int num = 0;
	// 必要なブロック分だけ初期化
	for (int z = 0; z < 10; z++) {
		for (int x = 0; x < 10; x++)
		{
			if (zeroStage3[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage0W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (firstStage3[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage1W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (secondStage3[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage2W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}
			if (thirdStage3[z][x] == 1) {

				stageBlock[num]->worldTransform_.position_ = stage3W[z][x]->position_;

				collider_[num]->Initialize(&stageBlock[num]->worldTransform_);
				collider_[num]->SetRadius(radius);

				num++;
			}

			if (zeroStage3[z][x] == 1) {
				objectCount++;
			}
			if (firstStage3[z][x] == 1) {
				objectCount++;
			}
			if (secondStage3[z][x] == 1) {
				objectCount++;
			}
			if (thirdStage3[z][x] == 1) {
				objectCount++;
			}
		}
	}
}
#pragma endregion

Collider Stage::GetCollider(int num)
{
	return *collider_[num];
}

int Stage::GetObjectcount()
{
	return objectCount;
}

int Stage::GetMapNum(int x, int y, int z)
{
	if (4 <= y)
	{
		return	thirdStage[z][x];
	}
	else if(2 <= y)
	{
		return	secondStage[z][x];
	}
	else
	{
		return	firstStage[z][x];
	}
}

WorldTransform Stage::GetWorldTransform(int num)
{
	return stageBlock[num]->worldTransform_;
}

Vector3 Stage::GetRadius()
{
	return radius_;
}
