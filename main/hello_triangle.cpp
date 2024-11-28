#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine.h"
#include "file_utility.h"
#include "scene.h"

namespace gpr5300
{
    class HelloTriangle final : public Scene
    {
    public:
        void Begin() override;
        void End() override;
        void Update(float dt) override;

        float elapsed_time_ = 0;

    private:
        GLuint vertexShader_ = 0;
        GLuint fragmentShader_ = 0;
        GLuint program_ = 0;
        GLuint vao_ = 0;

        unsigned int texture_;
    };

    void HelloTriangle::Begin()
    {
        //Load shaders
        const auto vertexContent = LoadFile("C:/Users/maxen/source/repos/samples-Tornade-10/data/shaders/hello_triangle/triangle.vert");
        const auto* ptr = vertexContent.data();
        vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader_, 1, &ptr, nullptr);
        glCompileShader(vertexShader_);
        //Check success status of shader compilation
        GLint success;
        glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            std::cerr << "Error while loading vertex shader\n";
        }
        const auto fragmentContent = LoadFile("C:/Users/maxen/source/repos/samples-Tornade-10/data/shaders/hello_triangle/triangle.frag");
        ptr = fragmentContent.data();
        fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader_, 1, &ptr, nullptr);
        glCompileShader(fragmentShader_);
        //Check success status of shader compilation

        glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            std::cerr << "Error while loading fragment shader\n";
        }
        //Load program/pipeline
        program_ = glCreateProgram();
        glAttachShader(program_, vertexShader_);
        glAttachShader(program_, fragmentShader_);
        glLinkProgram(program_);
        //Check if shader program was linked correctly

        glGetProgramiv(program_, GL_LINK_STATUS, &success);
        if (!success)
        {
            std::cerr << "Error while linking shader program\n";
        }

        //Load images
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load and generate the texture
        int width, height, nrChannels;
        unsigned char* data = stbi_load("C:/Users/maxen/source/repos/samples-Tornade-10/textures/container.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
        texture_ = texture;


        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        //Empty vao
        glCreateVertexArrays(1, &vao_);
    }

    void HelloTriangle::End()
    {
        //Unload program/pipeline
        glDeleteProgram(program_);

        glDeleteShader(vertexShader_);
        glDeleteShader(fragmentShader_);

        glDeleteVertexArrays(1, &vao_);

    }

    void HelloTriangle::Update(float dt)
    {
        elapsed_time_ += dt;


        glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
        glm::mat4 trans = glm::mat4(1.0f);

        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, elapsed_time_, glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    	vec = trans * vec;
        std::cout << vec.x << vec.y << vec.z << std::endl;

        unsigned int transformLoc = glGetUniformLocation(program_, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    
        //Draw program
        glUseProgram(program_);
        glBindTexture(GL_TEXTURE_2D, texture_);
        glBindVertexArray(vao_);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
int main(int argc, char** argv)
{
    gpr5300::HelloTriangle scene;
    gpr5300::Engine engine(&scene);
    engine.Run();

    return EXIT_SUCCESS;
}