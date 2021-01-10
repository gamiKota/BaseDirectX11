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


using namespace DirectX;


Quaternion GetRotation(XMFLOAT3X3 m)
{
	float elem[4];
	elem[0] =  m._11 - m._22 - m._33 + 1.0f;
	elem[1] = -m._11 + m._22 - m._33 + 1.0f;
	elem[2] = -m._11 - m._22 + m._33 + 1.0f;
	elem[3] =  m._11 + m._22 + m._33 + 1.0f;

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


// メモ
// クォータニオンをベクトルととらえる
// 各要素(x, y, z)は180を境に0～1～0に数値が正規化
// クォータニオンの4個のパラメータのノルムは１
// (そのためw要素が-になる場合もある)
// メリットとして、3x3の回転行列のもつ性質を失わずに４個のパラメータに圧縮したものがクォータニオン
// 逆に、クォータニオンが与えられたときに具体的な回転や姿勢をイメージしにくい


Transform::Transform() : m_position(float3()), m_rotation(Quaternion()), m_scale(float3()), m_tween(new Tween()), m_Parent(nullptr) {
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());
}


void Transform::Uninit() {
	delete m_tween;
}


void Transform::Update() {

}


void Transform::LastUpdate() {

	{	// テストTween
		m_tween->Update();

		if (m_tween->m_isTween == E_TWEEN::DO ||
			m_tween->m_isTween == E_TWEEN::END) {
			m_position = m_tween->GetResult();
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

	// クォータニオンは正規化されていることが前提
	m_rotation = Quaternion::Normalize(m_rotation);
	float3 rotate = Quaternion::RadianAngle(m_rotation);

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
	}

	// 方向ベクトルの更新(ピッチ、ヨー、ロール)
	// (3x3の回転行列にあたる、正規化されていて良いのかはわからない)
	m_right = float3::Normalize({ m_world._11, m_world._12, m_world._13 });
	m_up = float3::Normalize({ m_world._21, m_world._22, m_world._23 });
	m_forward = float3::Normalize({ m_world._31, m_world._32, m_world._33 });
}


void Transform::SetImGuiVal() {
#if _DEBUG
	ImGui::DragFloat3("position", (float*)&m_transform->m_position);
	ImGui::DragFloat3("rotation", (float*)&m_transform->m_rotation, 0.01f);
	ImGui::DragFloat3("scale", (float*)&m_transform->m_scale);

	ImGui::DragFloat3("forward", (float*)&m_transform->m_forward);
	ImGui::DragFloat3("up", (float*)&m_transform->m_up);
	ImGui::DragFloat3("right", (float*)&m_transform->m_right);
#endif
}


Tween* Transform::DOMove(float3 position, float time) {

	m_tween->DOTween(m_position, position, time);

	return m_tween;
}


// メモ
// Unityでは上方ベクトルが引数
void Transform::LookAt(Transform* target, float3 worldUp) {
	if (!target)	return;

	float3 z = float3::Normalize(target->m_position - m_position);
	float3 x = float3::Normalize(float3::Cross(worldUp, z));
	float3 y = float3::Normalize(float3::Cross(z, x));
	
	XMFLOAT3X3 m;
	XMStoreFloat3x3(&m, XMMatrixIdentity());
	m._11 = x.x; m._12 = y.x; m._13 = z.x;
	m._21 = x.y; m._22 = y.y; m._23 = z.y;
	m._31 = x.z; m._32 = y.z; m._33 = z.z;
	
	Quaternion rot = GetRotation(m);
	m_rotation = Quaternion::Normalize(rot);
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