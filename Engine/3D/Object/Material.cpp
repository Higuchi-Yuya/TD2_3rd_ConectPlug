#include "Material.h"
#include "Texture.h"

using namespace std;
/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* Material::device = nullptr;

void Material::StaticInitialize(ID3D12Device* device)
{
	Material::device = device;
}

Material* Material::Create()
{
	Material* instance = new Material;

	instance->Initialize();

	return instance;
}

void Material::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
	// �t�@�C���p�X������
	string filepath = directoryPath + filename;

	// ���j�R�[�h������ɕϊ�����
	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	textureIndex = Texture::LoadTexture(wfilepath);
}

void Material::Update()
{
	constMap->ambient = ambient;
	constMap->diffuse = diffuse;
	constMap->specular = specular;
	constMap->alpha = alpha;
	constBuff->Unmap(0, nullptr);
}

void Material::Initialize()
{
	// �萔�o�b�t�@�̐���
	CreateConstantBuffer();
}

void Material::CreateConstantBuffer()
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff);

	// �}�e���A���p�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	assert(SUCCEEDED(result));

	// �}�e���A���p�萔�o�b�t�@�փf�[�^�]��
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
}
