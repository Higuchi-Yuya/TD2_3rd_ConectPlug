#pragma once
#include "DirectXCommon.h"
#include "Texture.h"
#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#pragma comment(lib, "d3dcompiler.lib")

class SpriteManager
{
public:// �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		Vector4 color;//�F(RGBA)
		Matrix4 mat;// 3D�ϊ��s��
	};

	// ���_�f�[�^�\����
	struct Vertex
	{
		Vector3 pos; // xyz���W
		Vector2 uv;  // uv���W
	};

public:// �����o�֐�
	// ������
	void Initialize(DirectXCommon* dxcommon);

	// �`��O�ݒ�
	void PreDraw();

	// �`���ݒ�
	void PostDraw();

	// �e�N�X�`���ɕK�v�ȃR�}���h�ݒ�
	void SetTextureCommands(uint32_t index);

	//�w��ԍ��̃e�N�X�`���o�b�t�@���擾
	ID3D12Resource* GetTextureBuffer(uint32_t index)const { return Texture::textureBuffers_[index].Get(); }

public:
	static DirectXCommon* dxcommon_;
private:
	
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;		//�p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;		//���[�g�V�O�l�`��

	// ���ʊm�F
	HRESULT result = 0;
};

