#include "GameScene.h"
#include "Collision.h"
GameScene::~GameScene()
{
	// ���͉��
	delete input;
	// �X�v���C�g�̉��
	delete sprite;
	delete sprite2;
	// �I�u�W�F�N�g�̉��
	delete object3d;
	for (int i = 0; i < 500; i++) {
		delete obj_2[i];
	}
	
	delete objFighter;
	delete groundObj;

	delete point1;
	delete point2;
	delete point3;
	delete rayobj;

	// ���f���̉��
	delete model;
	delete model_2;
	delete modelFighter;
	delete groundModel;
	// �r���[�̉��
	delete view;
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
	//light->SetPointLightActive(0, true);
	//pointLightPos[0] = 0.5f;
	//pointLightPos[1] = 1.0f;
	//pointLightPos[2] = 0.0f;

	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);

	light->SetSpotLightActive(0, false);

	light->SetCircleShadowActive(0, true);

	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLight(light);

	// �e�N�X�`���n���h���̓ǂݍ���
	textureHandle = Texture::LoadTexture("skydome/Nebura.jpg");
	textureHandle2 = Texture::LoadTexture("texture.png");

	// �X�v���C�g�̏�����
	sprite = new Sprite();
	sprite2 = new Sprite();

	sprite->Initialize(textureHandle, { WinApp::window_width / 2,WinApp::window_height / 2 }, { 1280,720 });
	sprite2->Initialize(textureHandle2, { 200,200 });

	// ���f���̓ǂݍ���
	model = Model::LoadFromOBJ("skydome", true);
	model_2 = Model::LoadFromOBJ("Medama", true);
	groundModel = Model::LoadFromOBJ("ground");
	modelFighter = Model::LoadFromOBJ("chr_sword");
	
	// �I�u�W�F�N�g�̏�����
	object3d = Object3d::Create();
	for (size_t i = 0; i < 500; i++)
	{

		obj_2[i] = Object3d::Create();
		obj_2[i]->SetModel(model_2);
		obj_2[i]->worldTransform_.position_ = { -1,1,0 };
		obj_2[i]->worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
		obj_2[i]->worldTransform_.color_ = { 1.0f,1.0f,1.0f,1.0f };
	}

	objFighter = Object3d::Create();

	point1= Object3d::Create();
	point2=Object3d::Create();
	point3= Object3d::Create();
	rayobj= Object3d::Create();

	groundObj = Object3d::Create();


	objFighter->SetModel(modelFighter);
	objFighter->worldTransform_.position_ = fighterPos;


	object3d->SetModel(model);



	groundObj->SetModel(groundModel);

	// �r���[�v���W�F�N�V�����̏�����
	view = new ViewProjection;
	view->DebugCameraInitialze(input);
	view->target.y = 1.0f;
	view->SetDistance(3.0f);

	spritePos = sprite2->GetPosition();

	rotation0 = keisan.MakeAxisAngle({ 0.71f,0.71f,0.0f }, 0.3f);
	rotation1 = {-rotation0.x,-rotation0.y, -rotation0.z, -rotation0.w};//keisan.MakeAxisAngle({ 0.71f,0.0f,0.71f }, 3.141592f);

	interpolate0 = keisan.Slerp(rotation0, rotation1, 0.0f);
	interpolate1 = keisan.Slerp(rotation0, rotation1, 0.3f);
	interpolate2 = keisan.Slerp(rotation0, rotation1, 0.5f);
	interpolate3 = keisan.Slerp(rotation0, rotation1, 0.7f);
	interpolate4 = keisan.Slerp(rotation0, rotation1, 1.0f);

	// ���̏����l��ݒ�
	sphere.center = { 0,2,0 };
	sphere.radius = 1.0f;

	// ���ʂ̏����l��ݒ�
	plane.normal = { 0,1,0 };
	plane.distance = 0.0f;

	// �O�p�`�̏����l��ݒ�
	triangle.p0 = { -1.0f,0,-1.0f };
	triangle.p1 = { -1.0f,0,+1.0f };
	triangle.p2 = { +1.0f,0,-1.0f };

	triangle.normal = { 0.0f,1.0f,0.0f };

	// ���C�̏����l��ݒ�
	ray.start = { 0,1,0 };
	ray.dir = { 0,-1,0 };

	// �m�F�p�I�u�W�F
	point1->SetModel(model_2);
	point2->SetModel(model_2);
	point3->SetModel(model_2);

	point1->worldTransform_.scale_ = { 0.2f,0.2f,0.2f };
	point1->worldTransform_.position_ = triangle.p0;
	point2->worldTransform_.scale_ = { 0.2f,0.2f,0.2f };
	point2->worldTransform_.position_ = triangle.p1;
	point3->worldTransform_.scale_ = { 0.2f,0.2f,0.2f };
	point3->worldTransform_.position_ = triangle.p2;

	rayobj->SetModel(modelFighter);
	rayobj->worldTransform_.scale_ = { 0.2f,1.0f,0.2f };

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

	light->SetPointLightPos(0, Vector3(pointLightPos[0], pointLightPos[1], pointLightPos[2]));
	light->SetPointLightColor(0, Vector3(pointLightColor[0], pointLightColor[1], pointLightColor[2]));
	light->SetPointLightAtten(0, Vector3(pointLightAtten[0], pointLightAtten[1], pointLightAtten[2]));

	light->SetSpotLightDir(0, spotLightDir);
	light->SetSpotLightPos(0, spotLightPos);
	light->SetSpotLightColor(0, spotLightColor);
	light->SetSpotLightAtten(0, spotLightAtten);
	light->SetSpotLightFactorAngle(0, spotLightFactorAngle);

	light->SetCircleShadowDir(0, circleShadowDir);
	light->SetCircleShadowCasterPos(0, fighterPos);
	light->SetCircleShadowAtten(0, circleShadowAtten);
	light->SetCircleShadowFactorAngle(0, circleShadowFactorAngle);

	objFighter->worldTransform_.position_ = fighterPos;

	light->Update();

	//object3d->SetScale(scale_);
	object3d->Update();

	objFighter->worldTransform_.rotation_.y += 0.01f;
	objFighter->Update();
	//spritePos = sprite2->GetPosition();
	sprite2->SetPosition(spritePos);
	
	for (size_t i = 0; i < 500; i++)
	{
		obj_2[i]->worldTransform_.rotation_.y += 0.01f;
		obj_2[i]->Update();
	}


	groundObj->Update();

	view->DebugCameraUpdate();

	// ���ړ�
	{
		Vector3 moveY = { 0,0.01f,0 };
		if (input->PushKey(DIK_8)) { sphere.center += moveY; }
		else if (input->PushKey(DIK_2)) { sphere.center -= moveY; }

		Vector3 moveX = { 0.01f,0,0 };
		if (input->PushKey(DIK_6)) { sphere.center += moveX; }
		else if (input->PushKey(DIK_4)) { sphere.center -= moveX; }

	}
	// ���C����
	{
		Vector3 moveZ = { 0,0,0.01f };
		if (input->PushKey(DIK_UP)) { ray.start += moveZ; }
		else if (input->PushKey(DIK_DOWN)) { ray.start -= moveZ; }

		Vector3 moveX = { 0.01f,0,0 };
		if (input->PushKey(DIK_RIGHT)) { ray.start += moveX; }
		else if (input->PushKey(DIK_LEFT)) { ray.start -= moveX; }
	}

	point1->Update();
	point2->Update();
	point3->Update();

	rayobj->worldTransform_.position_ = ray.start;
	rayobj->Update();


	hit = Collision::CheckRay2Sphere(ray, sphere, &distance, &inter);
	hitRay = Collision::CheckRay2Triangle(ray, triangle, &distance, &inter);

}

void GameScene::ImguiUpdate()
{

	// �\�����ڂ̒ǉ�--------//
	ImGui::Begin("Sprite 1");

	//ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 100));
	
	ImGui::SliderFloat2("position", &spritePos.x, 0.0f, 1200.0f, "%.1f");

	ImGui::InputFloat4("interpolate0", &interpolate0.x, "%.2f");
	ImGui::InputFloat4("interpolate1", &interpolate1.x, "%.2f");
	ImGui::InputFloat4("interpolate2", &interpolate2.x, "%.2f");
	ImGui::InputFloat4("interpolate3", &interpolate3.x, "%.2f");
	ImGui::InputFloat4("interpolate4", &interpolate4.x, "%.2f");


	if (ImGui::Button("Reset")) {
		spritePos = { 200.0f,200.0f };
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

	ImGui::InputFloat3("sphere", &sphere.center.x);
	ImGui::Text("hit:%d", hit);
	ImGui::InputFloat3("sphere.inter", &inter.x);

	ImGui::InputFloat3("rayStart", &ray.start.x);
	ImGui::InputFloat3("rayDir", &ray.dir.x);
	ImGui::Text("hitRay:%d", hitRay);
	ImGui::InputFloat3("ray.inter", &inter.x);

	ImGui::End();
	// ---------------------//

}

void GameScene::Draw2DBack()
{
	//sprite->Draw();
}

void GameScene::Draw3D()
{
	object3d->Draw(view);
	
	for (size_t i = 0; i < 500; i++)
	{
		obj_2[i]->Draw(view);
	}
	
	objFighter->Draw(view);

	//point1->Draw(view);
	//point2->Draw(view);
	//point3->Draw(view);
	//rayobj->Draw(view);

	groundObj->Draw(view);
}

void GameScene::Draw2DFront()
{

	sprite2->Draw();
}
