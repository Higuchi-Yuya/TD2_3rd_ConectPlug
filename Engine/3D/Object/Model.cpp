#include "Model.h"
#include <DirectXTex.h>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include "Texture.h"

using namespace DirectX;
using namespace std;

ID3D12Device* Model::device = nullptr;

Model::Model()
{
}

Model::~Model()
{
	for (auto m : materials) {
		delete m.second;
	}
	materials.clear();
}

Model* Model::LoadFromOBJ(const std::string& modelname, bool smoothing)
{
	Model* model = new Model;

	// �I�u�W�F�N�g�t�@�C������f�[�^��ǂݍ���
	model->LoadFromOBJInternal(modelname, smoothing);

	// �o�b�t�@����
	model->CreateBuffers();

	return model;
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	//�t�@�C���X�g���[��
	std::ifstream file;
	// �}�e���A���t�@�C�����J��
	file.open(directoryPath + filename);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	Material* material = nullptr;

	// 1�s���ǂݍ���
	string line;
	while (getline(file, line)) {
		// ��s���̕�������X�g���[���ɕϊ�
		std::istringstream line_stream(line);
		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		// �擪�̃^�u�����͖�������
		if (key[0] == '\t') {
			key.erase(key.begin());// �擪�̕������폜
		}

		// �擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl") {
			// ���Ƀ}�e���A���������
			if (material) {
				// �}�e���A�����R���e�i�ɓo�^
				AddMaterial(material);
			}

			// �V�����}�e���A���𐶐�
			material = Material::Create();

			// �}�e���A�����ǂݍ���
			line_stream >> material->name;
		}

		// �擪������Ks�Ȃ�A���r�G���g�F
		if (key == "Ka") {
			line_stream >> material->ambient.x;
			line_stream >> material->ambient.y;
			line_stream >> material->ambient.z;

		}
		// �擪������Ks�Ȃ�f�B�t���[�Y�F
		if (key == "Kd") {
			line_stream >> material->diffuse.x;
			line_stream >> material->diffuse.y;
			line_stream >> material->diffuse.z;

		}

		// �擪������Ks�Ȃ�X�y�L�����[�F
		if (key == "Ks") {
			line_stream >> material->specular.x;
			line_stream >> material->specular.y;
			line_stream >> material->specular.z;

		}
		// �擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd") {
			// �e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> material->textureFilename;
			// �e�N�X�`���ǂݍ���
			material->LoadTexture(directoryPath, material->textureFilename);
		}
	}
	// �t�@�C�������
	file.close();

	if (material) {
		// �}�e���A����o�^
		AddMaterial(material);
	}
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial)
{
	// nullptr�`�F�b�N
	assert(device);
	assert(cmdList);

	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::srvHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	for (auto& m : materials) {
		Material* material = m.second;
		// �}�e���A���p�萔�o�b�t�@�r���[���Z�b�g
		cmdList->SetGraphicsRootConstantBufferView(3, material->GetConstantBuffer()->GetGPUVirtualAddress());

		// �V�F�[�_���\�[�X�r���[���Z�b�g
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::srvHeap->GetGPUDescriptorHandleForHeapStart();
		UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		srvGpuHandle.ptr += incrementSize * material->textureIndex;
		cmdList->SetGraphicsRootDescriptorTable(0, srvGpuHandle);
	}
	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void Model::AddSmoothData(unsigned short indexPosition, unsigned short indexVertex)
{
	smoothData[indexPosition].emplace_back(indexVertex);
}

void Model::CalculateSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr) {
		// �e�ʗp�̋��ʒ��_�R���N�V����
		std::vector<unsigned short>& v = itr->second;

		// �S���_�̖@���𕽋ς���
		XMVECTOR normal = {};
		for (unsigned short index : v) {
			normal += XMVectorSet(vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());

		// ���ʖ@�����g�p���邷�ׂĂ̒��_�f�[�^�ɏ�������
		for (unsigned short index : v) {
			vertices[index].normal = { normal.m128_f32[0],normal.m128_f32[1],normal.m128_f32[2] };
		}
	}
}

void Model::LoadFromOBJInternal(const std::string& modelname, bool smoothing)
{
	// �t�@�C���X�g���[��
	std::ifstream file;

	// obj�t�@�C�����J��
	//const string modelname = "skydome";
	const string filename = modelname + ".obj"; // "triangle_mat.obj"
	const string directoryPath = "Resources/" + modelname + "/"; // "Resources/triangle_mat/"
	file.open(directoryPath + filename); //"Resources/triangle_mat/triangle_mat.obj"
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	int indexCountTex = 0;
	int indexCountNoTex = 0;

	vector<Vector3> positions; // ���_���W
	vector<Vector3> normals; // �@���x�N�g��
	vector<Vector2> texcoords; // �e�N�X�`��UV
	// 1�s���ǂݍ���
	string line;
	while (getline(file, line)) {
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		// �擪������mtllib�Ȃ�}�e���A��
		if (key == "mtllib")
		{
			// �}�e���A���̃t�@�C�����ǂݍ���
			string filename;
			line_stream >> filename;

			// �}�e���A���ǂݍ���
			LoadMaterial(directoryPath, filename);
		}

		// �擪������v�Ȃ璸�_���W
		if (key == "v") {
			// X,Y,Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			// ���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
		}

		// �擪������vt�Ȃ�e�N�X�`��
		if (key == "vt") {
			// U,V�����ǂݍ���
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			// V�������]
			texcoord.y = 1.0f - texcoord.y;
			// �e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);

		}

		// �擪������VN�Ȃ�@���x�N�g��
		if (key == "vn") {
			// X,Y,Z�����ǂݍ���
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			// �@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}
		// �擪������usemtl�Ȃ�}�e���A�������蓖�Ă�
		if (key == "usemtl") {

			// �}�e���A���̖��ǂݍ���
			string materialName;
			line_stream >> materialName;

			// �}�e���A�����Ō������A�}�e���A�������蓖�Ă�
			auto itr = materials.find(materialName);
			if (itr != materials.end()) {

			}

		}
		// �擪������f�Ȃ�|���S���i�O�p�`�j
		if (key == "f") {
			int faceIndexCount = 0;
			// ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				// ���_�C���f�b�N�X�P���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				// ���_�ԍ�
				index_stream >> indexPosition;

				index_stream.seekg(1, ios_base::cur);// �X���b�V�����΂�
				// �}�e���A���A�e�N�X�`��������ꍇ
				if (materials.size() > 0) {
					index_stream >> indexTexcoord;
					index_stream.seekg(1, ios_base::cur);// �X���b�V�����΂�
					index_stream >> indexNormal;
					// ���_�f�[�^�̒ǉ�
					VertexPosNormalUv vertex{};
					vertex.pos = positions[indexPosition - 1];
					vertex.normal = normals[indexNormal - 1];
					vertex.uv = texcoords[indexTexcoord - 1];
					vertices.emplace_back(vertex);

					// �G�b�W�������p�̃f�[�^��ǉ�
					if (smoothing) {
						// v�L�[�i���W�f�[�^�j�̔ԍ��ƁA�S�č����������_�̃C���f�b�N�X���Z�b�g�œo�^����
						AddSmoothData(indexPosition, (unsigned short)GetVertexCount() - 1);
					}
				}
				else {
					char c;
					index_stream >> c;
					// �X���b�V��2�A���̏ꍇ�A���_�ԍ��̂�
					if (c == '/') {
						// ���_�f�[�^�̒ǉ�
						VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = { 0, 0, 1 };
						vertex.uv = { 0, 0 };
						vertices.emplace_back(vertex);

						// �G�b�W�������p�̃f�[�^��ǉ�
						if (smoothing) {
							// v�L�[�i���W�f�[�^�j�̔ԍ��ƁA�S�č����������_�̃C���f�b�N�X���Z�b�g�œo�^����
							AddSmoothData(indexPosition, (unsigned short)GetVertexCount() - 1);
						}
					}
					else {
						index_stream.seekg(-1, ios_base::cur); // 1�����߂�
						index_stream >> indexTexcoord;
						index_stream.seekg(1, ios_base::cur); // �X���b�V�����΂�
						index_stream >> indexNormal;
						// ���_�f�[�^�̒ǉ�
						VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = normals[indexNormal - 1];
						vertex.uv = { 0, 0 };
						vertices.emplace_back(vertex);

						// �G�b�W�������p�̃f�[�^��ǉ�
						if (smoothing) {
							// v�L�[�i���W�f�[�^�j�̔ԍ��ƁA�S�č����������_�̃C���f�b�N�X���Z�b�g�œo�^����
							AddSmoothData(indexPosition, (unsigned short)GetVertexCount() - 1);
						}
					}
				}
				// �C���f�b�N�X�f�[�^�̒ǉ�
				if (faceIndexCount >= 3) {
					// �l�p�`�|���S����4�_�ڂȂ̂ŁA
					// �l�p�`��0,1,2,3�̓� 2,3,0�ŎO�p�`���\�z����
					indices.emplace_back(indexCountTex - 1);
					indices.emplace_back(indexCountTex);
					indices.emplace_back(indexCountTex - 3);
				}
				else {
					indices.emplace_back(indexCountTex);
				}
				indexCountTex++;
				faceIndexCount++;
				//// �C���f�b�N�X�f�[�^�̒ǉ�
				//indices.emplace_back((unsigned short)indices.size());
			}
		}
	}
	// �t�@�C�������
	file.close();

	// ���_�@���̕��ςɂ��G�b�W�̕�����
	if (smoothing) {
		CalculateSmoothedVertexNormals();
	}
}

void Model::CreateBuffers()
{
	HRESULT result = S_FALSE;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());//vertices
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	CD3DX12_RESOURCE_DESC resourceDesc2 = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	// ���\�[�X�ݒ�
	resourceDesc.Width = sizeIB;

	// �C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc2, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&indexBuff));

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	//// ���\�[�X�f�X�N���}�e���A���p�o�b�t�@�Ƀ��T�C�Y
	//resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff);

	for (auto& m : materials) {
		Material* material = m.second;
		material->Update();
	}




}

void Model::AddMaterial(Material* material)
{
	// �R���e�i�ɓo�^
	materials.emplace(material->name, material);
}

void Model::SetDevice(ID3D12Device* device)
{
	Model::device = device;
	Material::StaticInitialize(device);
}