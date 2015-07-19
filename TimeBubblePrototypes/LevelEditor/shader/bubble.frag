uniform sampler2D texture;
uniform vec2 position;
uniform float radius;
//uniform float resolution;
uniform vec2 size;

void main()
{

	//vec2 uv = gl_TexCoord[0].st;
	
	//uv.y /= resolution;

	vec2 uv = gl_FragCoord.st;

    vec2 p = position;

//	p.y /= resolution;

	float d = sqrt((uv.x - p.x) * (uv.x - p.x) + (uv.y - p.y) * (uv.y - p.y));
	float r = radius;
	float n = 1.5f;
	float h = 0.0;
	
	if (d <= r)
	{
		float alpha = asin(d/r);
		
		float d2 = tan(alpha - alpha / n) * (cos(alpha) * r + h);
		
		uv -= (uv - p) * 1.0/d * d2;

	}

	//uv.y *= resolution;

	uv /= size;

	vec4 pixel = texture2D(texture, uv);

	//pixel = lerp(pixel, vec4(1.0,0.0,0.0,1.0), 0.5);
	
	gl_FragColor = pixel;

}

