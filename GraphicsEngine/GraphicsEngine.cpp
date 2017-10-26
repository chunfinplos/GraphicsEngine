#include "stdafx.h"
#include "GraphicsMgr.h"
#include "InputControlMgr.h"

using namespace std;


const short MAX_LOOPS = 1000;
const float TICK_TIME = 0.016666f; // 60 FPS
//const float TICK_TIME = 0.033333f; // 30 FPS (VSYNC must be enabled)
//const float TICK_TIME = 0.0666666f; // 15 FPS (VSYNC must be enabled)

GLFWwindow* _Window = nullptr;


int ExitProgram(int ExitCode);


int main(/*int argc, char* argv[]*/) {
    int ExitCode = -1;

    /* MANAGERS */
    GraphicsMgr* _GraphicsMgr = new GraphicsMgr();
    if (!_GraphicsMgr->InitGraphics(1024, 768)) return ExitProgram(ExitCode);
    InputControlMgr* _InputControlMgr = new InputControlMgr();
    

    float timeLogic = float(glfwGetTime());
    float lastTime = timeLogic;
    float timeReal = 0.0f;
    float deltaTime = 0.0f;
    long nbFrames = 0;
    short numLoops = 0;

    do {
        /* TIME */
        timeReal = float(glfwGetTime());
        deltaTime = 0.0f;
        numLoops = 0;
        while (timeReal - timeLogic > TICK_TIME && numLoops < MAX_LOOPS) {
            // LOGIC TICK CONSTANT TIME
            timeLogic += TICK_TIME;
            deltaTime += TICK_TIME;
            numLoops++;
        }

        nbFrames++;
        if (timeReal - lastTime >= 1.0) {
            printf("%i FPS\n", nbFrames);
            nbFrames = 0;
            lastTime += 1.0f;
        }

        _InputControlMgr->Tick(deltaTime);
        _GraphicsMgr->GetCamera()->SetMVP(mat4(1.0), _InputControlMgr->GetViewMatrix(),
                                                     _InputControlMgr->GetProjectionMatrix());
        _GraphicsMgr->Tick(deltaTime);

        if (_InputControlMgr->IsKeyOn(EXIT)) ExitCode = 0;
    } while (ExitCode < 0);

    delete _GraphicsMgr;
    delete _InputControlMgr;

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return ExitProgram(ExitCode);
}

int ExitProgram(int ExitCode) {
    switch (ExitCode) {
        case 0://ESC KEY
            break;
        default:
            break;
    }
    fprintf(stderr, "EXIT %i\n", ExitCode);
    return ExitCode;
}