#include "Plug.h"
#include "Stage.h"
#include "Socket.h"

//�R���X�g���N�^
Plug::Plug() {
	block_ = Object3d::Create();
	blockModel = new Model;
	blockModel = Model::LoadFromOBJ("block");

	plug_ = Object3d::Create();
	plugModel = new Model;
	plugModel = Model::LoadFromOBJ("plug");


	cordModel = new Model;
	cordModel = Model::LoadFromOBJ("cord");

	plugCollider_ = new Collider;

	blockCollider_ = new Collider;

	for (int i = 0; i < cordLength_; i++) {
		cord_[i].gameObject = Object3d::Create();
		cord_[i].gameObject->SetModel(cordModel);

		cord_[i].collider[0] = new Collider;
		cord_[i].collider[1] = new Collider;
		cord_[i].collider[2] = new Collider;
	}
}

//�f�X�g���N�^
Plug::~Plug() {

	for (int i = 0; i < cordLength_; i++) {
		delete cord_[i].gameObject;
		delete cord_[i].collider[0];
		delete cord_[i].collider[1];
		delete cord_[i].collider[2];
	}
	
	delete plugCollider_;
	delete blockCollider_;
	delete plug_;
	delete block_;
	delete cordModel;
	delete plugModel;
	delete blockModel;
}

//�����o�֐�
void Plug::Initialize(Vector3 pos, int face) {

	face_ = face;


	block_->SetModel(blockModel);
	block_->worldTransform_.position_ = pos;

	block_->worldTransform_.rotation_ = {
		0 ,
		MathFunc::Utility::Deg2Rad(90) * face_ ,
		0
	};



	plug_->SetModel(plugModel);
	plug_->worldTransform_.position_ = {
		block_->worldTransform_.position_.x + 2.0f * sin(MathFunc::Utility::Deg2Rad(90) * face_) ,
		block_->worldTransform_.position_.y ,
		block_->worldTransform_.position_.z + 2.0f * cos(MathFunc::Utility::Deg2Rad(90) * face_)
	};
	plug_->worldTransform_.rotation_ = {
		0,
		MathFunc::Utility::Deg2Rad(90),
		0,
	};
	plug_->Update();


	plugCollider_->Initialize(&plug_->worldTransform_);
	plugCollider_->SetRadius(1.001f);


	blockCollider_->Initialize(&block_->worldTransform_);
	blockCollider_->SetRadius(1.0f);

	for (int i = 0; i < cordLength_; i++) {
		cord_[i].gameObject->SetModel(cordModel);

		cord_[i].gameObject->worldTransform_.position_ = pos;

		cord_[i].gameObject->worldTransform_.scale_ = { 0.3f , 0.3f , 0.5f };

		cord_[i].angle = { 0 , 0 , 1.0f };

		//�����̈ʒu���w��
		cord_[i].start.position_ = {
			cord_[i].gameObject->worldTransform_.position_.x -
			(cord_[i].gameObject->worldTransform_.scale_.x * cord_[i].angle.x * 0.8f) ,
			cord_[i].gameObject->worldTransform_.position_.y -
			(cord_[i].gameObject->worldTransform_.scale_.y * cord_[i].angle.y * 0.8f) ,
			cord_[i].gameObject->worldTransform_.position_.z -
			(cord_[i].gameObject->worldTransform_.scale_.z * cord_[i].angle.z * 0.8f) ,
		};

		//��[�̈ʒu���w��
		cord_[i].end.position_ = {
			cord_[i].gameObject->worldTransform_.position_.x +
			(cord_[i].gameObject->worldTransform_.scale_.x * cord_[i].angle.z * 0.8f) ,
			cord_[i].gameObject->worldTransform_.position_.y +
			(cord_[i].gameObject->worldTransform_.scale_.y * cord_[i].angle.z * 0.8f) ,
			cord_[i].gameObject->worldTransform_.position_.z +
			(cord_[i].gameObject->worldTransform_.scale_.z * cord_[i].angle.z * 0.8f) ,
		};


		//3D�I�u�W�F�N�g�̍X�V
		cord_[i].start.Initialize();
		cord_[i].end.Initialize();
		cord_[i].oldPos.Initialize();
		cord_[i].oldStart.Initialize();
		cord_[i].oldEnd.Initialize();

		//�O��̃R�[�h��o�^
		if (i == 0) {
			cord_[i].next = &cord_[i + 1];
		}
		else if (i == cordLength_ - 1) {
			cord_[i].prev = &cord_[i - 1];
		}
		else {
			cord_[i].next = &cord_[i + 1];
			cord_[i].prev = &cord_[i - 1];
		}


		cord_[i].collider[0]->Initialize(&cord_[i].gameObject->worldTransform_);
		cord_[i].collider[0]->SetRadius(0.2f);


		cord_[i].collider[1]->Initialize(&cord_[i].start);
		cord_[i].collider[1]->SetRadius(0.2f);


		cord_[i].collider[2]->Initialize(&cord_[i].end);
		cord_[i].collider[2]->SetRadius(0.2f);

		//1�t���[���O�̍����Ɛ�[�̍��W�Ɍ��݂̍����Ɛ�[�̍��W����
		cord_[i].oldStart.position_ = cord_[i].start.position_;
		cord_[i].oldEnd.position_ = cord_[i].end.position_;

	}

}

void Plug::Update() {

	isLimit_ = false;

	block_->Update();
	blockCollider_->Update();
	CordUpdate();
	PlugUpdate();


}

void Plug::Draw(ViewProjection* viewProjection) {

	block_->Draw(viewProjection);
	plug_->Draw(viewProjection);
	for (int i = 0; i < cordLength_; i++) {
		cord_[i].gameObject->Draw(viewProjection);
	}

}

void Plug::Reset(Vector3 pos, int face)
{
	face_ = face;


	block_->SetModel(blockModel);
	block_->worldTransform_.position_ = pos;

	block_->worldTransform_.rotation_ = {
		0 ,
		MathFunc::Utility::Deg2Rad(90) * face_ ,
		0
	};

	plug_->worldTransform_.position_ = {
		block_->worldTransform_.position_.x + 2.0f * sin(MathFunc::Utility::Deg2Rad(90) * face_) ,
		block_->worldTransform_.position_.y ,
		block_->worldTransform_.position_.z + 2.0f * cos(MathFunc::Utility::Deg2Rad(90) * face_)
	};
	plug_->Update();


	plugCollider_->Initialize(&plug_->worldTransform_);
	plugCollider_->SetRadius(1.001f);


	blockCollider_->Initialize(&block_->worldTransform_);
	blockCollider_->SetRadius(1.0f);

	for (int i = 0; i < cordLength_; i++) {

		cord_[i].gameObject->worldTransform_.position_ = pos;

		cord_[i].gameObject->worldTransform_.scale_ = { 0.3f , 0.3f , 0.5f };

		cord_[i].angle = { 0 , 0 , 1.0f };

		//�����̈ʒu���w��
		cord_[i].start.position_ = {
			cord_[i].gameObject->worldTransform_.position_.x -
			(cord_[i].gameObject->worldTransform_.scale_.x * cord_[i].angle.x * 0.8f) ,
			cord_[i].gameObject->worldTransform_.position_.y -
			(cord_[i].gameObject->worldTransform_.scale_.y * cord_[i].angle.y * 0.8f) ,
			cord_[i].gameObject->worldTransform_.position_.z -
			(cord_[i].gameObject->worldTransform_.scale_.z * cord_[i].angle.z * 0.8f) ,
		};

		//��[�̈ʒu���w��
		cord_[i].end.position_ = {
			cord_[i].gameObject->worldTransform_.position_.x +
			(cord_[i].gameObject->worldTransform_.scale_.x * cord_[i].angle.z * 0.8f) ,
			cord_[i].gameObject->worldTransform_.position_.y +
			(cord_[i].gameObject->worldTransform_.scale_.y * cord_[i].angle.z * 0.8f) ,
			cord_[i].gameObject->worldTransform_.position_.z +
			(cord_[i].gameObject->worldTransform_.scale_.z * cord_[i].angle.z * 0.8f) ,
		};


		//3D�I�u�W�F�N�g�̍X�V
		cord_[i].start.Initialize();
		cord_[i].end.Initialize();
		cord_[i].oldPos.Initialize();
		cord_[i].oldStart.Initialize();
		cord_[i].oldEnd.Initialize();

		//�O��̃R�[�h��o�^
		if (i == 0) {
			cord_[i].next = &cord_[i + 1];
		}
		else if (i == cordLength_ - 1) {
			cord_[i].prev = &cord_[i - 1];
		}
		else {
			cord_[i].next = &cord_[i + 1];
			cord_[i].prev = &cord_[i - 1];
		}


		cord_[i].collider[0]->Initialize(&cord_[i].gameObject->worldTransform_);
		cord_[i].collider[0]->SetRadius(0.2f);


		cord_[i].collider[1]->Initialize(&cord_[i].start);
		cord_[i].collider[1]->SetRadius(0.2f);


		cord_[i].collider[2]->Initialize(&cord_[i].end);
		cord_[i].collider[2]->SetRadius(0.2f);

		//1�t���[���O�̍����Ɛ�[�̍��W�Ɍ��݂̍����Ɛ�[�̍��W����
		cord_[i].oldStart.position_ = cord_[i].start.position_;
		cord_[i].oldEnd.position_ = cord_[i].end.position_;

	}
	for (int i = 0; i < 6; i++)
	{
	isEnemyConnect[i] = false;
	}
	isConnect_ = false;
	isGrabbed_ = false;
	isReel_ = false;
	isLimit_ = false;

}

void Plug::CordUpdate() {

	//�R�[�h�̐擪�̍����̍��W�ɂ��v���O�̍��W����
	cord_[0].start.position_ = plug_->worldTransform_.position_;

	//�R�[�h�̏I�[�̐�[�̍��W���u���b�N�̈ʒu�ɑ��
	cord_[cordLength_ - 1].end.position_ = block_->worldTransform_.position_;


	if (isGrabbed_ == true) {
		direction = 0;

		int isCollision = false;

		isReel_ = false;

		//�擪�̃R�[�h�������Ă�����
		if (cord_[0].start.position_ != cord_[0].oldStart.position_) {
			//�����𐳂̕����ɕύX
			direction = 1;
		}
		//�I�[�̃R�[�h����������
		else if (cord_[cordLength_ - 1].end.position_ != cord_[cordLength_ - 1].oldEnd.position_) {
			//������{�̕����֕ύX
			direction = -1;
		}
		//�����łȂ����
		else {
			//�����̎w������Ȃ�
			direction = 0;
		}

		//���������Ȃ�
		if (direction == 1) {
			for (int i = 0; i < cordLength_; i++) {

				//��O�̃R�[�h������Ȃ�
				if (cord_[i].prev) {
					//���݂̃R�[�h�̍����̍��W�Ɉ�O�̃R�[�h�̐�[�̍��W����
					cord_[i].start.position_ = cord_[i].prev->end.position_;
				}

				//�R�[�h�̌������擾�����K��
				cord_[i].angle = cord_[i].end.position_ - cord_[i].start.position_;
				cord_[i].angle.normalize();

				//�R�[�h�̍����̈ʒu����ɃR�[�h�̐�[��3D�I�u�W�F�N�g�̍��W�ƌ������v�Z
				cord_[i].end.position_.x = cord_[i].start.position_.x + cord_[i].angle.x * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;
				cord_[i].end.position_.y = cord_[i].start.position_.y + cord_[i].angle.y * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;
				cord_[i].end.position_.z = cord_[i].start.position_.z + cord_[i].angle.z * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;

				cord_[i].gameObject->worldTransform_.position_.x = cord_[i].start.position_.x + cord_[i].angle.x * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;
				cord_[i].gameObject->worldTransform_.position_.y = cord_[i].start.position_.y + cord_[i].angle.y * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;
				cord_[i].gameObject->worldTransform_.position_.z = cord_[i].start.position_.z + cord_[i].angle.z * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;

				cord_[i].gameObject->worldTransform_.rotation_.y = atan2(cord_[i].angle.x, cord_[i].angle.z);

				float length = sqrt(cord_[i].angle.x * cord_[i].angle.x + cord_[i].angle.z * cord_[i].angle.z);

				cord_[i].gameObject->worldTransform_.rotation_.x = atan2(-cord_[i].angle.y, length);

				cord_[i].gameObject->Update();
				cord_[i].start.UpdateMatrix();
				cord_[i].end.UpdateMatrix();

				cord_[i].collider[0]->Update();
				cord_[i].collider[1]->Update();
				cord_[i].collider[2]->Update();

				for (int j = 0; j < stage_->GetObjectcount(); j++) {
					if (cord_[i].collider[0]->CheckCollision(stage_->GetCollider(j))) {
						cord_[i].gameObject->worldTransform_ = cord_[i].oldPos;
						cord_[i].start = cord_[i].oldStart;
						cord_[i].end = cord_[i].oldEnd;

						isCollision = true;

						break;
					}

					else if (cord_[i].collider[1]->CheckCollision(stage_->GetCollider(j))) {
						cord_[i].gameObject->worldTransform_ = cord_[i].oldPos;
						cord_[i].start = cord_[i].oldStart;
						cord_[i].end = cord_[i].oldEnd;

						isCollision = true;

						break;
					}

					else if (cord_[i].collider[2]->CheckCollision(stage_->GetCollider(j))) {
						cord_[i].gameObject->worldTransform_ = cord_[i].oldPos;
						cord_[i].start = cord_[i].oldStart;
						cord_[i].end = cord_[i].oldEnd;

						isCollision = true;

						break;
					}
				}
			}
		}
		//���������Ȃ�
		else if (direction == -1) {
			for (int i = cordLength_ - 1; 0 <= i; i--) {
				//����̃R�[�h������Ȃ�
				if (cord_[i].next) {
					//���݂̃R�[�h�̐�[�̍��W�Ɉ���̃R�[�h�̍����̍��W����
					cord_[i].end.position_ = cord_[i].next->start.position_;
				}

				//�R�[�h�̌������擾�����K��
				cord_[i].angle = cord_[i].start.position_ - cord_[i].end.position_;
				cord_[i].angle.normalize();

				//�R�[�h�̐�[�̈ʒu����ɃR�[�h�̍�����3D�I�u�W�F�N�g�̍��W�ƌ������v�Z
				cord_[i].start.position_.x = cord_[i].end.position_.x + cord_[i].angle.x * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;
				cord_[i].start.position_.y = cord_[i].end.position_.y + cord_[i].angle.y * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;
				cord_[i].start.position_.z = cord_[i].end.position_.z + cord_[i].angle.z * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;

				cord_[i].gameObject->worldTransform_.position_.x = cord_[i].end.position_.x + cord_[i].angle.x * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;
				cord_[i].gameObject->worldTransform_.position_.y = cord_[i].end.position_.y + cord_[i].angle.y * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;
				cord_[i].gameObject->worldTransform_.position_.z = cord_[i].end.position_.z + cord_[i].angle.z * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;

				cord_[i].gameObject->worldTransform_.rotation_.y = atan2(cord_[i].angle.x, cord_[i].angle.z);

				float length = sqrt(cord_[i].angle.x * cord_[i].angle.x + cord_[i].angle.z * cord_[i].angle.z);

				cord_[i].gameObject->worldTransform_.rotation_.x = atan2(-cord_[i].angle.y, length);

				cord_[i].gameObject->Update();
				cord_[i].start.UpdateMatrix();
				cord_[i].end.UpdateMatrix();

				cord_[i].collider[0]->Update();
				cord_[i].collider[1]->Update();
				cord_[i].collider[2]->Update();

				for (int j = 0; j < stage_->GetObjectcount(); j++) {
					if (cord_[i].collider[0]->CheckCollision(stage_->GetCollider(j))) {
						cord_[i].gameObject->worldTransform_ = cord_[i].oldPos;
						cord_[i].start.position_ = cord_[i].oldStart.position_;
						cord_[i].end.position_ = cord_[i].oldEnd.position_;

						isCollision = true;

						break;
					}

					else if (cord_[i].collider[1]->CheckCollision(stage_->GetCollider(j))) {
						cord_[i].gameObject->worldTransform_ = cord_[i].oldPos;
						cord_[i].start.position_ = cord_[i].oldStart.position_;
						cord_[i].end.position_ = cord_[i].oldEnd.position_;

						isCollision = true;

						break;
					}

					else if (cord_[i].collider[2]->CheckCollision(stage_->GetCollider(j))) {
						cord_[i].gameObject->worldTransform_ = cord_[i].oldPos;
						cord_[i].start.position_ = cord_[i].oldStart.position_;
						cord_[i].end.position_ = cord_[i].oldEnd.position_;

						isCollision = true;

						break;
					}
				}
			}
		}

		if (cord_[cordLength_ - 1].collider[2]->CheckCollision(*blockCollider_) == false) {
			cord_[cordLength_ - 1].end.position_ = cord_[cordLength_ - 1].oldEnd.position_;
			for (int i = cordLength_ - 1; 0 <= i; i--) {
				//����̃R�[�h������Ȃ�
				if (cord_[i].next) {
					//���݂̃R�[�h�̐�[�̍��W�Ɉ���̃R�[�h�̍����̍��W����
					cord_[i].end.position_ = cord_[i].next->start.position_;
				}

				//�R�[�h�̌������擾�����K��
				cord_[i].angle = cord_[i].start.position_ - cord_[i].end.position_;
				cord_[i].angle.normalize();

				//�R�[�h�̐�[�̈ʒu����ɃR�[�h�̍�����3D�I�u�W�F�N�g�̍��W�ƌ������v�Z
				cord_[i].start.position_.x = cord_[i].end.position_.x + cord_[i].angle.x * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;
				cord_[i].start.position_.y = cord_[i].end.position_.y + cord_[i].angle.y * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;
				cord_[i].start.position_.z = cord_[i].end.position_.z + cord_[i].angle.z * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;

				cord_[i].gameObject->worldTransform_.position_.x = cord_[i].end.position_.x + cord_[i].angle.x * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;
				cord_[i].gameObject->worldTransform_.position_.y = cord_[i].end.position_.y + cord_[i].angle.y * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;
				cord_[i].gameObject->worldTransform_.position_.z = cord_[i].end.position_.z + cord_[i].angle.z * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;

				cord_[i].gameObject->worldTransform_.rotation_.y = atan2(cord_[i].angle.x, cord_[i].angle.z);

				float length = sqrt(cord_[i].angle.x * cord_[i].angle.x + cord_[i].angle.z * cord_[i].angle.z);

				cord_[i].gameObject->worldTransform_.rotation_.x = atan2(-cord_[i].angle.y, length);

			}
		}

		if (isCollision == true) {

			for (int i = cordLength_ - 1; 0 <= i; i--) {
				//����̃R�[�h������Ȃ�
				if (cord_[i].next) {
					//���݂̃R�[�h�̐�[�̍��W�Ɉ���̃R�[�h�̍����̍��W����
					cord_[i].end.position_ = cord_[i].next->start.position_;
				}

				//�R�[�h�̌������擾�����K��
				cord_[i].angle = cord_[i].start.position_ - cord_[i].end.position_;
				cord_[i].angle.normalize();

				//�R�[�h�̐�[�̈ʒu����ɃR�[�h�̍�����3D�I�u�W�F�N�g�̍��W�ƌ������v�Z
				cord_[i].start.position_.x = cord_[i].end.position_.x + cord_[i].angle.x * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;
				cord_[i].start.position_.y = cord_[i].end.position_.y + cord_[i].angle.y * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;
				cord_[i].start.position_.z = cord_[i].end.position_.z + cord_[i].angle.z * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;

				cord_[i].gameObject->worldTransform_.position_.x = cord_[i].end.position_.x + cord_[i].angle.x * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;
				cord_[i].gameObject->worldTransform_.position_.y = cord_[i].end.position_.y + cord_[i].angle.y * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;
				cord_[i].gameObject->worldTransform_.position_.z = cord_[i].end.position_.z + cord_[i].angle.z * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;

				cord_[i].gameObject->worldTransform_.rotation_.y = atan2(cord_[i].angle.x, cord_[i].angle.z);

				float length = sqrt(cord_[i].angle.x * cord_[i].angle.x + cord_[i].angle.z * cord_[i].angle.z);

				cord_[i].gameObject->worldTransform_.rotation_.x = atan2(-cord_[i].angle.y, length);

				cord_[i].gameObject->Update();
				cord_[i].start.UpdateMatrix();
				cord_[i].end.UpdateMatrix();
			}

		}

	}
	else {
		if (isConnect_ == false) {

			isReel_ = true;

			for (int i = 0; i < cordLength_; i++) {

				if (cord_[i].next) {
					cord_[i].gameObject->worldTransform_.position_ = cord_[i].next->gameObject->worldTransform_.position_;
					cord_[i].gameObject->worldTransform_.rotation_ = cord_[i].next->gameObject->worldTransform_.rotation_;
					cord_[i].start.position_ = cord_[i].next->start.position_;
					cord_[i].oldStart.position_ = cord_[i].next->oldStart.position_;
					cord_[i].end.position_ = cord_[i].next->end.position_;
					cord_[i].oldEnd.position_ = cord_[i].next->oldEnd.position_;
				}
			}
		}
	}

	if (isReel_ == true) {
		CordReel();
	}

	for (int i = 0; i < cordLength_; i++) {

		//3D�I�u�W�F�N�g�̍X�V
		cord_[i].gameObject->Update();
		cord_[i].start.UpdateMatrix();
		cord_[i].end.UpdateMatrix();

		//1�t���[���O�̍����Ɛ�[�̍��W�Ɍ��݂̍����Ɛ�[�̍��W����
		cord_[i].oldPos = cord_[i].gameObject->worldTransform_;
		cord_[i].oldStart = cord_[i].start;
		cord_[i].oldEnd = cord_[i].end;

		cord_[i].oldPos.UpdateMatrix();
		cord_[i].oldStart.UpdateMatrix();
		cord_[i].oldEnd.UpdateMatrix();
	}

}

void Plug::PlugUpdate() {

	if (plug_->worldTransform_.position_ != cord_[0].start.position_) {
		plug_->worldTransform_.position_ = cord_[0].start.position_;
		isLimit_ = true;
	}
	if (direction != 0) {
		float angleY = cord_[0].gameObject->worldTransform_.rotation_.y + MathFunc::Utility::Deg2Rad(180);

		plug_->worldTransform_.rotation_.y = angleY;
	}
	else {
		plug_->worldTransform_.rotation_.y = cord_[0].gameObject->worldTransform_.rotation_.y;
	}


	plugCollider_->Update();

	for (int i = 0; i < socketNum; i++) {
		if (isGrabbed_ == false) {
			if (plugCollider_->CheckCollision(socket_[i]->GetCollider())) {

				

				float angleY = 0;
				// �k
				if (socket_[i]->GetFace() == 0) {
					angleY = MathFunc::Utility::Deg2Rad(90) * 2;
				}
				// ��
				if (socket_[i]->GetFace() == 1) {
					angleY = MathFunc::Utility::Deg2Rad(90) * 3;
				}
				// ��
				if (socket_[i]->GetFace() == 2) {
					angleY = MathFunc::Utility::Deg2Rad(90) * 0;
				}
				// ��
				if (socket_[i]->GetFace() == 3) {
					angleY = MathFunc::Utility::Deg2Rad(90) * 1;
				}
				plug_->worldTransform_.rotation_ = {
					0 ,
					angleY,
					0
				};

				if (socket_[i]->GetIsEnemy() == false) {
					plug_->worldTransform_.position_ = {
					socket_[i]->GetWorldTransform().position_.x + 2.0f * sin(MathFunc::Utility::Deg2Rad(90) * socket_[i]->GetFace()) ,
					socket_[i]->GetWorldTransform().position_.y ,
					socket_[i]->GetWorldTransform().position_.z + 2.0f * cos(MathFunc::Utility::Deg2Rad(90) * socket_[i]->GetFace())
					};
					isConnect_ = true;
					isReel_ = false;
				}
				
			}


			for (int i = 0; i < cordLength_; i++) {

				//��O�̃R�[�h������Ȃ�
				if (cord_[i].prev) {
					//���݂̃R�[�h�̍����̍��W�Ɉ�O�̃R�[�h�̐�[�̍��W����
					cord_[i].start.position_ = cord_[i].prev->end.position_;
				}

				//�R�[�h�̌������擾�����K��
				cord_[i].angle = cord_[i].end.position_ - cord_[i].start.position_;
				cord_[i].angle.normalize();

				//�R�[�h�̍����̈ʒu����ɃR�[�h�̐�[��3D�I�u�W�F�N�g�̍��W�ƌ������v�Z
				cord_[i].end.position_.x = cord_[i].start.position_.x + cord_[i].angle.x * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;
				cord_[i].end.position_.y = cord_[i].start.position_.y + cord_[i].angle.y * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;
				cord_[i].end.position_.z = cord_[i].start.position_.z + cord_[i].angle.z * cord_[i].gameObject->worldTransform_.scale_.z * 0.8 * 2;

				cord_[i].gameObject->worldTransform_.position_.x = cord_[i].start.position_.x + cord_[i].angle.x * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;
				cord_[i].gameObject->worldTransform_.position_.y = cord_[i].start.position_.y + cord_[i].angle.y * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;
				cord_[i].gameObject->worldTransform_.position_.z = cord_[i].start.position_.z + cord_[i].angle.z * cord_[i].gameObject->worldTransform_.scale_.z * 0.8;

				cord_[i].gameObject->worldTransform_.rotation_.y = atan2(cord_[i].angle.x, cord_[i].angle.z);

				float length = sqrt(cord_[i].angle.x * cord_[i].angle.x + cord_[i].angle.z * cord_[i].angle.z);

				cord_[i].gameObject->worldTransform_.rotation_.x = atan2(-cord_[i].angle.y, length);

				cord_[i].gameObject->Update();
				cord_[i].start.UpdateMatrix();
				cord_[i].end.UpdateMatrix();

				cord_[i].collider[0]->Update();
				cord_[i].collider[1]->Update();
				cord_[i].collider[2]->Update();

			}

		}
	}

	for (int i = 0; i < enemySocketNum; i++) {
		if (isGrabbed_ == false) {
			//�G
			if (plugCollider_->CheckCollision(enemySocket_[i]->GetCollider())) {
				float angleY = 0;
				// �k
				if (enemySocket_[i]->GetFace() == 0) {
					angleY = MathFunc::Utility::Deg2Rad(90) * 2;
				}
				// ��
				if (enemySocket_[i]->GetFace() == 1) {
					angleY = MathFunc::Utility::Deg2Rad(90) * 3;
				}
				// ��
				if (enemySocket_[i]->GetFace() == 2) {
					angleY = MathFunc::Utility::Deg2Rad(90) * 0;
				}
				// ��
				if (enemySocket_[i]->GetFace() == 3) {
					angleY = MathFunc::Utility::Deg2Rad(90) * 1;
				}
				plug_->worldTransform_.rotation_ = {
					0 ,
					angleY,
					0
				};

				if (enemySocket_[i]->GetIsEnemy() == false) {
					plug_->worldTransform_.position_ = {
					enemySocket_[i]->GetWorldTransform().position_.x + 2.0f * sin(MathFunc::Utility::Deg2Rad(90) * enemySocket_[i]->GetFace()) ,
					enemySocket_[i]->GetWorldTransform().position_.y ,
					enemySocket_[i]->GetWorldTransform().position_.z + 2.0f * cos(MathFunc::Utility::Deg2Rad(90) * enemySocket_[i]->GetFace())
					};
					isConnect_ = true;
					isReel_ = false;
				}
				else if (enemySocket_[i]->GetIsEnemy() == true && isConnect_ == false) {
					isEnemyConnect[i] = true;
					isReel_ = true;
				}

			}
		}
	}
	if (isReel_ == true) {

		if (plugCollider_->CheckCollision(*blockCollider_) == true) {

			plug_->worldTransform_.position_ = {
				block_->worldTransform_.position_.x + 2.0f * sin(MathFunc::Utility::Deg2Rad(90) * face_) ,
				block_->worldTransform_.position_.y ,
				block_->worldTransform_.position_.z + 2.0f * cos(MathFunc::Utility::Deg2Rad(90) * face_)
			};

			plug_->worldTransform_.rotation_ = block_->worldTransform_.rotation_;

			isReel_ = false;
		}
	}

	plug_->Update();

}

void Plug::CordReel() {

	for (int i = 0; i < cordLength_; i++) {

		if (cord_[i].next) {
			cord_[i].gameObject->worldTransform_.position_ = cord_[i].next->gameObject->worldTransform_.position_;
			cord_[i].gameObject->worldTransform_.rotation_ = cord_[i].next->gameObject->worldTransform_.rotation_;
			cord_[i].start.position_ = cord_[i].next->start.position_;
			cord_[i].oldStart.position_ = cord_[i].next->oldStart.position_;
			cord_[i].end.position_ = cord_[i].next->end.position_;
			cord_[i].oldEnd.position_ = cord_[i].next->oldEnd.position_;
		}
	}

}

WorldTransform Plug::GetPlugWorldTransform() {
	return plug_->worldTransform_;
}

void Plug::SetPlugWorldTransform(WorldTransform worldTransform) {
	plug_->worldTransform_ = worldTransform;
}

WorldTransform Plug::GetCordWorldTransform(int cordNum) {
	return cord_[cordNum].gameObject->worldTransform_;
}

int Plug::GetIsGrabbed() {
	return isGrabbed_;
}

void Plug::SetIsGrabbed(int isGrabbed) {
	isGrabbed_ = isGrabbed;
}

void Plug::SetStage(Stage* stage) {
	stage_ = stage;
}

void Plug::SetSocket(Socket* socket,bool isEnemy) {

	socket_.resize(socketNum + 1);

	socket_[socketNum] = socket;
	socket_[socketNum]->SetisEnemy(isEnemy);
	socketNum++;

}

void Plug::SetEnemySocket(Socket* enemySocket, bool isEnemy)
{
	enemySocket_.resize(enemySocketNum + 1);

	enemySocket_[enemySocketNum] = enemySocket;
	enemySocket_[enemySocketNum]->SetisEnemy(isEnemy);
	enemySocketNum++;
}

int Plug::GetIsConnect() {
	return isConnect_;
}

void Plug::SetIsConnect(int isConnect) {
	isConnect_ = isConnect;
}

int Plug::GetIsLimit() {
	return isLimit_;
}

Collider* Plug::GetPlugCollider() {
	return plugCollider_;
}

Collider* Plug::GetCordCollider(int cordNum, int position) {
	return cord_[cordNum].collider[position];
}

void Plug::SetWorldPos(Vector3 pos) {
	plug_->worldTransform_.position_ = pos;
}