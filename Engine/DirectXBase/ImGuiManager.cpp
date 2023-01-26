#include "ImGuiManager.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

DirectXCommon* ImGuiManager::dxCommon = nullptr;

ImGuiManager::ImGuiManager()
{
}

ImGuiManager::~ImGuiManager()
{

}

void ImGuiManager::Initialize(WinApp* winApp, DirectXCommon* dxCommon)
{
	this->dxCommon = dxCommon;

	// ImGui�̃R���e�L�X�g�𐶐�
	ImGui::CreateContext();

	// ImGui�̃X�^�C����ݒ�
	ImGui::StyleColorsDark();

	// win32�p�̏����֐����Ăяo��
	ImGui_ImplWin32_Init(winApp->GetHwnd());

	// ���ʊm�F
	HRESULT result;

	// �f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	// �f�X�N���v�^�q�[�v����
	result = this->dxCommon->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	// DirectX12�p������
	ImGui_ImplDX12_Init(
		this->dxCommon->GetDevice(),
		static_cast<int>(this->dxCommon->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	);

	// �t�H���g�ǉ�
	ImGuiIO& io = ImGui::GetIO();

	// �W���t�H���g��ǉ�����
	io.Fonts->AddFontDefault();

}

void ImGuiManager::Begin()
{
	// ImGui�t���[���J�n
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	// �`��O����
	ImGui::Render();
}

void ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();

	// �f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// �`��R�}���h�𔭍s
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

void ImGuiManager::Finalize()
{
	// ��n��
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// �f�X�N���v�^�q�[�v�����
	srvHeap_.Reset();
}
