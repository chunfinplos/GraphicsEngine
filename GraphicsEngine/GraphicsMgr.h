#pragma once
#include "Mgr.h"


using namespace glm;

struct sMVP {
    mat4 _ModelMatrix;
    mat4 _ViewMatrix;
    mat4 _ProjectionMatrix;

    mat4 _MVP;

    sMVP(mat4 M = mat4(1.0), mat4 V = mat4(1.0), mat4 P = mat4(1.0)) :
        _ModelMatrix(M),
        _ViewMatrix(V),
        _ProjectionMatrix(P) { _MVP = mat4(1.0); };

    void SetMVP(mat4 M, mat4 V, mat4 P) {
        _ModelMatrix = M;
        _ViewMatrix = V;
        _ProjectionMatrix = P;
        _MVP = P * V * M; // Inverse!!
    }
};

class GraphicsMgr : public Mgr {

private:
    /* PROGRAM */
    GLuint _ProgramID;
    GLuint _MatrixID;
    GLuint _ViewMatrixID;
    GLuint _ModelMatrixID;
    GLuint _TextureID;
    GLuint _LightID;
    GLuint _LightsOnID;

    /* VAO */
    GLuint _VertexArray;

    /* VBOs */
    GLuint _VertexBuffer;
    GLuint _UVBuffer;
    GLuint _NormalBuffer;
    GLuint _IndicesBuffer;

    /*** OBJ DATA ***/
    std::vector<unsigned short> _Indices;
    std::vector<vec3> _Vertices, _Normals;
    std::vector<vec2> _UVs;

    GLuint _Texture;

    sMVP* _Camera;

    void InitBuffers();
    void LocateProgram();

public:
    GraphicsMgr();
    int InitGraphics(const int HSize, const int VSize);

    void Tick(float DeltaTime);

    sMVP* GetCamera();

    ~GraphicsMgr();
};

