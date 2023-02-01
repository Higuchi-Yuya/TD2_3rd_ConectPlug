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
		StageClear,
		GameOver,
	};

	// �f�X�g���N�^
	~GameScene();

	// ����������
	void Initialize();

	// �X�v���C�g�̏���������
	void SpriteInitialize();

	// �X�v���C�g�̉��
	void DeleteSprite();

	// �X�v���C�g�̕`��
	void SpriteDraw();

	// �|�C���g���C�g�̏���
	void PointLightUpdate();

	// �X�V����
	void Update();

	// Imgui�̍X�V����
	void ImguiUpdate();

	// �`�揈��
	void Draw2DBack();

	void Draw3D();

	void Draw2DFront();

	void BlackOut();

	void Reset();

private:// �����o�ϐ�

	// ����
	Input* input = nullptr;

	// �I�u�W�F�N�g���ʂ̃��C�g�̏�����
	LightGroup* light = nullptr;
	
	// ���s����
	bool isActiveDirectional = true;

	// �_�����̃X�e�[�^�X
	Vector3 pointLightPos = { 0,1,0 };
	Vector3 pointLightColor = { 1,1,1 };
	Vector3 pointLightAtten = { 0.3f,0.1f,0.1f };
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

#pragma region �X�v���C�g
	// �e�N�X�`���n���h��
	int otamesiTexHandle = 0;

	// �^�C�g���V�[���̃X�v���C�g
	Sprite* titleRogo  = nullptr;
	Sprite* Spacekey   = nullptr;
	Sprite* titleStart = nullptr;

	// �X�e�[�W�I����ʂ̃X�v���C�g
	Sprite* tutorialFont = nullptr;
	Sprite* stage1Font   = nullptr;
	Sprite* stage2Font   = nullptr;
	Sprite* stage3Font   = nullptr;
	Sprite* slectButton  = nullptr;

	float plusSelectPos = 60.0f;

	// �Q�[���V�[����UI�X�v���C�g
	Sprite* gameUpkey    = nullptr;
	Sprite* gameDownkey  = nullptr;
	Sprite* gameLeftkey  = nullptr;
	Sprite* gameRightkey = nullptr;
	Sprite* gameMoveFont = nullptr;
	Sprite* gameHaveFont = nullptr;

	// �X�e�[�W�N���A���̃X�v���C�g
	Sprite* clearFont     = nullptr;
	Sprite* replayFont    = nullptr;
	Sprite* backTitleFont = nullptr;

	// �Q�[���I�[�o�[�̃X�v���C�g
	Sprite* gameOverFont = nullptr;

	

	// ��ʂ̐^�񒆂̃|�W�V����
	Vector2 displayCenter = { WinApp::window_width/2,WinApp::window_height/2 };

#pragma endregion

#pragma region �u���b�N�A�E�g�̕ϐ�
	int blackOutTex = 0;
	// �V�[�����ڂ̍����X�v���C�g
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

