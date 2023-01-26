#pragma once
#include "Vector3.h"
#include "Vector4.h"

// ��
struct Sphere
{
	// ���S���W
	Vector3 center = { 0,0,0 };

	// ���a
	float radius = 1.0f;
};

// ����
struct Plane
{
	// �@���x�N�g��
	Vector3 normal = { 0,1,0 };

	// ���_(0,0,0)����̋���
	float distance = 0.0f;
};

// �@���t���O�p�`�i���v��肪�\�ʁj
class Triangle
{
public:
	// ���_���W�R��
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;

	// �@���x�N�g��
	Vector3 normal;

	void ComputeNormal();
};

struct Ray
{
	// �n�_���W
	Vector3 start = { 0,0,0 };

	// ����
	Vector3 dir = { 1,0,0 };
};

