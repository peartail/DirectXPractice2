cbuffer MatrixBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

Texture2D shaderTex;
SamplerState sampleType;

struct VInputT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
	float4 color :COLOR;
};

struct PInputT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR;
};

PInputT VS(VInputT i)
{
	PInputT o;

	i.pos.w = 1.0f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	o.color = i.color;

	return o;
}

float4 PS(PInputT i) : SV_TARGET
{
	float4 texcolor;
	float4 color;

	texcolor = shaderTex.Sample(sampleType, i.tex);

	color = texcolor * i.color;


	return color;
}