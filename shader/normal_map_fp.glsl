#version 130

// Attributes passed from the vertex shader
in vec2 uv_interp;

// Uniform (global) buffer
uniform sampler2D texture_map;


void main() 
{
    gl_FragColor = texture(texture_map, uv_interp);
}
