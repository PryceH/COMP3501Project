#version 130

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;
in vec2 uv;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;
uniform mat4 normal_mat;
uniform vec3 position;

// Attributes forwarded to the fragment shader
out vec3 position_interp;
out vec3 normal_interp;
out vec4 color_interp;
out vec2 uv_interp;
out vec3 light_pos;
out vec3 light_col;
out vec3 view_pos;

// Material attributes (constants)
uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 view_position;


void main()
{
    gl_Position = projection_mat * view_mat * world_mat * vec4(vertex, 1.0);

    //position_interp = vec3(view_mat * world_mat * vec4(vertex, 1.0));
    
    normal_interp = vec3(normal_mat * vec4(normal, 0.0));

    color_interp = vec4(color, 1.0);

    uv_interp = uv;

    light_col = light_color;
    position_interp = vec3(world_mat * vec4(position, 1.0));
    light_pos = vec3(world_mat * vec4(light_position, 1.0));
    view_pos = vec3(world_mat * vec4(view_position, 1.0));
    //view_pos = view_position;
}
