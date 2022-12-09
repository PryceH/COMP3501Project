#version 400

// Attributes passed from the geometry shader
in vec4 frag_color;
in vec2 uv_interp;

// Uniform (global) buffer
uniform sampler2D texture_map; // Normal map

void main (void)
{
    // Very simple fragment shader, but we can do anything we want here
    // We could apply a texture to the particle, illumination, etc.
    // Retrieve texture value
    vec4 pixel = texture(texture_map, uv_interp);

    // Use texture in determining fragment colour
    gl_FragColor = pixel;
    //gl_FragColor = frag_color;
}
