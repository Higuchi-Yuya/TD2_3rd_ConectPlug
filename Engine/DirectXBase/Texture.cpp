#include "Texture.h"
using namespace DirectX;
using namespace Microsoft::WRL;

std::string Texture::kDefaultTextureDirectoryPath = "Resources/";
DirectXCommon* Texture::dxcommon_ = nullptr;
std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, Texture::kMaxSRVCount> Texture::textureBuffers_;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Texture::srvHeap = nullptr;
D3D12_RESOURCE_DESC Texture::textureResourceDesc{};


uint32_t Texture::LoadTexture(const std::string& fileName)
{
	// ���ʊm�F
	HRESULT result;

	//// �������Ղ��Z����
//const size_t textureWidth = 256;
//// �c�����s�N�Z����
//const size_t textureHeight = 256;
//// �z��̗v�f��
//const size_t imageDataCount = textureWidth * textureHeight;
//// �摜�C���[�W�f�[�^�z��
//XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];// �K����ŊJ������

//// �S�s�N�Z���̐F�̏�����
//for (size_t i = 0; i < imageDataCount; i++) {
//	imageData[i].x = 1.0f;  // R
//	imageData[i].y = 0.0f;  // G
//	imageData[i].z = 0.0f;  // B
//	imageData[i].w = 1.0f;  // A
//}

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// �f�B���N�g���p�X�ƃt�@�C������A�����ăt���p�X�𓾂�
	std::string fullPath = kDefaultTextureDirectoryPath + fileName;

	// ���C�h������ɕϊ������ۂ̕�����o�b�t�@�T�C�Y���v�Z
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, nullptr, 0);

	// ���C�h������ɕϊ�
	std::vector<wchar_t>wfilePath(filePathBufferSize);
	MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilePath.data(), filePathBufferSize);


	result = LoadFromWICFile(
		wfilePath.data(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	ScratchImage mipChain{};
	// �~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);


	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//���\�[�X�ݒ�
	
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;	// ��
	textureResourceDesc.Height = (UINT)metadata.height;	// ����
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// �e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff;
	result = dxcommon_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,				//�S�̈�փR�s�[
			img->pixels,			//���f�[�^�A�h���X
			(UINT)img->rowPitch,	//1���C���T�C�Y
			(UINT)img->slicePitch	//�S�T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	//result = texBuff->WriteToSubresource(
	//	0,
	//	nullptr,				//�S�̈�փR�s�[
	//	imageData,			//���f�[�^�A�h���X
	//	sizeof(XMFLOAT4) * textureWidth,	//1���C���T�C�Y
	//	sizeof(XMFLOAT4) * imageDataCount 	//�S�T�C�Y
	//);

	//// ���f�[�^���
	//delete[] imageData;
	
	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;
	
	for (int i = 0; i < textureBuffers_.size(); i++) {

		//�܂��e�N�X�`����񂪊��蓖�Ă��Ă��Ȃ��Ƃ���Ƀe�N�X�`����������
		if (!textureBuffers_[i]) {
			
			textureBuffers_[i] = texBuff;

			UINT incrementSize = dxcommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			// SRV�q�[�v�̐擪�n���h�����擾
			D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
			srvHandle.ptr += incrementSize * i;
			dxcommon_->GetDevice()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);
			//i��߂�l�Ƃ��ĕԂ�
			return i;
		}
	}
	
}

uint32_t Texture::LoadTexture(const wchar_t* fileName)
{
	// ���ʊm�F
	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		fileName,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	ScratchImage mipChain{};
	// �~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);


	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//���\�[�X�ݒ�

	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;	// ��
	textureResourceDesc.Height = (UINT)metadata.height;	// ����
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// �e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff;
	result = dxcommon_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,				//�S�̈�փR�s�[
			img->pixels,			//���f�[�^�A�h���X
			(UINT)img->rowPitch,	//1���C���T�C�Y
			(UINT)img->slicePitch	//�S�T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	for (int i = 0; i < textureBuffers_.size(); i++) {

		//�܂��e�N�X�`����񂪊��蓖�Ă��Ă��Ȃ��Ƃ���Ƀe�N�X�`����������
		if (!textureBuffers_[i]) {

			textureBuffers_[i] = texBuff;

			UINT incrementSize = dxcommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			// SRV�q�[�v�̐擪�n���h�����擾
			D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
			srvHandle.ptr += incrementSize * i;
			dxcommon_->GetDevice()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);
			//i��߂�l�Ƃ��ĕԂ�
			return i;
		}
	}
}

void Texture::StaticInitialize(DirectXCommon* dxcommon)
{
	dxcommon_ = dxcommon;

	// ���ʊm�F
	HRESULT result;

	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	// �ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = dxcommon_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc,
		IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
}
