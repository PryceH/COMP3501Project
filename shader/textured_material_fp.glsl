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
	vec3 v = normalize(view_pos);
	vec3 vv = normalize(position_interp - v);
	vec3 l = light_pos;
	vec3 lv = normalize(l - position_interp);
	vec3 n = normalize(normal_interp);
	vec3 r =  normalize(-lv+ 2 *dot(lv,n)*n);
	float diffuse = dot(n,lv);

	float spec = max(0.0,dot(vv, r));
	spec = pow(spec,50);
	float amb = 0.5;
   
    gl_FragColor = lightcol*pixel*diffuse + 
	lightcol*vec4(1,1,1,1)*0 + 
	lightcol*pixel*amb;
		
}
