/**
 * @file PhongPS.hlsl
 */


 // ���_�V�F�[�_����n�����f�[�^
struct PS_IN {
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float3 normal	: TEXCOORD1;
	float4 color	: TEXCOORD2;
	float4 wPos		: TEXCOORD3;
};


cbuffer Camera : register(b0) {
	float4	cameraPos;	// ���_���W(���[���h���)
	float4x4 view;
	float4x4 proj;
};
cbuffer Light : register(b1) {
	float4	LightDir;			// ��������
	float4	LightAmbient;		// �����F(�A���r�G���g)
	float4	LightDiffuse;		// �����F(�f�B�t���[�Y)
	float4	LightSpecular;		// �����F(�X�y�L����)
};
cbuffer World : register(b3) {
	float4x4 mtxWorld;
	float4x4 mtxTexture;
};
cbuffer Material : register(b4) {
	float4	g_Ambient;	// �A���r�G���g�F
	float4	g_Diffuse;	// �f�B�t���[�Y�F
	float4	g_Specular;	// �X�y�L�����F
	float4	g_Emissive;	// �G�~�b�V�u�F
}
cbuffer Player : register(b5) {
	float4	g_playerPos;	// ���W
}


Texture2D tex : register(t0);
SamplerState samp : register(s0);


float4 main(PS_IN PIN) : SV_Target{

	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	float4 totalDiffuse = float4(0.f, 0.f, 0.f, 0.f);
	float PI = 3.1415926535;	// �~����
	float rad1 = PI / 180.f;	// 1���̃��W�A���p

	// �`�悷��s�N�Z������_�����܂ł̃x�N�g��
	float3 dir = g_playerPos.xyz - PIN.wPos.xyz;
	float len = length(dir);
	dir = normalize(dir);

	// ���ς��疾�邳���v�Z
	float d = dot(PIN.normal, dir);
	d = d * 0.5f + 0.5f;

	// �����ɉ����Č����キ����(����)
	float attenuation = saturate(1.f / (1.f + 20 * len + 30 * len * len));
	d *= attenuation;
	totalDiffuse += d;

	// �ŏI�I�Ȗ��邳���v�Z
	color.rgb *= totalDiffuse.rgb;
	//float len2 = length(totalDiffuse);
	//if (len) {
	//
	//}
	discard;


	return color;
}


// EOF