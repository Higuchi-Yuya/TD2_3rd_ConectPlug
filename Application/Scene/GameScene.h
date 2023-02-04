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

	enum StageNum
	{
		Tutorial,
		Stage1,
		Stage2,
		Stage3,
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

	// ステージごとの更新処理
	void StageUpdate();

	// ステージごとの描画処理
	void StageDraw();

	// Imguiの更新処理
	void ImguiUpdate();

	// 描画処理
	void Draw2DBack();

	void Draw3D();

	void Draw2DFront();

	void BlackOut();

	void Reset();

	void StartCameraWork(ViewProjection* viewProjection_);

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

	//ビュー
	ViewProjection* viewProjection_ = nullptr;

	//カメラワークのフラグ
	bool isCameraStart_ = false;

	//プレイヤー
	Player* player_ = nullptr;
	//敵
	Enemy* enemy_[3];
	int enemyCount = 3;
	//プラグ
	Plug* plug_[3];
	//ソケット
	Socket* socket1_[3];
	//天球
	SkyBox* skyBox_ = nullptr;
	//ステージ
	Stage* stage_ = nullptr;

	//ドア
	Door* door_ = nullptr;

	// ランプ
	Lamp* lamp_[3];

	// シーン
	Scene scene = Scene::Title;

	// ステージ
	StageNum stageNum = StageNum::Tutorial;

#pragma region スプライト
	// テクスチャハンドル
	//ダミー
	int otamesiTexHandle_ = 0;
	////タイトル画面////
	int backTexture_ = 0;
	int start_ = 0;
	int title_ = 0;
	int space_ = 0;

	////ステージ選択////
	int tutorial_ = 0;
	int stage1_ = 0;
	int stage2_ =	 0;
	int stage3_ = 0;
	int select_ = 0;

	////ゲームシーン////
	int offGlabPlug_ = 0;
	int onGlabPlug_ = 0;
	//押されていないキー
	int offUp_ = 0;
	int offDown_ = 0;
	int offRight_ = 0;
	int offLeft_ = 0;
	//押されているキー
	int onUp_ = 0;
	int onDown_ = 0;
	int onRight_ = 0;
	int onLeft_ = 0;

	////ゲームクリア////
	int gameClear_ = 0;
	int backTitle_ = 0;
	int replay_ = 0;

	////ゲームオーバー//
	int gameOver_ = 0;

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
	Sprite* offUpKey    = nullptr;
	Sprite* offDownKey  = nullptr;
	Sprite* offLeftKey  = nullptr;
	Sprite* offRightKey = nullptr;
	Sprite* onUpKey = nullptr;
	Sprite* onDownKey = nullptr;
	Sprite* onLeftKey = nullptr;
	Sprite* onRightKey = nullptr;
	Sprite* offGlabFont = nullptr;
	Sprite* onGlabFont = nullptr;

	// ステージクリア時のスプライト
	Sprite* clearFont     = nullptr;
	Sprite* replayFont    = nullptr;
	Sprite* backTitleFont = nullptr;

	// ゲームオーバーのスプライト
	Sprite* gameOverFont = nullptr;

	//背景のスプライト
	Sprite* backGround = nullptr;

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

