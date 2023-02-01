#pragma once
#include "Object3d.h"
#include "Collider.h"


class Stage
{
public:// メンバ関数
	Stage();
	~Stage();

	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw(ViewProjection* viewProjection);

	// チュートリアルステージのセット
	void TutorialReset();

	// チュートリアルステージのポジションを作る
	void TutorialPosCreate();

	// チュートリアルステージのポジションのセット
	void TutorialPosSet();

	//アクセッサ
	Collider GetCollider(int num);
	int GetObjectcount();
	int GetMapNum(int x, int y, int z);
	WorldTransform GetWorldTransform(int num);
	Vector3 GetRadius();

private:// �����o�ϐ�
	//�}�b�v�z��̕ϐ�
	int mapNumX = 10;
	int mapNumZ = 10;

	Model* blockModel = nullptr;
	WorldTransform* stage0W[10][10];
	WorldTransform* stage1W[10][10];
	WorldTransform* stage2W[10][10];
	WorldTransform* stage3W[10][10];
	Object3d* stageBlock[500];
	Collider* collider_[500];

	//半径
	Vector3 radius_ = { 1,1,1 };

	// 必要なオブジェクトの個数のカウント
	int objectCount = 0;

	// ステージ構成
	
	// ステージのゼロ段目(ダメージ床など特殊ギミック用)
	int zeroStage[10][10] = {
		{0,0,1,1,0,0,0,0,0,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0},
	};

	// ステージの一段目
	int firstStage[10][10] = {
		{1,1,0,0,1,1,1,1,1,1},
		{1,1,0,0,0,0,0,0,1,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,1,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1},
	};
	// ステージの二段目
	int secondStage[10][10] = {
		{1,1,0,0,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1},
	};;
	// ステージの三段目
	int thirdStage[10][10] = {
		{1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1},
	};;

	//一個のブロックの半径
	float radius = 1.0f;

	// ダメージ床に必要なステータス

#pragma region damege
	//// 場所
	//WorldTransform* damegeW[10][10];

	//// 必要な分だけ生成するリスト
	//std::vector<GameObject3D*> damegeBlock;
	//
	//// 当たり判定のリスト
	//std::vector<Collider*>damegeCollider;
	//
	//// 必要な分の数
	//int damegeObjCount = 0;

#pragma endregion
};