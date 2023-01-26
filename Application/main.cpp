#pragma region �ǂݍ��ރw�b�_�[

#pragma comment(lib,"d3dcompiler.lib")

#include<iostream>

#include <math.h>

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SpriteManager.h"
#include "LightGroup.h"
#include "GameScene.h"

#pragma endregion

#pragma region ���܂��Ȃ�
// @brief �R���\�[����ʂɃt�H�[�}�b�g�t��������̕\��
// @param format �t�H�[�}�b�g(%d�Ƃ�%f�Ƃ���)
// @param �ϒ�����
// @remarks ���̊֐��̓f�o�b�N�p�ł��B�f�o�b�O���ɂ������삵�܂���
void DebugOutputFormatString(const char* format, ...) {
#ifdef _DEBUG
	va_list valist;
	va_start(valist, format);
	vprintf(format, valist);
	va_end(valist);
#endif
}


#pragma endregion

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR,  _In_ int) {

#pragma region WindowsAPI����������
	// �|�C���^
	WinApp* winApp = nullptr;

	// WindouwsAPI�̏�����
	winApp = new WinApp();
	winApp->Initialize();

#pragma endregion

#pragma region DirectX����������

	////////////////////////////////////////////////////
	//-------------DirectX12������������������-----------//
	//////////////////////////////////////////////////

	// Common�|�C���^
	DirectXCommon* dxCommon = nullptr;

	// DirectX�̏�����
	dxCommon = new DirectXCommon();
	dxCommon->Initalize(winApp);

	// ImGui�̏�����
	ImGuiManager* imguiManager = new ImGuiManager();
	imguiManager->Initialize(winApp,dxCommon);

	// ���͂̏�����
	Input::StaticInitialize(winApp);

	// �X�v���C�g�̏�����
	SpriteManager* spriteManager = nullptr;
	// �X�v���C�g���ʕ��̏�����
	spriteManager = new SpriteManager;
	spriteManager->Initialize(dxCommon);
	
	Sprite::StaticInitialize(spriteManager);

	// �e�N�X�`���̏�����
	Texture::StaticInitialize(dxCommon);
	
	// �I�u�W�F�N�g�̏�����
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	// �r���[�v���W�F�N�V�����̏�����
	ViewProjection::StaticInitialize(dxCommon->GetDevice());

	// ���C�g�̐ÓI������
	LightGroup::StaticInititalize(dxCommon->GetDevice());

	/////////////////////////////////////////////////////////
	//--------------DirectX12�����������@�����܂�-------------//
	///////////////////////////////////////////////////////
#pragma endregion

#pragma region �V�[���Ɏg���ϐ��̏�����
	GameScene* gameScene = new GameScene();
	gameScene->Initialize();

#pragma endregion

	//�Q�[�����[�v
	while (true) {
#pragma region �E�B���h�E���b�Z�[�W����

		// Windows�̃��b�Z�[�W����
		if (winApp->ProcessMessage()) {
			// �Q�[�����[�v�𔲂���
			break;
		}

#pragma endregion

#pragma region DirectX���t���[������
		/////////////////////////////////////////////////////
		//----------DireceX���t���[�������@��������------------//
		///////////////////////////////////////////////////


		gameScene->Update();


		//////////////////////////////////////////////
		//-------DireceX���t���[�������@�����܂�--------//
		////////////////////////////////////////////
#pragma region IMGUI�̍X�V����
	// ImGui�̍X�V����
		imguiManager->Begin();
		gameScene->ImguiUpdate();
		
		imguiManager->End();
#pragma endregion

#pragma endregion

#pragma region �`�揈��

		//�`��R�}���h��������
		dxCommon->PreDraw();
		
#pragma region �w�i�X�v���C�g�`��
		// �w�i�X�v���C�g�`��
		spriteManager->PreDraw();
		//-----�������� �w�i�X�v���C�g�`�� -----//
		gameScene->Draw2DBack();



		//-----�����܂� �w�i�X�v���C�g�`�� -----//
		spriteManager->PostDraw();
		// �[�x�o�b�t�@�N���A
		dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region �R�c���f���`��
		Object3d::PreDraw(dxCommon->GetCommandList());
		//-----�������� 3D���f���̕`�� -----//
		gameScene->Draw3D();

		
		//-----�����܂� 3D���f���̕`�� -----//
		Object3d::PostDraw();
#pragma endregion
		
#pragma region �O�i�X�v���C�g�`��
		// �`��O����
		spriteManager->PreDraw();
		//-----�������� 2D�`�� -------//
		gameScene->Draw2DFront();



		//-----�����܂� 2D�`�� -------//
		// �`��㏈��
		spriteManager->PostDraw();
#pragma endregion

#pragma region IMGUI�̕`��
		imguiManager->Draw();
#pragma endregion


		// �`��R�}���h�̏I��
		dxCommon->PostDraw();

#pragma endregion
	}
#pragma region  WindowsAPI��n��

	//�����N���X�͎g��Ȃ��̂œo�^����������

	// �Q�[���V�[���̉��
	delete gameScene;

	// ImGui�̃}�l�[�W���[�����
	imguiManager->Finalize();
	delete imguiManager;
	// �X�v���C�g�}�l�[�W���[�̉��
	delete spriteManager;

	// WindouwsAPI�̏I������
	winApp->Finalize();
	// WindouwsAPI���
	delete winApp;
	// DirectXcommon�̉��
	delete dxCommon;
#pragma endregion

	return 0;
}
