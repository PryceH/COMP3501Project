#version 130

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 color_interp;
in vec2 uv_interp;
in vec3 light_pos;

// Uniform (global) buffer
uniform sampler2D texture_map;
uniform vec3 view_position;

void main() 
{
    // Retrieve texture value
    vec4 pixel = texture(texture_map, uv_interp);


    gl_FragColor = pixel;



	

}
