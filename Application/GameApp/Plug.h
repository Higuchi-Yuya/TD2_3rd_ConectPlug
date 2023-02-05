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

	//メンバ関数
	void Initialize(Vector3 pos , int face);
	void Update();
	void Draw(ViewProjection* viewProjection);

	void Reset(Vector3 pos, int face);

private:
	//コードのリスト用構造体
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
	//コードの長さ(配列の要素数)
	static const int cordLength_ = 50;
	void CordUpdate();
	void PlugUpdate();

	void CordReel();

	//アクセッサ
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
	//コード
	Cord cord_[cordLength_];
	//構造体
private:


	//enum
public:
	enum FACE {
		NORTH ,
		EAST ,
		SOUTH ,
		WEST ,
	};

	//メンバ変数
private:
	int direction = 0;



	// モデル
	Model* cordModel = nullptr;
	Model* plugModel = nullptr;
	Model* blockModel = nullptr;

	//プラグ

	Object3d* block_ = nullptr;
	Collider* plugCollider_ = nullptr;
	Collider* blockCollider_ = nullptr;

	int face_ = NORTH;

	//フラグ
	int isConnect_ = false;
	bool isEnemyConnect[6] = {0,0,0,0,0,0};
	int isGrabbed_ = false;
	int isReel_ = false;
	int isLimit_ = false;

	Stage* stage_ = nullptr;
	//ソケット
	std::vector<Socket*> socket_;
	int socketNum = 0;
	//enemyソケット
	std::vector<Socket*> enemySocket_;
	int enemySocketNum = 0;

	// プラグを繋げた時の音
	Sound* connectSE = nullptr;
	bool isSE = false;
};