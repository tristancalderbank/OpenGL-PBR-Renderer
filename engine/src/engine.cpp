#include "engine.h"

Engine::Engine(EngineConfig &engineConfig) : mEngineConfig(engineConfig) {
    mWindowManager = std::make_shared<WindowManager>(engineConfig);
    mInputManager = std::make_shared<InputManager>(engineConfig, mWindowManager);
    mCameraManager = std::make_shared<CameraManager>(engineConfig, mWindowManager, mInputManager);
    mRenderManager = std::make_shared<RenderManager>(engineConfig, mWindowManager, mCameraManager);
}

void Engine::startup(std::shared_ptr<Scene> scene)
{
    std::cout << "Engine::startup";
    mWindowManager->startup(mInputManager);
    mRenderManager->startup(scene);
    mInputManager->startup();
    mCameraManager->startup();
}

void Engine::run()
{
    std::cout << "Engine::run\n";
    float lastFrameTime = 0.0f;
    float frameTimeDelta = 0.0f;

    while (!mWindowManager->windowShouldClose()) {
        float currentTime = glfwGetTime();
        frameTimeDelta = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        mInputManager->update();

        mRenderManager->startGuiFrame();

        mWindowManager->update();
        mCameraManager->update(frameTimeDelta);
        mRenderManager->render();

        mWindowManager->clearResizeFlag();
    }
}

void Engine::shutdown() {
    mRenderManager->shutdown();
    mWindowManager->shutdown();
}