#pragma once
#include "Sprite.h"
#include "Object3d.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Quaternion.h"
#include "LightGroup.h"
#include "ImGuiManager.h"
#include "CollisionPrimitive.h"

#include"Player.h"
#include"Plug.h"
#include "Socket.h"
#include "SkyBox.h"
#include "Stage.h"
#include "Enemy.h"
#include "LightGroup.h"
#include "Door.h"
#include "Lamp.h"

class GameScene
{
public:// メンバ関数

	// シーンチェンジ
	enum Scene
	{
		Title,
		StageSelect,
		Game,
		StageClear,
		GameOver,
	};

	// デストラクタ
	~GameScene();

	// 初期化処理
	void Initialize();

	// スプライトの初期化処理
	void SpriteInitialize();

	// スプライトの解放
	void DeleteSprite();

	// スプライトの描画
	void SpriteDraw();

	// ポイントライトの処理
	void PointLightUpdate();

	// 更新処理
	void Update();

	// Imguiの更新処理
	void ImguiUpdate();

	// 描画処理
	void Draw2DBack();

	void Draw3D();

	void Draw2DFront();

	void BlackOut();

	void Reset();

private:// メンバ変数

	// 入力
	Input* input = nullptr;

	// オブジェクト共通のライトの初期化
	LightGroup* light = nullptr;
	
	// 平行光源
	bool isActiveDirectional = true;

	// 点光源のステータス
	Vector3 pointLightPos = { 0,1,0 };
	Vector3 pointLightColor = { 1,1,1 };
	Vector3 pointLightAtten = { 0.3f,0.1f,0.1f };
	bool isActivePoint = false;

	// スポットライトのステータス
	Vector3 spotLightDir = { 0,-1,0 };
	Vector3 spotLightPos = { 0,5,0 };
	Vector3 spotLightColor = { 1,1,1 };
	Vector3 spotLightAtten = { 0.0f,0.0f,0.0f };
	Vector2 spotLightFactorAngle = { 20.0f,30.0f };
	bool isActiveSpot = false;

	// 丸影のステータス
	Vector3 circleShadowDir = { 0,-1,0 };
	Vector3 circleShadowAtten = { 1.0f,0.6f,0.0f };
	Vector2 circleShadowFactorAngle = { 0.0f,0.5f };
	bool isActiveCircleShadow = true;

	Vector3 fighterPos = { 1,0.0f,0 };

	// モデル

	// オブジェクト

	ViewProjection* viewProjection_ = nullptr;
	//プレイヤー
	Player* player_ = nullptr;
	//敵
	Enemy* enemy_ = nullptr;

	//プラグ
	Plug* plug_ = nullptr;
	//ソケット
	Socket* socket1_ = nullptr;
	//天球
	SkyBox* skyBox_ = nullptr;
	//ステージ
	Stage* stage_ = nullptr;

	//ドア
	Door* door_ = nullptr;

	// ランプ
	Lamp* lamp_ = nullptr;

	// シーン
	Scene scene = Scene::Title;

#pragma region スプライト
	// テクスチャハンドル
	int otamesiTexHandle = 0;

	// タイトルシーンのスプライト
	Sprite* titleRogo  = nullptr;
	Sprite* Spacekey   = nullptr;
	Sprite* titleStart = nullptr;

	// ステージ選択画面のスプライト
	Sprite* tutorialFont = nullptr;
	Sprite* stage1Font   = nullptr;
	Sprite* stage2Font   = nullptr;
	Sprite* stage3Font   = nullptr;
	Sprite* slectButton  = nullptr;

	float plusSelectPos = 60.0f;

	// ゲームシーンのUIスプライト
	Sprite* gameUpkey    = nullptr;
	Sprite* gameDownkey  = nullptr;
	Sprite* gameLeftkey  = nullptr;
	Sprite* gameRightkey = nullptr;
	Sprite* gameMoveFont = nullptr;
	Sprite* gameHaveFont = nullptr;

	// ステージクリア時のスプライト
	Sprite* clearFont     = nullptr;
	Sprite* replayFont    = nullptr;
	Sprite* backTitleFont = nullptr;

	// ゲームオーバーのスプライト
	Sprite* gameOverFont = nullptr;

	

	// 画面の真ん中のポジション
	Vector2 displayCenter = { WinApp::window_width/2,WinApp::window_height/2 };

#pragma endregion

#pragma region ブラックアウトの変数
	int blackOutTex = 0;
	// シーン推移の黒いスプライト
	Sprite* blackOut = nullptr;

	float blackAlpha = 0.0f;
	bool isClear = false;
	bool sceneChangeFlag = false;
	Scene oldScene = Scene::Title;
	bool resultChange = true;

	int notSousaTimer = 0;
	int notTimerMax = 0.8 * 60;
#pragma endregion

	
};

