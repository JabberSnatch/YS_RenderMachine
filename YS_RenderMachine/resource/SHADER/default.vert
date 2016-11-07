#version 450 core

in layout(location = 0) vec4 vPosition;

uniform mat4 mvp_matrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 world_position;

void main()
{
	world_position = mvp_matrix * vPosition;
	//world_position = projection * view * model * vPosition;
	gl_Position = world_position;
}
