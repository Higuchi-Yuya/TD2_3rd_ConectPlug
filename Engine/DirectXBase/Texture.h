#pragma once
#include<DirectXTex.h>
#include"DirectXCommon.h"
#include<wrl.h>
#include<array>
#include<string>
#include <d3dx12.h>
class Texture
{
public:// �����o�֐�

	// �e�N�X�`���ǂݍ���
	static uint32_t LoadTexture(const std::string& fileName = "NULL");
	
	static uint32_t LoadTexture(const wchar_t* fileName);

	// �e�N�X�`���ň�x�K�v�ȏ�����
	static void StaticInitialize(DirectXCommon* dxcommon);

public:// �ÓI�����o�ϐ�
	// SRV�̍ő��
	static const size_t kMaxSRVCount = 2056;
	// �f�t�H���g�e�N�X�`���i�[�f�B���N�g��
	static std::string kDefaultTextureDirectoryPath;

	// DirextX�̊�Ղ��؂�Ă���
	static DirectXCommon* dxcommon_;

	// SRV�q�[�v
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

	// �e�N�X�`���o�b�t�@
	static std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount>textureBuffers_;

private:// �����o�ϐ�
	// �e�N�X�`�����\�[�X�f�X�N
	static D3D12_RESOURCE_DESC textureResourceDesc;
};

