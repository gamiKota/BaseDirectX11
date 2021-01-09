/**
 * @file Data.cpp
 */


/**
 * @include
 */
#include "Data.h"
#include <DirectXMath.h>
#include "debugproc.h"


using namespace DirectX;

const Quaternion Quaternion::identity = Quaternion(0.f, 0.f, 0.f, 1.f);


bool AlmostEqualRelative(float A, float B, float maxRelDiff = FLT_EPSILON) {
	// Calculate the difference.
	float diff = fabsf(A - B);
	A = fabsf(A);
	B = fabsf(B);

	// Find the largest
	float largest = (B > A) ? B : A;

	if (diff <= largest * maxRelDiff)
		return true;

	return false;
}

Quaternion __GetRotation(XMFLOAT4X4 m)
{
	float elem[4];
	elem[0] = m._11 - m._22 - m._33 + 1.0f;
	elem[1] = -m._11 + m._22 - m._33 + 1.0f;
	elem[2] = -m._11 - m._22 + m._33 + 1.0f;
	elem[3] = m._11 + m._22 + m._33 + 1.0f;

	int biggestIdx = 0;
	for (int i = 0; i < 4; i++)
	{
		if (elem[i] > elem[biggestIdx])
		{
			biggestIdx = i;
		}
	}

	if (elem[biggestIdx] < 0)
	{
		//Debug.Log("Wrong matrix.");
		return Quaternion(0.f, 0.f, 0.f, 0.f);
	}

	float q[4];
	float v = sqrtf(elem[biggestIdx]) * 0.5f;
	q[biggestIdx] = v;
	float mult = 0.25f / v;

	switch (biggestIdx)
	{
	case 0:
		q[1] = (m._21 + m._12) * mult;
		q[2] = (m._13 + m._31) * mult;
		q[3] = (m._32 - m._23) * mult;
		break;
	case 1:
		q[0] = (m._21 + m._12) * mult;
		q[2] = (m._32 + m._23) * mult;
		q[3] = (m._13 - m._31) * mult;
		break;
	case 2:
		q[0] = (m._13 + m._31) * mult;
		q[1] = (m._32 + m._23) * mult;
		q[3] = (m._21 - m._12) * mult;
		break;
	case 3:
		q[0] = (m._32 - m._23) * mult;
		q[1] = (m._13 - m._31) * mult;
		q[2] = (m._21 - m._12) * mult;
		break;
	}

	return Quaternion(q[0], q[1], q[2], q[3]);
}




float3 float3::Cross(float3 data1, float3 data2) {
	float3 out = float3();
	out.x = data1.y * data2.z - data1.z * data2.y;
	out.y = data1.z * data2.x - data1.x * data2.z;
	out.z = data1.x * data2.y - data1.y * data2.x;
	return out;
}

float float3::Dot(float3 data1, float3 data2) {
	return (float)(data1.x * data2.x + data1.y * data2.y + data1.z * data2.z);
}



float Quaternion::Angle(Quaternion q1, Quaternion q2) {
	float f = Quaternion::Dot(Quaternion::Normalize(q1), Quaternion::Normalize(q2));
	if (f < 0.f) {
		f *= -1.f;
	}
	//float angle = acosf(min(f, 1.f)) * 2.f * radToDeg;
	return XMConvertToDegrees(acosf(min(f, 1.f)) * 2.f);
}

// zxyの順序
Quaternion Quaternion::Euler(float3 vec) {
	Quaternion qx = Quaternion::AngleAxis(vec.x, float3(1.f, 0.f, 0.f));
	Quaternion qy = Quaternion::AngleAxis(vec.y, float3(0.f, 1.f, 0.f));
	Quaternion qz = Quaternion::AngleAxis(vec.z, float3(0.f, 0.f, 1.f));
	return Quaternion::Normalize(qz * qx * qy);
}
Quaternion Quaternion::Euler(float x, float y, float z) {
	return Euler(float3(x, y, z));
}

// メモ
// 二番目の要素で挙動が可笑しい
// ジンバルロックかなんかが原因っぽい
// 特異点(±90)の判定をする
float3 Quaternion::RadianAngle(Quaternion q) {

	float _x = q.x;
	float _y = q.y;
	float _z = q.z;
	float _w = q.w;

	float x2 = _x * _x;
	float y2 = _y * _y;
	float z2 = _z * _z;

	float xy = _x * _y;
	float xz = _x * _z;
	float yz = _y * _z;
	float wx = _w * _x;
	float wy = _w * _y;
	float wz = _w * _z;

	// 1 - 2y^2 - 2z^2
	float m00 = 1.f - (2.f * y2) - (2.f * z2);

	// 2xy + 2wz
	float m01 = (2.f * xy) + (2.f * wz);

	// 2xy - 2wz
	float m10 = (2.f * xy) - (2.f * wz);

	// 1 - 2x^2 - 2z^2
	float m11 = 1.f - (2.f * x2) - (2.f * z2);

	// 2xz + 2wy
	float m20 = (2.f * xz) + (2.f * wy);

	// 2yz+2wx
	float m21 = (2.f * yz) - (2.f * wx);

	// 1 - 2x^2 - 2y^2
	float m22 = 1.f - (2.f * x2) - (2.f * y2);


	float tx, ty, tz;

	// 角度90を境に特異点が生まれる
	if (AlmostEqualRelative(m21, 1.f))
	{
		tx = -XM_PI * 0.5f;
		ty = 0.f;
		tz = atan2f(m10, m00);
	}
	else if (AlmostEqualRelative(m21, -1.f))
	{
		tx = XM_PI * 0.5f;
		ty = 0.f;
		tz = atan2f(m10, m00);
	}
	else
	{
		tx = asinf(-m21);
		ty = atan2f(m20, m22);
		tz = atan2f(m01, m11);
	}
	return float3(tx, ty, tz);
}
float3 Quaternion::EulerAngle(Quaternion q) {
	float3 _q = Quaternion::RadianAngle(q);
	return float3(XMConvertToDegrees(_q.x), XMConvertToDegrees(_q.y), XMConvertToDegrees(_q.z));
}

Quaternion Quaternion::Inverse(Quaternion rotation) {
	return Quaternion::Normalize(Quaternion(-rotation.x, -rotation.y, -rotation.z, rotation.w));
}

Quaternion Quaternion::Slerp(Quaternion q1, Quaternion q2, float t) {
	XMFLOAT4 f1, f2, result;
	Quaternion out = Quaternion();
	f1.w = q1.w; f1.x = q1.x; f1.y = q1.y; f1.z = q1.z;
	f2.w = q2.w; f2.x = q2.x; f2.y = q2.y; f2.z = q2.z;
	XMStoreFloat4(&result, XMQuaternionSlerp(XMLoadFloat4(&f1), XMLoadFloat4(&f2), t));
	out.x = result.x; out.y = result.y; out.z = result.z; out.w = result.w;
	return Quaternion::Normalize(out);
}

Quaternion Quaternion::LookRotation(float3 forward, float3 upwards) {
	//if (forward.x == 0.f && forward.y == 0.f && forward.z == 0.f)	return Quaternion();
	float3 z = float3::Normalize(forward);
	float3 x = float3::Normalize(float3::Cross(upwards, z));
	float3 y = float3::Normalize(float3::Cross(z, x));

	XMFLOAT4X4 m;
	XMStoreFloat4x4(&m, XMMatrixIdentity());
	m._11 = x.x; m._12 = y.x; m._13 = z.x;
	m._21 = x.y; m._22 = y.y; m._23 = z.y;
	m._31 = x.z; m._32 = y.z; m._33 = z.z;

	return Quaternion::Normalize(__GetRotation(m));
}

Quaternion Quaternion::RotateTowards(Quaternion q1, Quaternion q2, float maxAngle) {
	float num = Quaternion::Angle(q1, q2);
	if (num == 0.f) { return q2;}
	float t = min(1.f, maxAngle / num);
	return Quaternion::Slerp(q1, q2, t);
}

float Quaternion::Dot(Quaternion q1, Quaternion q2) {
	return (float)(q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w);
}

Quaternion Quaternion::AngleAxis(float angle, float3 axis) {
	float halfSin;
	float radian = XMConvertToRadians(angle);
	axis = float3::Normalize(axis);
	if (axis == float3()) return Quaternion();
	halfSin = sinf(radian * 0.5f);
	return Quaternion(axis.x * halfSin, axis.y * halfSin, axis.z * halfSin, cosf(radian * 0.5f));
}

Quaternion Quaternion::Normalize(Quaternion data) {
	if (data.x == 0 && data.y == 0 && data.z == 0 && data.w == 0.f)	return Quaternion();
	float scalar = sqrtf(data.x * data.x + data.y * data.y + data.z * data.z + data.w * data.w);
	return Quaternion(data.x /= scalar, data.y /= scalar, data.z /= scalar, data.w /= scalar);
}



int GetRandom(int min, int max) {
	return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}


// クウォータニオン同士の乗算
void QuaternionMultiply(Quaternion *pOut, Quaternion *pQ1, Quaternion *pQ2) {
	pOut->w = pQ1->w * pQ2->w - pQ1->x * pQ2->x - pQ1->y * pQ2->y - pQ1->z * pQ2->z;
	pOut->x = pQ1->w * pQ2->x + pQ2->w * pQ1->x + pQ1->y * pQ2->z - pQ1->z * pQ2->y;
	pOut->y = pQ1->w * pQ2->y + pQ2->w * pQ1->y + pQ1->z * pQ2->x - pQ1->x * pQ2->z;
	pOut->z = pQ1->w * pQ2->z + pQ2->w * pQ1->z + pQ1->x * pQ2->y - pQ1->y * pQ2->x;
}

// 任意軸回転
void Vec3RotationAxis(float3 *pQut, float3 *pIn, float3 *pAxis, float fDegree) {
	float rad, s, c;
	Quaternion q_src, q_rot0, q_rot1;
	Quaternion q_tmp, q_out;

	// 角度をラジアンに変換
	rad = XMConvertToRadians(fDegree);

	// 回転用クウォータニオン設定(Vector3)
	q_src.x = pIn->x;
	q_src.y = pIn->y;
	q_src.z = pIn->z;
	q_src.w = 0.f;

	s = sinf(rad * 0.5f);
	c = cosf(rad * 0.5f);

	q_rot0.x = pAxis->x * s;
	q_rot0.y = pAxis->y * s;
	q_rot0.z = pAxis->z * s;
	q_rot0.w = c;

	q_rot1.x = pAxis->x * -s;
	q_rot1.y = pAxis->y * -s;
	q_rot1.z = pAxis->z * -s;
	q_rot1.w = c;

	// クウォータニオンを掛ける
	QuaternionMultiply(&q_tmp, &q_rot1, &q_src);
	QuaternionMultiply(&q_out, &q_tmp, &q_rot0);

	// ベクトルに戻す
	pQut->x = q_out.x;
	pQut->y = q_out.y;
	pQut->z = q_out.z;
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
	Quaternion  posQuaternion = Quaternion(pos.x, pos.y, pos.z, 0.f);
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