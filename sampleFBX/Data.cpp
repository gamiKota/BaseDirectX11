/**
 * @file Data.cpp
 */


/**
 * @include
 */
#include "Data.h"
#include <DirectXMath.h>
#include "debugproc.h"


#define PI (3.1415926535f)
#define ConvertToRadians(deg) (deg * PI / 180.f)


using namespace DirectX;


const Quaternion Quaternion::identity = Quaternion(0.f, 0.f, 0.f, 1.f);


/*
�N�H�[�^�j�I���͕��f���Ɋ�Â��Ă���A�����I�ɗ�������̂͗e�Ղł͂���܂���B
�X�̃N�H�[�^�j�I���̐��� (x,y,z,w) �ɃA�N�Z�X������ύX�����肷�邱�Ƃ͂قƂ�ǂ���܂���B
�����Ă��A�����̉�] (�Ⴆ�΁ATransform ����擾) ���g�p���ĐV������]���\�z���邽�߂Ɏg�p���܂�
(�Ⴆ�΁A2 �̃��[�e�[�V�����̊Ԃ��~���ɕ�Ԃ���Ȃ�)�B
*/


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

// �o�O�ł͂Ȃ����ǁA�������΂���(�Ƃ�������]�p�x��������)
// ���]�o���Ȃ��Ȃ�ŁH
// �����720���Ȃ񂾂��ǂ���
// �Ȃ񂩊|������F�X���Ȃ��Ⴂ���Ȃ������u���L�i�T�\�E
Quaternion Quaternion::AngleAxis(float angle, float3 axis) {

	Quaternion quaternion;		//!< �쐬����N�H�[�^�j�I��
	float halfSin, halfCos;		//!< �������p�x�̔�����sin,cos
	float normal;
	float radian = ConvertToRadians(angle);

	quaternion = { 0.f, 0.f, 0.f, 0.f };
	// ��]���̒��������߂�
	normal = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
	if (normal <= 0.0f) return quaternion;

	// �����x�N�g���ցi�P�ʃx�N�g���F������1�j
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


Quaternion Quaternion::Normalize(Quaternion data) {
	if (data.x == 0 && data.y == 0 && data.z == 0 && data.w == 0.f)	return Quaternion();
	float scalar = sqrtf(data.x * data.x + data.y * data.y + data.z * data.z + data.w * data.w);
	data.x /= scalar;
	data.y /= scalar;
	data.z /= scalar;
	data.w /= scalar;
	return data;
}



int GetRandom(int min, int max) {
	return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}


// �N�E�H�[�^�j�I�����m�̏�Z
void QuaternionMultiply(Quaternion *pOut, Quaternion *pQ1, Quaternion *pQ2) {
	pOut->w = pQ1->w * pQ2->w - pQ1->x * pQ2->x - pQ1->y * pQ2->y - pQ1->z * pQ2->z;
	pOut->x = pQ1->w * pQ2->x + pQ2->w * pQ1->x + pQ1->y * pQ2->z - pQ1->z * pQ2->y;
	pOut->y = pQ1->w * pQ2->y + pQ2->w * pQ1->y + pQ1->z * pQ2->x - pQ1->x * pQ2->z;
	pOut->z = pQ1->w * pQ2->z + pQ2->w * pQ1->z + pQ1->x * pQ2->y - pQ1->y * pQ2->x;
}

// �C�ӎ���]
void Vec3RotationAxis(float3 *pQut, float3 *pIn, float3 *pAxis, float fDegree) {
	float rad, s, c;
	Quaternion q_src, q_rot0, q_rot1;
	Quaternion q_tmp, q_out;

	// �p�x�����W�A���ɕϊ�
	rad = XMConvertToRadians(fDegree);

	// ��]�p�N�E�H�[�^�j�I���ݒ�(Vector3)
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

	// �N�E�H�[�^�j�I�����|����
	QuaternionMultiply(&q_tmp, &q_rot1, &q_src);
	QuaternionMultiply(&q_out, &q_tmp, &q_rot0);

	// �x�N�g���ɖ߂�
	pQut->x = q_out.x;
	pQut->y = q_out.y;
	pQut->z = q_out.z;
}


 /**
  * @brief   �N�H�[�^�j�I���쐬
  * @param   axis    ��]�����鎲
  * @param   radian  ��]������p�x(���W�A��)
  * @return  �쐬�����N�H�[�^�j�I��
  */
Quaternion MakeQuaternion(float3 axis, float radian)
{
	Quaternion quaternion;		//!< �쐬����N�H�[�^�j�I��
	float halfSin, halfCos;		//!< �������p�x�̔�����sin,cos
	float normal;

	quaternion = { 0.f, 0.f, 0.f, 0.f };
	// ��]���̒��������߂�
	normal = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
	if (normal <= 0.0f) return quaternion;

	// �����x�N�g���ցi�P�ʃx�N�g���F������1�j
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
 * @brief   �N�H�[�^�j�I���̊|���Z
 * @param   left    �v�Z�̍��̍�
 * @param   right   �v�Z�̉E�̍�
 * @return  �v�Z�����N�H�[�^�j�I��
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
 * @brief   �N�H�[�^�j�I���ɂ���]
 * @param   axis    ��]����������
 * @param   pos     ��]������I�u�W�F�N�g�̍��W
 * @param   radius  ��]������p�x
 * @return  ��]��̍��W
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