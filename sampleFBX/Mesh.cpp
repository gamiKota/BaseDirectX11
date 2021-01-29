//=============================================================================
//
// ƒƒbƒVƒ…‹¤’Êˆ— [mesh.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Mesh.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "imgui.h"
#include "Geometory.h"
#include "System.h"


//=============================================================================
// •`‰æˆ—
//=============================================================================
void DrawMesh(MESH* pMesh, ID3D11ShaderResourceView* texture, DirectX::XMFLOAT4X4* matrix)
{
	if (!pMesh->isDraw)	return;

	ShaderManager* shader = &ShaderManager::GetInstance();

	SHADER_WORLD world;
	world.mWorld = XMMatrixTranspose(XMLoadFloat4x4(matrix));
	world.mTexture = XMMatrixTranspose(XMLoadFloat4x4(&pMesh->mtxTexture));
	shader->UpdateBuffer("MainWorld", &world);

	SHADER_LIGHT_SETTING light;
	light.light = (pMesh->light) ? XMFLOAT4(1.f, 1.f, 1.f, 1.f) : XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	shader->UpdateBuffer("MainLightSetting", &light);

	DrawPolygon();
}


// ROF