#include "Transform.h"
#include <math.h>
#include "GameObject.h"
#include "GameObjectUI.h"
#include "GameObjectMeshBase.h"
#include "Tween.h"
#include "debugproc.h"
#include "imgui.h"
#include "Camera.h"
#include "System.h"


#define PI (3.1415926535f)
#define ConvertToRadians(deg) (deg * PI / 180.f)


using namespace DirectX;


float normalize(float num, float min = 0, float max = 1) {
	float range = abs(max - min);
	if (num < min) {
		return num + range * ceil(abs(num - min) / range);
	}
	else if(max <= num) {
		return num - range * (floor(abs(num - max) / range) + 1);
	}
	return num;
}

//float3 QuaternionToEulerAngles(float q0, float q1, float q2, float q3)
//{
//	float q0q0 = q0 * q0;
//	float q0q1 = q0 * q1;
//	float q0q2 = q0 * q2;
//	float q0q3 = q0 * q3;
//	float q1q1 = q1 * q1;
//	float q1q2 = q1 * q2;
//	float q1q3 = q1 * q3;
//	float q2q2 = q2 * q2;
//	float q2q3 = q2 * q3;
//	float q3q3 = q3 * q3;
//
//	float pitch = atan2f(2.0f * (q1q2 + q0q3), q0q0 + q1q1 - q2q2 - q3q3);
//	float yaw = asinf(2.0f * (q0q2 - q1q3));
//	float roll = atan2f(2.0f * (q2q3 + q0q1), q0q0 - q1q1 - q2q2 + q3q3);
//
//	return float3(pitch, yaw, roll);
//}
float3 QuaternionToEulerAngles(Quaternion q)
{
	Quaternion r = q;
	float x = r.x;
	float y = r.y;
	float z = r.z;
	float w = r.w;

	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;

	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;

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

	// Approximately関数
	// 浮動小数点変数同士の比較演算
	// 値が近ければ同じ(近似値)としてtrueを返す
	//if (Mathf.Approximately(m21, 1.f))
	if (m21 == 1.f)
	{
		tx = PI / 2.f;
		ty = 0;
		tz = atan2f(m10, m00);
	}
	//else if (Mathf.Approximately(m21, -1.f))
	else if (m21 == -1.f)
	{
		tx = -PI / 2.f;
		ty = 0;
		tz = atan2f(m10, m00);
	}
	else
	{
		tx = asinf(-m21);
		ty = atan2f(m20, m22);
		tz = atan2f(m01, m11);
	}

	//tx *= Mathf.Rad2Deg;
	//ty *= Mathf.Rad2Deg;
	//tz *= Mathf.Rad2Deg;
	return float3(XMConvertToDegrees(tx), XMConvertToDegrees(ty), XMConvertToDegrees(tz));
}


// メモ
// クォータニオンをベクトルととらえる
// 各要素(x, y, z)は180を境に0～1～0に数値が正規化
// クォータニオンの4個のパラメータのノルムは１
// (そのためw要素が-になる場合もある)
// メリットとして、3x3の回転行列のもつ性質を失わずに４個のパラメータに圧縮したものがクォータニオン
// 逆に、クォータニオンが与えられたときに具体的な回転や姿勢をイメージしにくい


Transform::Transform() : m_position(float3()), m_rotate(Quaternion()), m_scale(float3()), m_tween(new Tween[3]()), m_Parent(nullptr) {
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());
}


void Transform::Uninit() {
	delete[] m_tween;
}


void Transform::Update() {

}


void Transform::LastUpdate() {

	for (int i = 0; i < 3; i++) {
		m_tween[i].Update();

		if (m_tween[i].m_isTween == E_TWEEN::DO ||
			m_tween[i].m_isTween == E_TWEEN::END) {	// うんち
			if (i == 0) {
				m_position = m_tween[i].GetResult();
			}
			//if (i == 1) {
			//	m_rotate = m_tween[i].GetResult();
			//}
			//if (i == 2) {
			//	m_scale = m_tween[i].GetResult();
			//}
		}
	}

	// 取り合えずこれで
	if (dynamic_cast<GameObjectUI*>(m_gameObject) != nullptr) {
		m_transform->m_position.z = 0;
		m_transform->m_scale.z = 0;
	}
	if (dynamic_cast<GameObjectMeshBase*>(m_gameObject) != nullptr) {
		m_transform->m_scale.z = 0;
	}

	//m_rotate.x = normalize(m_rotate.x, -XM_PI, XM_PI);
	//m_rotate.y = normalize(m_rotate.y, -XM_PI, XM_PI);
	//m_rotate.z = normalize(m_rotate.z, -XM_PI, XM_PI);


	float3 rotate = float3(m_rotate.x, m_rotate.y, m_rotate.z);
	//m_rotate = Quaternion::Normalize(m_rotate);
	//float3 rotate = QuaternionToEulerAngles(m_rotate.x, m_rotate.y, m_rotate.z, m_rotate.w);

	// メモ
	// 既に削除されたかどうかは今の所考えない
	// ビルボードだと出来ない処理
	// 本当に全てが親基準になる
	// Meshの場合、座標は親の大きさの範囲に指定される(0.5～0.5の範囲)
	if (m_Parent != nullptr) {
		XMMATRIX matrix = XMMatrixIdentity();	// 行列変換
		matrix = XMMatrixMultiply(matrix, XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z));
		matrix = XMMatrixMultiply(matrix, XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z));
		matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));

		matrix *= XMLoadFloat4x4(&m_Parent->m_transform->GetMatrix());
		XMStoreFloat4x4(&m_world, matrix);
	}
	else {
		XMMATRIX matrix = XMMatrixIdentity();	// 行列変換
		// 拡縮の変更
		matrix = XMMatrixMultiply(matrix, XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z));
		// 回転軸の変更
		matrix = XMMatrixMultiply(matrix, XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z));
		// 座標の変更
		matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
		// 行列に反映
		XMStoreFloat4x4(&m_world, matrix);

		//XMMatrixRotationQuaternion()
	}

	// 方向ベクトルの更新(ピッチ、ヨー、ロール)
	// (3x3の回転行列にあたる、正規化されていて良いのかはわからない)
	m_right = float3::Normalize({ m_world._11, m_world._12, m_world._13 });
	m_up = float3::Normalize({ m_world._21, m_world._22, m_world._23 });
	m_forward = float3::Normalize({ m_world._31, m_world._32, m_world._33 });

	//m_rotate = Quaternion::Normalize(m_rotate);
	if (m_gameObject->ToString() == "Player" || m_gameObject->ToString() == "Enemy (0)") {

		float3 EulerAngle = float3();
		
		
		Quaternion q1 = Quaternion::AngleAxis(90.f, float3(0.f, 1.f, 0.f));
		Quaternion q2 = Quaternion::AngleAxis(-70.f, float3(0.f, 1.f, 0.f));
		Quaternion q = q1 * q2;


		EulerAngle = QuaternionToEulerAngles(q);

		PrintDebugProc("Quaternion = %.2f, %.2f, %.2f, %.2f\n", q.x, q.y, q.z, q.w);
		PrintDebugProc("EulerAngle = %.2f, %.2f, %.2f\n", EulerAngle.x, EulerAngle.y, EulerAngle.z);
	}
}


void Transform::SetImGuiVal() {
#if _DEBUG
	ImGui::DragFloat3("position", (float*)&m_transform->m_position);
	ImGui::DragFloat3("rotation", (float*)&m_transform->m_rotate, 0.01f);
	ImGui::DragFloat3("scale", (float*)&m_transform->m_scale);

	ImGui::DragFloat3("forward", (float*)&m_transform->m_forward);
	ImGui::DragFloat3("up", (float*)&m_transform->m_up);
	ImGui::DragFloat3("right", (float*)&m_transform->m_right);
#endif
}


Tween* Transform::DOMove(float3 position, float time) {

	m_tween[0].DOTween(m_position, position, time);

	return &m_tween[0];
}


void Transform::LookAt(Transform* target, float angle) {
	if (!target)	return;
	float3 rotate = float3();	// 移動先回転軸の確保
	rotate.x = -atan2f(
		target->m_position.y - m_position.y,
		sqrtf(powf(target->m_position.z - m_position.z, 2) + powf(target->m_position.x - m_position.x, 2)));
	rotate.y = atan2f(
		target->m_position.x - m_position.x,
		target->m_position.z - m_position.z);

	
	float3 diffAngle = rotate - m_rotate;
	float3 unti = float3(1.f, 1.f, 1.f);

	if (diffAngle.x < -XM_PI) {
		unti.x = -1.f;
		diffAngle.x = (m_rotate.x - rotate.x) - XM_2PI;
	}
	else if (diffAngle.x > XM_PI) {
		unti.x = -1.f;
		diffAngle.x = (m_rotate.x - rotate.x) + XM_2PI;
	}
	if (diffAngle.y < -XM_PI) {
		unti.y = -1.f;
		diffAngle.y = (m_rotate.y - rotate.y) - XM_2PI;
	}
	else if (diffAngle.y > XM_PI) {
		unti.y = -1.f;
		diffAngle.y = (m_rotate.y - rotate.y) + XM_2PI;
	}

	// 角度制限
	if (angle > 0.f && angle < XM_2PI) {
		if (diffAngle.x > angle) {
			diffAngle.x = angle;
		}
		else if (diffAngle.x < -angle) {
			diffAngle.x = -angle;
		}
		if (diffAngle.y > angle) {
			diffAngle.y = angle;
		}
		else if (diffAngle.y < -angle) {
			diffAngle.y = -angle;
		}
	}

	m_rotate.x += diffAngle.x * unti.x;
	m_rotate.y += diffAngle.y * unti.y;
}


XMFLOAT4X4 Transform::GetMatrixBillboard() {

	XMFLOAT4X4& mView = CCamera::Get()->GetView();

	XMFLOAT4X4 mtxWorld;
	XMStoreFloat4x4(&mtxWorld, XMMatrixIdentity());

	mtxWorld._11 = mView._11 * m_scale.x;
	mtxWorld._12 = mView._21 * m_scale.x;
	mtxWorld._13 = mView._31 * m_scale.x;
	mtxWorld._21 = mView._12 * m_scale.y;
	mtxWorld._22 = mView._22 * m_scale.y;
	mtxWorld._23 = mView._32 * m_scale.y;
	mtxWorld._31 = mView._13 * m_scale.z;
	mtxWorld._32 = mView._23 * m_scale.z;
	mtxWorld._33 = mView._33 * m_scale.z;
	// 位置を反映
	mtxWorld._41 = m_position.x;
	mtxWorld._42 = m_position.y;
	mtxWorld._43 = m_position.z;

	return mtxWorld;
}


// EOF