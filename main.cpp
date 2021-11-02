#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";
// adjust viewport size with every window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}
//check if key is pressed
void processInput(GLFWwindow *window)
{
    // check whether ESC key was pressed
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    glfwInit();
    // setting major and minor version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //use the core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // for Mac OS X
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create the window object
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //set window as main context for current thread
    glfwMakeContextCurrent(window);

    // use glad to locate opengl function (OS specific)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // set opengl viewport dimens
    glViewport(0, 0, 800, 600);

    //register the callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window))
    {
        //called in each iteration
        processInput(window);

        //rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float vertices[] = {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f, 0.5f, 0.0f
        };

        //manage memory for vertices in GPU vertex buffer objects
        unsigned int VBO;
        //assign in a unique ID
        glGenBuffers(1, &VBO);

        unsigned int VAO;
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);
        //bind newly created buffer to the buffer type GL_ARRAY_BUFFER for the VBOs
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // copies user defined data(vertices:array) to the currently bound buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // vertex shader
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);

        //attach source and compile
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // check for compilation errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            // print the  error
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
                      infoLog << std::endl;
        }

        // compile the fragment shader
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check errors
        int fragSuccess;
        char fragInfoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);
        if(!fragSuccess)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, fragInfoLog);
            // print the  error
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
                      fragInfoLog << std::endl;
        }

        // creating shader program object
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();

        // link all the shaders
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
        int linkSuccess;
        char linkInfoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);
        if(!linkSuccess) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, linkInfoLog);
            // print the  error
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
                      linkInfoLog << std::endl;
        }

        //delete shaders after linkage
        //glDeleteShader(vertexShader);
        //glDeleteShader(fragmentShader);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(0);

        //use the program
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Double buffer technique to avoid showing artifacts, swap front with back buffer
        glfwSwapBuffers(window);
        //poll for keyboard or mouse etc
        glfwPollEvents();
    }

    // collect the garbage
    glfwTerminate();
    return 0;
}
