#version 130

// Passed from the vertex shader
in vec2 uv0;

// Passed from outside
uniform float timer;
uniform sampler2D texture_map;

void main() 
{
vec4 pixel;
vec2 pos = uv0;
pixel = texture(texture_map,uv0);
	

	

	
	if (uv0.x > 0.0 && uv0.y > 0.0)
	{
	vec2 pos = uv0;
		float x = uv0.x-0.5;
		float y = uv0.y-0.5;
		float v = abs(x*y)*5*abs(cos(timer));
	
		float offset  = (v);
		pos.x = pos.x - v;
		pos.y = pos.y - v;

		pixel = texture(texture_map, pos) ;
	}
	
    gl_FragColor = pixel;
	
}
