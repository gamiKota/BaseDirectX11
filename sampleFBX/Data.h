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
 * @class float3 : inheritance CCamera
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
 * @brief �N�H�[�^�j�I��
 */
class Quaternion : public float3 {
public:
	float w;

	static const Quaternion identity;	//!< �P����]�i����]��ԁj

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

	float3 EulerAngle() {
		return float3(DirectX::XMConvertToDegrees(x), DirectX::XMConvertToDegrees(y), DirectX::XMConvertToDegrees(z));
	}

	/**
	 * @brief �t�N�H�[�^�j�I��
	 * @param[in] rotation
	 * @return �����̋t�N�H�[�^�j�I��
	 */
	static Quaternion Inverse(Quaternion rotation);

	/**
	 * @brief ���ʐ��`���
	 * @param[in] q1
	 * @param[in] q2
	 * @param[in] t
	 * @return �Z�o���ꂽQuaternion
	 */
	static Quaternion Slerp(Quaternion q1, Quaternion q2, float t);

	/**
	 * @brief 2��Quaternion�̓���
	 * @param[in] q1
	 * @param[in] q2
	 * return �Z�o���ꂽQuaternion
	 */
	static Quaternion Dot(Quaternion q1, Quaternion q2);

	/**
	 * @brief 2��Quaternion�̊Ԃ��v�Z
	 * @param[in] angle
	 * @param[in] axis
	 * return �Z�o���ꂽQuaternion
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


	Quaternion operator = (float3 data) {
		x = data.x;
		y = data.y;
		z = data.z;
		return *this;
	}

	Quaternion operator * (Quaternion data) {
		Quaternion quaternion;
		float   num1, num2, num3, num4;
	
		num1 = this->w * data.w;
		num2 = -this->x * data.x;
		num3 = -this->y * data.y;
		num4 = -this->z * data.z;
		quaternion.w = num1 + num2 + num3 + num4;
	
		num1 = this->w * data.x;
		num2 = this->x * data.w;
		num3 = this->y * data.z;
		num4 = -this->z * data.y;
		quaternion.x = num1 + num2 + num3 + num4;
	
		num1 = this->w * data.y;
		num2 = this->y * data.w;
		num3 = this->z * data.x;
		num4 = -this->x * data.z;
		quaternion.y = num1 + num2 + num3 + num4;
	
		num1 = this->w * data.z;
		num2 = this->z * data.w;
		num3 = this->x * data.y;
		num4 = -this->y * data.x;
		quaternion.z = num1 + num2 + num3 + num4;
	
		// q1 * q2 = s1 * s2 - v1 * v2 + s1 * v2 + s2 * v1 + v1 * v2
		// q1��q2�Av1��v2�̓x�N�g��
		// s1��s2��Quaternion�̃X�J���[����
	
		return   quaternion;
	}
};


/**
 * @brief   �N�H�[�^�j�I���쐬
 * @param   axis    ��]�����鎲
 * @param   radian  ��]������p�x(���W�A��)
 * @return  �쐬�����N�H�[�^�j�I��
 */
Quaternion MakeQuaternion(float3 axis, float radian);


/**
 * @brief   �N�H�[�^�j�I���̊|���Z
 * @param   left    �v�Z�̍��̍�
 * @param   right   �v�Z�̉E�̍�
 * @return  �v�Z�����N�H�[�^�j�I��
 */
Quaternion CalcQuaternion(Quaternion left, Quaternion right);


/**
 * @brief   �N�H�[�^�j�I���ɂ���]
 * @param   axis    ��]����������
 * @param   pos     ��]������I�u�W�F�N�g�̍��W
 * @param   radius  ��]������p�x
 * @return  ��]��̍��W
 */
float3 RotateQuaternionPosition(float3 axis, float3 pos, float radius);


// EOF