#include "stdafx.h"
#include "GraphicsMgr.h"
#include "Utils.h"


using namespace glm;

extern GLFWwindow* _Window;

GraphicsMgr::GraphicsMgr() {
    _ProgramID = - 1;
    
    _VertexArray = -1;

    _VertexBuffer = -1;
    _UVBuffer = -1;
    _NormalBuffer = -1;
    _IndicesBuffer = -1;

    _Texture = -1;
}

int GraphicsMgr::InitGraphics(const int HSize, const int VSize) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    /* Default Hints */
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _Window = glfwCreateWindow(HSize, VSize, "GRAPHICS", NULL, NULL);
    if (!_Window) {
        fprintf(stderr, "Failed to open GLFW window");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(_Window);

    /* Window position */
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(_Window, mode->width - HSize - 10, 30);

    /* GLEW */
    glewExperimental = true;// IMPORTANT
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(_Window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Locate in program */
    LocateProgram();

    /*** OBJ DATA ***/
    Utils::LoadOBJ("..\\Resources\\models\\suzanne.obj", _Indices, _Vertices, _UVs, _Normals);
    _Texture = Utils::LoadDDS("..\\Resources\\textures\\uvmapSuzanne.DDS");

    InitBuffers();

    _Camera = new sMVP();

    printf("Graphics STARTED\n");
    return 1;
}

void GraphicsMgr::InitBuffers() {
    /*** Vertex Array Object ***/
    glGenVertexArrays(1, &_VertexArray);
    glBindVertexArray(_VertexArray);

    
    /* Vertex Buffer */
    glGenBuffers(1, &_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _Vertices.size() * sizeof(vec3), &_Vertices[0], GL_STATIC_DRAW);

    /* UVs Buffer */
    glGenBuffers(1, &_UVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _UVBuffer);
    glBufferData(GL_ARRAY_BUFFER, _UVs.size() * sizeof(vec2), &_UVs[0], GL_STATIC_DRAW);

    /* Normal Buffer */
    glGenBuffers(1, &_NormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _NormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, _Normals.size() * sizeof(vec3), &_Normals[0], GL_STATIC_DRAW);

    /* Indices Buffer */
    glGenBuffers(1, &_IndicesBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IndicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _Indices.size() * sizeof(unsigned int), &_Indices[0], GL_STATIC_DRAW);

    printf("Buffers READY\n");
}

void GraphicsMgr::LocateProgram() {
    _ProgramID = Utils::LoadShaders("..\\Resources\\shaders\\SimpleVertexShader.shader",
                                    "..\\Resources\\shaders\\SimpleFragmentShader.shader");
    
    _MatrixID = glGetUniformLocation(_ProgramID, "MVP");
    _ModelMatrixID = glGetUniformLocation(_ProgramID, "M");
    _ViewMatrixID = glGetUniformLocation(_ProgramID, "V");
    
    _TextureID = glGetUniformLocation(_ProgramID, "textureSampler");
    
    _LightID = glGetUniformLocation(_ProgramID, "LightPosition_worldspace");
    _LightsOnID = glGetUniformLocation(_ProgramID, "LightsOn");

    printf("Program LOADED\n");
}

void GraphicsMgr::Tick(float DeltaTime) {
    Mgr::Tick(DeltaTime);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(_ProgramID);

    /* Camera MVP */
    glUniformMatrix4fv(_MatrixID, 1, GL_FALSE, &_Camera->_MVP[0][0]);
    glUniformMatrix4fv(_ModelMatrixID, 1, GL_FALSE, &_Camera->_ModelMatrix[0][0]);
    glUniformMatrix4fv(_ViewMatrixID, 1, GL_FALSE, &_Camera->_ViewMatrix[0][0]);

    /* Light */
    vec3 lightPos = vec3(0, 0, 20.0f);
    glUniform3f(_LightID, lightPos.x, lightPos.y, lightPos.z);
    glUniform1i(_LightsOnID, true);

    /* Texture */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _Texture);
    glUniform1i(_TextureID, 0);

    
    /* Vertices */
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _VertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    /* UVs */
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, _UVBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    /* Normals */
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, _NormalBuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    /* Indices */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IndicesBuffer);
    glDrawElements(GL_TRIANGLES, _Indices.size(), GL_UNSIGNED_SHORT, (void*)0);


    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    // Swap buffers
    glfwSwapBuffers(_Window);
    glfwPollEvents();
}

sMVP* GraphicsMgr::GetCamera() {
    return _Camera;
}

GraphicsMgr::~GraphicsMgr() {
    delete _Camera;

    // Delete VBO
    glDeleteBuffers(1, &_VertexBuffer);
    glDeleteBuffers(1, &_UVBuffer);
    glDeleteBuffers(1, &_NormalBuffer);
    glDeleteBuffers(1, &_IndicesBuffer);
    // Delete program
    glDeleteProgram(_ProgramID);
    // Delete Textures
    glDeleteTextures(1, &_Texture);
    // Delete VAO
    glDeleteVertexArrays(1, &_VertexArray);
}
