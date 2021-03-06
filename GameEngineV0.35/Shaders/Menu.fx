float4x4 WorldViewProj : WorldViewProjection;
float4 color = {1,1,1,1};
uniform extern float fTime : TIME;
texture texture0;

sampler Sampler0
{
    Texture = texture0;
    MipFilter = None;
    MinFilter = None;
    MagFilter = None;
    AddressU = MIRROR;
    AddressV = MIRROR;
};


struct VS_INPUT
{
    float3 position	: POSITION;
	float2 tex0 : TEXCOORD0;
	float4 color : COLOR0;
};

struct VS_OUTPUT
{
	float4 screenposition : POSITION;
	float2 tex0  : TEXCOORD0;
	float4 color : COLOR0;
};

VS_OUTPUT VertexShader0( VS_INPUT IN )
{
	VS_OUTPUT OUT;
	OUT.screenposition = mul( float4(IN.position, 1) , WorldViewProj);
	OUT.tex0 = IN.tex0;
	OUT.color = IN.color;
	return OUT;
}

float4 PixelShader0( VS_OUTPUT IN ) : COLOR
{
	// Note: Hard coded.
	float2 mouse  = {1, 1};

	float  cos_time = cos( fTime ) * 0.05;
	float2 offset = float2( cos( -fTime + IN.tex0.x * IN.tex0.y * 75.0f ), cos( fTime + IN.tex0.x * 75.0f ) ) * 0.012f ;
	offset *= 1 / distance(IN.tex0, mouse );

	float4 color  = tex2D( Sampler0, IN.tex0 + offset );
	color.b       += cos_time * 0.3f;
	color.g       += cos_time * 0.03f;
	color.r       -= cos_time * 2;
	color         *= 0.82f;
	color.a = 1.0f;

	return color;
}

technique Technique0
{
	pass Pass0
	{
		Lighting = FALSE;
		AlphaBlendEnable = TRUE;
		VertexShader = compile vs_2_0 VertexShader0();
		PixelShader  = compile ps_2_0 PixelShader0();
	}
}


