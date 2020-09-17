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