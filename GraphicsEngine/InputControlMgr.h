#pragma once
#include "Mgr.h"


using namespace glm;

struct sKeyData {
    bool _Active = false;
    int _Counter = 0;
    bool _On = false;
};

class InputControlMgr : public Mgr {

private:
    const float _Speed_ = 3.0f;
    const float _MouseSpeed_ = 0.005f;
    const float _CameraRadius_ = 20.0f;
    const float _FoV = 45.0f;

    int _HSize;
    int _VSize;

    float _HorizontalAngle;
    float _VerticalAngle;

    vec3 _Position;
    mat4 _ViewMatrix;
    mat4 _ProjectionMatrix;

    /* KEYS */
    std::map<std::string, sKeyData> _KeyBoardData;

    void ComputeMVP(float DeltaTime);
    void ComputeKeys(float DeltaTime);
    void ComputeOption(int KeyCode, sKeyData &KeyData);

public:
    InputControlMgr(int HSize = 1024, int VSize = 768);

    void Tick(float DeltaTime);

    vec3 GetPosition();
    mat4 GetViewMatrix();
    mat4 GetProjectionMatrix();

    bool IsKeyOn(std::string KeyName);

    ~InputControlMgr();
};

