#version 140

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 color_interp;
in vec2 uv_interp;
in vec3 light_pos;
in vec3 view_pos;
// Uniform (global) buffer
uniform sampler2D texture_map;
uniform vec3 view_position;


void main() 
{
    // Retrieve texture value
    vec4 pixel = texture(texture_map, uv_interp);

//	pixel = vec4(0.3,0.4,0.6,1.0);
	//pixel = color_interp;
	vec4 lightcol = vec4(1,1,1,1);
	vec3 v = view_pos;
	vec3 vv = normalize(v - position_interp);
	vec3 l = light_pos;
	vec3 lv = normalize(l - position_interp);
	vec3 h = normalize((vv+lv)/2);
	vec3 n = normalize(normal_interp);
	vec3 r = normalize(-lv+ 2 *dot(lv,n)*n);
	float diffuse = dot(n,lv);

	//Phong(V.R)
	float spec = max(0.0,dot(n,h));
	//float spec = max(0.0,dot(view_mat,r));

	spec = pow(spec,41);
	vec4 ambcol = vec4(1,0,0,1);
	//spec = 0;
	//diffuse = 0;
 float amb = 0.4;

	//pixel = vec4(0.4,0.8,0.9,1.0);
   
    gl_FragColor = lightcol*pixel*diffuse +
	lightcol*vec4(1,1,1,1)*spec +
	lightcol*pixel*amb;
		
   //gl_FragColor = vec4(1,position_interp.x,0,1);
}
