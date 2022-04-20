#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    Shader(const std::string &vertex_path, const std::string &fragment_path)
    {
        std::string vertex_shader_code;
        std::string fragment_shader_code;
        std::ifstream vert_shader_file;
        std::ifstream frag_shader_file;
        vert_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        frag_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vert_shader_file.open(vertex_path);
            frag_shader_file.open(fragment_path);
            std::stringstream vert_shader_stream, frag_shader_stream;
            vert_shader_stream << vert_shader_file.rdbuf();
            frag_shader_stream << frag_shader_file.rdbuf();
            vert_shader_file.close();
            frag_shader_file.close();

            vertex_shader_code = vert_shader_stream.str();
            fragment_shader_code = frag_shader_stream.str();
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
        }

        const char* vertex_shader_source = vertex_shader_code.c_str();
        const char* fragment_shader_source = fragment_shader_code.c_str();
        
        unsigned int vertex, fragment;
        int error_status;
        char error_log[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertex_shader_source, nullptr);
        glCompileShader(vertex);

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragment_shader_source, nullptr);
        glCompileShader(fragment);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &error_status);
        if (!error_status)
        {
            glGetShaderInfoLog(vertex, 512, nullptr, error_log);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << error_log << "\n";
        }

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &error_status);
        if (!error_status)
        {
            glGetShaderInfoLog(fragment, 512, nullptr, error_log);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << error_log << "\n";
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &error_status);
        if (!error_status)
        {
            glGetProgramInfoLog(ID, 512, nullptr, error_log);
            std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << error_log << "\n";
        }
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    };

    void use()
    {
        glUseProgram(ID);
    };

    void set_bool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    };
    void set_int(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    };
    void set_float(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    };

    void set_vec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    };
    void set_vec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    };
    void set_vec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    };

    void set_vec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    };
    void set_vec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    };
    void set_vec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    };

    void set_mat2(const std::string &name, const glm::mat2 &value) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    };
    void set_mat3(const std::string &name, const glm::mat3 &value) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    };
    void set_mat4(const std::string &name, const glm::mat4 &value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    };
    
    ~Shader()
    {
        glDeleteProgram(ID);
    };
};

#endif