cbuffer MatrixBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer TranslationBuffer :register(b1)
{
	float textureTrans;
};

Texture2D shaderTex;
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
};


PInType VS(VInType i)
{
	PInType o;

	i.pos.w = 1.f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	return o;
}


float4 PS(PInType i) : SV_TARGET
{
	i.tex.x += textureTrans;

	return shaderTex.Sample(sampleType, i.tex);
}

