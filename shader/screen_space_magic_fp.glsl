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

	// wavering
    float vibration_magnitude = max(10.0, 1.0);
	float offset_x  = 0.02*(sin(timer*10+vibration_magnitude*pos.y));
    float offset_y  = 0.02*(sin(timer*10+vibration_magnitude*pos.x));
    pos.x = pos.x + offset_x;
    pos.y = pos.y + offset_y;

    pixel = texture(texture_map, pos) ;

    float frag1 = mod(timer, 5);
    float frag2 = mod(timer-1, 5);
    float frag3 = mod(timer-2, 5);
    float frag4 = mod(timer-3, 5);
    
    if (uv0.y < frag1/4 && uv0.y > frag1/4-0.25)
    {
		float g = (pixel.r+pixel.g+pixel.b)/1.0;
		pixel = vec4(g*0.1,g*0.3,g*0.5,1);
    }
    if (uv0.y < (frag2)/4 && uv0.y > (frag2)/4-0.25)
    {
		float g = (pixel.r+pixel.g+pixel.b)/2.0;
		pixel = vec4(g*0.6,g*0.9,g*1.0,1);
    }
    if (uv0.y < (frag3)/4 && uv0.y > (frag3)/4-0.25)
    {
		float g = (pixel.r+pixel.g+pixel.b)/3.0;
		pixel = vec4(g*1.2,g*1.1,g*0.8,1);
    }
    if (uv0.y < (frag4)/4 && uv0.y > (frag4)/4-0.25)
    {
		float g = (pixel.r+pixel.g+pixel.b)/4.0;
		pixel = vec4(g*0.7,g*0.4,g*0.2,1);
    }
    gl_FragColor = pixel;
}
