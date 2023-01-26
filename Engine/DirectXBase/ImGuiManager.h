#pragma once
#include <imgui.h>
#include "WinApp.h"
#include "DirectXCommon.h"
class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();

	// ������
	void Initialize(WinApp *winApp, DirectXCommon *dxCommon);

	// ImGui��t�J�n
	void Begin();

	// ImGui��t�I��
	void End();

	// ��ʂւ̕`��
	void Draw();

	// ��n��
	void Finalize();




private:
	static DirectXCommon* dxCommon;

	// SRV�p�̃f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
};