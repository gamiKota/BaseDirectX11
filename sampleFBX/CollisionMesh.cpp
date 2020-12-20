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


float3 Intersect3D(
	float3 nv, // 平面の法線ベクトル
	float3 p,  // 平面上の任意の1点
	float3 a,  // 線分の端点
	float3 b   // 線分の端点
);


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
// 奇跡的に交点が0だと当たらない判定になる可能がある
bool CollisionMesh::isMesh2Mesh(Transform* mesh1, Transform* mesh2) {

	bool result = true;
	float3 linePos[4];
	float3 PlanePos[4];
	GetVertex(mesh2, linePos);
	GetVertex(mesh1, PlanePos);

	float3 vec0 = float3::Normalize(float3(mesh1->m_up.x, mesh1->m_up.z, mesh1->m_up.y));
	float3 vec1[4];
	float3 vec2;
	float3 InterPos = float3();

	for (int i = 0; i < 4; i++) {
		vec1[i] = linePos[i] - mesh1->m_position;
	}

	//if (float3::Dot(vec1[0], vec0) * float3::Dot(vec1[1], vec0) <= 0.f) {
	//}
	//else if (float3::Dot(vec1[0], vec0) * float3::Dot(vec1[2], vec0) <= 0.f) {
	//}
	//else if (float3::Dot(vec1[1], vec0) * float3::Dot(vec1[3], vec0) <= 0.f) {
	//}
	//else if (float3::Dot(vec1[2], vec0) * float3::Dot(vec1[3], vec0) <= 0.f) {
	//}
	//else {
	//	result = false;
	//}

	if (float3::Dot(vec1[0], vec0) * float3::Dot(vec1[1], vec0) <= 0.f) {
		InterPos = Intersect3D(vec0, PlanePos[0], linePos[0], linePos[1]);
	}
	else if (float3::Dot(vec1[0], vec0) * float3::Dot(vec1[2], vec0) <= 0.f) {
		InterPos = Intersect3D(vec0, PlanePos[0], linePos[0], linePos[2]);
	}
	else if (float3::Dot(vec1[1], vec0) * float3::Dot(vec1[3], vec0) <= 0.f) {
		InterPos = Intersect3D(vec0, PlanePos[0], linePos[1], linePos[3]);
	}
	else if (float3::Dot(vec1[2], vec0) * float3::Dot(vec1[3], vec0) <= 0.f) {
		InterPos = Intersect3D(vec0, PlanePos[0], linePos[2], linePos[3]);
	}
	else {
		result = false;
	}
	
	//InterPos = Intersect3D(vec0, pos, linePos[0], linePos[1]);
	//if (InterPos.x != FLT_MIN && InterPos.y != FLT_MIN && InterPos.z != FLT_MIN) {	// 交点あり
	//	if ()
	//}

	if (InterPos.x == 0.f && InterPos.y == 0.f && InterPos.z == 0.f) {
		PrintDebugProc("No InterPos\n");
	}
	else {
		// 板を左回りで頂点と中心の内積を求める
		// 無限平面との交点がポリゴン内にない場合、
		// 頂点と中心の内積よりも角度が広がる
		// 交点とのベクトルなので平面空間上で計算が出来る
		

		PrintDebugProc("InterPos = %.2f, %.2f, %.2f\n", InterPos.x, InterPos.y, InterPos.z);
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


float3 Intersect3D(
	float3 nv, // 平面の法線ベクトル
	float3 p,  // 平面上の任意の1点
	float3 a,  // 線分の端点
	float3 b   // 線分の端点
) {

	// 返り値
	float3 ret = float3(FLT_MIN, FLT_MIN, FLT_MIN);

	// ベクトルの準備
	float3 pa, pb; // p->a, p->bのベクトル
	pa = a - p;
	pb = b - p;

	// 内積計算
	float dot_a = float3::Dot(pa, nv); // paベクトルと法線ベクトルの内積
	float dot_b = float3::Dot(pb, nv); // pbベクトルと法線ベクトルの内積

	// 内積が0の場合交点がなく、直線が平面に含まれる
	float MIN_DOT_TH = 0.00001f;
	if (fabsf(dot_a) < MIN_DOT_TH) { dot_a = 0; }
	if (fabsf(dot_b) < MIN_DOT_TH) { dot_b = 0; }

	// 交点なし
	if (dot_a == 0 && dot_b == 0) {
		return ret; // FLT_MIN で返す
	}

	// 交点なし
	if (dot_a * dot_b > 0) {
		return ret; // FLT_MIN で返す
	}

	// 交点あり
	float3 ab = b - a;
	float ratio = fabsf(dot_a) / (fabsf(dot_a) + fabsf(dot_b));
	ret = a + ab * (ratio);

	return ret; // 計算結果を返す
}



// EOF