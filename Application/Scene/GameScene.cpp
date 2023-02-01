#include "GameScene.h"
#include "Collision.h"
GameScene::~GameScene()
{
	// 入力解放
	delete input;

	// モデルの解放

	delete player_;
	delete enemy_;
	delete plug_;
	delete socket1_;
	delete stage_;
	delete skyBox_;
	delete door_;
	delete lamp_;

	// スプライトの解放
	DeleteSprite();

	// ビューの解放
	delete viewProjection_;
	// ライトの解放
	delete light;
}

void GameScene::Initialize()
{
	// 入力の初期化
	input = new Input();
	input->Initialize();

	// ライトの生成
	light = LightGroup::Create();
	// ライト設定
	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, false);
	light->SetDirLightActive(2, false);


	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);

	light->SetSpotLightActive(0, false);

	light->SetCircleShadowActive(0, true);

	// 3Dオブジェクトにライトをセット
	Object3d::SetLight(light);

	// モデルの読み込み
	viewProjection_ = new ViewProjection;
	viewProjection_->Initialize();
	viewProjection_->eye = { 5,30,-25 };
	viewProjection_->target = { 5,0,0 };

	// オブジェクトの初期化
	skyBox_ = new SkyBox;
	skyBox_->Initialize();

	stage_ = new Stage;
	stage_->Initialize();

	socket1_ = new Socket;
	socket1_->Initialize(Vector3(-2, 0, 2), socket1_->SOUTH);

	plug_ = new Plug();
	plug_->Initialize(Vector3(11.0, 0, -9.0), plug_->WEST);
	plug_->SetStage(stage_);
	plug_->SetSocket(socket1_);

	door_ = new Door;
	door_->Initialize();
	door_->SetPlug(plug_);

	enemy_ = new Enemy;
	enemy_->Initialize(enemy_->EAST, 2);//引数で敵の向きと回転量を決める
	enemy_->SetStage(stage_);

	player_ = new Player();
	player_->Initialize();
	player_->SetStage(stage_);
	player_->SetPlug(plug_);
	player_->SetDoor(door_);
	player_->SetEnemy(enemy_);

	lamp_ = new Lamp;
	lamp_->Initialize();
	lamp_->SetisShining(false);

	SpriteInitialize();


	// とりあえず一回だけ更新を行う
	light->SetCircleShadowDir(0, circleShadowDir);
	light->SetCircleShadowCasterPos(0, player_->GetWorldPosition());
	light->SetCircleShadowAtten(0, circleShadowAtten);
	light->SetCircleShadowFactorAngle(0, circleShadowFactorAngle);
	light->Update();

	viewProjection_->UpdateMatrix();

	//敵更新
	enemy_->Update();
	//プラグ更新
	plug_->Update();
	//player更新
	player_->Update();
	//ステージ更新
	stage_->Update();
	//天球更新
	skyBox_->Update();
	//ソケット更新
	socket1_->Update();
	//ドア更新
	door_->Update();

	// ランプ
	lamp_->SetisShining(plug_->GetIsConnect());
	lamp_->Update();
}

void GameScene::SpriteInitialize()
{
	// 仮の白い板ポリ
	otamesiTexHandle_ = Texture::LoadTexture("kariWhite.png");
	//タイトル画面
	backTexture_ = Texture::LoadTexture("backGround.png");
	title_ = Texture::LoadTexture("title.png");
	space_ = Texture::LoadTexture("space.png");
	start_ = Texture::LoadTexture("start.png");
	//ステージ選択
	tutorial_ = Texture::LoadTexture("tutorial.png");
	stage1_ = Texture::LoadTexture("stage1.png");
	stage2_ = Texture::LoadTexture("stage2.png");
	stage3_ = Texture::LoadTexture("stage3.png");
	select_ = Texture::LoadTexture("select.png");
	//ゲームクリア
	gameClear_ = Texture::LoadTexture("gameClear.png");
	backTitle_ = Texture::LoadTexture("backTitle.png");
	replay_ = Texture::LoadTexture("replay.png");
	//ゲームオーバー
	gameOver_ = Texture::LoadTexture("gameOver.png");


	// シーンチェンジで使うハンドル
	blackOutTex = Texture::LoadTexture("kakusi.png");

	//背景のスプライトの初期化
	backGround = new Sprite;
	backGround->Initialize(backTexture_, { displayCenter.x, displayCenter.y }, { 1280,720 });

	// タイトルシーンのスプライトの初期化
	titleRogo = new Sprite;
	Spacekey = new Sprite;
	titleStart = new Sprite;



	titleRogo->Initialize(title_, { displayCenter.x, displayCenter.y - 50 }, { 900,450 });
	titleStart->Initialize(start_, { displayCenter.x, displayCenter.y + 150 }, { 200, 50 });
	Spacekey->Initialize(space_, { displayCenter.x, displayCenter.y + 220 }, { 200,50 });


	// ステージ選択画面のスプライトの初期化
	tutorialFont = new Sprite;
	stage1Font = new Sprite;
	stage2Font = new Sprite;
	stage3Font = new Sprite;
	slectButton = new Sprite;

	tutorialFont->Initialize(tutorial_, { displayCenter.x, displayCenter.y - 150 }, { 200,100 });
	stage1Font->Initialize(stage1_, { displayCenter.x - 300, displayCenter.y + 80 }, { 200,100 });
	stage2Font->Initialize(stage2_, { displayCenter.x, displayCenter.y + 80 }, { 200,100 });
	stage3Font->Initialize(stage3_, { displayCenter.x + 300, displayCenter.y + 80 }, { 200,100 });
	slectButton->Initialize(select_, { displayCenter.x, displayCenter.y - 70 }, { 50,50 });

	// ゲームシーンのスプライトの初期化
	gameUpkey = new Sprite;
	gameDownkey = new Sprite;
	gameLeftkey = new Sprite;
	gameRightkey = new Sprite;
	gameMoveFont = new Sprite;
	gameHaveFont = new Sprite;

	// ステージクリアのスプライトの初期化
	clearFont = new Sprite;
	replayFont = new Sprite;
	backTitleFont = new Sprite;

	clearFont->Initialize(gameClear_, { displayCenter.x, displayCenter.y - 50 }, { 720,360 });
	replayFont->Initialize(replay_, { displayCenter.x - 200,displayCenter.y + 150 }, { 200, 50 });
	backTitleFont->Initialize(backTitle_, { displayCenter.x + 200,displayCenter.y + 150 }, { 200, 50 });
	// ゲームオーバーのスプライトの初期化
	gameOverFont = new Sprite;
	gameOverFont->Initialize(gameOver_, { displayCenter.x, displayCenter.y - 50 }, { 720,360 });


	blackOut = new Sprite;
	blackOut->Initialize(blackOutTex, displayCenter, { 1280 * 2,720 * 2 });
	blackOut->SetColor({ 1,1,1,blackAlpha });
}

void GameScene::DeleteSprite()
{
	//背景のスプライト
	delete backGround;

	// タイトルシーンのスプライト
	delete titleRogo;
	delete Spacekey;
	delete titleStart;

	// ステージ選択画面のスプライト
	delete tutorialFont;
	delete stage1Font;
	delete stage2Font;
	delete stage3Font;
	delete slectButton;

	// ゲームシーンのUIスプライト
	delete gameUpkey;
	delete gameDownkey;
	delete gameLeftkey;
	delete gameRightkey;
	delete gameMoveFont;
	delete gameHaveFont;

	// ステージクリア時のスプライト
	delete clearFont;
	delete replayFont;
	delete backTitleFont;

	// ゲームオーバーのスプライト
	delete gameOverFont;

	delete blackOut;
}

void GameScene::SpriteDraw()
{
	switch (scene)
	{
	case GameScene::Title:
		backGround->Draw();
		titleRogo->Draw();
		titleStart->Draw();
		Spacekey->Draw();

		break;
	case GameScene::StageSelect:
		backGround->Draw();
		tutorialFont->Draw();
		stage1Font->Draw();
		stage2Font->Draw();
		stage3Font->Draw();
		slectButton->Draw();

		break;
	case GameScene::Game:
		break;
	case GameScene::StageClear:
		backGround->Draw();
		clearFont->Draw();
		replayFont->Draw();
		backTitleFont->Draw();
		slectButton->Draw();
		break;
	case GameScene::GameOver:
		backGround->Draw();
		gameOverFont->Draw();
		replayFont->Draw();
		backTitleFont->Draw();
		slectButton->Draw();
		break;
	default:
		break;
	}
	blackOut->Draw();

}

void GameScene::PointLightUpdate()
{
	pointLightPos = { lamp_->GetPos().x, lamp_->GetPos().y ,lamp_->GetPos().z - 1 };
	if (lamp_->GetisShining() == true) {
		light->SetPointLightActive(0, true);
	}
	else if (lamp_->GetisShining() == false) {
		light->SetPointLightActive(0, false);
	}

	light->SetPointLightPos(0, pointLightPos);
	light->SetPointLightColor(0, pointLightColor);
	light->SetPointLightAtten(0, pointLightAtten);
}

void GameScene::Update()
{
	// 入力の更新
	input->Update();

	// 数字の0キーが押されていたら
	if (input->PushKey(DIK_0))
	{
		OutputDebugStringA("Hit 0\n");  // 出力ウィンドウに「Hit 0」と表示
	}
	notSousaTimer++;

	switch (scene)
	{
	case GameScene::Title:
		if (notSousaTimer >= notTimerMax) {
			if (input->TriggerKey(DIK_SPACE)) {
				// 次のシーンをセット
				oldScene = Scene::Title;
				sceneChangeFlag = true;

			}
		}
		break;
	case GameScene::StageSelect:

		if (notSousaTimer >= notTimerMax) {

			if (input->TriggerKey(DIK_UP)) {
				slectButton->SetPosition({ tutorialFont->GetPosition().x, tutorialFont->GetPosition().y + plusSelectPos + 20 });
			}
			if (input->TriggerKey(DIK_DOWN)) {
				slectButton->SetPosition({ stage2Font->GetPosition().x,stage2Font->GetPosition().y + plusSelectPos + 20 });
			}
			if (input->TriggerKey(DIK_RIGHT) && slectButton->GetPosition().y != tutorialFont->GetPosition().y + plusSelectPos + 20) {
				slectButton->SetPosition({ slectButton->GetPosition().x + 300,slectButton->GetPosition().y });
				if (slectButton->GetPosition().x >= stage3Font->GetPosition().x)
				{
					slectButton->SetPosition({ stage3Font->GetPosition().x ,stage3Font->GetPosition().y + plusSelectPos + 20 });
				}

			}
			if (input->TriggerKey(DIK_LEFT) && slectButton->GetPosition().y != tutorialFont->GetPosition().y + plusSelectPos + 20) {
				slectButton->SetPosition({ slectButton->GetPosition().x - 300,slectButton->GetPosition().y });
				if (slectButton->GetPosition().x <= stage1Font->GetPosition().x) {
					slectButton->SetPosition({ stage1Font->GetPosition().x ,stage1Font->GetPosition().y + plusSelectPos + 20 });

				}
			}


			// ステージ決定
			if (input->TriggerKey(DIK_SPACE)) {
				// チュートリアルステージ
				Vector2 tutorialPos = { tutorialFont->GetPosition().x, tutorialFont->GetPosition().y + plusSelectPos + 20 };
				if (slectButton->GetPosition().x == tutorialPos.x &&
					slectButton->GetPosition().y == tutorialPos.y) {
					oldScene = Scene::StageSelect;
					sceneChangeFlag = true;
				}
				// ステージ１
				else if (slectButton->GetPosition().x == stage1Font->GetPosition().x) {
					oldScene = Scene::StageSelect;
					sceneChangeFlag = true;
				}
				// ステージ２
				else if (slectButton->GetPosition().x == stage2Font->GetPosition().x &&
					slectButton->GetPosition().y == stage2Font->GetPosition().y + plusSelectPos + 20) {
					oldScene = Scene::StageSelect;
					sceneChangeFlag = true;
				}
				// ステージ３
				else if (slectButton->GetPosition().x == stage3Font->GetPosition().x) {
					oldScene = Scene::StageSelect;
					sceneChangeFlag = true;

				}
			}
		}
		break;
	case GameScene::Game:
		//PointLightUpdate();

		light->SetSpotLightDir(0, spotLightDir);
		light->SetSpotLightPos(0, spotLightPos);
		light->SetSpotLightColor(0, spotLightColor);
		light->SetSpotLightAtten(0, spotLightAtten);
		light->SetSpotLightFactorAngle(0, spotLightFactorAngle);

		light->SetCircleShadowDir(0, circleShadowDir);
		light->SetCircleShadowCasterPos(0, player_->GetWorldPosition());
		light->SetCircleShadowAtten(0, circleShadowAtten);
		light->SetCircleShadowFactorAngle(0, circleShadowFactorAngle);
		light->Update();


		viewProjection_->UpdateMatrix();

		//敵更新
		enemy_->Update();
		//プラグ更新
		plug_->Update();

		//player更新
		//player_->SetPlug(plug_);
		player_->Update();
		//ステージ更新
		stage_->Update();
		//天球更新
		skyBox_->Update();
		//ソケット更新
		socket1_->Update();
		//ドア更新
		door_->Update();

		// ランプ
		lamp_->SetisShining(plug_->GetIsConnect());
		lamp_->Update();

		// プレイヤーがドアを通ったら
		if (player_->GetIsClear() == true) {
			isClear = true;
			oldScene = Scene::Game;
			sceneChangeFlag = true;
		}

		// プレイヤーが死亡したらゲームオーバーに移動
		if (player_->GetIsAlive() == false && isClear == false) {
			oldScene = Scene::Game;
			sceneChangeFlag = true;
		}

		// とりあえずループの確認用のZキーでタイトルに戻る
		if (input->TriggerKey(DIK_Z)) {
			scene = Scene::StageSelect;
		}

		break;
	case GameScene::StageClear:
		if (notSousaTimer >= notTimerMax) {
			if (input->TriggerKey(DIK_RIGHT)) {
				slectButton->SetPosition({ backTitleFont->GetPosition().x,backTitleFont->GetPosition().y + plusSelectPos });
				oldScene = Scene::StageClear;
				resultChange = true;
			}
			if (input->TriggerKey(DIK_LEFT)) {
				slectButton->SetPosition({ replayFont->GetPosition().x,replayFont->GetPosition().y + plusSelectPos });
				oldScene = Scene::StageClear;
				resultChange = false;
			}
			if (input->TriggerKey(DIK_SPACE)) {
				if (resultChange == false) {

					sceneChangeFlag = true;
				}
				else if (resultChange == true) {

					sceneChangeFlag = true;
				}
			}
		}
		break;
	case GameScene::GameOver:
		if (notSousaTimer >= notTimerMax) {
			if (input->TriggerKey(DIK_RIGHT)) {
				slectButton->SetPosition({ backTitleFont->GetPosition().x,backTitleFont->GetPosition().y + plusSelectPos });
				oldScene = Scene::GameOver;
				resultChange = true;
			}
			if (input->TriggerKey(DIK_LEFT)) {
				slectButton->SetPosition({ replayFont->GetPosition().x,replayFont->GetPosition().y + plusSelectPos });
				oldScene = Scene::GameOver;
				resultChange = false;
			}
			if (input->TriggerKey(DIK_SPACE)) {
				if (resultChange == false) {

					sceneChangeFlag = true;
				}
				else if (resultChange == true) {

					sceneChangeFlag = true;
				}
			}
		}
		break;
	default:
		break;
	}



	// シーン推移
	BlackOut();


}
void GameScene::ImguiUpdate()
{

	// 表示項目の追加--------//
	ImGui::Begin("Sprite 1");

	//ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 100));

	Vector3 pos = player_->GetWorldPosition();
	ImGui::InputFloat3("plugRotation", &pos.x);
	ImGui::InputFloat3("cord0Rotation", &plug_->cord_[0].gameObject->worldTransform_.rotation_.x);
	if (ImGui::Button("Reset")) {

	}

	ImGui::End();

	ImGui::Begin("Light");
	ImGui::SetNextWindowSize(ImVec2(500, 200));

	// 平行光源
	if (ImGui::TreeNode("DirectionalLight")) {
		ImGui::Checkbox("Is Active", &isActiveDirectional);
		if (isActiveDirectional == true) {
			light->SetDirLightActive(0, true);
		}
		else if (isActiveDirectional == false) {
			light->SetDirLightActive(0, false);
		}

		ImGui::TreePop();
	}
	// ポイントライト
	if (ImGui::TreeNode("PointLight")) {
		ImGui::Checkbox("Is Active", &isActivePoint);

		if (isActivePoint == true) {
			light->SetPointLightActive(0, true);
		}
		else if (isActivePoint == false) {
			light->SetPointLightActive(0, false);
		}

		ImGui::InputFloat3("pointLightPos", &pointLightPos.x);
		ImGui::ColorEdit3("pointLightColor", &pointLightColor.x, ImGuiColorEditFlags_Float);
		ImGui::InputFloat3("pointLightAtten", &pointLightAtten.x);

		ImGui::TreePop();
	}
	// スポットライト
	if (ImGui::TreeNode("spotLight")) {
		ImGui::Checkbox("Is Active", &isActiveSpot);

		if (isActiveSpot == true) {
			light->SetSpotLightActive(0, true);
		}
		else if (isActiveSpot == false) {
			light->SetSpotLightActive(0, false);
		}

		ImGui::InputFloat3("spotLightPos", &spotLightPos.x);
		ImGui::InputFloat3("spotLightDir", &spotLightDir.x);
		ImGui::InputFloat3("spotLightAtten", &spotLightAtten.x);
		ImGui::ColorEdit3("spotLightColor", &spotLightColor.x, ImGuiColorEditFlags_Float);
		ImGui::InputFloat2("spotLightFactorAngle", &spotLightFactorAngle.x);

		ImGui::TreePop();
	}
	// 丸影
	if (ImGui::TreeNode("circleShadow")) {
		ImGui::Checkbox("Is Active", &isActiveCircleShadow);

		if (isActiveCircleShadow == true) {
			light->SetCircleShadowActive(0, true);
		}
		else if (isActiveCircleShadow == false) {
			light->SetCircleShadowActive(0, false);
		}
		ImGui::InputFloat3("circleShadowDir", &circleShadowDir.x);
		//ImGui::ColorEdit3("spotLightColor", &spotLightColor.x, ImGuiColorEditFlags_Float);
		ImGui::InputFloat3("circleShadowAtten", &circleShadowAtten.x);
		ImGui::InputFloat2("circleShadowFactorAngle", &circleShadowFactorAngle.x);
		ImGui::InputFloat3("fighterPos", &fighterPos.x);

		ImGui::TreePop();
	}

	ImGui::End();

	// 当たり判定-----------------------//
	ImGui::Begin("Collision");
	ImGui::SetNextWindowSize(ImVec2(500, 100));


	ImGui::End();
	// ---------------------//

}

void GameScene::Draw2DBack()
{
	//sprite->Draw();
}

void GameScene::Draw3D()
{
	switch (scene)
	{
	case GameScene::Title:
		break;
	case GameScene::StageSelect:

		break;
	case GameScene::Game:
		//3D描画
		//プラグ
		plug_->Draw(viewProjection_);
		//天球
		skyBox_->Draw(viewProjection_);
		//ステージ
		stage_->Draw(viewProjection_);
		//ソケット
		socket1_->Draw(viewProjection_);
		//エネミー
		enemy_->Draw(viewProjection_);
		//プレイヤー
		player_->Draw(viewProjection_);
		//ドア
		door_->Draw(viewProjection_);
		// ランプ
		lamp_->Draw(viewProjection_);
		break;
	case GameScene::StageClear:
		break;
	case GameScene::GameOver:
		break;
	default:
		break;
	}

}

void GameScene::Draw2DFront()
{
	SpriteDraw();
}

void GameScene::BlackOut()
{
	if (sceneChangeFlag == true) {
		switch (scene)
		{
		case GameScene::Scene::Title:
			if (oldScene == Scene::StageClear || oldScene == Scene::GameOver) {
				blackAlpha -= 0.025f;
				blackOut->SetColor({ 1,1,1,blackAlpha });
				if (blackAlpha <= 0) {
					blackAlpha = 0;
					sceneChangeFlag = false;
				}
			}
			else {
				blackAlpha += 0.025f;
				blackOut->SetColor({ 1,1,1,blackAlpha });
				if (blackAlpha >= 1) {
					blackAlpha = 1;
					scene = Scene::StageSelect;
					Reset();
				}
			}

			break;
		case GameScene::Scene::StageSelect:
			if (oldScene == Scene::Title || oldScene == Scene::Game) {
				blackAlpha -= 0.025f;
				blackOut->SetColor({ 1,1,1,blackAlpha });
				if (blackAlpha <= 0) {
					blackAlpha = 0;
					sceneChangeFlag = false;
				}
			}
			else {
				blackAlpha += 0.025f;
				blackOut->SetColor({ 1,1,1,blackAlpha });
				if (blackAlpha >= 1) {
					blackAlpha = 1;
					scene = Scene::Game;
					Reset();
				}
			}
			break;
		case GameScene::Scene::Game:
			if (oldScene == Scene::StageSelect || oldScene == Scene::GameOver || oldScene == Scene::StageClear) {
				blackAlpha -= 0.025f;
				blackOut->SetColor({ 1,1,1,blackAlpha });
				if (blackAlpha <= 0) {
					blackAlpha = 0;
					sceneChangeFlag = false;
				}
			}
			// ゲームシーンからリザルトシーン
			else {
				blackAlpha += 0.025f;
				blackOut->SetColor({ 1,1,1,blackAlpha });
				if (blackAlpha >= 1) {
					blackAlpha = 1;
					if (isClear == false) {
						scene = Scene::GameOver;
						Reset();
					}
					else if (isClear == true) {
						scene = Scene::StageClear;
						Reset();
					}

				}
			}
			break;
		case GameScene::Scene::StageClear:
			if (oldScene == Scene::Game) {
				blackAlpha -= 0.025f;
				blackOut->SetColor({ 1,1,1,blackAlpha });
				if (blackAlpha <= 0) {
					blackAlpha = 0;
					sceneChangeFlag = false;
				}
			}
			else {
				blackAlpha += 0.025f;
				blackOut->SetColor({ 1,1,1,blackAlpha });
				if (blackAlpha >= 1) {
					blackAlpha = 1;

					if (resultChange == false) {
						scene = Scene::Game;
						Reset();
					}
					else if (resultChange == true) {
						scene = Scene::Title;
						Reset();
					}
				}
			}
			break;

		case GameScene::Scene::GameOver:
			if (oldScene == Scene::Game) {
				blackAlpha -= 0.025f;
				blackOut->SetColor({ 1,1,1,blackAlpha });
				if (blackAlpha <= 0) {
					blackAlpha = 0;
					sceneChangeFlag = false;
				}
			}
			else {
				blackAlpha += 0.025f;
				blackOut->SetColor({ 1,1,1,blackAlpha });
				if (blackAlpha >= 1) {
					blackAlpha = 1;

					if (resultChange == false) {
						scene = Scene::Game;
						Reset();
					}
					else if (resultChange == true) {
						scene = Scene::Title;
						Reset();
					}
				}
			}
			break;
		default:
			break;
		}
	}
}

void GameScene::Reset()
{
	switch (scene)
	{
	case GameScene::Title:
		notSousaTimer = 0;
		break;
	case GameScene::StageSelect:

		notSousaTimer = 0;


		slectButton->SetPosition({ tutorialFont->GetPosition().x, tutorialFont->GetPosition().y + plusSelectPos + 20 });

		break;
	case GameScene::Game:
		notSousaTimer = 0;
		isClear = false;
		player_->Reset();
		enemy_->Reset(enemy_->EAST, 2);
		plug_->Reset(Vector3(12.0, 0, -10.0), plug_->WEST);
		socket1_->Reset(Vector3(-2, 0, 2), socket1_->SOUTH);
		door_->Reset();
		lamp_->Reset();
		stage_->TutorialReset();
		break;
	case GameScene::StageClear:
		notSousaTimer = 0;
		resultChange = true;
		slectButton->SetPosition({ backTitleFont->GetPosition().x,backTitleFont->GetPosition().y + plusSelectPos });
		break;
	case GameScene::GameOver:
		notSousaTimer = 0;
		resultChange = true;
		slectButton->SetPosition({ backTitleFont->GetPosition().x,backTitleFont->GetPosition().y + plusSelectPos });
		break;
	default:
		break;
	}


}
