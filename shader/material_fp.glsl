// Material with no illumination simulation
#version 130

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 color_interp;
in vec2 uv_interp;
in vec3 light_pos;
in vec3 light_col;
in vec3 view_pos;

// Uniform (global) buffer
uniform sampler2D texture_map;


void main() 
{
    // Retrieve texture value
    vec4 pixel = texture(texture_map, uv_interp);

	vec4 lightcol = vec4(light_col,1);
	vec3 v = view_pos;
	vec3 vv = normalize(v - position_interp);
	vec3 l = light_pos;
	vec3 lv = normalize(l - position_interp);
	vec3 h = normalize((vv+lv)/2);

	float diffuse = dot(normal_interp,lv);
	float spec = max(0.0,dot(normal_interp,h));

	spec = pow(spec,132);

	//pixel = vec4(0.4,0.8,0.9,1.0);
    // Use texture in determining fragment colour
    gl_FragColor = lightcol*pixel*diffuse + 
	lightcol*vec4(1,1,1,1)*spec + 
	lightcol*pixel*1;
}