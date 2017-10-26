#include "stdafx.h"
#include "InputControlMgr.h"


extern GLFWwindow* _Window;


using namespace glm;

InputControlMgr::InputControlMgr(int HSize, int VSize) : _HSize(HSize), _VSize(VSize) {
    _HorizontalAngle = pi<float>();
    _VerticalAngle = 0.0f;
    _Position = vec3(0, 0, _CameraRadius_);

    //45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    _ProjectionMatrix = perspective(radians(_FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    _ViewMatrix = lookAt(_Position, vec3(0, 0, 0), vec3(0, 1, 0));

    /* KEYS */
    _KeyBoardData[EXIT] = sKeyData();
    _KeyBoardData[OPTION] = sKeyData();

    glfwSetCursorPos(_Window, HSize / 2, VSize / 2);
}

void InputControlMgr::Tick(float DeltaTime) {
    Mgr::Tick(DeltaTime);

    /* MOUSE */
    double X, Y;
    glfwGetCursorPos(_Window, &X, &Y);
    glfwSetCursorPos(_Window, _HSize / 2, _VSize / 2);

    _HorizontalAngle += _MouseSpeed_ * float(_HSize / 2 - X);
    _VerticalAngle += _MouseSpeed_ * float(_VSize / 2 - Y);

    /* WORLD */
    ComputeMVP(DeltaTime);

    /* KEYS */
    ComputeKeys(DeltaTime);
}

void InputControlMgr::ComputeMVP(float DeltaTime) {
    // Direction : Spherical coordinates to Cartesian coordinates conversion
    vec3 Direction(cos(_VerticalAngle) * sin(_HorizontalAngle),
                   sin(_VerticalAngle),
                   cos(_VerticalAngle) * cos(_HorizontalAngle));

    // Right vector
    vec3 Right = vec3(sin(_HorizontalAngle - half_pi<float>()),
                      0,
                      cos(_HorizontalAngle - half_pi<float>()));

    // Up vector
    vec3 Up = cross(Right, Direction);

    // Move forward
    if (glfwGetKey(_Window, GLFW_KEY_W) == GLFW_PRESS) {
        _Position += Direction * DeltaTime * _Speed_;
    }
    // Move backward
    if (glfwGetKey(_Window, GLFW_KEY_S) == GLFW_PRESS) {
        _Position -= Direction * DeltaTime * _Speed_;
    }
    // Strafe right
    if (glfwGetKey(_Window, GLFW_KEY_D) == GLFW_PRESS) {
        _Position += Right * DeltaTime * _Speed_;
    }
    // Strafe left
    if (glfwGetKey(_Window, GLFW_KEY_A) == GLFW_PRESS) {
        _Position -= Right * DeltaTime * _Speed_;
    }

    _ViewMatrix = lookAt(_Position, _Position + Direction, Up);
}

void InputControlMgr::ComputeKeys(float DeltaTime) {
    ComputeOption(GLFW_KEY_ESCAPE, _KeyBoardData[EXIT]);
    ComputeOption(GLFW_KEY_SPACE, _KeyBoardData[OPTION]);
}

void InputControlMgr::ComputeOption(int KeyCode, sKeyData &KeyData) {
    if (glfwGetKey(_Window, KeyCode) == GLFW_PRESS) {
        KeyData._Active = true;
    }

    if (KeyData._Active && glfwGetKey(_Window, KeyCode) == GLFW_RELEASE) {
        KeyData._Active = false;
        ++KeyData._Counter %= 2;
        printf("%i", KeyData._Counter);
    }
    KeyData._On = KeyData._Counter;
}

vec3 InputControlMgr::GetPosition() {
    return _Position;
}

mat4 InputControlMgr::GetViewMatrix() {
    return _ViewMatrix;
}

mat4 InputControlMgr::GetProjectionMatrix() {
    return _ProjectionMatrix;
}

bool InputControlMgr::IsKeyOn(std::string KeyName) {
    if (_KeyBoardData.find(KeyName) != _KeyBoardData.end()) {
        return _KeyBoardData[KeyName]._On;
    }
    else return false;
}

InputControlMgr::~InputControlMgr() {}
