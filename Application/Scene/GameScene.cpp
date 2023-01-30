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
	socket1_->Initialize(Vector3(0, 0, -9), socket1_->SOUTH);

	plug_ = new Plug();
	plug_->Initialize(Vector3(11.0, 0, -9.0), plug_->WEST);
	plug_->SetStage(stage_);
	plug_->SetSocket(socket1_);

	door_ = new Door;
	door_->Initialize();
	door_->SetPlug(plug_);

	enemy_ = new Enemy;
	enemy_->Initialize( enemy_->EAST, 2);//引数で敵の向きと回転量を決める
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


	switch (scene)
	{
	case GameScene::Title:
		if (input->TriggerKey(DIK_SPACE)) {
			// 次のシーンをセット
			scene = Scene::Game;
			// 次のシーンのリセットをする
			Reset();
		}
		break;
	case GameScene::StageSelect:
		break;
	case GameScene::Game:
		light->SetPointLightPos(0, Vector3(pointLightPos[0], pointLightPos[1], pointLightPos[2]));
		light->SetPointLightColor(0, Vector3(pointLightColor[0], pointLightColor[1], pointLightColor[2]));
		light->SetPointLightAtten(0, Vector3(pointLightAtten[0], pointLightAtten[1], pointLightAtten[2]));

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

		// とりあえずループの確認用のZキーでタイトルに戻る
		if (input->TriggerKey(DIK_Z)) {
			scene = Scene::Title;
		}

		break;
	default:
		break;
	}

	// 球移動

}

void GameScene::ImguiUpdate()
{

	// 表示項目の追加--------//
	ImGui::Begin("Sprite 1");

	//ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 100));

	ImGui::InputFloat3("plugRotation", &plug_->plug_->worldTransform_.rotation_.x);
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
		else if(isActiveDirectional == false) {
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

		ImGui::InputFloat3("pointLightPos", pointLightPos);
		ImGui::ColorEdit3("pointLightColor", pointLightColor, ImGuiColorEditFlags_Float);
		ImGui::InputFloat3("pointLightAtten", pointLightAtten);

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
	default:
		break;
	}

}

void GameScene::Draw2DFront()
{

}

void GameScene::Reset()
{
	switch (scene)
	{
	case GameScene::Title:
		break;
	case GameScene::StageSelect:
		break;
	case GameScene::Game:
		player_->Reset();
		enemy_->Reset(enemy_->EAST, 2);
		plug_->Reset(Vector3(11.0, 0, -9.0), plug_->NORTH);
		socket1_->Reset(Vector3(0, 0, -9), socket1_->SOUTH);
		door_->Reset();
		lamp_->Reset();
		stage_->TutorialReset();
		break;
	default:
		break;
	}

	
}
