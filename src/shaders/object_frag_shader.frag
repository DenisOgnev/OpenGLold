#version 330 core

in vec3 normal;
in vec3 frag_position;

out vec4 frag_color;

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 view_position;

void main()
{
    vec3 ambient = light_color * material.ambient;

    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_position - frag_position);
    float diffuse_coef = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = light_color * (diffuse_coef * material.diffuse);

    vec3 view_direction = normalize(view_position - frag_position);
    vec3 reflected_light_direction = reflect(-light_direction, norm);
    float specular_coef = pow(max(dot(view_direction, reflected_light_direction), 0.0), material.shininess);
    vec3 specular = light_color * (specular_coef * material.specular);

    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(result, 1.0);
}