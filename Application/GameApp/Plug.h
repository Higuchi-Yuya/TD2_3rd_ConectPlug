#include "Object3d.h"
#include "Collider.h"
#include "MathFunc.h"
class Stage;
class Socket;
class Player;

class Plug {
public:
	Plug();
	~Plug();

	//メンバ関数
	void Initialize(Vector3 pos , int face);
	void Update();
	void Draw(ViewProjection* viewProjection);

private:

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
	void SetSocket(Socket* socket);

	int GetIsConnect();
	void SetIsConnect(int isConnect);

	int GetIsLimit();

	Collider* GetPlugCollider();
	Collider* GetCordCollider(int cordNum , int position);


	//構造体
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
	//コードの長さ(配列の要素数)
	static const int cordLength_ = 20;

	//コード
	Cord cord_[cordLength_];

	// モデル
	Model* cordModel = nullptr;
	Model* plugModel = nullptr;
	Model* blockModel = nullptr;

	//プラグ
	Object3d* plug_ = nullptr;
	Object3d* block_ = nullptr;
	Collider* plugCollider_ = nullptr;
	Collider* blockCollider_ = nullptr;

	int face_ = NORTH;

	//フラグ
	int isConnect_ = false;
	int isGrabbed_ = false;
	int isReel_ = false;
	int isLimit_ = false;

	Stage* stage_ = nullptr;

	std::vector<Socket*> socket_;
	int socketNum = 0;

};