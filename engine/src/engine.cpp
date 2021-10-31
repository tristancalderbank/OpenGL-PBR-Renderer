#include "engine.h"

void Engine::startup()
{
    renderManager.startup();
}

void Engine::run()
{
    renderManager.render();
}