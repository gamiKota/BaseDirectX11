#include "Transform.h"
#include "GameObject.h"
#include "Tween.h"
#include "debugproc.h"
#include "imgui.h"
#include "Camera.h"
#include "System.h"


using namespace DirectX;

bool transformRotMatToQuaternion(Quaternion &q, XMFLOAT4X4 matrix);
void transformQuaternionToRotMat(XMFLOAT4X4 &matrix, Quaternion q);



Transform::Transform() : m_position(float3()), m_rotate(Quaternion()), m_scale(float3()), m_tween(new Tween[3]()) {
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
			if (i == 1) {
				m_rotate = m_tween[i].GetResult();
			}
			if (i == 2) {
				m_scale = m_tween[i].GetResult();
			}
		}
	}



	XMMATRIX matrix = XMMatrixIdentity();	// 行列変換
	// 拡縮の変更
	matrix = XMMatrixMultiply(matrix, XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z));
	// 回転軸の変更
	matrix = XMMatrixMultiply(matrix, XMMatrixRotationRollPitchYaw(m_rotate.x, m_rotate.y, m_rotate.z));
	// 座標の変更
	matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
	// 行列に反映
	XMStoreFloat4x4(&m_world, matrix);

	// 前方向の更新
	m_forward = { m_world._31, m_world._32, m_world._33 };
	// 横方向の更新
	m_right = { m_world._11, m_world._12, m_world._13 };
	// 上方向の更新
	m_up = { m_world._21, m_world._22, m_world._23 };
}


void Transform::SetImGuiVal() {
#if _DEBUG
	ImGui::DragFloat3("position", (float*)&m_transform->m_position);
	ImGui::DragFloat3("rotation", (float*)&m_transform->m_rotate, 0.01f);
	ImGui::DragFloat3("scale", (float*)&m_transform->m_scale, 0.01f);
#endif
}


Tween* Transform::DOMove(float3 position, float time) {

	m_tween[0].DOTween(m_position, position, time);

	return &m_tween[0];
}


void Transform::LookAt(Transform* target) {
	if (!target)	return;
	float3 rotate = float3();	// 移動先回転軸の確保
	rotate.x = -atan2f(
		target->m_position.y - m_position.y,
		sqrtf(powf(target->m_position.z - m_position.z, 2) + powf(target->m_position.x - m_position.x, 2)));
	rotate.y = atan2f(
		target->m_position.x - m_position.x,
		target->m_position.z - m_position.z);

	m_rotate.x = rotate.x;
	m_rotate.y = rotate.y;
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


///////////////////////////////////////////////
// クォータニオン→回転行列変換
//
// m11-m33 : 回転行列成分（出力）
// qx, qy, qz, qw : クォータニオン成分
//
// ※注意：
// 行列成分はDirectX形式（行方向が軸の向き）です
// OpenGL形式（列方向が軸の向き）の場合は
// 転置した値を格納するようにして下さい。

void transformQuaternionToRotMat(XMFLOAT4X4 &matrix, Quaternion q) {
	matrix._11 = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
	matrix._12 = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
	matrix._13 = 2.0f * q.x * q.z - 2.0f * q.w * q.y;

	matrix._21 = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
	matrix._22 = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
	matrix._23 = 2.0f * q.y * q.z + 2.0f * q.w * q.x;

	matrix._31 = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
	matrix._32 = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
	matrix._33 = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
}


///////////////////////////////////////////////
// 回転行列→クォータニオン変換
//
// qx, qy, qz, qw : クォータニオン成分（出力）
// m11-m33 : 回転行列成分
//
// ※注意：
// 行列成分はDirectX形式（行方向が軸の向き）です
// OpenGL形式（列方向が軸の向き）の場合は
// 転置した値を入れて下さい。

bool transformRotMatToQuaternion(Quaternion &q, XMFLOAT4X4 matrix) {
	// 最大成分を検索
	float elem[4]; // 0:x, 1:y, 2:z, 3:w
	elem[0] = matrix._11 - matrix._22 - matrix._33 + 1.0f;
	elem[1] = -matrix._11 + matrix._22 - matrix._33 + 1.0f;
	elem[2] = -matrix._11 - matrix._22 + matrix._33 + 1.0f;
	elem[3] = matrix._11 + matrix._22 + matrix._33 + 1.0f;

	unsigned biggestIndex = 0;
	for (int i = 1; i < 4; i++) {
		if (elem[i] > elem[biggestIndex])
			biggestIndex = i;
	}

	if (elem[biggestIndex] < 0.0f)
		return false; // 引数の行列に間違いあり！

	// 最大要素の値を算出
	float *pq[4] = { &q.x, &q.y, &q.z, &q.w };
	float v = sqrtf(elem[biggestIndex]) * 0.5f;
	*pq[biggestIndex] = v;
	float mult = 0.25f / v;

	switch (biggestIndex) {
	case 0: // x
		*pq[1] = (matrix._12 + matrix._21) * mult;
		*pq[2] = (matrix._31 + matrix._13) * mult;
		*pq[3] = (matrix._23 - matrix._32) * mult;
		break;
	case 1: // y
		*pq[0] = (matrix._12 + matrix._21) * mult;
		*pq[2] = (matrix._23 + matrix._32) * mult;
		*pq[3] = (matrix._31 - matrix._13) * mult;
		break;
	case 2: // z
		*pq[0] = (matrix._31 + matrix._13) * mult;
		*pq[1] = (matrix._23 + matrix._32) * mult;
		*pq[3] = (matrix._12 - matrix._21) * mult;
		break;
	case 3: // w
		*pq[0] = (matrix._23 - matrix._32) * mult;
		*pq[1] = (matrix._31 - matrix._13) * mult;
		*pq[2] = (matrix._12 - matrix._21) * mult;
		break;
	}

	return true;
}




void Transform::LookAtA(Transform* target) {
	//float3 rotate = float3();	// 移動先回転軸の確保
	//rotate.x = XMConvertToDegrees(-atan2f(
	//	target->m_position.y - m_position.y,
	//	sqrtf(powf(target->m_position.z - m_position.z, 2) + powf(target->m_position.x - m_position.x, 2))));
	//rotate.y = XMConvertToDegrees(atan2f(
	//	target->m_position.x - m_position.x,
	//	target->m_position.z - m_position.z));
	//rotate.z = m_rotate.z;
	//
	//m_DORotate.start	= m_rotate;
	//m_DORotate.end		= rotate;
	//m_DORotate.time		= 5;

	//PrintDebugProc("LookAtA = %.2f, %.2f, %.2f\n", rotate.x, rotate.y, rotate.z);
	//float3 rot = float3(rotate.x - m_rotate.x, rotate.y - m_rotate.y, rotate.z - m_rotate.z);
	//PrintDebugProc("LookAtA = %.2f, %.2f, %.2f\n", rot.x, rot.y, rot.z);
	//// ここカーブにしたい...
	//if (abs(rot.x) <= 10.f && abs(rot.y) <= 10.f && abs(rot.z) <= 10.f) {
	//	rot *= 0.1f;
	//}
	//else {
	//	rot *= 0.01f;
	//}
	//m_rotate.x += rot.x;
	//m_rotate.y += rot.y;
}


// EOF