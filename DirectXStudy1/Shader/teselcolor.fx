cbuffer MatrixBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer TessellationBuffer : register(b1)
{
	float tesselationAmount;
	float3 padding;
};


struct VInputType
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct HullIType
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct ConstOType
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};

struct HullOType
{
	float3 pos : POSITION;
	float4 color :COLOR;
};

struct PInType
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

//////////////////////////////////////////
////////////// Vertex Shader /////////////
//////////////////////////////////////////

HullIType VS(VInputType i)
{
	HullIType o;

	i.pos.w = 1.0f;

	o.pos = i.pos;

	o.color = i.color;

	return o;
}
//////////////////////////////////////////
////////////// Patch Const Funtion////////
//////////////////////////////////////////

ConstOType CPS(InputPatch<HullIType,3> i,uint patchid : SV_PrimitiveID)
{
	ConstOType o;

	o.edges[0] = tesselationAmount;
	o.edges[1] = tesselationAmount;
	o.edges[2] = tesselationAmount;

	o.inside = tesselationAmount;

	return o;
}



////////////////////////////////////////////////////////
////////////// Hull Shader /////////////////////////////
////////////////////////////////////////////////////////
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CPS")]

HullOType CHS(InputPatch<HullIType, 3> patch, uint pointid : SV_OutputControlPointID, uint patchid : SV_PrimitiveID)
{
	HullOType o;

	o.pos = patch[pointid].pos;

	o.color = patch[pointid].color;

	return o;
}


////////////////////////////////////////////////////////
//////////////Domain  Shader////////////////////////////
////////////////////////////////////////////////////////

[domain("tri")]

PInType CDS(ConstOType i, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HullOType, 3> patch)
{
	float3 vertexpos;
	PInType o;

	vertexpos = uvwCoord.x * patch[0].pos + uvwCoord.y * patch[1].pos + uvwCoord.z * patch[2].pos;

	o.pos = mul(float4(vertexpos, 1), world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);
	
	o.color = patch[0].color;

	return o;
}

////////////////////////////////////////////////////////
////////////// Color Shader ////////////////////////////
////////////////////////////////////////////////////////

float4 PS(PInType i) : SV_TARGET
{
	return i.color;
}