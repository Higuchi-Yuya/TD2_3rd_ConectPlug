#include "GameScene.h"
#include "Collision.h"
GameScene::~GameScene()
{
	// ���͉��
	delete input;

	// ���f���̉��

	delete player_;
	delete enemy_;
	delete plug_;
	delete socket1_;
	delete stage_;
	delete skyBox_;
	delete door_;
	delete lamp_;

	// �r���[�̉��
	delete viewProjection_;
	// ���C�g�̉��
	delete light;
}

void GameScene::Initialize()
{
	// ���͂̏�����
	input = new Input();
	input->Initialize();

	// ���C�g�̐���
	light = LightGroup::Create();
	// ���C�g�ݒ�
	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, false);
	light->SetDirLightActive(2, false);


	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);

	light->SetSpotLightActive(0, false);

	light->SetCircleShadowActive(0, true);

	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLight(light);

	// ���f���̓ǂݍ���
	viewProjection_ = new ViewProjection;
	viewProjection_->Initialize();
	viewProjection_->eye = { 5,30,-25 };
	viewProjection_->target = { 5,0,0 };

	// �I�u�W�F�N�g�̏�����
	skyBox_ = new SkyBox;
	skyBox_->Initialize();

	stage_ = new Stage;
	stage_->Initialize();

	socket1_ = new Socket;
	socket1_->Initialize(Vector3(0, 0, -9), socket1_->EAST);

	plug_ = new Plug();
	plug_->Initialize(Vector3(11.0, 0, -9.0), plug_->WEST);
	plug_->SetStage(stage_);
	plug_->SetSocket(socket1_);

	door_ = new Door;
	door_->Initialize();
	door_->SetPlug(plug_);

	player_ = new Player();
	player_->Initialize();
	player_->SetStage(stage_);
	player_->SetPlug(plug_);
	player_->SetDoor(door_);

	enemy_ = new Enemy;
	enemy_->Initialize( enemy_->EAST, 2);//�����œG�̌����Ɖ�]�ʂ����߂�
	enemy_->SetStage(stage_);



	lamp_ = new Lamp;
	lamp_->Initialize();
	lamp_->SetisShining(false);
}

void GameScene::Update()
{
	// ���͂̍X�V
	input->Update();

	// ������0�L�[��������Ă�����
	if (input->PushKey(DIK_0))
	{
		OutputDebugStringA("Hit 0\n");  // �o�̓E�B���h�E�ɁuHit 0�v�ƕ\��
	}


	switch (scene)
	{
	case GameScene::Title:
		if (input->TriggerKey(DIK_SPACE)) {
			// ���̃V�[�����Z�b�g
			scene = Scene::Game;
			// ���̃V�[���̃��Z�b�g������
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
		//player�X�V
		player_->Update();
		//�G�X�V
		enemy_->Update();
		//�v���O�X�V
		plug_->Update();
		//�X�e�[�W�X�V
		stage_->Update();
		//�V���X�V
		skyBox_->Update();
		//�\�P�b�g�X�V
		socket1_->Update();
		//�h�A�X�V
		door_->Update();

		// �����v
		lamp_->SetisShining(plug_->GetIsConnect());
		lamp_->Update();

		// �Ƃ肠�������[�v�̊m�F�p��Z�L�[�Ń^�C�g���ɖ߂�
		if (input->TriggerKey(DIK_Z)) {
			scene = Scene::Title;
		}

		break;
	default:
		break;
	}

	// ���ړ�

}

void GameScene::ImguiUpdate()
{

	// �\�����ڂ̒ǉ�--------//
	ImGui::Begin("Sprite 1");

	//ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 100));



	if (ImGui::Button("Reset")) {

	}

	ImGui::End();

	ImGui::Begin("Light");
	ImGui::SetNextWindowSize(ImVec2(500, 200));

	// ���s����
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
	// �|�C���g���C�g
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
	// �X�|�b�g���C�g
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
	// �ۉe
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

	// �����蔻��-----------------------//
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
		//3D�`��
		//�v���O
		plug_->Draw(viewProjection_);
		//�V��
		skyBox_->Draw(viewProjection_);
		//�X�e�[�W
		stage_->Draw(viewProjection_);
		//�\�P�b�g
		socket1_->Draw(viewProjection_);
		//�G�l�~�[
		enemy_->Draw(viewProjection_);
		//�v���C���[
		player_->Draw(viewProjection_);
		//�h�A
		door_->Draw(viewProjection_);
		// �����v
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
		plug_->Reset(Vector3(11.0, 0, -9.0), plug_->WEST);
		socket1_->Reset(Vector3(0, 0, -9), socket1_->EAST);
		door_->Reset();
		lamp_->Reset();
		stage_->TutorialReset();
		break;
	default:
		break;
	}

	
}
