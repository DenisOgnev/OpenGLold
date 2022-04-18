#version 330 core

in vec3 normal;
in vec3 frag_position;

out vec4 frag_color;

uniform vec3 light_color;
uniform vec3 object_color;
uniform vec3 light_position;
uniform vec3 view_position;

void main()
{
    float ambient_coef = 0.1;
    vec3 ambient = ambient_coef * light_color;

    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_position - frag_position);
    float diffuse_coef = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diffuse_coef * light_color;

    float specular_strength = 1.0;
    vec3 view_direction = normalize(view_position - frag_position);
    vec3 reflected_light_direction = reflect(-light_direction, norm);
    float specular_coef = pow(max(dot(view_direction, reflected_light_direction), 0.0), 32);
    vec3 specular = specular_strength * specular_coef * light_color;

    vec3 result = (ambient + diffuse + specular) * object_color;

    frag_color = vec4(result, 1.0);
}