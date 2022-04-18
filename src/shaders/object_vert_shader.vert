#version 330 core

layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;

out vec3 lighting_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_matrix;

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 view_position;

void main()
{
    vec3 position = vec3(model * vec4(input_position, 1.0));
    vec3 normal = normal_matrix * input_normal;
    gl_Position = projection * view * model * vec4(input_position, 1.0);


    float ambient_coef = 0.1;
    vec3 ambient = ambient_coef * light_color;

    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_position - position);
    float diffuse_coef = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diffuse_coef * light_color;

    float specular_strength = 0.5;
    vec3 view_direction = normalize(view_position - position);
    vec3 reflected_light_direction = reflect(-light_direction, norm);
    float specular_coef = pow(max(dot(view_direction, reflected_light_direction), 0.0), 32);
    vec3 specular = specular_strength * specular_coef * light_color;

    lighting_color = ambient + diffuse + specular;
}