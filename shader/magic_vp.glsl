#version 400

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;
in vec3 particle_property;
in float lifespan;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 normal_mat;
uniform float timer;
uniform vec3 camera_position;

// Attributes forwarded to the geometry shader
out vec3 vertex_color;
out float timestep;

// Simulation parameters (constants)
uniform vec3 up_vec = vec3(0.0, 1.0, 0.0);
uniform vec3 object_color = vec3(0.8, 0.8, 0.8);
float grav = -0.12; // Gravity
float speed = particle_property.z; // Controls the speed of the explosion
float upward = -2.0; // additional y velocity for all particles

void main()
{
    // Let time cycle every four seconds
    float circtime = (timer-particle_property.x) - lifespan * floor((timer-particle_property.x) / lifespan);
    float t = circtime; // Our time parameter
    
    // Let's first work in model space (apply only world matrix)
    vec4 position = world_mat * vec4(vec3(vertex.x, vertex.y, vertex.z), 1.0);
    vec4 norm = normal_mat * vec4(normal, 0.0);
	
	// add "upward" to y speed to launch the particles vertically -- can easily omit
    position.y += upward+speed*up_vec.y*t;
    
    // Now apply view transformation
    gl_Position = view_mat * position;
        
    // Define outputs
    // Define color of vertex
    //vertex_color = color.rgb; // Color defined during the construction of the particles
    //vertex_color = object_color; // Uniform color 
    //vertex_color = vec3(t, 0.0, 1-t); // red-purple dynamic color
    vertex_color = vec3(1.0, 0.0, 0.0); // red-yellow dynamic color

    // Forward time step to geometry shader
    timestep = t;
}
