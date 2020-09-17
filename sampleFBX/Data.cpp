/**
 * @file Data.cpp
 */


/**
 * @include
 */
#include "Data.h"
#include <DirectXMath.h>


using namespace DirectX;


Quaternion Quaternion::Euler(float3 vec) {
	return Quaternion(0.f, XMConvertToRadians(vec.x), XMConvertToRadians(vec.y), XMConvertToRadians(vec.z));
}
Quaternion Quaternion::Euler(float x, float y, float z) {
	return Euler(float3(x, y, z));
}


 /**
  * @brief   クォータニオン作成
  * @param   axis    回転させる軸
  * @param   radian  回転させる角度(ラジアン)
  * @return  作成したクォータニオン
  */
Quaternion MakeQuaternion(float3 axis, float radian)
{
	Quaternion quaternion;		//!< 作成するクォータニオン
	float halfSin, halfCos;		//!< 動かす角度の半分のsin,cos
	float normal;

	quaternion = { 0.f, 0.f, 0.f, 0.f };
	// 回転軸の長さを求める
	normal = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
	if (normal <= 0.0f) return quaternion;

	// 方向ベクトルへ（単位ベクトル：長さは1）
	normal = 1.0f / sqrtf(normal);
	axis = axis * normal;


	halfSin = sinf(radian * 0.5f);
	halfCos = cosf(radian * 0.5f);

	quaternion.w = halfCos;
	quaternion.x = axis.x * halfSin;
	quaternion.y = axis.y * halfSin;
	quaternion.z = axis.z * halfSin;

	return quaternion;
}


/**
 * @brief   クォータニオンの掛け算
 * @param   left    計算の左の項
 * @param   right   計算の右の項
 * @return  計算したクォータニオン
 */
Quaternion CalcQuaternion(Quaternion left, Quaternion right)
{
	Quaternion quaternion;
	float   num1, num2, num3, num4;

	num1 = left.w * right.w;
	num2 = -left.x * right.x;
	num3 = -left.y * right.y;
	num4 = -left.z * right.z;
	quaternion.w = num1 + num2 + num3 + num4;

	num1 = left.w * right.x;
	num2 = left.x * right.w;
	num3 = left.y * right.z;
	num4 = -left.z * right.y;
	quaternion.x = num1 + num2 + num3 + num4;

	num1 = left.w * right.y;
	num2 = left.y * right.w;
	num3 = left.z * right.x;
	num4 = -left.x * right.z;
	quaternion.y = num1 + num2 + num3 + num4;

	num1 = left.w * right.z;
	num2 = left.z * right.w;
	num3 = left.x * right.y;
	num4 = -left.y * right.x;
	quaternion.z = num1 + num2 + num3 + num4;

	return   quaternion;
}


/**
 * @brief   クォータニオンによる回転
 * @param   axis    回転させたい軸
 * @param   pos     回転させるオブジェクトの座標
 * @param   radius  回転させる角度
 * @return  回転後の座標
 */
float3 RotateQuaternionPosition(float3 axis, float3 pos, float radius)
{
	Quaternion  complexNumber, complexConjugateNumber;
	Quaternion  posQuaternion = { 0.f, pos.x, pos.y, pos.z };
	float3		resultPosition;

	if (axis.x == 0.f && axis.y == 0.f && axis.z == 0.f ||
		radius == 0.f) {
		return pos;
	}

	complexNumber = MakeQuaternion(axis, radius);
	complexConjugateNumber = MakeQuaternion(axis, -radius);

	posQuaternion = CalcQuaternion(complexNumber, posQuaternion);
	posQuaternion = CalcQuaternion(posQuaternion, complexConjugateNumber);

	resultPosition.x = posQuaternion.x;
	resultPosition.y = posQuaternion.y;
	resultPosition.z = posQuaternion.z;

	return resultPosition;
}


// EOF