#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "Error!" << std::endl;
    }

    // TODO : remove this scope
    {
        // instead create vb and ib on heap using new instead of stack 
        //vertex positions
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, //0
             0.5f, -0.5f, 1.0f, 0.0f, //1
             0.5f,  0.5f, 1.0f, 1.0f, //2
            -0.5f,  0.5f, 0.0f, 1.0f  //3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // VERTEX ARRAY OBJECT
        VertexArray va;

        // VERTEX BUFFER 
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        // specifies the layout
        VertexBufferLayout layout;
        layout.Push<float>(2); // vertices
        layout.Push<float>(2); // tex coords
        va.AddBuffer(vb, layout);

        // INDEX BUFFER 
        IndexBuffer ib(indices, 6);

        // SHADER
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.6f, 0.0f, 0.7f, 1.0f);

        // SET projection matrix
        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

        Texture texture("res/textures/eagle.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);
        shader.SetUniformMat4("u_MVP", proj);

        //Unbind everything
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", 0.1f, 0.0f, 0.7f, 1.0f);

            va.Bind(); 
            ib.Bind();

            // in real world this function should take in va, ib, material(which has shader info,
            // plus all the data)
            renderer.Draw(va, ib, shader);


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}