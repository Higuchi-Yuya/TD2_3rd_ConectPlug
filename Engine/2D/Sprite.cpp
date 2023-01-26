#include "Sprite.h"
#include "WinApp.h"

SpriteManager* Sprite::spriteManager_ = nullptr;

void Sprite::Initialize(uint32_t textureNum, Vector2 position, Vector2 size, Vector4 color)
{
	// ���̉摜�̃T�C�Y�ɓK��
	if (textureNum != UINT32_MAX) {
		textureIndex_ = textureNum;
		AbjustTextureSize();
		//�e�N�X�`���T�C�Y���X�v���C�g�̃T�C�Y�ɓK�p
		size_ = textureSize_;
	}

	// �����Ŏ����Ă������̂���
	position_.x = position.x;
	position_.y = position.y;
	size_ = size;
	color_ = color;

	// ���_�f�[�^

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	result = spriteManager_->dxcommon_->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾

	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);

	//�萔�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(SpriteManager::ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@����
	result = spriteManager_->dxcommon_->GetDevice()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	constMap->color = Vector4(1, 0, 0, 1.0f);
	
	// �P�ʍs�����
	constMap->mat.identity();

	// ���W�ϊ�
	constMap->mat.m[0][0] = 2.0f / WinApp::window_width;
	constMap->mat.m[1][1] = -2.0f / WinApp::window_height;
	constMap->mat.m[3][0] = -1.0f;
	constMap->mat.m[3][1] = 1.0f;
}

void Sprite::StaticInitialize(SpriteManager* spriteManager)
{
	spriteManager_ = spriteManager;
}

void Sprite::Updata()
{
	//���_�f�[�^�������o�ϐ��Ōv�Z
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	// ���E���]
	if (isFlipX_) {
		left = -left;
		right = -right;
	}

	// �㉺���]
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	// ���_�f�[�^�̐ݒ�
	vertices[LB].pos = { left,bottom,0.0f };
	vertices[LT].pos = { left,top,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,top,0.0f };

	//�e�N�X�`���o�b�t�@�擾
	ID3D12Resource* textureBuffer = spriteManager_->GetTextureBuffer(textureIndex_);

	//�w��ԍ��̃e�N�X�`�����ǂݍ��ݍς݂Ȃ�
	if (textureBuffer) {
		//�e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

		float tex_left = textureLeftTop_.x / resDesc.Width;
		float tex_right = (textureLeftTop_.x + textureSize_.x) / resDesc.Width;
		float tex_top = textureLeftTop_.y / resDesc.Height;
		float tex_bottom = (textureLeftTop_.y + textureSize_.y) / resDesc.Height;

		//���_��UV�ɔ��f
		vertices[LB].uv = { tex_left,tex_bottom };
		vertices[LT].uv = { tex_left,tex_top };
		vertices[RB].uv = { tex_right,tex_bottom };
		vertices[RT].uv = { tex_right,tex_top };
	}

	// ���_�f�[�^��GPU�ɓ]��
	std::copy(std::begin(vertices), std::end(vertices), vertMap);

	// �s��̐ݒ�
	Matrix4 matRot;
	Matrix4 matTrans;

	matRot.identity();
	matTrans.identity();

	matRot.rotation(rotation_);
	matTrans.translate({ position_.x,position_.y,0.0f });

	Matrix4 matProjection;
	matProjection.identity();
	matProjection.m[0][0] = 2.0f / WinApp::window_width;
	matProjection.m[1][1] = -2.0f / WinApp::window_height;
	matProjection.m[3][0] = -1.0f;
	matProjection.m[3][1] = 1.0f;

	// �s��̊|���Z
	matWorld.identity();
	matWorld *= matRot;
	matWorld *= matTrans;

	// �s��̓]��
	constMap->mat = matWorld * matProjection;

	// �F��]��
	constMap->color = color_;
}

void Sprite::Draw()
{
	//��\���Ȃ珈���I��
	if (isInvisible_) {
		return;
	}
	spriteManager_->SetTextureCommands(textureIndex_);

	Updata();

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spriteManager_->dxcommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spriteManager_->dxcommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	// �`��R�}���h
	spriteManager_->dxcommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
}

void Sprite::AbjustTextureSize()
{
	ID3D12Resource* textureBuffer = spriteManager_->GetTextureBuffer(textureIndex_);
	assert(textureBuffer);
	
	// �e�N�X�`�����擾
	D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

	textureSize_.x = static_cast<float>(resDesc.Width);
	textureSize_.y = static_cast<float>(resDesc.Height);
}
