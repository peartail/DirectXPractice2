cbuffer MatrixBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer ClipPlaneBuffer : register(b2)
{
	float4 clipPlane;
};

cbuffer LightBuffer : register(b1)
{
	float4 ambiColor;
	float4 difColor;
	float3 lightDir;
};

Texture2D shaderTex;
SamplerState sampleType;

struct VInType
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PInType
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float clip : SV_ClipDistance0;
};


PInType VS(VInType i)
{
	PInType o;
	matrix rprojworld;

	i.pos.w = 1.f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	o.normal = mul(i.normal, (float3x3)world);
	
	o.normal = normalize(o.normal);

	o.clip = dot(mul(i.pos, world), clipPlane);

	return o;
}


float4 PS(PInType i) : SV_TARGET
{
	float4 texcolor;
	float3 ldir;
	float lintens;
	float4 color;

	texcolor = shaderTex.Sample(sampleType, i.tex);

	color = ambiColor;

	ldir = -lightDir;

	lintens = saturate(dot(i.normal, ldir));

	if (lintens > 0.0f)
	{
		color += (difColor * lintens);
	}

	color = saturate(color);

	color = color * texcolor;

	return color;
}

