#version 450 core

in layout(location = 0) vec4 vPosition;

uniform mat4 mvp_matrix;

out vec4 world_position;

void main()
{
	world_position = mvp_matrix * vPosition;
	gl_Position = world_position;
}
