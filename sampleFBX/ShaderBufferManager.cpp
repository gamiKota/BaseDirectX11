#include "ShaderBufferManager.h"
#include "ShaderBuffer.h"


void ShaderBufferManager::Initialize() {
	m_bufferMap.clear();
	this->Create("MainCamera", sizeof(SHADER_CAMERA), 0);
	this->Create("MainLight", sizeof(SHADER_LIGHT), 1);
	this->Create("MainLightSetting", sizeof(SHADER_LIGHT_SETTING), 2);
	this->Create("MainWorld", sizeof(SHADER_WORLD), 3);
	this->Create("Material", sizeof(SHADER_MATERIAL), 4);
	this->Create("Bone", sizeof(SHADER_BONE), 5);
	//m_bufferMap["World"]		= new ShaderBuffer();
	//m_bufferMap["Material"]		= new ShaderBuffer();
	//m_bufferMap["Bone"]			= new ShaderBuffer();
}

void ShaderBufferManager::Terminate() {
	delete m_bufferMap["Bone"];
	delete m_bufferMap["Material"];
	delete m_bufferMap["MainWorld"];
	delete m_bufferMap["MainLightSetting"];
	delete m_bufferMap["MainLight"];
	delete m_bufferMap["MainCamera"];
	m_bufferMap.clear();
}

void ShaderBufferManager::Update(std::string bufName, void* pData) {
	m_bufferMap[bufName]->UpdateSource(pData);
}

void ShaderBufferManager::Bind(std::string bufName) {
	m_bufferMap[bufName]->BindPS(m_registerMap[bufName]);
	m_bufferMap[bufName]->BindVS(m_registerMap[bufName]);
	m_bufferMap[bufName]->BindGS(m_registerMap[bufName]);
}

void ShaderBufferManager::Create(std::string bufName, UINT size, int regNum) {
	m_bufferMap[bufName] = new ShaderBuffer;
	m_bufferMap[bufName]->Create(size, false);
	m_registerMap[bufName] = regNum;
}