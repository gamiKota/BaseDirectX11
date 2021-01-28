#include "FBXPlayer.h"
#include "FBXLoader.h"
#include <vector>
#include <map>
#include <Shlwapi.h>
#include <DirectXMath.h>
#include "../Texture.h"

#pragma comment(lib, "Shlwapi.lib")

struct FBXVertex
{
	float pos[3];
	float color[4];
	float uv[2];
	float normal[3];
	float weight[4];
	long index[4];
};

class FBXResourceManager
{
public:
	struct Model
	{
		ggfbx::MeshList mesh;
		ggfbx::BoneListInfo bone;
		ggfbx::MaterialList material;
		ggfbx::AnimationInfo anime;
		FBXVertex** ppVtx;
		bool* pUseIndex;
	};
private:
	typedef std::pair<std::string, Model*> ModelKey;
	typedef std::vector<ModelKey> ModelLibrary;
	typedef std::map<std::string, ID3D11ShaderResourceView*> TextureMap;
public:
	FBXResourceManager()
	{
	}
	~FBXResourceManager()
	{
		ModelLibrary::iterator it = m_modelLibrary.begin();
		while (it != m_modelLibrary.end())
		{
			ggfbx::Cleanup(&it->second->mesh, &it->second->bone, &it->second->material, &it->second->anime);
			delete it->second;
			++it;
		}
		m_modelLibrary.clear();

		TextureMap::iterator texIt = m_textureMap.begin();
		while (texIt != m_textureMap.end())
		{
			texIt->second->Release();
			++texIt;
		}
		m_textureMap.clear();
	}

	int Load(const char* model)
	{
		ModelLibrary::iterator it = m_modelLibrary.begin();
		while (it != m_modelLibrary.end())
		{
			if (it->first == model)
			{
				return static_cast<int>(it - m_modelLibrary.end());
			}
			++it;
		}

		// 新規追加
		ModelKey key(model, new Model);
		if (ggfbx::Load(model,
			&key.second->mesh, &key.second->bone, &key.second->material, &key.second->anime))
		{
			// 頂点データ作成
			MakeVertexData(
				&key.second->ppVtx,
				&key.second->pUseIndex,
				key.second->mesh);
			// テクスチャ読み込み
			auto matIt = key.second->material.begin();
			while (matIt != key.second->material.end())
			{
				auto texIt = matIt->textureList.begin();
				while (texIt != matIt->textureList.end())
				{
					if (m_textureMap.find(texIt->c_str()) == m_textureMap.end())
					{
						if (texIt->length() > 0)
						{
							ID3D11ShaderResourceView* pTexture;
							char filePath[MAX_PATH];
							strcpy_s(filePath, texIt->c_str());
							if (!PathFileExistsA(filePath))
							{
								char fileName[MAX_PATH];
								strcpy_s(fileName, PathFindFileNameA(filePath));
								strcpy_s(filePath, model);
								PathFindFileNameA(filePath)[0] = '\0';
								strcat_s(filePath, fileName);
							}
							if (SUCCEEDED(LoadTextureFromFile(filePath, &pTexture)))
							{
								m_textureMap.insert(std::pair<std::string, ID3D11ShaderResourceView*>(
									texIt->c_str(), pTexture));
							}
						}
					}
					texIt++;
				}
				++matIt;
			}

			m_modelLibrary.push_back(key);
			return static_cast<int>(m_modelLibrary.size() - 1);
		}
		else
		{
			delete key.second;
		}
		return -1;
	}

	const Model* Get(int index)
	{
		if (m_modelLibrary.size() <= index) { return nullptr; }
		return m_modelLibrary[index].second;
	}
	ID3D11ShaderResourceView* GetTexture(const char* name)
	{
		auto it = m_textureMap.find(name);
		if (it != m_textureMap.end())
		{
			return it->second;
		}
		return nullptr;
	}

private:
	void MakeVertexData(FBXVertex*** pppVtx, bool** ppUseIndex, ggfbx::MeshList& mesh)
	{
		*pppVtx = new FBXVertex*[mesh.size()];
		*ppUseIndex = new bool[mesh.size()];
		auto meshIt = mesh.begin();
		while (meshIt != mesh.end())
		{
			// インデックスバッファを利用して描画するか検証
			bool& isIdxDraw = (*ppUseIndex)[meshIt - mesh.begin()];
			isIdxDraw = true;
			if (meshIt->normalInfo.mapping == ggfbx::MAPPING_INDEX)
			{
				isIdxDraw = false;
			}
			if (meshIt->uvList.size() > 0 && meshIt->uvList[0].mapping == ggfbx::MAPPING_INDEX)
			{
				// 単一uvのみに対応
				isIdxDraw = false;
			}

			// 頂点バッファ作成
			int vtxNum = static_cast<int>(isIdxDraw ? meshIt->vertexList.size() : meshIt->indexList.size());
			FBXVertex* pVtx = (*pppVtx)[meshIt - mesh.begin()] = new FBXVertex[vtxNum];
			for (int i = 0; i < vtxNum; ++i)
			{
				// それぞれのインデックスを取得
				int vtxIdx = i;
				int normIdx = i;
				int uvIdx = i;
				if (!isIdxDraw)
				{
					vtxIdx = meshIt->indexList[i];
					if (meshIt->normalInfo.mapping == ggfbx::MAPPING_VERTEX)
					{
						normIdx = vtxIdx;
					}
					if (meshIt->uvList.size() > 0 && meshIt->uvList[0].mapping == ggfbx::MAPPING_VERTEX)
					{
						uvIdx = vtxIdx;
					}
				}

				// 頂点
				DirectX::XMVECTOR vec;
				DirectX::XMFLOAT3 fVec3;
				vec = DirectX::XMVector3TransformCoord(
					DirectX::XMVectorSet(
						meshIt->vertexList[vtxIdx].x,
						meshIt->vertexList[vtxIdx].y,
						meshIt->vertexList[vtxIdx].z, 0),
					DirectX::XMMatrixSet(
						meshIt->transform._11, meshIt->transform._12, meshIt->transform._13, meshIt->transform._14,
						meshIt->transform._21, meshIt->transform._22, meshIt->transform._23, meshIt->transform._24,
						meshIt->transform._31, meshIt->transform._32, meshIt->transform._33, meshIt->transform._34,
						meshIt->transform._41, meshIt->transform._42, meshIt->transform._43, meshIt->transform._44)
				);
				DirectX::XMStoreFloat3(&fVec3, vec);
				pVtx->pos[0] = fVec3.x;
				pVtx->pos[1] = fVec3.y;
				pVtx->pos[2] = fVec3.z;
				// カラー
				pVtx->color[0] = pVtx->color[1] = pVtx->color[2] = pVtx->color[3] = 1.0f;
				// UV
				if (meshIt->uvList.size() > 0)
				{
					pVtx->uv[0] = meshIt->uvList[0].list[uvIdx].x;
					pVtx->uv[1] = 1.0f - meshIt->uvList[0].list[uvIdx].y;
				}
				else
				{
					pVtx->uv[0] = pVtx->uv[1] = 0.0f;
				}
				// 法線
				if (meshIt->normalInfo.list.size() > 0)
				{
					vec = DirectX::XMVector3TransformCoord(
						DirectX::XMVectorSet(
							meshIt->normalInfo.list[normIdx].x,
							meshIt->normalInfo.list[normIdx].y,
							meshIt->normalInfo.list[normIdx].z, 0),
						DirectX::XMMatrixSet(
							meshIt->transform._11, meshIt->transform._12, meshIt->transform._13, 0,
							meshIt->transform._21, meshIt->transform._22, meshIt->transform._23, 0,
							meshIt->transform._31, meshIt->transform._32, meshIt->transform._33, 0,
							0, 0, 0, 1)
					);
					DirectX::XMStoreFloat3(&fVec3, vec);
					pVtx->normal[0] = fVec3.x;
					pVtx->normal[1] = fVec3.y;
					pVtx->normal[2] = fVec3.z;
				}
				else
				{
					pVtx->normal[0] = pVtx->normal[1] = 0.0f;
					pVtx->normal[2] = 1.0f;
				}
				// ウェイト
				if (meshIt->skinInfo.weightList.size() > 0)
				{
					for (int i = 0; i < ggfbx::MeshInfo::SkinInfo::WeightInfo::WEIGHT_NUM_MAX; ++i)
					{
						auto value = meshIt->skinInfo.weightList[vtxIdx].value[i];
						pVtx->weight[i] = value.weight;
						pVtx->index[i] = value.index;
					}
				}
				else
				{
					for (int i = 0; i < ggfbx::MeshInfo::SkinInfo::WeightInfo::WEIGHT_NUM_MAX; ++i)
					{
						pVtx->weight[i] = 0.0f;
						pVtx->index[i] = 0;
					}
					pVtx->weight[0] = 1.0f;
				}

				++pVtx;
			}

			++meshIt;
		}
	}

private:
	ModelLibrary m_modelLibrary;
	TextureMap m_textureMap;
};

FBXResourceManager g_fbxManager;

FBXPlayer::FBXPlayer()
{
}
FBXPlayer::~FBXPlayer()
{
}


HRESULT FBXPlayer::LoadModel(const char* file)
{
	m_modelID = g_fbxManager.Load(file);
	return m_modelID != -1 ? S_OK : E_FAIL;
}

int FBXPlayer::GetMeshNum()
{
	auto model = g_fbxManager.Get(m_modelID);
	if (model == nullptr) { return 0; }
	return static_cast<int>(model->mesh.size());
}
void* FBXPlayer::GetVertexData(int mesh)
{
	auto model = g_fbxManager.Get(m_modelID);
	if (model == nullptr) { return nullptr; }
	return model->ppVtx[mesh];
}
UINT FBXPlayer::GetVertexSize(int mesh)
{
	return sizeof(FBXVertex);
}
UINT FBXPlayer::GetVertexCount(int mesh)
{
	auto model = g_fbxManager.Get(m_modelID);
	if (model == nullptr) { return 0; }
	if (model->pUseIndex[mesh])
	{
		return static_cast<UINT>(model->mesh[mesh].vertexList.size());
	}
	else
	{
		return static_cast<UINT>(model->mesh[mesh].indexList.size());
	}
}

const unsigned long* FBXPlayer::GetIndexData(int mesh)
{
	auto model = g_fbxManager.Get(m_modelID);
	if (model == nullptr) { return nullptr; }
	return model->mesh[mesh].indexList.data();
}
UINT FBXPlayer::GetIndexCount(int mesh)
{
	auto model = g_fbxManager.Get(m_modelID);
	if (model == nullptr) { return 0; }
	if (model->pUseIndex[mesh])
	{
		return static_cast<UINT>(model->mesh[mesh].indexList.size());
	}
	return 0;
}


ID3D11ShaderResourceView* FBXPlayer::GetTexture(int mesh)
{
	auto model = g_fbxManager.Get(m_modelID);
	if (model == nullptr) { return nullptr; }
	auto matIt = model->material.begin();
	while (matIt != model->material.end())
	{
		if (!matIt->textureList.empty())
		{
			if (matIt->name == model->mesh[mesh].materialName)
			{
				return g_fbxManager.GetTexture(matIt->textureList[0].c_str());
			}
		}
		++matIt;
	}
	return nullptr;
}