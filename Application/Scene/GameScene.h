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
public:// �����o�֐�

	// �V�[���`�F���W
	enum Scene
	{
		Title,
		StageSelect,
		Game,
	};

	// �f�X�g���N�^
	~GameScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// Imgui�̍X�V����
	void ImguiUpdate();

	// �`�揈��
	void Draw2DBack();

	void Draw3D();

	void Draw2DFront();

	void Reset();

private:// �����o�ϐ�

	// ����
	Input* input = nullptr;

	// �I�u�W�F�N�g���ʂ̃��C�g�̏�����
	LightGroup* light = nullptr;
	
	// ���s����
	bool isActiveDirectional = true;

	// �_�����̃X�e�[�^�X
	float pointLightPos[3] = { 0,1,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };
	bool isActivePoint = false;

	// �X�|�b�g���C�g�̃X�e�[�^�X
	Vector3 spotLightDir = { 0,-1,0 };
	Vector3 spotLightPos = { 0,5,0 };
	Vector3 spotLightColor = { 1,1,1 };
	Vector3 spotLightAtten = { 0.0f,0.0f,0.0f };
	Vector2 spotLightFactorAngle = { 20.0f,30.0f };
	bool isActiveSpot = false;

	// �ۉe�̃X�e�[�^�X
	Vector3 circleShadowDir = { 0,-1,0 };
	Vector3 circleShadowAtten = { 1.0f,0.6f,0.0f };
	Vector2 circleShadowFactorAngle = { 0.0f,0.5f };
	bool isActiveCircleShadow = true;

	Vector3 fighterPos = { 1,0.0f,0 };

	// ���f��

	// �I�u�W�F�N�g

	ViewProjection* viewProjection_ = nullptr;
	//�v���C���[
	Player* player_ = nullptr;
	//�G
	Enemy* enemy_ = nullptr;

	//�v���O
	Plug* plug_ = nullptr;
	//�\�P�b�g
	Socket* socket1_ = nullptr;
	//�V��
	SkyBox* skyBox_ = nullptr;
	//�X�e�[�W
	Stage* stage_ = nullptr;

	//�h�A
	Door* door_ = nullptr;

	// �����v
	Lamp* lamp_ = nullptr;

	// �V�[��
	Scene scene = Scene::Title;
};

