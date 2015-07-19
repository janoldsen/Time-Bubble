


uniform sampler2D texture;
uniform vec2 sun_pos;
uniform float weight;

const int NUM_SAMPLES = 40;

void main()
{
    float density = 1.0;
	//float weight = 6.0;
	float exposure = 0.005;
	
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	
	

	vec2 diff = gl_TexCoord[0].st - sun_pos.xy;
	

	vec2 texCoord = gl_TexCoord[0].st;
	
	diff *= 1.0/NUM_SAMPLES * density;

	float decay = 1.0;


	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		texCoord -= diff;
		
		vec4 sample = texture2D(texture, texCoord);

	
		sample *= decay * weight;

		pixel += sample;

		decay *= 1.0;

	}
			

	pixel *= exposure;


//	clamp(pixel.rgb,0.0, 0.1);

	
    gl_FragColor = pixel;

}
