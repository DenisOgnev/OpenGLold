#version 330 core

layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;

out vec3 normal;
out vec3 frag_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_matrix;

void main()
{
    frag_position = vec3(model * vec4(input_position, 1.0));
    normal = normal_matrix * input_normal;
    gl_Position = projection * view * model * vec4(input_position, 1.0);
}