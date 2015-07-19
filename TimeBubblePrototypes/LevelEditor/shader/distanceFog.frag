


uniform sampler2D texture;
uniform float depth;
uniform float time;
uniform float fog_Strength;
uniform vec4 fog_Color;
uniform bool clouds;


vec2 hashGrad( vec2 p)
{
	p = vec2 (dot(p, vec2(127.1, 311.7)),
			  dot(p, vec2(269.5, 183.3)) );
	return -1.0 + 2.0*fract(sin(p)* 43758.5453123);
}


float noiseGrad(in vec2 p)
{
	vec2 i = floor( p );
	vec2 f = fract( p );
	
	vec2 u = f*f*(3.0-2.0*f);
	
	return mix( mix( dot( hashGrad( i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ), 
                     dot( hashGrad( i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( hashGrad( i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ), 
                     dot( hashGrad( i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
	
}



void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	vec4 _fog_Color = fog_Color;
	vec4 tex_Color = gl_Color * pixel;

	_fog_Color.a = pixel.a;

	float fog_Factor = exp( -(depth - 1.0) * fog_Strength);

	float f = 0.0;
	if (clouds)
	{
		vec2 uv = gl_FragCoord.xy;

		
	
		uv.x += time * 25.0;

		uv *= 0.005;
		mat2 m = mat2(1.6, 1.2, -1.2, 1.6);
		for (int i = 0; i < 3; i++)
		{
			f+= 0.5 / float(i+1) * noiseGrad(uv);
			uv = m * uv;
		}

		//f = 0.3 + 0.9 * f;

		f *= 2.0;

		f = clamp(f, 0.0, 1.0);
	}


	gl_FragColor = mix( mix(_fog_Color, pixel, fog_Factor ), pixel, f) * gl_Color;
	
	//gl_FragColor = vec4(f, f, f, 1.0);

}