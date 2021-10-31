#include "engine.h"

/**
 *
 * @param argc number of args
 * @param argv name, viewport_width, viewport_height
 * @return
 */
int main(int argc, const char * argv[])
{
    Engine engine;

    engine.startup();
    engine.run();

    return 0;
}