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


// メモ
// クォータニオンをベクトルととらえる
// 各要素(x, y, z)は180を境に0〜1〜0に数値が正規化
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
	// Meshの場合、座標は親の大きさの範囲に指定される(0.5〜0.5の範囲)
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
	m_rotation = Quaternion::LookRotation(target->m_position - m_position, worldUp);
}
void Transform::LookAt(float3 target, float3 worldUp) {
	m_rotation = Quaternion::LookRotation(target - m_position, worldUp);
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