#include "MathFunc.h"

//スケーリング行列を設定する関数
void MathFunc::Affine::SetMatScale(Matrix4& affineMat , Vector3 scale) {
	//スケーリング行列を宣言
	Matrix4 matScale = {
		scale.x , 0 , 0 , 0 ,
		0 , scale.y , 0 , 0 ,
		0 , 0 , scale.z , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//行列の計算
	affineMat *= matScale;

}

//回転行列を設定する関数
void MathFunc::Affine::SetMatRotation(Matrix4& affineMat , Vector3 rotation) {

	//回転行列を宣言
	//Z軸回転
	Matrix4 matRotZ = {
		(float)cos(rotation.z) , (float)sin(rotation.z) , 0 , 0 ,
		(float)-sin(rotation.z) , (float)cos(rotation.z) , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//X軸回転
	Matrix4 matRotX = {
		1 , 0 , 0 , 0 ,
		0 , (float)cos(rotation.x) , (float)sin(rotation.x) , 0 ,
		0 , -(float)sin(rotation.x) , (float)cos(rotation.x) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//Y軸回転
	Matrix4 matRotY = {
		(float)cos(rotation.y) , 0 , (float)-sin(rotation.y) , 0 ,
		0 , 1 , 0 , 0 ,
		(float)sin(rotation.y) , 0 , (float)cos(rotation.y) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//行列の計算
	affineMat *= matRotZ;
	affineMat *= matRotX;
	affineMat *= matRotY;

}

//平行移動行列の設定をする関数
void MathFunc::Affine::SetMatTranslation(Matrix4& affineMat , Vector3 translation) {

	//平行移動行列の宣言
	Matrix4 matTranslation = {
		1 , 0 , 0 , 0 ,
		0 , 1 , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		translation.x , translation.y , translation.z , 1 ,
	};

	//行列の計算
	affineMat *= matTranslation;
}
#pragma endregion

#pragma region//ワールド変換行列を生成する関数
//スケーリング行列を生成する関数
Matrix4 MathFunc::Affine::CreateMatScale(Vector3 scale) {

	//スケーリング行列を宣言
	Matrix4 matScale = {
		scale.x , 0 , 0 , 0 ,
		0 , scale.y , 0 , 0 ,
		0 , 0 , scale.z , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//行列の計算
	return matScale;

}

//回転行列を生成する関数
Matrix4 MathFunc::Affine::CreateMatRotation(Vector3 rotation) {

	//回転行列を宣言
	Matrix4 matRot;
	matRot.identity();

	//Z軸回転
	Matrix4 matRotZ = {
		(float)cos(rotation.z) , (float)sin(rotation.z) , 0 , 0 ,
		(float)-sin(rotation.z) , (float)cos(rotation.z) , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//X軸回転
	Matrix4 matRotX = {
		1 , 0 , 0 , 0 ,
		0 , (float)cos(rotation.x) , (float)sin(rotation.x) , 0 ,
		0 , (float)-sin(rotation.x) , (float)cos(rotation.x) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//Y軸回転
	Matrix4 matRotY = {
		(float)cos(rotation.y) , 0 , (float)-sin(rotation.y) , 0 ,
		0 , 1 , 0 , 0 ,
		(float)sin(rotation.y) , 0 , (float)cos(rotation.y) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//行列の計算
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	return matRot;

}


//平行移動行列の生成をする関数
Matrix4 MathFunc::Affine::CreateMatTranslation(Vector3 translation) {

	//平行移動行列の宣言
	Matrix4 matTranslation = {
		1 , 0 , 0 , 0 ,
		0 , 1 , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		translation.x , translation.y , translation.z , 1 ,
	};

	//行列の計算
	return matTranslation;
}

#pragma endregion

//XMMATRIXをMatrix4に変換する関数
Matrix4 MathFunc::Utility::ConvertXMMATRIXtoMatrix4(XMMATRIX xmMatrix) {
	Matrix4 matrix4;
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			matrix4.m[i][j] = xmMatrix.r[i].m128_f32[j];

		}
	}

	return matrix4;
}

//ビュー行列を生成する関数
Matrix4 MathFunc::Utility::CreatMatView(Vector3 eye , Vector3 target , Vector3 up){

	Matrix4 matView;

	XMFLOAT3 xmEye;
	xmEye.x = eye.x;
	xmEye.y = eye.y;
	xmEye.z = eye.z;

	XMFLOAT3 xmTarget;
	xmTarget.x = target.x;
	xmTarget.y = target.y;
	xmTarget.z = target.z;

	XMFLOAT3 xmUp;
	xmUp.x = up.x;
	xmUp.y = up.y;
	xmUp.z = up.z;

	XMMATRIX xmMatView = XMMatrixLookAtLH(
		XMLoadFloat3(&xmEye) , XMLoadFloat3(&xmTarget) , XMLoadFloat3(&xmUp));


	matView = MathFunc::Utility::ConvertXMMATRIXtoMatrix4(xmMatView);

	return matView;

}

#pragma region//度数法⇔弧度法の関数

//度数法を弧度法に変換する関数
float MathFunc::Utility::Deg2Rad(float Deg) {

	return Deg * MathFunc::PI / 180;

}

//弧度法を度数法に変換する関数
float MathFunc::Utility::Rad2Deg(float rad) {

	return rad / MathFunc::PI * 180;

}

#pragma endregion

//ベクトルと行列の掛け算をする関数
Vector3  MathFunc::Utility::MulVector3AndMatrix4(Vector3 vec , Matrix4 mat) {

	Vector3 ans;

	ans.x =
		mat.m[0][0] * vec.x +
		mat.m[1][0] * vec.y +
		mat.m[2][0] * vec.z +
		mat.m[3][0] * 0;

	ans.y =
		mat.m[0][1] * vec.x +
		mat.m[1][1] * vec.y +
		mat.m[2][1] * vec.z +
		mat.m[3][1] * 0;

	ans.z =
		mat.m[0][2] * vec.x +
		mat.m[1][2] * vec.y +
		mat.m[2][2] * vec.z +
		mat.m[3][2] * 0;

	return ans;

}

Matrix4 MathFunc::Utility::PerspectiveFovLH(float fovAngleY , float aspectRatio , float nearZ , float farZ) {

	float h = 1 / tan(fovAngleY * 0.5);
	float w = h / aspectRatio;
	float a = farZ / (farZ - nearZ);
	float b = (-nearZ * farZ) / (farZ - nearZ);

	Matrix4 perspectiveFovLH = {
		w , 0 , 0 , 0 ,
		0 , h , 0 , 0 ,
		0 , 0 , a , 1 ,
		0 , 0 , b , 0
	};


	return perspectiveFovLH;
}

double MathFunc::Ease::In(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	return start + (move * time * time);
}

double MathFunc::Ease::Out(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	return start + (move * (1 - (1 - time) * (1 - time)));
}

double MathFunc::Ease::InOut(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	if (time < 0.5)
	{
		return start + (move * (2 * time * time));
	}
	else
	{
		return start + move * (1 - (((-2 * time + 2) * (-2 * time + 2)) / 2));
	}
}

double MathFunc::Ease::In_Back(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	double c1 = 1.70158;
	double c3 = (c1 + 1);
	return start + (move * (c3 * time * time * time - c1 * time * time));
}

double MathFunc::Ease::Out_Back(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	double c1 = 1.70158;
	double c3 = (c1 + 1);
	return start + (move * (1 + c3 * ((time - 1) * (time - 1) * (time - 1)) + c1 * ((time - 1) * (time - 1))));
}

double MathFunc::Ease::InOut_Back(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	double c1 = 1.70158;
	double c2 = c1 * 1.525;

	if (time < 0.5)
	{
		return start + move * ((((2 * time) * (2 * time)) * ((c2 + 1) * 2 * time - c2)) / 2);
	}
	else
	{
		return start + move * ((((2 * time - 2) * (2 * time - 2)) * ((c2 + 1) * (time * 2 - 2) + c2) + 2) / 2);
	}
}

double MathFunc::Ease::Out_Bounce(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	double n1 = 7.5625;
	double d1 = 2.75;
	if (time < 1 / d1) {
		return start + move * (n1 * time * time);
	}
	else if (time < 2 / d1) {
		return start + move * (n1 * (time -= 1.5 / d1) * time + 0.75);
	}
	else if (time < 2.5 / d1) {
		return start + move * (n1 * (time -= 2.25 / d1) * time + 0.9375);
	}
	else {
		return start + move * (n1 * (time -= 2.625 / d1) * time + 0.984375);
	}
}

double MathFunc::Ease::In_Bounce(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	double n1 = 7.5625;
	double d1 = 2.75;
	time = 1 - time;
	if (time < 1 / d1) {
		return start + move * (1 - (n1 * time * time));
	}
	else if (time < 2 / d1) {
		return start + move * (1 - (n1 * (time -= 1.5 / d1) * time + 0.75));
	}
	else if (time < 2.5 / d1) {
		return start + move * (1 - (n1 * (time -= 2.25 / d1) * time + 0.9375));
	}
	else {
		return start + move * (1 - (n1 * (time -= 2.625 / d1) * time + 0.984375));
	}

}

double MathFunc::Ease::InOut_Bounce(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	double n1 = 7.5625;
	double d1 = 2.75;
	if (time < 0.5)
	{
		time = 1 - time * 2;
		if (time < 1 / d1) {
			return start + move * ((1 - (n1 * time * time)) / 2);
		}
		else if (time < 2 / d1) {
			return start + move * ((1 - (n1 * (time -= 1.5 / d1) * time + 0.75)) / 2);
		}
		else if (time < 2.5 / d1) {
			return start + move * ((1 - (n1 * (time -= 2.25 / d1) * time + 0.9375)) / 2);
		}
		else {
			return start + move * ((1 - (n1 * (time -= 2.625 / d1) * time + 0.984375)) / 2);
		}
	}
	else
	{
		time = time * 2 - 1;
		if (time < 1 / d1) {
			return start + move * ((n1 * time * time) / 2 + 0.5);
		}
		else if (time < 2 / d1) {
			return start + move * ((n1 * (time -= 1.5 / d1) * time + 0.75) / 2 + 0.5);
		}
		else if (time < 2.5 / d1) {
			return start + move * ((n1 * (time -= 2.25 / d1) * time + 0.9375) / 2 + 0.5);
		}
		else {
			return start + move * ((n1 * (time -= 2.625 / d1) * time + 0.984375) / 2 + 0.5);
		}
	}
}