#include "engine.h"

void Engine::startup(std::shared_ptr<Scene> scene)
{
    renderManager.startup(scene);
}

void Engine::run()
{
    renderManager.render();
}