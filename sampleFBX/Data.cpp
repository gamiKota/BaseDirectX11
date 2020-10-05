/**
 * @file Data.cpp
 */


/**
 * @include
 */
#include "Data.h"
#include <DirectXMath.h>


using namespace DirectX;


const Quaternion Quaternion::identity = Quaternion(0.f, 0.f, 0.f, 1.f);


Quaternion Quaternion::Euler(float3 vec) {
	Quaternion q = Quaternion(XMConvertToRadians(vec.x), XMConvertToRadians(vec.y), XMConvertToRadians(vec.z), 1.f);
	return q;
}
Quaternion Quaternion::Euler(float x, float y, float z) {
	return Euler(float3(x, y, z));
}

Quaternion Quaternion::Inverse(Quaternion rotation) {
	Quaternion Result;
	float LengthSq = rotation.x * rotation.x + rotation.y * rotation.y + rotation.z * rotation.z + rotation.w * rotation.w;
	Result.x = -rotation.x / LengthSq;
	Result.y = -rotation.y / LengthSq;
	Result.z = -rotation.z / LengthSq;
	Result.w = rotation.w / LengthSq;
	return Result;
}

Quaternion Quaternion::Slerp(Quaternion q1, Quaternion q2, float t) {
	XMFLOAT4 f1, f2, result;
	Quaternion out = Quaternion();
	f1.w = q1.w; f1.x = q1.x; f1.y = q1.y; f1.z = q1.z;
	f2.w = q2.w; f2.x = q2.x; f2.y = q2.y; f2.z = q2.z;
	XMStoreFloat4(&result, XMQuaternionSlerp(XMLoadFloat4(&f1), XMLoadFloat4(&f2), t));
	out.x = result.x; out.y = result.y; out.z = result.z; out.w = result.w;
	return out;
}

Quaternion Quaternion::Dot(Quaternion q1, Quaternion q2) {
	XMFLOAT4 f1, f2, result;
	Quaternion out = Quaternion();
	f1.w = q1.w; f1.x = q1.x; f1.y = q1.y; f1.z = q1.z;
	f2.w = q2.w; f2.x = q2.x; f2.y = q2.y; f2.z = q2.z;
	XMStoreFloat4(&result, XMQuaternionDot(XMLoadFloat4(&f1), XMLoadFloat4(&f2)));
	out.x = result.x; out.y = result.y; out.z = result.z; out.w = result.w;
	return out;
}

// バグではないけど、挙動が可笑しい(というより回転角度が小さい)
// 一回転出来ないなんで？
// 一周が720°なんだけどうんこ
// なんか掛けたり色々しなきゃいけなさそうブルキナサソウ
Quaternion Quaternion::AngleAxis(float angle, float3 axis) {
	if (axis.x == 0.f && axis.y == 0.f && axis.z == 0.f || angle == 0.f) return Quaternion();
	float RadiansAngle = XMConvertToRadians(angle);
	XMVECTOR axisRot; //回転用軸
	XMFLOAT4 result;
	Quaternion out = Quaternion();
	
	axisRot = XMVector3Normalize(XMVectorSet(axis.x, axis.y, axis.z, 1.f));
	XMStoreFloat4(&result, XMQuaternionRotationAxis(axisRot, RadiansAngle));

	out.x = result.x;
	out.y = result.y;
	out.z = result.z;
	out.w = result.w;
	return out;
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