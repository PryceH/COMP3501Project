#version 400

// Attributes passed from the geometry shader
in vec4 frag_color;
in vec2 tex_coord;

// Uniform (global) buffer
uniform sampler2D tex_samp;

// Simulation parameters (constants)
uniform vec3 object_color = vec3(0.6, 0.2, 0.01);


void main (void)
{
    // Get pixel from texture
    vec4 outval = texture(tex_samp, tex_coord);
    // Adjust specified object color according to the grayscale texture value
    outval = vec4(object_color.r, object_color.g, object_color.b, frag_color.a);
    // Set output fragment color
    gl_FragColor = outval;
}
