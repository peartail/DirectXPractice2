cbuffer PerFrameBuffer
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer FogBuffer
{
	float fogstart;
	float fogend;
};

Texture2D sahderTexture;
SamplerState sampleType;

struct VInType
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct PInType
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float fogFactor : FOG;
};

PInType VS(VInType i)
{
	PInType o;
	
	i.pos.w = 1.0f;
	
	o.pos = mul(i.pos,world);
	o.pos = mul(o.pos,view);
	
	//linear 안개
	//o.fogFactor = saturate((fogend - o.pos.z) / (fogend - fogstart));
	

	//exponential 안개
	o.fogFactor = 1.0 / pow(2.71828, o.pos.z * 0.025);

	o.pos = mul(o.pos, proj);
	
	o.tex = i.tex;
	
	return o;
}

float4 PS(PInType i) : SV_TARGET
{
	float4 textureColor;
	float4 fogColor;
	float4 finalColor;
	
	textureColor = sahderTexture.Sample(sampleType, i.tex);
	
	fogColor = float4(0.5f,0.5f,0.5f,1.0f);
	
	finalColor = i.fogFactor * textureColor + (1.0 - i.fogFactor) * fogColor;

	return finalColor;
}
