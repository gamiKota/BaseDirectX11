/**
 * @file Data.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * include
 */
#include <DirectXMath.h>


/**
 * @class float3 : inheritance DirectX::XMFLOAT
 */
class float3 : public DirectX::XMFLOAT3 {

public:
	float3() {
		x = y = z = 0.f;
	}
	float3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	static float3 lerp3(float3 start, float3 end, float t) {
		if (t > 1.f)	t = 1.f;
		if (t < 0.f)	t = 0.f;
		return float3(((1.f - t) * start.x + t * end.x), ((1.f - t) * start.y + t * end.y), ((1.f - t) * start.z + t * end.z));
	}

	static float3 Normalize(float3 data) {
		if (data.x == 0 && data.y == 0 && data.z == 0)	return float3();
		float scalar = sqrtf(data.x * data.x + data.y * data.y + data.z * data.z);
		data /= scalar;
		return data;
	}

	static float Dot(float3 data1, float3 data2);

	static float3 Cross(float3 data1, float3 data2);

	static float Length(float3 pos1, float3 pos2) {
		return sqrtf((pos2.x - pos1.x) * (pos2.x - pos1.x) +
			(pos2.y - pos1.y) * (pos2.y - pos1.y) +
			(pos2.z - pos1.z) * (pos2.z - pos1.z));
	}

	float3 operator + (float3 data) {
		return float3(x + data.x, y + data.y, z + data.z);
	}
	float3 operator - (float3 data) {
		return float3(x - data.x, y - data.y, z - data.z);
	}
	float3 operator * (float3 data) {
		return float3(x * data.x, y * data.y, z * data.z);
	}
	float3 operator / (float3 data) {
		return float3(x / data.x, y / data.y, z / data.z);
	}
	float3 operator += (float3 data) {
		x += data.x;
		y += data.y;
		z += data.z;
		return *this;
	}
	float3 operator -= (float3 data) {
		x -= data.x;
		y -= data.y;
		z -= data.z;
		return *this;
	}
	float3 operator *= (float3 data) {
		x *= data.x;
		y *= data.y;
		z *= data.z;
		return *this;
	}
	float3 operator /= (float3 data) {
		x /= data.x;
		y /= data.y;
		z /= data.z;
		return *this;
	}
	float3 operator + (float data) {
		return float3(x + data, y + data, z + data);
	}
	float3 operator - (float data) {
		return float3(x - data, y - data, z - data);
	}
	float3 operator * (float data) {
		return float3(x * data, y * data, z * data);
	}
	float3 operator / (float data) {
		return float3(x / data, y / data, z / data);
	}
	float3 operator += (float data) {
		x += data;
		y += data;
		z += data;
		return *this;
	}
	float3 operator -= (float data) {
		x -= data;
		y -= data;
		z -= data;
		return *this;
	}
	float3 operator *= (float data) {
		x *= data;
		y *= data;
		z *= data;
		return *this;
	}
	float3 operator /= (float data) {
		x /= data;
		y /= data;
		z /= data;
		return *this;
	}
	bool operator == (float3 data)
	{
		bool b = false;
		b = this->x == data.x && this->y == data.y && this->z == data.z;
		return b;
	}
	bool operator != (float3 data)
	{
		return !(*this == data);
	}
};


/**
 * @brief クォータニオン
 */
class Quaternion {
public:
	float x;
	float y;
	float z;
	float w;

	static const Quaternion identity;	//!< 恒等回転（無回転状態）

public:
	Quaternion() {
		x = y = z = 0.f;
		w = 1.f;
	}
	Quaternion(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	/**
	 * @brief オイラー角変換
	 * @return オイラー角
	 */
	static float3 EulerAngle(Quaternion q);

	/**
	 * @brief ラジアン角変換
	 * @return ラジアン角
	 */
	static float3 RadianAngle(Quaternion q);

	/**
	 * @brief 逆クォータニオン
	 * @param[in] rotation
	 * @return 引数の逆クォータニオン
	 */
	static Quaternion Inverse(Quaternion rotation);

	/**
	 * @brief 球面線形補間
	 * @param[in] q1
	 * @param[in] q2
	 * @param[in] t
	 * @return 算出されたQuaternion
	 */
	static Quaternion Slerp(Quaternion q1, Quaternion q2, float t);

	/**
	 * @brief 2つのQuaternionの内積
	 * @param[in] q1
	 * @param[in] q2
	 * return 算出されたQuaternion
	 */
	static Quaternion Dot(Quaternion q1, Quaternion q2);

	/**
	 * @brief 軸回転クォータニオンの作成
	 * @param[in] angle
	 * @param[in] axis
	 * return 算出されたQuaternion
	 */
	static Quaternion AngleAxis(float angle, float3 axis);

	/**
	 * @brief Euler
	 * @param[in] float3
	 * @return Quaternion
	 */
	static Quaternion Euler(float3 vec);
	/**
	 * @brief Euler
	 * @param[in] float x, float y, float z
	 * @return Quaternion
	 */
	static Quaternion Euler(float x, float y, float z);

	/**
	 * @brief Normalize
	 * @param[in] data
	 * @return Quaternion型
	 */
	static Quaternion Normalize(Quaternion data);

	Quaternion operator = (float3 data) {
		x = data.x;
		y = data.y;
		z = data.z;
		return *this;
	}
	//Quaternion operator = (float3 data) = delete;
	//Quaternion operator = (Quaternion data) = delete;

	Quaternion operator + (Quaternion data) {
		Quaternion q;
		q.x = x + data.x;
		q.y = y + data.y;
		q.z = z + data.z;
		q.w = w + data.w;
		return Quaternion::Normalize(q);
	}

	Quaternion operator * (Quaternion data) {
		Quaternion q;
		q.x =  w * data.x - z * data.y + y * data.z + x * data.w;
		q.y =  z * data.x + w * data.y - x * data.z + y * data.w;
		q.z = -y * data.x + x * data.y + w * data.z + z * data.w;
		q.w = -x * data.x - y * data.y - z * data.z + w * data.w;
		return Quaternion::Normalize(q);
	}
	Quaternion operator *= (Quaternion data) {
		*this = *this * data;
		return *this;
	}
};


int GetRandom(int min, int max);

void QuaternionMultiply(Quaternion *pOut, Quaternion *pQ1, Quaternion *pQ2);
void Vec3RotationAxis(float3 *pQut, float3 *pIn, float3 *pAxis, float fDegree);


/**
 * @brief   クォータニオン作成
 * @param   axis    回転させる軸
 * @param   radian  回転させる角度(ラジアン)
 * @return  作成したクォータニオン
 */
Quaternion MakeQuaternion(float3 axis, float radian);


/**
 * @brief   クォータニオンの掛け算
 * @param   left    計算の左の項
 * @param   right   計算の右の項
 * @return  計算したクォータニオン
 */
Quaternion CalcQuaternion(Quaternion left, Quaternion right);


/**
 * @brief   クォータニオンによる回転
 * @param   axis    回転させたい軸
 * @param   pos     回転させるオブジェクトの座標
 * @param   radius  回転させる角度
 * @return  回転後の座標
 */
float3 RotateQuaternionPosition(float3 axis, float3 pos, float radius);


// EOF