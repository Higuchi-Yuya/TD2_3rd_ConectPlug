#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include <d3d12.h>
#include <d3dx12.h>
#include <string>

class Material
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:// �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1
	{
		Vector3 ambient; // �A���r�G���g�W��
		float pad1; // �p�f�B���O
		Vector3 diffuse; // �f�B�t���[�Y�W��
		float pad2; // �p�f�B���O
		Vector3 specular; // �X�y�L�����[�W��
		float alpha; // �A���t�@
	};
public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// �}�e���A������
	/// </summary>
	/// <returns>�������ꂽ�}�e���A��</returns>
	static Material* Create();


private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;

public:
	std::string name;// �}�e���A����
	Vector3 ambient; // �A���r�G���g�e���x
	Vector3 diffuse; // �f�B�t���[�Y�e���x
	Vector3 specular; // �X�y�L�����[�e���x
	float alpha; // �A���t�@
	std::string textureFilename; // �e�N�X�`���t�@�C����

	// �e�N�X�`���ԍ�
	uint32_t textureIndex = 0;
public:
	/// <summary>
	/// �萔�o�b�t�@�̎擾
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetConstantBuffer() { return constBuff.Get(); }

	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="directoryPath">�ǂݍ��݃f�B���N�g���p�X</param>

	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

private:

	// �}�e���A���p�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;

	ConstBufferDataB1* constMap = nullptr;

private:
	// �R���X�g���N�^
	Material() {
		ambient = { 0.3f, 0.3f, 0.3f };
		diffuse = { 0.0f, 0.0f, 0.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 1.0f;
	}
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �萔�o�b�t�@�̐���
	/// </summary>
	void CreateConstantBuffer();
};

