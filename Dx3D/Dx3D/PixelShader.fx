float4 main() : COLOR
{
	return float4(1.0f, 0.0f, 0.0f, 0.5f);
}

technique Shader
{
    pass Pass_0
    {
        PixelShader = compile ps_2_0 main();
    }
}