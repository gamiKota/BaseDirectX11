/**
 * @file PhongPS.hlsl
 */


 // ���_�V�F�[�_����n�����f�[�^
struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : TEXCOORD2;
	float4 wPos : TEXCOORD3;
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


Texture2D tex : register(t0);
SamplerState samp : register(s0);


float4 main(PS_IN PIN) : SV_Target{

	float4 color;
	color = tex.Sample(samp, PIN.uv);

	// �e�̌v�Z
	// �����̃x�N�g���Ɩ@���̓��ς���Â����v�Z����
	// �����̃x�N�g���𔽓](-�̒l���|����)���āA�����������ڂɂȂ�悤�Ɍv�Z����
	float3 L = normalize(-LightDir.xyz);
	float3 N = normalize(PIN.normal);

	// ����
	float d = dot(N, L);

	// 1�`0�͈̔͂ɕ␳����
	d = d * 0.5f + 0.5f;

	// �Ƃ�Ԃ�(���˂̌v�Z)
	float3 V = PIN.wPos.xyz - cameraPos.xyz;
	// reflect... �������œn�����@���̃x�N�g������A�������̃x�N�g�����ǂ̂悤�ɔ��˂��邩�v�Z
	float3 R = reflect(V, N);
	R = normalize(R);
	float s = dot(L, R);
	// �X�y�L�����[��1�`0�͈̔͂ɂȂ��l�𖳎�����
	// saturate... ������0�`1�͈̔͂Ɋۂ߂�
	s = saturate(s);
	// �Ƃ�Ԃ������͂�苭���ۗ��悤�Ɍv�Z���ʂ���Z����
	s = pow(s, 50);


	float4 diffuseLight		= LightDiffuse;		//!< �����̐F
	float4 ambientLight		= LightAmbient;		//!< ���肩�甽�˂����F
	float4 specularLight	= LightSpecular;	//!< �P���Č�������̐F
	float4 materialDiffuse	= g_Diffuse;		//!< ���̂����˕Ԃ��₷����
	float4 materialAmbient	= g_Ambient;		//!< ���̂����˕Ԃ��₷������
	float4 materialSpecular = g_Specular;		//!< ���̂��������ۂ���(���ˌ��̌W��)

	// ���}�e���A�����l�������e��
	float3 diffuse = (float3)(diffuseLight * materialDiffuse);
	float3 ambient = (float3)(ambientLight * materialAmbient);
	float3 specular = (float3)(specularLight * materialSpecular);
	float3 lambert = diffuse * d + ambient;
	float3 phong = specular * s;

	color.rgb *= lambert + phong;

	return color;
}


// EOF