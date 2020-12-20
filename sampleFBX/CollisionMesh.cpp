/**
 * @file CollisionMesh.cpp
 */


/**
 * @include
 */
#include "CollisionMesh.h"
#include "Transform.h"
#include "debugproc.h"
#include "System.h"


using namespace DirectX;



float3 GetVertexPos(float3 offsetPos, XMFLOAT4X4 *basisMtx) {
	XMFLOAT4X4 mWorld;
	XMMATRIX matrix = XMMatrixIdentity();
	matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(offsetPos.x, offsetPos.y, offsetPos.z));
	matrix *= XMLoadFloat4x4(basisMtx);
	XMStoreFloat4x4(&mWorld, matrix);
	return float3(mWorld._41, mWorld._42, mWorld._43);
}


// メモ
// 取り合えず今は無限平面
// 線分要素にするのはmesh2
// mesh1が無限平面

// 平行なベクトルの外積の大きさは0
// ポリゴンの法線ベクトルと線分の点から平面を貫通しているかどうかをチェック
// ポリゴン平面までの距離から内分比を算出して貫通点の座標を確定
// ポリゴン内部に貫通点が含まれるかをチェック
bool CollisionMesh::isMesh2Mesh(Transform* mesh1, Transform* mesh2) {

	bool result = false;
	float3 pos[4];
	GetVertex(mesh2, pos);

	float3 vec0 = float3::Normalize(float3(mesh1->m_up.x, mesh1->m_up.z, mesh1->m_up.y));
	float3 vec1[4];
	for (int i = 0; i < 4; i++) {
		vec1[i] = pos[i] - mesh1->m_position;
	}

	if (float3::Dot(vec1[0], vec0) * float3::Dot(vec1[1], vec0) <= 0.f ||
		float3::Dot(vec1[0], vec0) * float3::Dot(vec1[2], vec0) <= 0.f ||
		float3::Dot(vec1[1], vec0) * float3::Dot(vec1[3], vec0) <= 0.f ||
		float3::Dot(vec1[2], vec0) * float3::Dot(vec1[3], vec0) <= 0.f) {
		result = true;
	}

	return result;
}


void CollisionMesh::GetVertex(Transform* mesh, float3 vertex[4]) {
	// 頂点位置の取得
	// 基準ワールド空間を直接対象のオブジェクトに指定するので、
	// オフセット位置はUV座標になる
	vertex[0] = GetVertexPos(float3(-0.5f,  0.5f, 0.f), &mesh->m_transform->GetMatrix()); // 左上
	vertex[1] = GetVertexPos(float3( 0.5f,  0.5f, 0.f), &mesh->m_transform->GetMatrix()); // 右上
	vertex[2] = GetVertexPos(float3(-0.5f, -0.5f, 0.f), &mesh->m_transform->GetMatrix()); // 左下
	vertex[3] = GetVertexPos(float3( 0.5f, -0.5f, 0.f), &mesh->m_transform->GetMatrix()); // 右下
}



// EOF