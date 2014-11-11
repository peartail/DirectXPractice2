cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer ClipPlaneBuffer
{
	float4 clipplane;
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
	float clip : SV_ClipDistance0;
};


PInType VS(VInType i)
{
	PInType o;

	o.pos = mul(i.pos, world);

	o.clip = dot(o.pos, clipplane);

	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	return o;
}


float4 PS(PInType i) : SV_TARGET
{
	return shaderTex.Sample(sampleType, i.tex);
}

