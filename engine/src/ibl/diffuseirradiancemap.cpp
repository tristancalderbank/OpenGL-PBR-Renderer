#include "ibl/diffuseirradiancemap.h"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "constants.h"
#include "diffuseirradiancecube.h"
#include "timer.h"
#include "ibl/cubemapframebuffer.h"
#include "ibl/hdricube.h"

DiffuseIrradianceMap::DiffuseIrradianceMap(const std::string &engineRoot, const std::string &hdriPath) {
    std::string hdriVertexShaderPath = engineRoot + "/src/ibl/shaders/hdricube.vert";
    std::string hdriFragmentShaderPath = engineRoot + "/src/ibl/shaders/hdricube.frag";

    hdriShader = std::make_unique<Shader>(hdriVertexShaderPath.c_str(), hdriFragmentShaderPath.c_str());

    std::string diffuseIrradianceVertexShaderPath = engineRoot + "/src/ibl/shaders/diffuseirradiance.vert";
    std::string diffuseIrradianceFragmentShaderPath = engineRoot + "/src/ibl/shaders/diffuseirradiance.frag";

    diffuseIrradianceShader = std::make_unique<Shader>(diffuseIrradianceVertexShaderPath.c_str(), diffuseIrradianceFragmentShaderPath.c_str());

    environmentFramebuffer = std::make_unique<CubemapFramebuffer>(cubemapWidth, cubemapHeight);
    diffuseIrradianceFramebuffer = std::make_unique<CubemapFramebuffer>(diffuseIrradianceMapWidth, diffuseIrradianceMapHeight);
    hdriCube = std::make_unique<HDRICube>(hdriPath);
}

void DiffuseIrradianceMap::compute() {
    Timer timer;

    glm::mat4 model = constants::mIndentity4;
    glm::mat4 cameraAngles[] =
        {
            glm::lookAt(constants::origin, constants::unitX, -constants::unitY),
            glm::lookAt(constants::origin, -constants::unitX, -constants::unitY),
            glm::lookAt(constants::origin, constants::unitY, constants::unitZ),
            glm::lookAt(constants::origin, -constants::unitY, -constants::unitZ),
            glm::lookAt(constants::origin, constants::unitZ, -constants::unitY),
            glm::lookAt(constants::origin, -constants::unitZ, -constants::unitY)
        };
    glm::mat4 projection = glm::perspective(
        glm::radians(90.0f), // 90 degrees to cover one face
        1.0f, // its a square
        0.1f,
        2.0f);

    glViewport(0, 0, cubemapWidth, cubemapHeight);

    // render the equirectangular HDR texture to a cubemap
    environmentFramebuffer->bind();
    hdriShader->use();

    // render to each side of the cubemap
    for (auto i = 0; i < 6; i++) {
        hdriShader->setModelViewProjectionMatrices(model, cameraAngles[i], projection);
        environmentFramebuffer->setCubeFace(i);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        hdriCube->Draw(*hdriShader);
    }

    // now render to a second cubemap which is just an average convolution of the
    // first cubemap
    auto diffuseIrradianceCube = DiffuseIrradianceCube(environmentFramebuffer->getCubemapTextureId());
    glViewport(0, 0, diffuseIrradianceMapWidth, diffuseIrradianceMapHeight);
    diffuseIrradianceFramebuffer->bind();
    diffuseIrradianceShader->use();

    // render to each side of the cubemap
    for (auto i = 0; i < 6; i++) {
        diffuseIrradianceShader->setModelViewProjectionMatrices(model, cameraAngles[i], projection);
        diffuseIrradianceFramebuffer->setCubeFace(i);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        diffuseIrradianceCube.Draw(*diffuseIrradianceShader);
    }

    timer.logDifference("Rendered diffuse irradiance map");
}

unsigned int DiffuseIrradianceMap::getCubemapId() {
   return diffuseIrradianceFramebuffer->getCubemapTextureId();
}

unsigned int DiffuseIrradianceMap::getEnvCubemapId() {
    return environmentFramebuffer->getCubemapTextureId();
}