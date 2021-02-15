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
#include "PlayerState.h"
#include "Collision.h"
#include "Material.h"
#include "System.h"


using namespace DirectX;


static const float CIRCLE_SIZE = 300.f;



float3 LockOnMarker(float3 target) {
	float3 OutPos = float3();

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
	XMVECTOR vViewProj = XMLoadFloat3(&target);

	// ビュー変換とプロジェクション変換
	vViewProj = XMVector3Transform(vViewProj, view);
	vViewProj = XMVector3Transform(vViewProj, proj);

	// zで割って-1~1の範囲に収める(正規化)
	// zを正規化しないと負の値か正の値でカメラの後ろ判定がとれそう
	XMFLOAT3 norPos;
	XMStoreFloat3(&norPos, vViewProj);
	norPos.x /= norPos.z; norPos.y /= norPos.z;
	//norPos.z /= norPos.z;

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
	obj->m_ps = E_PS::PS_COLOR;

	if (m_target == nullptr) {
		m_target = GameObject::FindGameObjectWithTag("Enemy");
	}
}


void TargetCtr::Update() {
	// ロックオンマーカー
	float3 centerPos = m_target->GetComponent<CollisionBox>()->m_vCenter;
	float3 marker = LockOnMarker(m_target->m_transform->m_position);
	m_transform->m_position = marker;

	GameObjectUI* obj = dynamic_cast<GameObjectUI*>(m_gameObject);
	obj->m_material->m_diffuse.w = 0.f;

	// メモ
	// メインターゲット以外のロックオンマーカーの表示は、
	// サブターゲットはメインターゲットを切り替えて1秒、もしくは何かしらを攻撃している敵
	// 画面外ミニロックオンマーカーは何かしらを攻撃している時、被弾時にはATTACKに切り替えて1秒表示
	if (marker.z < 0.f ||
		(m_transform->m_position.x + m_transform->m_scale.x * 0.5f < -(float)SCREEN_CENTER_X ||
		 m_transform->m_position.x - m_transform->m_scale.x * 0.5f >  (float)SCREEN_CENTER_X ||
		 m_transform->m_position.y + m_transform->m_scale.y * 0.5f < -(float)SCREEN_CENTER_Y ||
		 m_transform->m_position.y - m_transform->m_scale.y * 0.5f >  (float)SCREEN_CENTER_Y)) {	// 画面外
		// プレイヤーと相手の座標からベクトルを求める
		// その位置関係をスクリーンに持ってくる
		// 真上から見る(ミニマップ？)
		// 真上から見て位置関係(角度)を割り出す(現状は円表示になる)
		// 画面比で割ったらなんか動いたっぽい
		// 画面内に収まるように正規化
		// 回転情報は取り合えずプレイヤーのY軸回転を持ってくる(もっと安全な情報が良い)
		// ロックオン状態だと注視点がプレイヤー座標からターゲット座標に切り替わるため、
		// 視覚的にはズレた様に表示されてしまう

		// ロックオン時は3D座標上でベクトルを出してそっからスクリーン変換が一番簡単そう

		//GameObject* player = GameObject::Find("Player");
		//CCamera* camera = CCamera::Get();

		//// 座標の初期化
		//m_transform->m_position = float3();
		//float tempX;
		//float tempZ;
		//float vecX;
		//float vecZ;
		//GameObject* playerTarget = player->GetComponent<PlayerState>()->GetTarget();
		////float3 playerMaker = LockOnMarker(player->m_transform);

		//// プレイヤーのターゲット指定状態で処理を分ける
		//// ターゲットと密着してる時に挙動が可笑しい
		//if (playerTarget != nullptr) {
		//	// 3D上でのベクトル
		//	float3 vec3D = m_target->m_transform->m_position - playerTarget->m_transform->m_position;
		//	// 正規化
		//	vec3D = float3::Normalize(vec3D);
		//	// 座標ひ変換
		//	float3 pos3D = playerTarget->m_transform->m_position;
		//	pos3D += vec3D;
		//	// スクリーン座標に変換
		//	pos3D = LockOnMarker(pos3D);
		//	// 補正(ここ汚ないし原因もわかんない)
		//	if (pos3D.y > 1.f) {
		//		pos3D.y = m_colY;
		//	}
		//	else {
		//		m_colY = pos3D.y;
		//	}
		//	// 画面比に合わせたベクトルに変換
		//	pos3D *= (float)SCREEN_RATIO;
		//	// 正規化
		//	pos3D = float3::Normalize(float3(pos3D.x, pos3D.y, 0.f)) * CIRCLE_SIZE;
		//	// 座標に反映
		//	m_transform->m_position += pos3D;
		//	// ターゲット方向の更新
		//	float angle = atan2(m_transform->m_position.y, m_transform->m_position.x);
		//	m_transform->m_rotation = Quaternion::AngleAxis(angle, float3(0.f, 0.f, 1.f));
		//}
		//else {
		//	// 二次元ベクトルに変換
		//	float3 playerRot = Quaternion::RadianAngle(player->m_transform->m_rotation);
		//	tempX = m_target->m_transform->m_position.x - player->m_transform->m_position.x;
		//	tempZ = m_target->m_transform->m_position.z - player->m_transform->m_position.z;
		//	vecX = tempX * cosf(playerRot.y) - tempZ * sinf(playerRot.y);
		//	vecZ = tempX * sinf(playerRot.y) + tempZ * cosf(playerRot.y);
		//	// 画面比に合わせたベクトルに補正
		//	vecX *= (float)SCREEN_RATIO;
		//	vecZ *= (float)SCREEN_RATIO;
		//	// 正規化
		//	float3 vec = float3::Normalize(float3(vecX, vecZ, 0.f)) * CIRCLE_SIZE;
		//	// 座標に反映
		//	m_transform->m_position += vec;
		//	// ターゲット方向の更新
		//	float angle = XMConvertToDegrees(atan2(m_transform->m_position.y, m_transform->m_position.x));
		//	m_transform->m_rotation = Quaternion::AngleAxis(angle, float3(0.f, 0.f, 1.f));
		//}

		//obj->m_texture = E_TEXTURE_ROCK_ICON_OUTCAMERA_MINI;
		//obj->m_color = float3(1.f, 0.f, 0.f);
		//obj->m_layer = E_LAYER::UI;

	}
	else {	// 画面内
		if (m_target == GameObject::Find("Player")->GetComponent<PlayerState>()->GetTarget()) {
			obj->m_texture = E_TEXTURE_ROCK_ICON_INCAMERA_MAIN;
			m_transform->m_scale = float3(100.f, 100.f, 0.f);
			obj->m_layer = (E_LAYER)((int)E_LAYER::UI + 2);
			obj->m_material->m_diffuse = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
		}
		else {
			obj->m_texture = E_TEXTURE_ROCK_ICON_INCAMERA_SUB;
			obj->m_material->m_diffuse = XMFLOAT4(1.f, 0.6f, 0.f, 1.f);
			m_transform->m_scale = float3(80.f, 80.f, 0.f);
			obj->m_layer = (E_LAYER)((int)E_LAYER::UI + 1);
		}
		m_transform->m_rotation = Quaternion::AngleAxis(0.f, float3(0.f, 0.f, 1.f));
	}
}


// EOF