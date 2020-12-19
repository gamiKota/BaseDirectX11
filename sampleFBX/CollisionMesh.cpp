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


// このやり方じゃだめ
// 後から回転成分を足すやり方じゃないとズレズレのズレ

// 0の空間を基準で動かしてるので、
// これを対象を基準とした空間にすれば解決しそう...？
// 親子関係にしてローカルの値をぶち込む
float3 GetVertexPos(float3 pos, float3 rotate, float3 scale, XMFLOAT4X4 *mxt) {
	XMMATRIX matrix = XMLoadFloat4x4(mxt);
	//matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(pos.x, pos.y, pos.z));
	//matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(pos.x, pos.y, pos.z));
	//matrix = XMMatrixMultiply(matrix, XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z));
	//matrix = XMMatrixMultiply(matrix, XMMatrixScaling(scale.x, scale.y, scale.z));
	XMFLOAT4X4 OutPos;
	XMStoreFloat4x4(&OutPos, matrix);
	return float3(OutPos._41, OutPos._42, OutPos._43);
}


// 普通にベクトルでやった方がいい説


bool CollisionMesh::isMesh2Mesh(Transform* mesh1, Transform* mesh2) {

	// 平面上でしか計算出来てない
	// 座標と拡縮から平面上での頂点座標を求める(回転なし状態)
	// そこから回転を掛け合わせて現在の頂点位置を割り出す

	float3 pos1, pos2, pos3, pos4;

	float3 ScaleHalf;
	float3 rotate = float3(mesh2->m_rotate.x, mesh2->m_rotate.y, mesh2->m_rotate.z);
	ScaleHalf.x = mesh2->m_scale.x * 0.5f;
	ScaleHalf.y = mesh2->m_scale.y * 0.5f;
	
	pos1 = float3(mesh2->m_position.x - ScaleHalf.x, mesh2->m_position.y + ScaleHalf.y, 0.f);	// 左上
	pos2 = float3(mesh2->m_position.x + ScaleHalf.x, mesh2->m_position.y + ScaleHalf.y, 0.f);	// 右上
	pos3 = float3(mesh2->m_position.x - ScaleHalf.x, mesh2->m_position.y - ScaleHalf.y, 0.f);	// 左下
	pos4 = float3(mesh2->m_position.x + ScaleHalf.x, mesh2->m_position.y + ScaleHalf.y, 0.f);	// 右下

	//pos1 = GetVertexPos(pos1, rotate);
	//pos2 = GetVertexPos(pos2, rotate);
	//pos3 = GetVertexPos(pos3, rotate);
	//pos4 = GetVertexPos(pos4, rotate);


	//float3 temp1 = obj->m_transform->m_position;
	//float3 temp2 = m_transform->m_position;
	//float angleX = -m_transform->m_rotate.x;
	//float angleY = -m_transform->m_rotate.y;
	//float angleZ = -m_transform->m_rotate.z;
	//obj->m_transform->m_position.z = (temp1.z - temp2.z) * cosf(angleX) - (temp1.y - temp2.y) * sinf(angleX) + temp2.z;
	//obj->m_transform->m_position.y = (temp1.z - temp2.z) * sinf(angleX) + (temp1.y - temp2.y) * cosf(angleX) + temp2.y;


	return false;
}


// mesh1をmesh2の頂点座標に持っていく
float3 CollisionMesh::test(Transform* mesh1, Transform* mesh2) {

	// 座標を初期値空間にもってくる
	float3 mesh1Pos = mesh1->m_position;
	float3 mesh2Pos = mesh2->m_position;

	float3 pos1, pos2, pos3, pos4;

	float3 ScaleHalf;
	float3 rotate = float3(mesh2->m_rotate.x, mesh2->m_rotate.y, mesh2->m_rotate.z);
	ScaleHalf.x = mesh2->m_scale.x * 0.5f;
	ScaleHalf.y = mesh2->m_scale.y * 0.5f;

	pos1 = float3(mesh2Pos.x - ScaleHalf.x, mesh2Pos.y + ScaleHalf.y, mesh2Pos.z);	// 左上
	pos2 = float3(mesh2Pos.x + ScaleHalf.x, mesh2Pos.y + ScaleHalf.y, mesh2Pos.z);	// 右上
	pos3 = float3(mesh2Pos.x - ScaleHalf.x, mesh2Pos.y - ScaleHalf.y, mesh2Pos.z);	// 左下
	pos4 = float3(mesh2Pos.x + ScaleHalf.x, mesh2Pos.y + ScaleHalf.y, mesh2Pos.z);	// 右下

	//float3 temp1 = obj->m_transform->m_position;
	//float3 temp2 = m_transform->m_position;
	//float angleX = -m_transform->m_rotate.x;
	//float angleY = -m_transform->m_rotate.y;
	//float angleZ = -m_transform->m_rotate.z;
	//obj->m_transform->m_position.z = (temp1.z - temp2.z) * cosf(angleX) - (temp1.y - temp2.y) * sinf(angleX) + temp2.z;
	//obj->m_transform->m_position.y = (temp1.z - temp2.z) * sinf(angleX) + (temp1.y - temp2.y) * cosf(angleX) + temp2.y;

	pos1 = GetVertexPos(pos1, rotate, ScaleHalf * 2.f, &mesh2->GetMatrix());
	//pos2 = GetVertexPos(pos2, rotate, ScaleHalf * 2.f);
	//pos3 = GetVertexPos(pos3, rotate, ScaleHalf * 2.f);
	//pos4 = GetVertexPos(pos4, rotate, ScaleHalf * 2.f);

	//pos1 = GetVertexPos()

	return pos1;
}



// EOF