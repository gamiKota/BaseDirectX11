
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 lightPos : TEXCOORD1;
};

Texture2D tex : register(t0);
Texture2D shadow : register(t7);
SamplerState samp : register(s0);

float4 main(PS_IN PIN) : SV_Target
{
	float4 color = tex.Sample(samp, PIN.uv);
	
	// �������猩�����W��UV�l�Ŏ擾
	float3 shadowUV = PIN.lightPos.xyz / PIN.lightPos.w;
	// �[�x�l���擾
	float depth = PIN.lightPos.z / PIN.lightPos.w;
	// �e�N�X�`���̐[�x�l���擾
	float shadowDepth = shadow.Sample(samp, shadowUV.xy).r;

	//// �ő�[�x�X��
	//float  maxDepthSlope = max(abs(ddx(shadowUV.z)), abs(ddy(shadowUV.z)));
	//float  shadowThreshold = 1.0f;      // �V���h�E�ɂ��邩�ǂ�����臒l
	//float  bias = 0.01f;				// �Œ�o�C�A�X�ł�
	//float  slopeScaledBias = 0.001f;     // �[�x�X��
	//float  depthBiasClamp = 0.1f;		// �o�C�A�X�N�����v�l
	//float  shadowBias = bias + slopeScaledBias * maxDepthSlope;
	//shadowBias = min(shadowBias, depthBiasClamp);
	//float3 shadowColor = float3(0.25f, 0.25f, 0.25f);
	////shadowThreshold = shadow.SampleCmpLevelZero(samp, shadowUV.xy, shadowUV.z - shadowBias);
	////shadowColor = lerp(shadowColor, float3(1.0f, 1.0f, 1.0f), shadowThreshold);
	//shadowColor = lerp(shadowColor, float3(1.0f, 1.0f, 1.0f), shadowDepth);
	//color.rgb *= shadowColor;

	// �`�悷��[�x�l�̕����傫��(�J�������牓��)�Ȃ�e�ɂȂ�
	// �e�N�X�`���̐[�x�l�ƕ`��̐[�x�l�������悤�ȋ����̏ꍇ�A
	// �v�Z�̌덷�ŉe�ɂȂ�Ȃ��������e�Ƃ݂Ȃ����(�V���h�E�A�N�l)�B
	// �덷���l�����Đ[�x�l��␳���Ă�����(�o�C�A�X)
	// �e�̌�������
	if (depth > shadowDepth + 0.00001f) {
		color.rgb *= 0.3f;
	}

	return color;
}