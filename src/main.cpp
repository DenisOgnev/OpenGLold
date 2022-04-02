#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"
#include "camera.hpp"
#include "stb_image.h"

const int width = 800;
const int height = 600;
const std::string name = "OpenGL";

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float last_x = width / 2.0f;
float last_y = height / 2.0f;
bool first_mouse = true;

float delta_time = 0.0f;
float last_frame = 0.0f;

glm::vec3 light_pos(1.2f, 1.0f, 2.0f);

void input_process(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_TRUE)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_TRUE)
    {
        camera.process_keyboard_input(FORWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_TRUE)
    {
        camera.process_keyboard_input(BACKWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_TRUE)
    {
        camera.process_keyboard_input(LEFT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_TRUE)
    {
        camera.process_keyboard_input(RIGHT, delta_time);
    }
}

void framesize_buffer_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double x_pos_in, double y_pos_in)
{
    float x_pos = static_cast<float>(x_pos_in);
    float y_pos = static_cast<float>(y_pos_in);

    if (first_mouse)
    {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos;

    last_x = x_pos;
    last_y = y_pos;

    camera.process_mouse_input(x_offset, y_offset);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    camera.process_mouse_scroll(static_cast<float>(y_offset));
}

int main()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n";
        glfwTerminate();
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create a window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framesize_buffer_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }
    glEnable(GL_DEPTH_TEST);


    float vertices[] = 
    {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    const std::string object_vertex_path = "../../src/shaders/object_vert_shader.vert";
    const std::string object_fragment_path = "../../src/shaders/object_frag_shader.frag";
    const std::string light_vertex_path = "../../src/shaders/light_vert_shader.vert";
    const std::string light_fragment_path = "../../src/shaders/light_frag_shader.frag";
    Shader object_shader(object_vertex_path, object_fragment_path);
    Shader light_shader(light_vertex_path, light_fragment_path);

    unsigned int VAO, VBO, lightVAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);


    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);




    while (!glfwWindowShouldClose(window))
    {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        input_process(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        object_shader.use();
        object_shader.set_vec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));
        object_shader.set_vec3("object_color", glm::vec3(1.0f, 0.5f, 0.31f));

        glm::mat4 model(1.0f);
        glm::mat4 view = camera.get_view_matrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)width / (float)height, 0.1f, 100.0f);
        object_shader.set_mat4("model", model);
        object_shader.set_mat4("view", view);
        object_shader.set_mat4("projection", projection);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        light_shader.use();
        model = glm::translate(model, light_pos);
        model = glm::scale(model, glm::vec3(0.2f));
        light_shader.set_mat4("model", model);
        light_shader.set_mat4("view", view);
        light_shader.set_mat4("projection", projection);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);        

        glBindVertexArray(0);


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);


    glfwTerminate();
    return 0;
}