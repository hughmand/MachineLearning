#define GLEW_STAIC
#include"glew.h"
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;

    ShaderProgramSource(std::string first, std::string second) {
        VertexSource = first;
        FragmentSource = second;
    }

};


static ShaderProgramSource ParseShader(const std::string& filepath)
{

    enum class ShaderType {

        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {

        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;

            }
            else if (line.find("fragment") != std::string::npos) {

                type = ShaderType::FRAGMENT;
            }

        }
        else
        {

            ss[(int)type] << line << std::endl;

        }
    }
    return ShaderProgramSource(ss[0].str(), ss[1].str());
}


static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = &source[0];
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragement") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{

    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}






int displayfunc (void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;




    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {

        std::cout << "error" << std::endl;
    };
    std::cout << glGetString(GL_VERSION) << std::endl;

    //openGl is a state machine, so 
    //define buffer - data from the CPU
    //define shader - the stuff to run on the GPU using the buffer
    //execute

    //vertex prepared by CPU
    float positions[10] = {
        -1.0, -1.0,
        -1.0, -0.9,
        -0.9, -1.0,
        -0.9, -0.9

    };


    unsigned int indices[] = { 0, 1, 2,
                                1,2,3};


    unsigned int buffer_id;
    glGenBuffers(1, &buffer_id); //generates a single buffer with ID buffer_id
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id); //selects the buffer as active, and tells us this is an Array buffer - for the coordinates
    glBufferData(GL_ARRAY_BUFFER, 5 * 2 * sizeof(float), positions, GL_STATIC_DRAW);


    // a vertex is just a point on geometry. It is not just a position. It can contain lots of other attributes such as colour, normals

    //vertex attributes - tells openGL how the buffer is structured
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0); //the pointer is the offset within each vertex for the current vertex. Call once for every attribute per vertex
    glEnableVertexAttribArray(0); //enables the vertex


    unsigned int index_buffer_object;
    glGenBuffers(1, &index_buffer_object); //generates a single buffer with ID buffer_id
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object); //selects the buffer as active, and tells us this is an Array buffer - for the coordinates
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 9 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


    ShaderProgramSource source = ParseShader("C:/dev/MLC/Shader/Shaders/basic.shader");
    std::cout << source.VertexSource << std::endl;
    std::cout << source.FragmentSource << std::endl;
    //vertex shaders
    //A shader is a program that runs on the GPU. Vertex and Fragment/Pixel use 90% of the time. Geometry and compute shaders exist too (ML)
    //vertex shader is called before the fragment shader 
    //Vertex shader called for each vertex. Tells openGL where the Vertex should be location wise

    //fragment shader is responsible for figuring out the colour of a single pixel 
    //Gets called many, many times more than the vertex shader



    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    //indx buffer lets you reuse existing verticies
    //everything is written with triangles, because it iss the fewest points required to uniquely define a plane.

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); //use without index buffer
        // glDrawElements for index buffer

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}