#include "Collider.h"

Collider::Collider() {

}

Collider::~Collider() {

}

void Collider::Initialize(WorldTransform* parent) {
	worldTransform_.Initialize();
	worldTransform_.parent_ = parent;
}

void Collider::Update() {
	worldTransform_.UpdateMatrix();
}

bool Collider::CheckCollision(Collider collider) {
	//プレイヤー
	float xMin = worldTransform_.matWorld_.m[3][0] - radius_;

	float xMax = worldTransform_.matWorld_.m[3][0] + radius_;

	float yMin = worldTransform_.matWorld_.m[3][1] - radius_;

	float yMax = worldTransform_.matWorld_.m[3][1] + radius_;

	float zMin = worldTransform_.matWorld_.m[3][2] - radius_;

	float zMax = worldTransform_.matWorld_.m[3][2] + radius_;

	//ステージ

	float cXMin = collider.worldTransform_.matWorld_.m[3][0] - collider.radius_;

	float cXMax = collider.worldTransform_.matWorld_.m[3][0] + collider.radius_;

	float cYMin = collider.worldTransform_.matWorld_.m[3][1] - collider.radius_;

	float cYMax = collider.worldTransform_.matWorld_.m[3][1] + collider.radius_;

	float cZMin = collider.worldTransform_.matWorld_.m[3][2] - collider.radius_;

	float cZMax = collider.worldTransform_.matWorld_.m[3][2] + collider.radius_;

	if ((xMin < cXMax && xMax > cXMin) && (yMin < cYMax && yMax > cYMin) && (zMin < cZMax && zMax > cZMin)) {
		return true;
	}
	return false;
}

bool Collider::boxCollision(Vector3 a, Vector3 b, Vector3 aRadius, Vector3 bPlusRadius,Vector3 bMinusRadius)
{
	//a(当てられる側)
	float pXMin = a.x - aRadius.x;

	float pXMax = a.x + aRadius.x;

	float pYMin = a.y - aRadius.y;

	float pYMax = a.y + aRadius.y;

	float pZMin = a.z - aRadius.z;

	float pZMax = a.z + aRadius.z;

	//b(当たる側)
	float eXMin = b.x - bMinusRadius.x;

	float eXMax = b.x + bPlusRadius.x;

	float eYMin = b.y - bMinusRadius.y;

	float eYMax = b.y + bPlusRadius.y;

	float eZMin = b.z - bMinusRadius.z;

	float eZMax = b.z + bPlusRadius.z;

	if ((pXMin < eXMax && pXMax > eXMin) && (pYMin < eYMax && pYMax > eYMin) && (pZMin < eZMax && pZMax > eZMin)) {
		return true;
	}
	return false;
}

bool Collider::boxCollision(Vector3 a, Vector3 b, Vector3 aRadius, Vector3 bRadius)
{
	//a(当てられる側)
	float pXMin = a.x - aRadius.x;

	float pXMax = a.x + aRadius.x;

	float pYMin = a.y - aRadius.y;

	float pYMax = a.y + aRadius.y;

	float pZMin = a.z - aRadius.z;

	float pZMax = a.z + aRadius.z;

	//b(当たる側)
	float eXMin = b.x - bRadius.x;

	float eXMax = b.x + bRadius.x;

	float eYMin = b.y - bRadius.y;

	float eYMax = b.y + bRadius.y;

	float eZMin = b.z - bRadius.z;

	float eZMax = b.z + bRadius.z;

	if ((pXMin < eXMax && pXMax > eXMin) && (pYMin < eYMax && pYMax > eYMin) && (pZMin < eZMax && pZMax > eZMin)) {
		return true;
	}
	return false;
	return false;
}

void Collider::SetRadius(float radius)
{
	radius_ = radius;
}

Vector3 Collider::GetWorldPos()
{
	Vector3 v = {
		worldTransform_.matWorld_.m[3][0],
		worldTransform_.matWorld_.m[3][1] ,
		worldTransform_.matWorld_.m[3][2]
	};

	return v;
}
