#include "GameScene.h"
#include "Collision.h"
GameScene::~GameScene()
{
	// 入力解放
	delete input;

	// モデルの解放

	delete player_;
	for (int i = 0; i < 3; i++) {
		delete plug_[i];
		delete socket1_[i];
		delete lamp_[i];
		
	}
	for (int i = 0; i < 6; i++) {
		delete enemy_[i];
	}
	delete stage_;
	delete skyBox_;
	delete door_;


	// スプライトの解放
	DeleteSprite();

	// ビューの解放
	delete viewProjection_;
	// ライトの解放
	delete light;

	// サウンドの解放
	delete titleBGM;
	delete gameBGM;
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

	//カメラの初期化と位置設定
	viewProjection_ = new ViewProjection;
	viewProjection_->Initialize();
	viewProjection_->eye = {5,50,-5};
	viewProjection_->target = { 5,0,0 };

	// オブジェクトの初期化
	skyBox_ = new SkyBox;
	skyBox_->Initialize();

	stage_ = new Stage;
	stage_->Initialize();

	for (int i = 0; i < 3; i++) {
		socket1_[i] = new Socket;
		socket1_[i]->Initialize(Vector3(-2, 0, 3), socket1_[i]->SOUTH);

		plug_[i] = new Plug();
		plug_[i]->Initialize(Vector3(11.0, 0, -9.0), plug_[i]->WEST);
		plug_[i]->SetStage(stage_);

	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			plug_[i]->SetSocket(socket1_[j], false);

		}
	}
	for (int i = 0; i < enemyCount; i++) {

		enemy_[i] = new Enemy;
		enemy_[i]->Initialize({ 0,0,-2 }, enemy_[i]->EAST, 2);//引数で敵の向きと回転量を決める
		enemy_[i]->SetStage(stage_);
		for (int j = 0; j < 3; j++) {
			enemy_[i]->SetPlug(plug_[j]);
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < enemyCount; j++) {
			plug_[i]->SetEnemySocket(enemy_[j]->GetSocket(), true);
		}
	}

	
	



	door_ = new Door;
	door_->Initialize();
	

	player_ = new Player();
	player_->Initialize();
	player_->SetStage(stage_);
	for (int i = 0; i < 3; i++) {
		door_->SetPlug(plug_[i]);
		player_->SetPlug(plug_[i]);
		lamp_[i] = new Lamp;
		lamp_[i]->Initialize({ 4,2 + (float)i * 100,2 });
		lamp_[i]->SetisShining(false);
	}
	for (int i = 0; i < enemyCount; i++) {
		
		player_->SetEnemy(enemy_[i]);
	}
	player_->SetDoor(door_);
	
	// サウンドの初期化
	titleBGM = new Sound;
	gameBGM = new Sound;
	selectSE = new Sound;
	titleBGM->SoundLoadWave("Resources/Sound/TitleBGMConnectPlug.wav");
	gameBGM->SoundLoadWave("Resources/Sound/GameBGMConnectPlug.wav");
	selectSE->SoundLoadWave("Resources/Sound/ConnectSE2.wav");
	isTitleBGM = true;
	isGameBGM = true;
	

	SpriteInitialize();


	// とりあえず一回だけ更新を行う
	light->SetCircleShadowDir(0, circleShadowDir);
	light->SetCircleShadowCasterPos(0, player_->GetWorldPosition());
	light->SetCircleShadowAtten(0, circleShadowAtten);
	light->SetCircleShadowFactorAngle(0, circleShadowFactorAngle);
	light->Update();

	viewProjection_->UpdateMatrix();

	for (int i = 0; i < 3; i++) {
		//ソケット更新
		socket1_[i]->Update();
		//プラグ更新
		plug_[i]->Update();
		// ランプ
		lamp_[i]->SetisShining(plug_[i]->GetIsConnect());
		lamp_[i]->Update();
	}

	//敵更新
	for (int i = 0; i < enemyCount; i++) {
		enemy_[i]->Update();
	}
	//player更新
	player_->Update();
	//ステージ更新
	stage_->Update();
	//天球更新
	skyBox_->Update();
	
	//ドア更新
	door_->Update();

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
	//ゲームシーン
	offGlabPlug_ = Texture::LoadTexture("offGlabPlug.png");
	onGlabPlug_ = Texture::LoadTexture("onGlabPlug.png");
	//押されていないキー
	offUp_ = Texture::LoadTexture("offUp.png");
	offDown_ = Texture::LoadTexture("offDown.png");
	offRight_ = Texture::LoadTexture("offRight.png");
	offLeft_ = Texture::LoadTexture("offLeft.png");
	//押されているキー
	onUp_ = Texture::LoadTexture("onUp.png");
	onDown_ = Texture::LoadTexture("onDown.png");
	onRight_ = Texture::LoadTexture("onRight.png");
	onLeft_ = Texture::LoadTexture("onLeft.png");
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
	//矢印キー
	//押されていない
	offUpKey = new Sprite;
	offDownKey = new Sprite;
	offLeftKey = new Sprite;
	offRightKey = new Sprite;
	//押されている
	onUpKey = new Sprite;
	onDownKey = new Sprite;
	onLeftKey = new Sprite;
	onRightKey = new Sprite;
	//状態
	offGlabFont = new Sprite;
	onGlabFont = new Sprite;

	//各キーの初期化
	//押されていない
	offUpKey->Initialize(offUp_, { displayCenter.x - 500, displayCenter.y + 40 }, { 50,50 });
	offDownKey->Initialize(offDown_, { displayCenter.x - 500, displayCenter.y + 160 }, { 50,50 });
	offLeftKey->Initialize(offLeft_, { displayCenter.x - 560, displayCenter.y + 100 }, { 50,50 });
	offRightKey->Initialize(offRight_, { displayCenter.x - 440, displayCenter.y + 100 }, { 50,50 });
	//押されている
	onUpKey->Initialize(onUp_, { displayCenter.x - 500, displayCenter.y + 40 }, { 50,50 });
	onDownKey->Initialize(onDown_, { displayCenter.x - 500, displayCenter.y + 160 }, { 50,50 });
	onLeftKey->Initialize(onLeft_, { displayCenter.x - 560, displayCenter.y + 100 }, { 50,50 });
	onRightKey->Initialize(onRight_, { displayCenter.x - 440, displayCenter.y + 100 }, { 50,50 });
	//状態の初期化
	offGlabFont->Initialize(offGlabPlug_, { displayCenter.x - 500, displayCenter.y - 100 }, { 300,75 });
	onGlabFont->Initialize(onGlabPlug_, { displayCenter.x - 500, displayCenter.y - 100 }, { 300,75 });


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
	delete offUpKey;
	delete offDownKey;
	delete offLeftKey;
	delete offRightKey;
	delete onUpKey;
	delete onDownKey;
	delete onLeftKey;
	delete onRightKey;
	delete offGlabFont;
	delete onGlabFont;

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
		//対応したキーが押されていたら変化させる
		if (player_->GetIsUp() == true)
		{
			onUpKey->Draw();
		}
		else
		{
			offUpKey->Draw();
		}
		if (player_->GetIsDown() == true)
		{
			onDownKey->Draw();
		}
		else
		{
			offDownKey->Draw();
		}
		if (player_->GetIsRight() == true)
		{
			onRightKey->Draw();
		}
		else
		{
			offRightKey->Draw();
		}
		if (player_->GetIsLeft() == true)
		{
			onLeftKey

				->Draw();
		}
		else
		{
			offLeftKey->Draw();
		}
		//プラグをつかんでいるか
		for (int i = 0; i < 3; i++) {
			if (plug_[i]->GetIsGrabbed() == true)
			{
				onGlabFont->Draw();
				break;
			}
			else
			{
				offGlabFont->Draw();
			}
		}
		
		Spacekey->Draw();
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
	/*pointLightPos = { lamp_->GetPos().x, lamp_->GetPos().y ,lamp_->GetPos().z - 1 };
	if (lamp_->GetisShining() == true) {
		light->SetPointLightActive(0, true);
	}
	else if (lamp_->GetisShining() == false) {
		light->SetPointLightActive(0, false);
	}*/

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
		if (isTitleBGM == true) {
			titleBGM->SoundPlayWave(true, 0.8f);
			isTitleBGM = false;
		}
		Spacekey->SetPosition(Vector2{ displayCenter.x, displayCenter.y + 220 });

		if (notSousaTimer >= notTimerMax) {
			if (input->TriggerKey(DIK_SPACE)) {
				// 次のシーンをセット
				oldScene = Scene::Title;
				selectSE->SoundPlayWave(false, 1.0f);
				sceneChangeFlag = true;

			}
		}
		break;
	case GameScene::StageSelect:

		if (notSousaTimer >= notTimerMax) {

			if (input->TriggerKey(DIK_UP)) {
				slectButton->SetPosition({ tutorialFont->GetPosition().x, tutorialFont->GetPosition().y + plusSelectPos + 20 });
				selectSE->SoundPlayWave(false, 1.0f);
			}
			if (input->TriggerKey(DIK_DOWN)) {
				slectButton->SetPosition({ stage2Font->GetPosition().x,stage2Font->GetPosition().y + plusSelectPos + 20 });
				selectSE->SoundPlayWave(false, 1.0f);
			}
			if (input->TriggerKey(DIK_RIGHT) && slectButton->GetPosition().y != tutorialFont->GetPosition().y + plusSelectPos + 20) {
				slectButton->SetPosition({ slectButton->GetPosition().x + 300,slectButton->GetPosition().y });
				if (slectButton->GetPosition().x >= stage3Font->GetPosition().x)
				{
					slectButton->SetPosition({ stage3Font->GetPosition().x ,stage3Font->GetPosition().y + plusSelectPos + 20 });
				}
				selectSE->SoundPlayWave(false, 1.0f);
			}
			if (input->TriggerKey(DIK_LEFT) && slectButton->GetPosition().y != tutorialFont->GetPosition().y + plusSelectPos + 20) {
				slectButton->SetPosition({ slectButton->GetPosition().x - 300,slectButton->GetPosition().y });
				if (slectButton->GetPosition().x <= stage1Font->GetPosition().x) {
					slectButton->SetPosition({ stage1Font->GetPosition().x ,stage1Font->GetPosition().y + plusSelectPos + 20 });

				}
				selectSE->SoundPlayWave(false, 1.0f);
			}


			// ステージ決定
			if (input->TriggerKey(DIK_SPACE)) {
				// チュートリアルステージ
				Vector2 tutorialPos = { tutorialFont->GetPosition().x, tutorialFont->GetPosition().y + plusSelectPos + 20 };
				if (slectButton->GetPosition().x == tutorialPos.x &&
					slectButton->GetPosition().y == tutorialPos.y) {
					oldScene = Scene::StageSelect;
					stageNum = StageNum::Tutorial;
					sceneChangeFlag = true;
				}
				// ステージ１
				else if (slectButton->GetPosition().x == stage1Font->GetPosition().x) {
					oldScene = Scene::StageSelect;
					stageNum = StageNum::Stage1;
					sceneChangeFlag = true;
				}
				// ステージ２
				else if (slectButton->GetPosition().x == stage2Font->GetPosition().x &&
					slectButton->GetPosition().y == stage2Font->GetPosition().y + plusSelectPos + 20) {
					oldScene = Scene::StageSelect;
					stageNum = StageNum::Stage2;
					sceneChangeFlag = true;
				}
				// ステージ３
				else if (slectButton->GetPosition().x == stage3Font->GetPosition().x) {
					oldScene = Scene::StageSelect;
					stageNum = StageNum::Stage3;
					sceneChangeFlag = true;

				}
				titleBGM->StopWave();
				selectSE->SoundPlayWave(false, 1.0f);
			}
		}
		break;
	case GameScene::Game:
		// ゲームのBGMを鳴らす
		if (isGameBGM == true) {
			gameBGM->SoundPlayWave(true, 0.8f);
			isGameBGM = false;
		}
		Spacekey->SetPosition(Vector2{ displayCenter.x - 500, displayCenter.y - 200 });

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

		//カメラワーク
		StartCameraWork(viewProjection_);
		viewProjection_->UpdateMatrix();

		if (isCameraStart_ == false)
		{
			// ステージごとの更新処理
			StageUpdate();

			//ドア更新
			door_->Update();
			//player更新
			//player_->SetPlug(plug_);
			player_->Update();
			//ステージ更新
			stage_->Update();
			//天球更新
			skyBox_->Update();

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
				oldScene = Scene::Game;
				isGameReset = true;
				sceneChangeFlag = true;
			}
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
					oldScene = Scene::StageClear;
					gameBGM->StopWave();
					sceneChangeFlag = true;
				}
				else if (resultChange == true) {
					oldScene = Scene::StageClear;
					gameBGM->StopWave();
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
					oldScene = Scene::GameOver;
					gameBGM->StopWave();
					sceneChangeFlag = true;
				}
				else if (resultChange == true) {
					oldScene = Scene::GameOver;
					gameBGM->StopWave();
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

void GameScene::StageUpdate()
{
	switch (stageNum)
	{
	case GameScene::Tutorial:
		//敵更新
		for (int i = 0; i < 1; i++) {
			enemy_[i]->Update();
		}


		for (int i = 0; i < 1; i++) {
			//プラグ更新
			plug_[i]->Update();
			//ソケット更新
			socket1_[i]->Update();
			

			// ランプ
			lamp_[i]->SetisShining(plug_[i]->GetIsConnect());
			lamp_[i]->Update();
		}
		break;
	case GameScene::Stage1:
		//敵更新
		for (int i = 0; i < 2; i++) {
			enemy_[i]->Update();
		}


		for (int i = 0; i < 2; i++) {
			//プラグ更新
			plug_[i]->Update();
			//ソケット更新
			socket1_[i]->Update();


			// ランプ
			lamp_[i]->SetisShining(plug_[i]->GetIsConnect());
			lamp_[i]->Update();
		}
		break;
	case GameScene::Stage2:
		//敵更新
		for (int i = 0; i < 6; i++) {
			enemy_[i]->Update();
		}


		for (int i = 0; i < 1; i++) {
			//プラグ更新
			plug_[i]->Update();
			//ソケット更新
			socket1_[i]->Update();


			// ランプ
			lamp_[i]->SetisShining(plug_[i]->GetIsConnect());
			lamp_[i]->Update();
		}
		break;
	case GameScene::Stage3:
		//敵更新
		for (int i = 0; i < 2; i++) {
			enemy_[i]->Update();
		}


		for (int i = 0; i < 2; i++) {
			//プラグ更新
			plug_[i]->Update();
			//ソケット更新
			socket1_[i]->Update();


			// ランプ
			lamp_[i]->SetisShining(plug_[i]->GetIsConnect());
			lamp_[i]->Update();
		}
		break;
	default:
		break;
	}
}

void GameScene::StageDraw()
{
	switch (stageNum)
	{
	case GameScene::Tutorial:
		for (int i = 0; i < 1; i++) {
			//プラグ
			plug_[i]->Draw(viewProjection_);
			//ソケット
			socket1_[i]->Draw(viewProjection_);
			// ランプ
			lamp_[i]->Draw(viewProjection_);
		}
		for (int i = 0; i < 1; i++) {
			//エネミー
			enemy_[i]->Draw(viewProjection_);
		}
		break;
	case GameScene::Stage1:
		for (int i = 0; i < 2; i++) {
			//プラグ
			plug_[i]->Draw(viewProjection_);
			//ソケット
			socket1_[i]->Draw(viewProjection_);
			// ランプ
			lamp_[i]->Draw(viewProjection_);
		}
		for (int i = 0; i < 2; i++) {
			//エネミー
			enemy_[i]->Draw(viewProjection_);
		}
		break;
	case GameScene::Stage2:
		for (int i = 0; i < 1; i++) {
			//プラグ
			plug_[i]->Draw(viewProjection_);
			//ソケット
			socket1_[i]->Draw(viewProjection_);
			// ランプ
			lamp_[i]->Draw(viewProjection_);
		}
		for (int i = 0; i < 6; i++) {
			//エネミー
			enemy_[i]->Draw(viewProjection_);
		}
		break;
	case GameScene::Stage3:
		for (int i = 0; i < 2; i++) {
			//プラグ
			plug_[i]->Draw(viewProjection_);
			//ソケット
			socket1_[i]->Draw(viewProjection_);
			// ランプ
			lamp_[i]->Draw(viewProjection_);
		}
		for (int i = 0; i < 2; i++) {
			//エネミー
			enemy_[i]->Draw(viewProjection_);
		}
		break;
	default:
		break;
	}
}

void GameScene::ImguiUpdate()
{

	// 表示項目の追加--------//
	ImGui::Begin("Sprite 1");

	//ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 100));

	Vector3 pos = player_->GetWorldPosition();
	ImGui::InputFloat3("plugRotation", &pos.x);
	//ImGui::InputFloat3("cord0Rotation", &plug_->cord_[0].gameObject->worldTransform_.rotation_.x);
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
		StageDraw();

		//天球
		skyBox_->Draw(viewProjection_);
		//ステージ
		stage_->Draw(viewProjection_);
		
		//プレイヤー
		player_->Draw(viewProjection_);
		//ドア
		door_->Draw(viewProjection_);
		
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

//シーン遷移
void GameScene::BlackOut()
{
	if (sceneChangeFlag == true) {
		switch (scene)
		{
		case GameScene::Scene::Title:
			if (oldScene == Scene::StageClear || 
				oldScene == Scene::GameOver) {

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
			if (oldScene == Scene::Title || 
				oldScene == Scene::Game) {

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
			if (oldScene == Scene::StageSelect || 
				oldScene == Scene::GameOver || 
				oldScene == Scene::StageClear||
				oldScene==Scene::GameReset) {

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
					if (isGameReset == true) {
						oldScene = Scene::GameReset;
						gameBGM->StopWave();
						scene = Scene::Game;
						Reset();
					}
					else if (isClear == false) {
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
		isTitleBGM = true;
		notSousaTimer = 0;
		break;
	case GameScene::StageSelect:

		notSousaTimer = 0;


		slectButton->SetPosition({ tutorialFont->GetPosition().x, tutorialFont->GetPosition().y + plusSelectPos + 20 });

		break;
	case GameScene::Game:
		
		

		switch (stageNum)
		{
		case GameScene::Tutorial:
			stage_->TutorialReset();

			viewProjection_->eye = { 5,50,-5 };
			viewProjection_->target = { 5,0,0 };
			notSousaTimer = 0;
			isCameraStart_ = true;
			isClear = false;
			player_->Reset({ 7, 0, -5 });
			// エネミーのリセット
			enemy_[0]->Reset({0,0,-2}, enemy_[0]->EAST, 2,0);
			enemy_[1]->Reset({ 0,100,-2 }, enemy_[1]->EAST, 2,1);
			enemy_[2]->Reset({ 0,100,-2 }, enemy_[2]->EAST, 2,2);
			enemy_[3]->Reset({ 0,100,-2 }, enemy_[2]->EAST, 2,3);
			enemy_[4]->Reset({ 0,100,-2 }, enemy_[2]->EAST, 2,4);
			enemy_[5]->Reset({ 0,100,-2 }, enemy_[2]->EAST, 2,5);

			// プラグのリセット
			plug_[0]->Reset(Vector3(12.0, 0, -10.0), plug_[0]->WEST);
			plug_[1]->Reset(Vector3(12.0, 100, -10.0), plug_[1]->WEST);
			plug_[2]->Reset(Vector3(12.0, 100, -10.0), plug_[2]->WEST);

			// ソケットのリセット
			socket1_[0]->Reset(Vector3(-2, 0, 2), socket1_[0]->SOUTH);
			socket1_[1]->Reset(Vector3(-2, 100, 2), socket1_[1]->SOUTH);
			socket1_[2]->Reset(Vector3(-2, 100, 2), socket1_[2]->SOUTH);
			door_->Reset(1);

			// ランプのリセット
			lamp_[0]->Reset({4,2,2});
			lamp_[1]->Reset({ 6,100,2 });
			lamp_[2]->Reset({ 8,100,2 });
			break;
		case GameScene::Stage1:
			stage_->Stage1Reset();

			viewProjection_->eye = { 5,50,-5 };
			viewProjection_->target = { 5,0,0 };
			notSousaTimer = 0;
			isCameraStart_ = true;
			isClear = false;
			player_->Reset({ 7, 0, -5 });
			door_->Reset(2);

			// エネミーのリセット
			enemy_[0]->Reset({ 12,0,-2 }, enemy_[0]->WEST, 2,0);
			enemy_[1]->Reset({ -2,0,-10 }, enemy_[1]->EAST, 2,1);
			enemy_[2]->Reset({ 0,100,-2 }, enemy_[2]->EAST, 2,2);
			enemy_[3]->Reset({ 0,100,-2 }, enemy_[2]->EAST, 2,3);
			enemy_[4]->Reset({ 0,100,-2 }, enemy_[2]->EAST, 2,4);
			enemy_[5]->Reset({ 0,100,-2 }, enemy_[2]->EAST, 2,5);

			
			// プラグのリセット
			plug_[0]->Reset(Vector3(4.0, 2, -12.0), plug_[0]->NORTH);
			plug_[1]->Reset(Vector3(12.0, 0, -8.0), plug_[1]->WEST);
			plug_[2]->Reset(Vector3(12.0, 100, -10.0), plug_[2]->WEST);

			// ソケットのリセット
			socket1_[0]->Reset(Vector3(-2, 2, 2), socket1_[0]->SOUTH);
			socket1_[1]->Reset(Vector3(6, 0, 4), socket1_[1]->SOUTH);
			socket1_[2]->Reset(Vector3(-2, 100, 2), socket1_[2]->SOUTH);

			// ランプのリセット
			lamp_[0]->Reset({ 4,2,2 });
			lamp_[1]->Reset({ 6,2,2 });
			lamp_[2]->Reset({ 8,100,2 });
			break;
		case GameScene::Stage2:
			stage_->Stage2Reset();

			viewProjection_->eye = { 5,50,-5 };
			viewProjection_->target = { 5,0,0 };
			notSousaTimer = 0;
			isCameraStart_ = true;
			isClear = false;
			player_->Reset({ 6, 0, -10 });
			door_->Reset(1);

			// エネミーのリセット
			enemy_[0]->Reset({ -2,0,0 }, enemy_[0]->EAST, 2,0);
			enemy_[1]->Reset({ -2,100,-8 }, enemy_[1]->EAST, 2,1);
			enemy_[2]->Reset({ -2,0,-12 }, enemy_[2]->NORTH, 2,2);
			enemy_[3]->Reset({ 8,0,+2 }, enemy_[3]->SOUTH, 2,3);
			enemy_[4]->Reset({ 10,0,-2 }, enemy_[4]->WEST, 2,4);
			enemy_[5]->Reset({ 12,0,-10 }, enemy_[5]->NORTH, 2,5);

			// プラグのリセット
			plug_[0]->Reset(Vector3(4.0, 0, -12.0), plug_[0]->NORTH);
			plug_[1]->Reset(Vector3(12.0, 100, -10.0), plug_[1]->WEST);
			plug_[2]->Reset(Vector3(12.0, 100, -10.0), plug_[2]->WEST);

			// ソケットのリセット
			socket1_[0]->Reset(Vector3(6, 0, 2), socket1_[0]->SOUTH);
			socket1_[1]->Reset(Vector3(-2, 100, 2), socket1_[1]->SOUTH);
			socket1_[2]->Reset(Vector3(-2, 100, 2), socket1_[2]->SOUTH);

			// ランプのリセット
			lamp_[0]->Reset({ 4,2,2 });
			lamp_[1]->Reset({ 8,100,2 });
			lamp_[2]->Reset({ 8,100,2 });
			break;
		case GameScene::Stage3:
			stage_->Stage3Reset();

			viewProjection_->eye = { 5,50,-5 };
			viewProjection_->target = { 5,0,0 };
			notSousaTimer = 0;
			isCameraStart_ = true;
			isClear = false;
			player_->Reset({ 6, 0, -10 });
			door_->Reset(2);

			// エネミーのリセット
			enemy_[0]->Reset({ 12,0,-2 }, enemy_[0]->WEST, 2,0);
			enemy_[1]->Reset({ -2,0,-10 }, enemy_[1]->EAST, 2,1);
			enemy_[2]->Reset({ 0,100,-2 }, enemy_[2]->EAST, 2,2);
			enemy_[3]->Reset({ 0,100,-2 }, enemy_[2]->EAST, 2,3);
			enemy_[4]->Reset({ 0,100,-2 }, enemy_[2]->EAST, 2,4);
			enemy_[5]->Reset({ 0,100,-2 }, enemy_[2]->EAST, 2,5);

			// プラグのリセット
			plug_[0]->Reset(Vector3(4.0, 2, -12.0), plug_[0]->NORTH);
			plug_[1]->Reset(Vector3(12.0, 0, -8.0), plug_[1]->WEST);
			plug_[2]->Reset(Vector3(12.0, 100, -10.0), plug_[2]->WEST);

			// ソケットのリセット
			socket1_[0]->Reset(Vector3(-2, 2, 2), socket1_[0]->SOUTH);
			socket1_[1]->Reset(Vector3(6, 0, 4), socket1_[1]->SOUTH);
			socket1_[2]->Reset(Vector3(-2, 100, 2), socket1_[2]->SOUTH);

			// ランプのリセット
			lamp_[0]->Reset({ 4,2,2 });
			lamp_[1]->Reset({ 6,2,2 });
			lamp_[2]->Reset({ 8,100,2 });
			break;
		default:
			break;
		}

		// とりあえず一回だけ更新を行う
		light->SetCircleShadowDir(0, circleShadowDir);
		light->SetCircleShadowCasterPos(0, player_->GetWorldPosition());
		light->SetCircleShadowAtten(0, circleShadowAtten);
		light->SetCircleShadowFactorAngle(0, circleShadowFactorAngle);
		light->Update();

		viewProjection_->UpdateMatrix();

		//敵更新
		for (int i = 0; i < enemyCount; i++) {
			enemy_[i]->Update();
		}
		for (int i = 0; i < 3; i++) {
			//プラグ更新
			plug_[i]->Update();
			//ソケット更新
			socket1_[i]->Update();
			// ランプ
			lamp_[i]->SetisShining(plug_[i]->GetIsConnect());
			lamp_[i]->Update();
		}
		//player更新
		player_->Update();
		//ステージ更新
		stage_->Update();
		//天球更新
		skyBox_->Update();
		
		//ドア更新
		door_->Update();
		
		// gameBGMを鳴らす
		isGameBGM = true;
		// リセットボタンのフラグ
		isGameReset = false;
		

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

void GameScene::StartCameraWork(ViewProjection* viewProjection_)
{
		
	if (isCameraStart_ == true)
	{
		viewProjection_->eye.y -= 0.25f;
		viewProjection_->eye.z -= 0.21f;
	if (viewProjection_->eye.y <= 30 && viewProjection_->eye.z <= -22)
		{
			viewProjection_->eye.y = 30;
			viewProjection_->eye.z = -22;
			isCameraStart_ = false;
		}
	}
}
