#pragma once
#include <d3d12.h>
#include<dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include <chrono>
#include "WinApp.h"

class DirectXCommon
{
public:// �����o�֐�
	// ������
	void Initalize(WinApp* winApp);

	// �`��O����
	void PreDraw();

	// �`��㏈��
	void PostDraw();

	/// <summary>
	/// �[�x�o�b�t�@�̃N���A
	/// </summary>
	void ClearDepthBuffer();

	// �f�o�C�X�擾
	ID3D12Device* GetDevice() const { return device.Get();}

	// �R�}���h���X�g�擾
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList.Get(); }
	
	// �o�b�N�o�b�t�@�̐����擾
	size_t GetBackBufferCount() const { return backBuffers.size(); }

private:
	// �f�o�C�X�̏�����
	void InitalizeDevice();

	// �R�}���h�֘A�̏�����
	void InitialzeCommand();

	// �X���b�v�`�F�[���̏�����
	void InitializeSwapchain(WinApp* winApp);

	// �����_�[�^�[�Q�b�g�r���[�̏�����
	void InitializeRenderTargetView();

	// �[�x�o�b�t�@�̏�����
	void InitializeDepthBuffer();

	// �t�F���X�̏�����
	void InitializeFence();

	// FPS�Œ菉����
	void InitializeFixFPS();

	// FPS�Œ�X�V
	void UpdateFixFPS();

private:
	// WindowsAPI
	WinApp* winApp = nullptr;
	// DirectX12�f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	// DXGI�t�@�N�g��
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;

	// �R�}���h�֘A
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;

	// �X���b�v�`�F�[���֘A
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// �����_�[�^�[�Q�b�g�r���[�֘A
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// �o�b�N�o�b�t�@
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

	// �[�x�o�b�t�@�֘A
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;

	// ���\�[�X�o���A
	D3D12_RESOURCE_BARRIER barrierDesc{};

	// �t�F���X
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

	// ���ʊm�F
	HRESULT result;

	// �L�^����(FPS�Œ�p)
	std::chrono::steady_clock::time_point reference_;
};

