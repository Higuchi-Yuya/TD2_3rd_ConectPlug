#include "LightGroup.h"
#include <cassert>
#include <d3dx12.h>
// �ÓI�����o�ϐ��̎���
ID3D12Device* LightGroup::device = nullptr;

void LightGroup::StaticInititalize(ID3D12Device* device)
{
	// �ď������`�F�b�N
	assert(!LightGroup::device);
	// nullptr�`�F�b�N
	assert(device);
	// �ÓI�����o�ϐ��̃Z�b�g
	LightGroup::device = device;
}

LightGroup* LightGroup::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	LightGroup* instance = new LightGroup();
	// ������
	instance->Initialize();
	// ���������C���X�^���X��Ԃ�
	return instance;
}

void LightGroup::Initialize()
{
	// ���C�g�̃f�t�H���g�̐ݒ�
	DefaultLightSetting();
	// �o�b�t�@�̐���
	CreateConstBuffer();
	// �o�b�t�@�̓]��
	TransferConstBuffer();
}

void LightGroup::Update()
{
	// �l���X�V���������������萔�o�b�t�@�ɓ]������
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(4, constBuff->GetGPUVirtualAddress());

}

void LightGroup::TransferConstBuffer()
{
	// ����
	constMap->ambientColor = ambientColor_;

	// ���s����
	for (int i = 0; i < DirLightNum; i++) {
		// ���C�g���L���Ȃ�ݒ��]��
		if (dirLights_[i].IsActive()) {
			constMap->dirLights[i].active = 1;
			constMap->dirLights[i].lightv = -dirLights_[i].GetLightDir();
			constMap->dirLights[i].lightcolor = dirLights_[i].GetLightColor();
		}
		// ���C�g�������Ȃ烉�C�g�F��0��
		else {
			constMap->dirLights[i].active = 0;
		}
	}

	// �_����
	for (int i = 0; i < PointLightNum; i++) {
		// ���C�g���L���Ȃ�ݒ��]��
		if (pointLights_[i].IsActive()) {
			constMap->pointLights[i].active = 1;
			constMap->pointLights[i].lightpos = pointLights_[i].GetLightPos();
			constMap->pointLights[i].lightcolor = pointLights_[i].GetLightColor();
			constMap->pointLights[i].lightatten = pointLights_[i].GetLightAtten();
		}
		// ���C�g�������Ȃ烉�C�g�F��0��
		else {
			constMap->pointLights[i].active = 0;
		}
	}

	// �X�|�b�g���C�g
	for (int i = 0; i < SpotLightNum; i++) {
		// ���C�g���L���Ȃ�ݒ��]��
		if (spotLights_[i].IsActive()) {
			constMap->spotLights[i].active = 1;
			constMap->spotLights[i].lightv = -spotLights_[i].GetLightDir();
			constMap->spotLights[i].lightpos = spotLights_[i].GetLightPos();
			constMap->spotLights[i].lightcolor = spotLights_[i].GetLightColor();
			constMap->spotLights[i].lightatten = spotLights_[i].GetLightAtten();
			constMap->spotLights[i].lightfactoranglecos = spotLights_[i].GetLightFactorAngleCos();
		}
		// ���C�g�������Ȃ烉�C�g�F��0��
		else {
			constMap->spotLights[i].active = 0;
		}
	}

	// �ۉe
	for (int i = 0; i < CircleShadowNum; i++) {
		// �L���Ȃ�ݒ��]��
		if (circleShadows_[i].IsActive()) {
			constMap->circleShadows[i].active = 1;
			constMap->circleShadows[i].dir = -circleShadows_[i].GetDir();
			constMap->circleShadows[i].casterPos = circleShadows_[i].GetCasterPos();
			constMap->circleShadows[i].distanceCasterLight = circleShadows_[i].GetDistanceCasterLight();
			constMap->circleShadows[i].atten = circleShadows_[i].GetAtten();
			constMap->circleShadows[i].factorAngleCos = circleShadows_[i].GetFactorAngleCos();
		}
		// �����Ȃ�F��0��
		else {
			constMap->circleShadows[i].active = 0;
		}
	}
}

void LightGroup::DefaultLightSetting()
{
	dirLights_[0].SetActive(true);
	dirLights_[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights_[0].SetLightDir({ 0.0f, -1.0f, 0.0f });

	dirLights_[1].SetActive(true);
	dirLights_[1].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights_[1].SetLightDir({ +0.5f, +0.1f, +0.2f });

	dirLights_[2].SetActive(true);
	dirLights_[2].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights_[2].SetLightDir({ -0.5f, +0.1f, -0.2f });
}

void LightGroup::SetAmbientColor(const Vector3& color)
{
	ambientColor_ = color;
	dirty = true;
}

void LightGroup::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < DirLightNum);

	dirLights_[index].SetActive(active);
}

void LightGroup::SetDirLightDir(int index, const Vector3& lightdir)
{
	assert(0 <= index && index < DirLightNum);

	dirLights_[index].SetLightDir(lightdir);
	dirty = true;
}

void LightGroup::SetDirLightColor(int index, const Vector3& lightcolor)
{
	assert(0 <= index && index < DirLightNum);

	dirLights_[index].SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::SetPointLightActive(int index, bool active)
{
	assert(0 <= index && index < PointLightNum);

	pointLights_[index].SetActive(active);
}

void LightGroup::SetPointLightPos(int index, const Vector3& lightpos)
{
	assert(0 <= index && index < PointLightNum);

	pointLights_[index].SetLightPos(lightpos);
	dirty = true;
}

void LightGroup::SetPointLightColor(int index, const Vector3& lightcolor)
{
	assert(0 <= index && index < PointLightNum);

	pointLights_[index].SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::SetPointLightAtten(int index, const Vector3& lightAtten)
{
	assert(0 <= index && index < PointLightNum);

	pointLights_[index].SetLightAtten(lightAtten);
	dirty = true;
}

void LightGroup::SetSpotLightActive(int index, bool active)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights_[index].SetActive(active);
}

void LightGroup::SetSpotLightDir(int index, const Vector3& lightdir)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights_[index].SetLightDir(lightdir);
	dirty = true;
}

void LightGroup::SetSpotLightPos(int index, const Vector3& lightpos)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights_[index].SetLightPos(lightpos);
	dirty = true;
}

void LightGroup::SetSpotLightColor(int index, const Vector3& lightcolor)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights_[index].SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::SetSpotLightAtten(int index, const Vector3& lightAtten)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights_[index].SetLightAtten(lightAtten);
	dirty = true;
}

void LightGroup::SetSpotLightFactorAngle(int index, const Vector2& lightFactorAngle)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights_[index].SetLightFactorAngle(lightFactorAngle);
	dirty = true;
}

void LightGroup::SetCircleShadowActive(int index, bool active)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows_[index].SetActive(active);
}

void LightGroup::SetCircleShadowCasterPos(int index, const Vector3& casterPos)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows_[index].SetCasterPos(casterPos);
	dirty = true;
}

void LightGroup::SetCircleShadowDir(int index, const Vector3& lightdir)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows_[index].SetDir(lightdir);
	dirty = true;
}

void LightGroup::SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows_[index].SetDistanceCasterLight(distanceCasterLight);
	dirty = true;
}

void LightGroup::SetCircleShadowAtten(int index, const Vector3& lightAtten)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows_[index].SetAtten(lightAtten);
	dirty = true;
}

void LightGroup::SetCircleShadowFactorAngle(int index, const Vector2& lightFactorAngle)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows_[index].SetFactorAngle(lightFactorAngle);
	dirty = true;
}

void LightGroup::CreateConstBuffer()
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�Ƃ̃f�[�^�����N
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
}

