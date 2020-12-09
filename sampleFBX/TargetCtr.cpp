/**
 * @file TargetCtr.cpp
 */


/**
 * @include
 */
#include "TargetCtr.h"
#include <DirectXMath.h>
#include "Graphics.h"
#include "debugproc.h"
#include "TextureManager.h"
#include "Camera.h"
#include "GameObject.h"
#include "GameObjectUI.h"
#include "GameObject3D.h"
#include "Player.h"
#include "System.h"


using namespace DirectX;


//ベクトルの長さを計算する
float get_vector_length(float3 v) {
	return powf((v.x * v.x) + (v.y * v.y) + (v.z * v.z), 0.5);
}

//ベクトル内積
float dot_product(float3 vl, float3 vr) {
	return vl.x * vr.x + vl.y * vr.y + vl.z * vr.z;
}


float3 LockOnMarker(Transform* target) {
	float3 OutPos = float3();
	
	// カメラの向いてるベクトルと
	// 対象物のカメラに対してのベクトルが正と負で
	// カメラに対しての後ろ判定がとれそう
	float3 CameraVec = GameObject::Find("Player")->m_transform->m_forward;
	float3 TargetVec = float3::Normalize(CCamera::Get()->m_transform->m_position - target->m_transform->m_position);
	// 二つのベクトルの長さを取得
	float length_A = get_vector_length(CameraVec);
	float length_B = get_vector_length(TargetVec);
	//内積とベクトル長さを使ってcosθを求める
	float cos_sita = dot_product(CameraVec, TargetVec) / (length_A * length_B);
	//cosθからθを求める
	float sita = acos(cos_sita);
	//ラジアンでなく0～180の角度でほしい場合はコメント外す
	sita = sita * 180.f / XM_PI;
	
	PrintDebugProc("sita = %.2f\n", sita);
	if (sita < 90.f) {
		return float3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.f);
	}


	// ビュー行列と射影行列の取得
	XMMATRIX view = XMLoadFloat4x4(&CCamera().Get()->GetView());
	XMMATRIX proj = XMLoadFloat4x4(&CCamera().Get()->GetProj());

	// ビューポート行列（スクリーン行列）の作成
	float w = (float)SCREEN_CENTER_X;
	float h = (float)SCREEN_CENTER_Y;

	XMMATRIX viewport = XMLoadFloat4x4(&XMFLOAT4X4(
		w,  0, 0, 0,
		0, -h, 0, 0,
		0,  0, 1, 0,
		w,  h, 0, 1
	));

	XMVECTOR vScreenPos;
	XMVECTOR vViewProj = XMLoadFloat3(&target->m_position);

	// ビュー変換とプロジェクション変換
	vViewProj = XMVector3Transform(vViewProj, view);
	vViewProj = XMVector3Transform(vViewProj, proj);

	// zで割って-1~1の範囲に収める(正規化)
	XMFLOAT3 norPos;
	XMStoreFloat3(&norPos, vViewProj);
	norPos.x /= norPos.z; norPos.y /= norPos.z; norPos.z /= norPos.z;
	XMVECTOR vec = XMLoadFloat3(&norPos);
	// スクリーン変換
	vScreenPos = XMVector3Transform(vec, viewport);

	XMFLOAT3 ScreenPos;
	XMStoreFloat3(&ScreenPos, vScreenPos);

	OutPos.x =  ScreenPos.x - SCREEN_CENTER_X;
	OutPos.y = -ScreenPos.y + SCREEN_CENTER_Y;
	OutPos.z =  ScreenPos.z;

	return OutPos;
}


void TargetCtr::Start() {
	// 変数初期化
	m_transform->m_scale = float3(100.f, 100.f, 0.f);
	GameObjectUI* obj = dynamic_cast<GameObjectUI*>(m_gameObject);
	obj->m_shader = E_SHADER_2D::_COLOR;

	if (m_target == nullptr) {
		m_target = GameObject::FindGameObjectWithTag("Enemy");
	}
}


void TargetCtr::Update() {
	// ロックオンマーカー
	m_transform->m_position = LockOnMarker(m_target->m_transform);

	GameObjectUI* obj = dynamic_cast<GameObjectUI*>(m_gameObject);
	// 画面外
	// 挙動が可笑しい原因
	// あくまでも座標は3Dオブジェクトを追ってるからスクリーン座標へ変換した時の値が望んでるものとは限らない
	// 取り合えずくそコードで実装
	if (m_transform->m_position.x + m_transform->m_scale.x * 0.5f < -(float)SCREEN_CENTER_X ||
		m_transform->m_position.x - m_transform->m_scale.x * 0.5f >  (float)SCREEN_CENTER_X	||
		m_transform->m_position.y - m_transform->m_scale.y * 0.5f < -(float)SCREEN_CENTER_Y ||
		m_transform->m_position.y + m_transform->m_scale.y * 0.5f >  (float)SCREEN_CENTER_Y) {

		float angle = atan2(GameObject::Find("Player")->m_transform->m_position.z - m_target->m_transform->m_position.z,
			GameObject::Find("Player")->m_transform->m_position.x - m_target->m_transform->m_position.x);

		m_transform->m_position.x = 0.f - cosf(angle + GameObject::Find("Player")->m_transform->m_rotate.y) * SCREEN_CENTER_X;
		m_transform->m_position.y = 0.f - sinf(angle + GameObject::Find("Player")->m_transform->m_rotate.y) * SCREEN_CENTER_X;

		if (m_transform->m_position.x + m_transform->m_scale.x * 0.5f < -(float)SCREEN_CENTER_X) {
			m_transform->m_position.x = -(float)SCREEN_CENTER_X + m_transform->m_scale.x * 0.5f;
		}
		if (m_transform->m_position.x - m_transform->m_scale.x * 0.5f > (float)SCREEN_CENTER_X) {
			m_transform->m_position.x = (float)SCREEN_CENTER_X - m_transform->m_scale.x * 0.5f;
		}
		if (m_transform->m_position.y - m_transform->m_scale.y * 0.5f < -(float)SCREEN_CENTER_Y) {
			m_transform->m_position.y = -(float)SCREEN_CENTER_Y + m_transform->m_scale.y * 0.5f;
		}
		if (m_transform->m_position.y + m_transform->m_scale.y * 0.5f > (float)SCREEN_CENTER_Y) {
			m_transform->m_position.y = (float)SCREEN_CENTER_Y - m_transform->m_scale.y * 0.5f;
		}
		obj->m_texture = E_TEXTURE_ROCK_ICON_OUTCAMERA_MINI;
		obj->m_color = float3(1.f, 0.f, 0.f);
		obj->m_layer = E_LAYER::UI;
		m_transform->m_rotate.z = atan2(m_transform->m_position.y, m_transform->m_position.x) * 180.f / XM_PI;
	}
	// 画面内
	else {
		if (m_target == GameObject::Find("Player")->GetComponent<PlayerCtr>()->m_target) {
			obj->m_texture = E_TEXTURE_ROCK_ICON_INCAMERA_MAIN;
			obj->m_color = float3(1.f, 0.f, 0.f);
			m_transform->m_scale = float3(100.f, 100.f, 0.f);
			obj->m_layer = (E_LAYER)((int)E_LAYER::UI + 2);
		}
		else {
			obj->m_texture = E_TEXTURE_ROCK_ICON_INCAMERA_SUB;
			obj->m_color = float3(1.f, 0.6f, 0.f);
			m_transform->m_scale = float3(80.f, 80.f, 0.f);
			obj->m_layer = (E_LAYER)((int)E_LAYER::UI + 1);
		}
		m_transform->m_rotate.z = 0.f;
	}
}


// EOF