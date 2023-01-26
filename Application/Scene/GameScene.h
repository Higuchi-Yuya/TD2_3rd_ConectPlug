#pragma once
#include "Sprite.h"
#include "Object3d.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Quaternion.h"
#include "LightGroup.h"
#include "ImGuiManager.h"
#include "CollisionPrimitive.h"

class GameScene
{
public:// �����o�֐�
	
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
	Vector3 circleShadowAtten = { 1.5f,0.6f,0.0f };
	Vector2 circleShadowFactorAngle = { 0.0f,0.5f };
	bool isActiveCircleShadow = true;

	Vector3 fighterPos = { 1,0.0f,0 };

	// �e�N�X�`���n���h��
	int textureHandle;
	int textureHandle2;

	// �X�v���C�g
	Sprite* sprite = nullptr;
	Sprite* sprite2 = nullptr;

	// ���f��
	Model* model = nullptr;
	Model* model_2 = nullptr;
	Model* modelFighter = nullptr;
	Model* groundModel = nullptr;

	// �I�u�W�F�N�g
	Object3d* object3d = nullptr;
	Object3d* obj_2[500];

	Object3d* point1 = nullptr;
	Object3d* point2 = nullptr;
	Object3d* point3 = nullptr;

	Object3d* rayobj = nullptr;

	Object3d* objFighter = nullptr;
	Object3d* groundObj = nullptr;


	// �r���[�v���W�F�N�V����
	ViewProjection* view = nullptr;

	// Quaternion�m�F
	Quaternion keisan;
	Quaternion rotation0;
	Quaternion rotation1;
	Quaternion interpolate0;
	Quaternion interpolate1;
	Quaternion interpolate2;
	Quaternion interpolate3;
	Quaternion interpolate4;

	// �����蔻��
	Sphere sphere;
	Plane plane;
	Triangle triangle;
	Ray ray;
	float distance;
	Vector3 inter;

	bool hit;
	bool hitRay;

	Vector2 spritePos;
	char buf[256] = "";
	float f = 0.0f;
};

