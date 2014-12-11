cbuffer MatrixBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
	matrix lightviewmat;
	matrix lightprojmat;
};

cbuffer LightBuffer2 : register(b1)
{
	float3 lightPosition;
	float padding;
};

SamplerState csampleType : register(s0);

Texture2D dmtex : register(t0);


struct VInputT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PInputT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPos : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
};

PInputT VS(VInputT i)
{
	PInputT o;
	float4 worldPos; 

	i.pos.w = 1.0f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.lightViewPos = mul(i.pos, world);
	o.lightViewPos = mul(o.lightViewPos, lightviewmat);
	o.lightViewPos = mul(o.lightViewPos, lightprojmat);

	o.tex = i.tex;

	o.normal = mul(i.normal, (float3x3)world);

	o.normal = normalize(o.normal);

	worldPos = mul(i.pos, world);

	o.lightPos = lightPosition.xyz - worldPos.xyz;
	o.lightPos = normalize(o.lightPos);
	

	return o;
}

float4 PS(PInputT i) : SV_TARGET
{
	float bias;
	float4 color;
	float2 projTexcoord;
	float depthv;
	float ldepthv;
	float lintens;


	bias = 0.001;

	color = float4(0, 0, 0, 1);
	
	projTexcoord.x = i.lightViewPos.x / i.lightViewPos.w / 2.f + 0.5f;
	projTexcoord.y = -i.lightViewPos.y / i.lightViewPos.w / 2.0f + 0.5f;

	if ((saturate(projTexcoord.x) == projTexcoord.x) && (saturate(projTexcoord.y) == projTexcoord.y))
	{
		depthv = dmtex.Sample(csampleType, projTexcoord).r;

		ldepthv = i.lightViewPos.z / i.lightViewPos.w;

		ldepthv = ldepthv - bias;

		if (ldepthv < depthv)
		{
			lintens = saturate(dot(i.normal, i.lightPos));

			if (lintens > 0)
			{
				color = float4(1, 1, 1, 1);
			}
		}
	}

	return color;
}