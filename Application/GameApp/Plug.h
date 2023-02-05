#include "Object3d.h"
#include "Collider.h"
#include "MathFunc.h"

class Stage;
class Socket;
class Player;
class Sound;
class Plug {
public:
	Plug();
	~Plug();

	//�����o�֐�
	void Initialize(Vector3 pos , int face);
	void Update();
	void Draw(ViewProjection* viewProjection);

	void Reset(Vector3 pos, int face);

private:
	//�R�[�h�̃��X�g�p�\����
	typedef struct Cord {

		Object3d* gameObject = nullptr;
		WorldTransform start;
		WorldTransform end;
		WorldTransform oldPos;
		WorldTransform oldStart;
		WorldTransform oldEnd;
		Vector3 angle;
		Cord* prev = nullptr;
		Cord* next = nullptr;
		Collider* collider[3];

	};
	//�R�[�h�̒���(�z��̗v�f��)
	static const int cordLength_ = 50;
	void CordUpdate();
	void PlugUpdate();

	void CordReel();

	//�A�N�Z�b�T
public:

	WorldTransform GetPlugWorldTransform();
	void SetPlugWorldTransform(WorldTransform worldTransform);
	void SetWorldPos(Vector3 pos);

	WorldTransform GetCordWorldTransform(int cordNum);

	int GetIsGrabbed();
	void SetIsGrabbed(int isGrabbed);

	void SetStage(Stage* stage);
	void SetSocket(Socket* socket,bool isEnemy);
	void SetEnemySocket(Socket* enemySocket, bool isEnemy);

	int GetIsConnect();
	void SetIsConnect(int isConnect);

	bool GetIsEnemyConnect(int num) { return isEnemyConnect[num]; }


	int GetIsLimit();

	Collider* GetPlugCollider();
	Collider* GetCordCollider(int cordNum , int position);

	Object3d* plug_ = nullptr;
	//�R�[�h
	Cord cord_[cordLength_];
	//�\����
private:


	//enum
public:
	enum FACE {
		NORTH ,
		EAST ,
		SOUTH ,
		WEST ,
	};

	//�����o�ϐ�
private:
	int direction = 0;



	// ���f��
	Model* cordModel = nullptr;
	Model* plugModel = nullptr;
	Model* blockModel = nullptr;

	//�v���O

	Object3d* block_ = nullptr;
	Collider* plugCollider_ = nullptr;
	Collider* blockCollider_ = nullptr;

	int face_ = NORTH;

	//�t���O
	int isConnect_ = false;
	bool isEnemyConnect[6] = {0,0,0,0,0,0};
	int isGrabbed_ = false;
	int isReel_ = false;
	int isLimit_ = false;

	Stage* stage_ = nullptr;
	//�\�P�b�g
	std::vector<Socket*> socket_;
	int socketNum = 0;
	//enemy�\�P�b�g
	std::vector<Socket*> enemySocket_;
	int enemySocketNum = 0;

	// �v���O���q�������̉�
	Sound* connectSE = nullptr;
	bool isSE = false;
};