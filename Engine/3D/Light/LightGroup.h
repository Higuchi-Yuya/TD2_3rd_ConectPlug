#pragma once
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"
#include <d3d12.h>
#include <wrl.h>
class LightGroup
{
private:
	// namespace�̏ȗ�
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public:// �萔
	// ���s�����̐�
	static const int DirLightNum = 3;
	// �_�����̐�
	static const int PointLightNum = 3;
	// �X�|�b�g���C�g�̐�
	static const int SpotLightNum = 3;
	// �ۉe�̐�
	static const int CircleShadowNum = 1;

public:// �T�u�N���X
	struct ConstBufferData
	{
		// �����̐F
		Vector3 ambientColor;
		float pad1;
		// ���s�����p
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		// �_�����p
		PointLight::ConstBufferData pointLights[PointLightNum];
		// �X�|�b�g���C�g�p
		SpotLight::ConstBufferData spotLights[SpotLightNum];
		// �ۉe�p
		CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
	};

private:// �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;

public:// �ÓI�����o�֐�
	// �ÓI������
	static void StaticInititalize(ID3D12Device* device);

public:// �����o�֐�

	// �C���X�^���X����
	static LightGroup* Create();

	// ������
	void Initialize();

	// �X�V����
	void Update();

	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="color">���C�g�F</param>
	void SetAmbientColor(const Vector3& color);

	/// <summary>
	/// ���s�����̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetDirLightActive(int index, bool active);

	/// <summary>
	/// ���s�����̃��C�g�������Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g����</param>
	void SetDirLightDir(int index, const Vector3& lightdir);

	/// <summary>
	/// ���s�����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightcolor">���C�g�F</param>
	void SetDirLightColor(int index, const Vector3& lightcolor);

	/// <summary>
	/// �_�����̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetPointLightActive(int index, bool active);

	/// <summary>
	/// �_�����̃��C�g���W���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightpos">���C�g���W</param>
	void SetPointLightPos(int index, const Vector3& lightpos);

	/// <summary>
	/// �_�����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightcolor">���C�g�F</param>
	void SetPointLightColor(int index, const Vector3& lightcolor);

	/// <summary>
	/// �_�����̃��C�g���������W�����Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightatten">���C�g���������W��</param>
	void SetPointLightAtten(int index, const Vector3& lightAtten);

	/// <summary>
	/// �X�|�b�g���C�g�̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetSpotLightActive(int index, bool active);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g�������Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g����</param>
	void SetSpotLightDir(int index, const Vector3& lightdir);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g���W���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightpos">���C�g���W</param>
	void SetSpotLightPos(int index, const Vector3& lightpos);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightcolor">���C�g�F</param>
	void SetSpotLightColor(int index, const Vector3& lightcolor);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g���������W�����Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightatten">���C�g���������W��</param>
	void SetSpotLightAtten(int index, const Vector3& lightAtten);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g�����p�x���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightFactorAngle">x:�����J�n�p�x y:�����I���p�x</param>
	void SetSpotLightFactorAngle(int index, const Vector2& lightFactorAngle);

	/// <summary>
	/// �ۉe�̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetCircleShadowActive(int index, bool active);

	/// <summary>
	/// �ۉe�̃L���X�^�[���W���Z�b�g
	/// </summary>
	/// <param name="index">�ԍ�</param>
	/// <param name="lightpos">�L���X�^�[���W</param>
	void SetCircleShadowCasterPos(int index, const Vector3& casterPos);

	/// <summary>
	/// �ۉe�̕������Z�b�g
	/// </summary>
	/// <param name="index">�ԍ�</param>
	/// <param name="lightdir">����</param>
	void SetCircleShadowDir(int index, const Vector3& lightdir);

	/// <summary>
	/// �ۉe�̃L���X�^�[�ƃ��C�g�̋������Z�b�g
	/// </summary>
	/// <param name="index">�ԍ�</param>
	/// <param name="distanceCasterLight">�L���X�^�[�ƃ��C�g�̋���</param>
	void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight);

	/// <summary>
	/// �ۉe�̋��������W�����Z�b�g
	/// </summary>
	/// <param name="index">�ԍ�</param>
	/// <param name="lightatten">���������W��</param>
	void SetCircleShadowAtten(int index, const Vector3& lightAtten);

	/// <summary>
	/// �ۉe�̌����p�x���Z�b�g
	/// </summary>
	/// <param name="index">�ԍ�</param>
	/// <param name="lightFactorAngle">x:�����J�n�p�x y:�����I���p�x</param>
	void SetCircleShadowFactorAngle(int index, const Vector2& lightFactorAngle);

private:// �v���C�x�[�g�����o�֐�

	/// <summary>
	/// �萔�o�b�t�@����
	/// </summary>
	void CreateConstBuffer();

	/// <summary>
	/// �萔�o�b�t�@�]��
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// �W���̃��C�g�ݒ�
	/// </summary>
	void DefaultLightSetting();

private:// �����o�ϐ�

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;

	// �萔�o�b�t�@�̃}�b�v
	ConstBufferData* constMap = nullptr;

	// �����̐F
	Vector3 ambientColor_ = { 1,1,1 };

	// ���s�����̔z��
	DirectionalLight dirLights_[DirLightNum];

	// �_�����̔z��
	PointLight pointLights_[PointLightNum];

	// �X�|�b�g���C�g�̔z��
	SpotLight spotLights_[SpotLightNum];

	// �ۉe�̔z��
	CircleShadow circleShadows_[CircleShadowNum];

	// �_�[�e�B�t���O
	bool dirty = false;

};

